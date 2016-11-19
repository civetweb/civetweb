/* Copyright (c) 2013-2016 the Civetweb developers
 * Copyright (c) 2004-2013 Sergey Lyubka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#if defined(_WIN32)

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS /* Disable deprecation warning in VS2005 */
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN /* Required for some functions (tray icons, ...) */
#endif

#else

#define _XOPEN_SOURCE 600 /* For PATH_MAX on linux */
/* This should also be sufficient for "realpath", according to
 * http://man7.org/linux/man-pages/man3/realpath.3.html, but in
 * reality it does not seem to work. */
/* In case this causes a problem, disable the warning:
 * #pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
 * #pragma clang diagnostic ignored "-Wimplicit-function-declaration"
 */
#endif

#ifndef IGNORE_UNUSED_RESULT
#define IGNORE_UNUSED_RESULT(a) ((void)((a) && 1))
#endif

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define NO_RETURN [[noreturn]]
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#define NO_RETURN _Noreturn
#else
#define NO_RETURN
#endif

/* Use same defines as in civetweb.c before including system headers. */
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE /* For fseeko(), ftello() */
#endif
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64 /* Use 64-bit file offsets by default */
#endif
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS /* <inttypes.h> wants this for C++ */
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS /* C++ wants that for INT64_MAX */
#endif

#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>

#include "civetweb.h"

#define printf                                                                 \
	DO_NOT_USE_THIS_FUNCTION__USE_fprintf /* Required for unit testing */

#if defined(_WIN32)                                                            \
    && !defined(__SYMBIAN32__) /* WINDOWS / UNIX include block */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 /* for tdm-gcc so we can use getconsolewindow */
#endif
#undef UNICODE
#include <windows.h>
#include <winsvc.h>
#include <shlobj.h>
#include <io.h>

#define getcwd(a, b) (_getcwd(a, b))
#if !defined(__MINGW32__)
extern char *_getcwd(char *buf, size_t size);
#endif
static int sGuard = 0; /* test if any dialog is already open */

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#ifndef S_ISDIR
#define S_ISDIR(x) ((x)&_S_IFDIR)
#endif

#define DIRSEP '\\'
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define sleep(x) (Sleep((x)*1000))
#define WINCDECL __cdecl
#define abs_path(rel, abs, abs_size) (_fullpath((abs), (rel), (abs_size)))

#else /* defined(_WIN32) && !defined(__SYMBIAN32__) - WINDOWS / UNIX include   \
         block */

#include <unistd.h>
#include <sys/utsname.h>
#include <sys/wait.h>

#define DIRSEP '/'
#define WINCDECL
#define abs_path(rel, abs, abs_size) (realpath((rel), (abs)))

#endif /* defined(_WIN32) && !defined(__SYMBIAN32__) - WINDOWS / UNIX include  \
          block */

#ifndef PATH_MAX
#define PATH_MAX (1024)
#endif

#define MAX_OPTIONS (50)
#define MAX_CONF_FILE_LINE_SIZE (8 * 1024)

struct tuser_data {
	char *first_message;
};

static int g_exit_flag = 0;         /* Main loop should exit */
static char g_server_base_name[40]; /* Set by init_server_name() */
static const char *g_server_name;   /* Set by init_server_name() */
static const char *g_icon_name;     /* Set by init_server_name() */
static char g_config_file_name[PATH_MAX] =
    "";                          /* Set by process_command_line_arguments() */
static struct mg_context *g_ctx; /* Set by start_civetweb() */
static struct tuser_data
    g_user_data; /* Passed to mg_start() by start_civetweb() */

#if !defined(CONFIG_FILE)
#define CONFIG_FILE "civetweb.conf"
#endif /* !CONFIG_FILE */

#if !defined(PASSWORDS_FILE_NAME)
#define PASSWORDS_FILE_NAME ".htpasswd"
#endif

/* backup config file */
#if !defined(CONFIG_FILE2) && defined(__linux__)
#define CONFIG_FILE2 "/usr/local/etc/civetweb.conf"
#endif

enum { OPTION_TITLE, OPTION_ICON, NUM_MAIN_OPTIONS };

static struct mg_option main_config_options[] = {
    {"title", CONFIG_TYPE_STRING, NULL},
    {"icon", CONFIG_TYPE_STRING, NULL},
    {NULL, CONFIG_TYPE_UNKNOWN, NULL}};


static void WINCDECL
signal_handler(int sig_num)
{
	g_exit_flag = sig_num;
}


static NO_RETURN void
die(const char *fmt, ...)
{
	va_list ap;
	char msg[200] = "";

	va_start(ap, fmt);
	(void)vsnprintf(msg, sizeof(msg) - 1, fmt, ap);
	msg[sizeof(msg) - 1] = 0;
	va_end(ap);

#if defined(_WIN32)
	MessageBox(NULL, msg, "Error", MB_OK);
#else
	fprintf(stderr, "%s\n", msg);
#endif

	exit(EXIT_FAILURE);
}


#ifdef WIN32
static int MakeConsole(void);
#endif


static void
show_server_name(void)
{
#ifdef WIN32
	(void)MakeConsole();
#endif

	fprintf(stderr, "CivetWeb v%s, built on %s\n", mg_version(), __DATE__);
}


static NO_RETURN void
show_usage_and_exit(const char *exeName)
{
	const struct mg_option *options;
	int i;

	if (exeName == 0 || *exeName == 0) {
		exeName = "civetweb";
	}

	show_server_name();

	fprintf(stderr, "\nUsage:\n");
	fprintf(stderr, "  Start server with a set of options:\n");
	fprintf(stderr, "    %s [config_file]\n", exeName);
	fprintf(stderr, "    %s [-option value ...]\n", exeName);
	fprintf(stderr, "  Show system information:\n");
	fprintf(stderr, "    %s -I\n", exeName);
	fprintf(stderr, "  Add user/change password:\n");
	fprintf(stderr,
	        "    %s -A <htpasswd_file> <realm> <user> <passwd>\n",
	        exeName);
	fprintf(stderr, "  Remove user:\n");
	fprintf(stderr, "    %s -R <htpasswd_file> <realm> <user>\n", exeName);
	fprintf(stderr, "\nOPTIONS:\n");

	options = mg_get_valid_options();
	for (i = 0; options[i].name != NULL; i++) {
		fprintf(stderr,
		        "  -%s %s\n",
		        options[i].name,
		        ((options[i].default_value == NULL)
		             ? "<empty>"
		             : options[i].default_value));
	}

	options = main_config_options;
	for (i = 0; options[i].name != NULL; i++) {
		fprintf(stderr,
		        "  -%s %s\n",
		        options[i].name,
		        ((options[i].default_value == NULL)
		             ? "<empty>"
		             : options[i].default_value));
	}

	exit(EXIT_FAILURE);
}


#if defined(_WIN32) || defined(USE_COCOA)
static const char *config_file_top_comment =
    "# Civetweb web server configuration file.\n"
    "# For detailed description of every option, visit\n"
    "# https://github.com/civetweb/civetweb/blob/master/docs/UserManual.md\n"
    "# Lines starting with '#' and empty lines are ignored.\n"
    "# To make a change, remove leading '#', modify option's value,\n"
    "# save this file and then restart Civetweb.\n\n";

static const char *
get_url_to_first_open_port(const struct mg_context *ctx)
{
	static char url[100];
	const char *open_ports = mg_get_option(ctx, "listening_ports");
	int a, b, c, d, port, n;

	if (sscanf(open_ports, "%d.%d.%d.%d:%d%n", &a, &b, &c, &d, &port, &n)
	    == 5) {
		snprintf(url,
		         sizeof(url),
		         "%s://%d.%d.%d.%d:%d",
		         open_ports[n] == 's' ? "https" : "http",
		         a,
		         b,
		         c,
		         d,
		         port);
	} else if (sscanf(open_ports, "%d%n", &port, &n) == 1) {
		snprintf(url,
		         sizeof(url),
		         "%s://localhost:%d",
		         open_ports[n] == 's' ? "https" : "http",
		         port);
	} else {
		snprintf(url, sizeof(url), "%s", "http://localhost:8080");
	}

	return url;
}


#ifdef ENABLE_CREATE_CONFIG_FILE
static void
create_config_file(const struct mg_context *ctx, const char *path)
{
	const struct mg_option *options;
	const char *value;
	FILE *fp;
	int i;

	/* Create config file if it is not present yet */
	if ((fp = fopen(path, "r")) != NULL) {
		fclose(fp);
	} else if ((fp = fopen(path, "a+")) != NULL) {
		fprintf(fp, "%s", config_file_top_comment);
		options = mg_get_valid_options();
		for (i = 0; options[i].name != NULL; i++) {
			value = mg_get_option(ctx, options[i].name);
			fprintf(fp,
			        "# %s %s\n",
			        options[i].name,
			        value ? value : "<value>");
		}
		fclose(fp);
	}
}
#endif
#endif


static char *
sdup(const char *str)
{
	size_t len;
	char *p;

	len = strlen(str) + 1;
	if ((p = (char *)malloc(len)) != NULL) {
		memcpy(p, str, len);
	}
	return p;
}


static const char *
get_option(char **options, const char *option_name)
{
	int i = 0;
	const char *opt_value = NULL;

	/* TODO (low, api makeover): options should be an array of key-value-pairs,
	 * like
	 *     struct {const char * key, const char * value} options[]
	 * but it currently is an array with
	 *     options[2*i] = key, options[2*i + 1] = value
	 * (probably with a MG_LEGACY_INTERFACE definition)
	 */
	while (options[2 * i] != NULL) {
		if (strcmp(options[2 * i], option_name) == 0) {
			opt_value = options[2 * i + 1];
			break;
		}
		i++;
	}
	return opt_value;
}


static int
set_option(char **options, const char *name, const char *value)
{
	int i, type;
	const struct mg_option *default_options = mg_get_valid_options();

	for (i = 0; main_config_options[i].name != NULL; i++) {
		if (0 == strcmp(name, main_config_options[i].name)) {
			/* This option is evaluated by main.c, not civetweb.c - just skip it
			 * and return OK */
			return 1;
		}
	}

	type = CONFIG_TYPE_UNKNOWN;
	for (i = 0; default_options[i].name != NULL; i++) {
		if (!strcmp(default_options[i].name, name)) {
			type = default_options[i].type;
		}
	}
	switch (type) {
	case CONFIG_TYPE_UNKNOWN:
		/* unknown option */
		return 0;
	case CONFIG_TYPE_NUMBER:
		/* integer number > 0, e.g. number of threads */
		if (atol(value) < 0) {
			/* invalid number */
			return 0;
		}
		break;
	case CONFIG_TYPE_STRING:
		/* any text */
		break;
	case CONFIG_TYPE_BOOLEAN:
		/* boolean value, yes or no */
		if ((0 != strcmp(value, "yes")) && (0 != strcmp(value, "no"))) {
			/* invalid boolean */
			return 0;
		}
		break;
	case CONFIG_TYPE_FILE:
	case CONFIG_TYPE_DIRECTORY:
		/* TODO (low): check this option when it is set, instead of calling
		 * verify_existence later */
		break;
	case CONFIG_TYPE_EXT_PATTERN:
		/* list of file extentions */
		break;
	default:
		die("Unknown option type - option %s", name);
	}

	for (i = 0; i < MAX_OPTIONS; i++) {
		if (options[2 * i] == NULL) {
			options[2 * i] = sdup(name);
			options[2 * i + 1] = sdup(value);
			options[2 * i + 2] = NULL;
			break;
		} else if (!strcmp(options[2 * i], name)) {
			free(options[2 * i + 1]);
			options[2 * i + 1] = sdup(value);
			break;
		}
	}

	if (i == MAX_OPTIONS) {
		die("Too many options specified");
	}

	if (options[2 * i] == NULL || options[2 * i + 1] == NULL) {
		die("Out of memory");
	}

	/* option set correctly */
	return 1;
}


static int
read_config_file(const char *config_file, char **options)
{
	char line[MAX_CONF_FILE_LINE_SIZE], *p;
	FILE *fp = NULL;
	size_t i, j, line_no = 0;

	/* Open the config file */
	fp = fopen(config_file, "r");
	if (fp == NULL) {
		/* Failed to open the file. Keep errno for the caller. */
		return 0;
	}

	/* Load config file settings first */
	if (fp != NULL) {
		fprintf(stderr, "Loading config file %s\n", config_file);

		/* Loop over the lines in config file */
		while (fgets(line, sizeof(line), fp) != NULL) {

			if (!line_no && !memcmp(line, "\xEF\xBB\xBF", 3)) {
				/* strip UTF-8 BOM */
				p = line + 3;
			} else {
				p = line;
			}
			line_no++;

			/* Ignore empty lines and comments */
			for (i = 0; isspace(*(unsigned char *)&line[i]);)
				i++;
			if (p[i] == '#' || p[i] == '\0') {
				continue;
			}

			/* Skip spaces, \r and \n at the end of the line */
			for (j = strlen(line) - 1;
			     isspace(*(unsigned char *)&line[j])
			         || iscntrl(*(unsigned char *)&line[j]);)
				line[j--] = 0;

			/* Find the space character between option name and value */
			for (j = i; !isspace(*(unsigned char *)&line[j]) && (line[j] != 0);)
				j++;

			/* Terminate the string - then the string at (line+i) contains the
			 * option name */
			line[j] = 0;
			j++;

			/* Trim additional spaces between option name and value - then
			 * (line+j) contains the option value */
			while (isspace(line[j]))
				j++;

			/* Set option */
			if (!set_option(options, line + i, line + j)) {
				fprintf(stderr,
				        "%s: line %d is invalid, ignoring it:\n %s",
				        config_file,
				        (int)line_no,
				        p);
			}
		}

		(void)fclose(fp);
	}
	return 1;
}


static void
process_command_line_arguments(int argc, char *argv[], char **options)
{
	char *p;
	size_t i, cmd_line_opts_start = 1;
#ifdef CONFIG_FILE2
	FILE *fp = NULL;
#endif

	/* Should we use a config file ? */
	if ((argc > 1) && (argv[1] != NULL) && (argv[1][0] != '-')
	    && (argv[1][0] != 0)) {
		/* The first command line parameter is a config file name. */
		snprintf(g_config_file_name,
		         sizeof(g_config_file_name) - 1,
		         "%s",
		         argv[1]);
		cmd_line_opts_start = 2;
	} else if ((p = strrchr(argv[0], DIRSEP)) == NULL) {
		/* No config file set. No path in arg[0] found.
		 * Use default file name in the current path. */
		snprintf(g_config_file_name,
		         sizeof(g_config_file_name) - 1,
		         "%s",
		         CONFIG_FILE);
	} else {
		/* No config file set. Path to exe found in arg[0].
		 * Use default file name next to the executable. */
		snprintf(g_config_file_name,
		         sizeof(g_config_file_name) - 1,
		         "%.*s%c%s",
		         (int)(p - argv[0]),
		         argv[0],
		         DIRSEP,
		         CONFIG_FILE);
	}
	g_config_file_name[sizeof(g_config_file_name) - 1] = 0;

#ifdef CONFIG_FILE2
	fp = fopen(g_config_file_name, "r");

	/* try alternate config file */
	if (fp == NULL) {
		fp = fopen(CONFIG_FILE2, "r");
		if (fp != NULL) {
			strcpy(g_config_file_name, CONFIG_FILE2);
		}
	}
	if (fp != NULL) {
		fclose(fp);
	}
#endif

	/* read all configurations from a config file */
	if (0 == read_config_file(g_config_file_name, options)) {
		if (cmd_line_opts_start == 2) {
			/* If config file was set in command line and open failed, die. */
			/* Errno will still hold the error from fopen. */
			die("Cannot open config file %s: %s",
			    g_config_file_name,
			    strerror(errno));
		}
		/* Otherwise: CivetWeb can work without a config file */
	}

	/* If we're under MacOS and started by launchd, then the second
	   argument is process serial number, -psn_.....
	   In this case, don't process arguments at all. */
	if (argv[1] == NULL || memcmp(argv[1], "-psn_", 5) != 0) {
		/* Handle command line flags.
		   They override config file and default settings. */
		for (i = cmd_line_opts_start; argv[i] != NULL; i += 2) {
			if (argv[i][0] != '-' || argv[i + 1] == NULL) {
				show_usage_and_exit(argv[0]);
			}
			if (!set_option(options, &argv[i][1], argv[i + 1])) {
				fprintf(
				    stderr,
				    "command line option is invalid, ignoring it:\n %s %s\n",
				    argv[i],
				    argv[i + 1]);
			}
		}
	}
}


static void
init_server_name(int argc, const char *argv[])
{
	int i;
	assert(sizeof(main_config_options) / sizeof(main_config_options[0])
	       == NUM_MAIN_OPTIONS + 1);
	assert((strlen(mg_version()) + 12) < sizeof(g_server_base_name));
	snprintf(g_server_base_name,
	         sizeof(g_server_base_name),
	         "CivetWeb V%s",
	         mg_version());

	g_server_name = g_server_base_name;
	for (i = 0; i < argc - 1; i++) {
		if ((argv[i][0] == '-')
		    && (0 == strcmp(argv[i] + 1,
		                    main_config_options[OPTION_TITLE].name))) {
			g_server_name = (const char *)(argv[i + 1]);
		}
	}
	g_icon_name = NULL;
	for (i = 0; i < argc - 1; i++) {
		if ((argv[i][0] == '-')
		    && (0 == strcmp(argv[i] + 1,
		                    main_config_options[OPTION_ICON].name))) {
			g_icon_name = (const char *)(argv[i + 1]);
		}
	}
}


static int
log_message(const struct mg_connection *conn, const char *message)
{
	const struct mg_context *ctx = mg_get_context(conn);
	struct tuser_data *ud = (struct tuser_data *)mg_get_user_data(ctx);

	fprintf(stderr, "%s\n", message);

	if (ud->first_message == NULL) {
		ud->first_message = sdup(message);
	}

	return 0;
}


static int
is_path_absolute(const char *path)
{
#ifdef _WIN32
	return path != NULL
	       && ((path[0] == '\\' && path[1] == '\\') || /* UNC path, e.g.
	                                                      \\server\dir */
	           (isalpha(path[0]) && path[1] == ':'
	            && path[2] == '\\')); /* E.g. X:\dir */
#else
	return path != NULL && path[0] == '/';
#endif
}


static void
verify_existence(char **options, const char *option_name, int must_be_dir)
{
	struct stat st;
	const char *path = get_option(options, option_name);

#ifdef _WIN32
	wchar_t wbuf[1024];
	char mbbuf[1024];
	int len;

	if (path) {
		memset(wbuf, 0, sizeof(wbuf));
		memset(mbbuf, 0, sizeof(mbbuf));
		len = MultiByteToWideChar(CP_UTF8,
		                          0,
		                          path,
		                          -1,
		                          wbuf,
		                          (int)sizeof(wbuf) / sizeof(wbuf[0]) - 1);
		wcstombs(mbbuf, wbuf, sizeof(mbbuf) - 1);
		path = mbbuf;
		(void)len;
	}
#endif

	if (path != NULL && (stat(path, &st) != 0
	                     || ((S_ISDIR(st.st_mode) ? 1 : 0) != must_be_dir))) {
		die("Invalid path for %s: [%s]: (%s). Make sure that path is either "
		    "absolute, or it is relative to civetweb executable.",
		    option_name,
		    path,
		    strerror(errno));
	}
}


static void
set_absolute_path(char *options[],
                  const char *option_name,
                  const char *path_to_civetweb_exe)
{
	char path[PATH_MAX] = "", absolute[PATH_MAX] = "";
	const char *option_value;
	const char *p;

	/* Check whether option is already set */
	option_value = get_option(options, option_name);

	/* If option is already set and it is an absolute path,
	   leave it as it is -- it's already absolute. */
	if (option_value != NULL && !is_path_absolute(option_value)) {
		/* Not absolute. Use the directory where civetweb executable lives
		   be the relative directory for everything.
		   Extract civetweb executable directory into path. */
		if ((p = strrchr(path_to_civetweb_exe, DIRSEP)) == NULL) {
			IGNORE_UNUSED_RESULT(getcwd(path, sizeof(path)));
		} else {
			snprintf(path,
			         sizeof(path) - 1,
			         "%.*s",
			         (int)(p - path_to_civetweb_exe),
			         path_to_civetweb_exe);
			path[sizeof(path) - 1] = 0;
		}

		strncat(path, "/", sizeof(path) - strlen(path) - 1);
		strncat(path, option_value, sizeof(path) - strlen(path) - 1);

		/* Absolutize the path, and set the option */
		IGNORE_UNUSED_RESULT(abs_path(path, absolute, sizeof(absolute)));
		set_option(options, option_name, absolute);
	}
}


#ifdef USE_LUA

#include "civetweb_lua.h"
#include "civetweb_private_lua.h"

static int
run_lua(const char *file_name)
{
	struct lua_State *L;
	int lua_ret;
	int func_ret = EXIT_FAILURE;
	const char *lua_err_txt;

#ifdef WIN32
	(void)MakeConsole();
#endif

	L = luaL_newstate();
	if (L == NULL) {
		fprintf(stderr, "Error: Cannot create Lua state\n");
		return EXIT_FAILURE;
	}
	civetweb_open_lua_libs(L);

	lua_ret = luaL_loadfile(L, file_name);
	if (lua_ret != LUA_OK) {
		/* Error when loading the file (e.g. file not found, out of memory, ...)
		 */
		lua_err_txt = lua_tostring(L, -1);
		fprintf(stderr, "Error loading file %s: %s\n", file_name, lua_err_txt);
	} else {
		/* The script file is loaded, now call it */
		lua_ret = lua_pcall(L,
		                    /* no arguments */ 0,
		                    /* zero or one return value */ 1,
		                    /* errors as strint return value */ 0);
		if (lua_ret != LUA_OK) {
			/* Error when executing the script */
			lua_err_txt = lua_tostring(L, -1);
			fprintf(stderr,
			        "Error running file %s: %s\n",
			        file_name,
			        lua_err_txt);
		} else {
			/* Script executed */
			if (lua_type(L, -1) == LUA_TNUMBER) {
				func_ret = (int)lua_tonumber(L, -1);
			} else {
				func_ret = EXIT_SUCCESS;
			}
		}
	}
	lua_close(L);

	return func_ret;
}
#endif


#ifdef USE_DUKTAPE

#include "duktape.h"

static int
run_duktape(const char *file_name)
{
	duk_context *ctx = NULL;

#ifdef WIN32
	(void)MakeConsole();
#endif

	ctx = duk_create_heap_default();
	if (!ctx) {
		fprintf(stderr, "Failed to create a Duktape heap.\n");
		goto finished;
	}

	if (duk_peval_file(ctx, file_name) != 0) {
		fprintf(stderr, "%s\n", duk_safe_to_string(ctx, -1));
		goto finished;
	}
	duk_pop(ctx); /* ignore result */

finished:
	duk_destroy_heap(ctx);

	return 0;
}
#endif


#if defined(__MINGW32__) || defined(__MINGW64__)
/* For __MINGW32/64_MAJOR/MINOR_VERSION define */
#include <_mingw.h>
#endif


static void
start_civetweb(int argc, char *argv[])
{
	struct mg_callbacks callbacks;
	char *options[2 * MAX_OPTIONS + 1];
	int i;

	/* Start option -I:
	 * Show system information and exit
	 * This is very useful for diagnosis. */
	if (argc > 1 && !strcmp(argv[1], "-I")) {
		const char *version = mg_version();
#if defined(_WIN32)
#if !defined(__SYMBIAN32__)
		DWORD dwVersion = 0;
		DWORD dwMajorVersion = 0;
		DWORD dwMinorVersion = 0;
		SYSTEM_INFO si;

		GetSystemInfo(&si);

#ifdef _MSC_VER
#pragma warning(push)
// GetVersion was declared deprecated
#pragma warning(disable : 4996)
#endif
		dwVersion = GetVersion();
#ifdef _MSC_VER
#pragma warning(pop)
#endif

		dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
		dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

		(void)MakeConsole();
		fprintf(stdout, "\n%s\n", g_server_name);
		fprintf(stdout,
		        "%s - Windows %u.%u\n",
		        g_server_base_name,
		        (unsigned)dwMajorVersion,
		        (unsigned)dwMinorVersion);

		fprintf(stdout,
		        "CPU: type %u, cores %u, mask %x\n",
		        (unsigned)si.wProcessorArchitecture,
		        (unsigned)si.dwNumberOfProcessors,
		        (unsigned)si.dwActiveProcessorMask);

#else
		fprintf(stdout, "\n%s\n", g_server_name);
		fprintf(stdout, "%s - Symbian\n", g_server_base_name);
#endif
#else
		struct utsname name;
		memset(&name, 0, sizeof(name));
		uname(&name);
		fprintf(stdout, "\n%s\n", g_server_name);
		fprintf(stdout,
		        "%s - %s %s (%s) - %s\n",
		        g_server_base_name,
		        name.sysname,
		        name.version,
		        name.release,
		        name.machine);
#endif

		fprintf(stdout, "Features:");
		if (mg_check_feature(1)) {
			fprintf(stdout, " Files");
		}
		if (mg_check_feature(2)) {
			fprintf(stdout, " HTTPS");
		}
		if (mg_check_feature(4)) {
			fprintf(stdout, " CGI");
		}
		if (mg_check_feature(8)) {
			fprintf(stdout, " IPv6");
		}
		if (mg_check_feature(16)) {
			fprintf(stdout, " WebSockets");
		}
		if (mg_check_feature(32)) {
			fprintf(stdout, " Lua");
		}
		fprintf(stdout, "\n");

#ifdef USE_LUA
		fprintf(stdout,
		        "Lua Version: %u (%s)\n",
		        (unsigned)LUA_VERSION_NUM,
		        LUA_RELEASE);
#endif

		fprintf(stdout, "Version: %s\n", version);

		fprintf(stdout, "Build: %s\n", __DATE__);

/* http://sourceforge.net/p/predef/wiki/Compilers/ */
#if defined(_MSC_VER)
		fprintf(stdout,
		        "MSC: %u (%u)\n",
		        (unsigned)_MSC_VER,
		        (unsigned)_MSC_FULL_VER);
#elif defined(__MINGW64__)
		fprintf(stdout,
		        "MinGW64: %u.%u\n",
		        (unsigned)__MINGW64_VERSION_MAJOR,
		        (unsigned)__MINGW64_VERSION_MINOR);
		fprintf(stdout,
		        "MinGW32: %u.%u\n",
		        (unsigned)__MINGW32_MAJOR_VERSION,
		        (unsigned)__MINGW32_MINOR_VERSION);
#elif defined(__MINGW32__)
		fprintf(stdout,
		        "MinGW32: %u.%u\n",
		        (unsigned)__MINGW32_MAJOR_VERSION,
		        (unsigned)__MINGW32_MINOR_VERSION);
#elif defined(__clang__)
		fprintf(stdout,
		        "clang: %u.%u.%u (%s)\n",
		        __clang_major__,
		        __clang_minor__,
		        __clang_patchlevel__,
		        __clang_version__);
#elif defined(__GNUC__)
		fprintf(stdout,
		        "gcc: %u.%u.%u\n",
		        (unsigned)__GNUC__,
		        (unsigned)__GNUC_MINOR__,
		        (unsigned)__GNUC_PATCHLEVEL__);
#elif defined(__INTEL_COMPILER)
		fprintf(stdout, "Intel C/C++: %u\n", (unsigned)__INTEL_COMPILER);
#elif defined(__BORLANDC__)
		fprintf(stdout, "Borland C: 0x%x\n", (unsigned)__BORLANDC__);
#elif defined(__SUNPRO_C)
		fprintf(stdout, "Solaris: 0x%x\n", (unsigned)__SUNPRO_C);
#else
		fprintf(stdout, "Other\n");
#endif
		/* Determine 32/64 bit data mode.
		 * see https://en.wikipedia.org/wiki/64-bit_computing */
		fprintf(stdout,
		        "Data model: i:%u/%u/%u/%u, f:%u/%u/%u, c:%u/%u, "
		        "p:%u, s:%u, t:%u\n",
		        (unsigned)sizeof(short),
		        (unsigned)sizeof(int),
		        (unsigned)sizeof(long),
		        (unsigned)sizeof(long long),
		        (unsigned)sizeof(float),
		        (unsigned)sizeof(double),
		        (unsigned)sizeof(long double),
		        (unsigned)sizeof(char),
		        (unsigned)sizeof(wchar_t),
		        (unsigned)sizeof(void *),
		        (unsigned)sizeof(size_t),
		        (unsigned)sizeof(time_t));

		exit(EXIT_SUCCESS);
	}

	/* Edit passwords file: Add user or change password, if -A option is
	 * specified */
	if (argc > 1 && !strcmp(argv[1], "-A")) {
		if (argc != 6) {
			show_usage_and_exit(argv[0]);
		}
		exit(mg_modify_passwords_file(argv[2], argv[3], argv[4], argv[5])
		         ? EXIT_SUCCESS
		         : EXIT_FAILURE);
	}

	/* Edit passwords file: Remove user, if -R option is specified */
	if (argc > 1 && !strcmp(argv[1], "-R")) {
		if (argc != 5) {
			show_usage_and_exit(argv[0]);
		}
		exit(mg_modify_passwords_file(argv[2], argv[3], argv[4], NULL)
		         ? EXIT_SUCCESS
		         : EXIT_FAILURE);
	}

	/* Call Lua with additional CivetWeb specific Lua functions, if -L option
	 * is specified */
	if (argc > 1 && !strcmp(argv[1], "-L")) {

#ifdef USE_LUA
		if (argc != 3) {
			show_usage_and_exit(argv[0]);
		}
		exit(run_lua(argv[2]));
#else
		show_server_name();
		fprintf(stderr, "\nError: Lua support not enabled\n");
		exit(EXIT_FAILURE);
#endif
	}

	/* Call Duktape, if -E option is specified */
	if (argc > 1 && !strcmp(argv[1], "-E")) {

#ifdef USE_DUKTAPE
		if (argc != 3) {
			show_usage_and_exit(argv[0]);
		}
		exit(run_duktape(argv[2]));
#else
		show_server_name();
		fprintf(stderr, "\nError: Ecmascript support not enabled\n");
		exit(EXIT_FAILURE);
#endif
	}

	/* Show usage if -h or --help options are specified */
	if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-H")
	                  || !strcmp(argv[1], "--help"))) {
		show_usage_and_exit(argv[0]);
	}

	options[0] = NULL;
	set_option(options, "document_root", ".");

	/* Update config based on command line arguments */
	process_command_line_arguments(argc, argv, options);

	/* Make sure we have absolute paths for files and directories */
	set_absolute_path(options, "document_root", argv[0]);
	set_absolute_path(options, "put_delete_auth_file", argv[0]);
	set_absolute_path(options, "cgi_interpreter", argv[0]);
	set_absolute_path(options, "access_log_file", argv[0]);
	set_absolute_path(options, "error_log_file", argv[0]);
	set_absolute_path(options, "global_auth_file", argv[0]);
#ifdef USE_LUA
	set_absolute_path(options, "lua_preload_file", argv[0]);
#endif
	set_absolute_path(options, "ssl_certificate", argv[0]);

	/* Make extra verification for certain options */
	verify_existence(options, "document_root", 1);
	verify_existence(options, "cgi_interpreter", 0);
	verify_existence(options, "ssl_certificate", 0);
	verify_existence(options, "ssl_ca_path", 1);
	verify_existence(options, "ssl_ca_file", 0);
#ifdef USE_LUA
	verify_existence(options, "lua_preload_file", 0);
#endif

	/* Setup signal handler: quit on Ctrl-C */
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);

	/* Initialize user data */
	memset(&g_user_data, 0, sizeof(g_user_data));

	/* Start Civetweb */
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.log_message = &log_message;
	g_ctx = mg_start(&callbacks, &g_user_data, (const char **)options);

	/* mg_start copies all options to an internal buffer.
	 * The options data field here is not required anymore. */
	for (i = 0; options[i] != NULL; i++) {
		free(options[i]);
	}

	/* If mg_start fails, it returns NULL */
	if (g_ctx == NULL) {
		die("Failed to start %s:\n%s",
		    g_server_name,
		    ((g_user_data.first_message == NULL) ? "unknown reason"
		                                         : g_user_data.first_message));
	}
}


static void
stop_civetweb(void)
{
	mg_stop(g_ctx);
	free(g_user_data.first_message);
	g_user_data.first_message = NULL;
}


#ifdef _WIN32
/* Win32 has a small GUI.
 * Define some GUI elements and Windows message handlers. */

enum {
	ID_ICON = 100,
	ID_QUIT,
	ID_SETTINGS,
	ID_SEPARATOR,
	ID_INSTALL_SERVICE,
	ID_REMOVE_SERVICE,
	ID_STATIC,
	ID_GROUP,
	ID_PASSWORD,
	ID_SAVE,
	ID_RESET_DEFAULTS,
	ID_RESET_FILE,
	ID_RESET_ACTIVE,
	ID_STATUS,
	ID_CONNECT,
	ID_ADD_USER,
	ID_ADD_USER_NAME,
	ID_ADD_USER_REALM,
	ID_INPUT_LINE,

	/* All dynamically created text boxes for options have IDs starting from
   ID_CONTROLS, incremented by one. */
	ID_CONTROLS = 200,

	/* Text boxes for files have "..." buttons to open file browser. These
   buttons have IDs that are ID_FILE_BUTTONS_DELTA higher than associated
   text box ID. */
	ID_FILE_BUTTONS_DELTA = 1000
};


static HICON hIcon;
static SERVICE_STATUS ss;
static SERVICE_STATUS_HANDLE hStatus;
static const char *service_magic_argument = "--";
static NOTIFYICONDATA TrayIcon;


static void WINAPI
ControlHandler(DWORD code)
{
	if (code == SERVICE_CONTROL_STOP || code == SERVICE_CONTROL_SHUTDOWN) {
		ss.dwWin32ExitCode = 0;
		ss.dwCurrentState = SERVICE_STOPPED;
	}
	SetServiceStatus(hStatus, &ss);
}


static void WINAPI
ServiceMain(void)
{
	ss.dwServiceType = SERVICE_WIN32;
	ss.dwCurrentState = SERVICE_RUNNING;
	ss.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

	hStatus = RegisterServiceCtrlHandler(g_server_name, ControlHandler);
	SetServiceStatus(hStatus, &ss);

	while (ss.dwCurrentState == SERVICE_RUNNING) {
		Sleep(1000);
	}
	stop_civetweb();

	ss.dwCurrentState = SERVICE_STOPPED;
	ss.dwWin32ExitCode = (DWORD)-1;
	SetServiceStatus(hStatus, &ss);
}


static void
show_error(void)
{
	char buf[256];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	              NULL,
	              GetLastError(),
	              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	              buf,
	              sizeof(buf),
	              NULL);
	MessageBox(NULL, buf, "Error", MB_OK);
}


static void *
align(void *ptr, uintptr_t alig)
{
	uintptr_t ul = (uintptr_t)ptr;
	ul += alig;
	ul &= ~alig;
	return ((void *)ul);
}


static void
save_config(HWND hDlg, FILE *fp)
{
	char value[2000] = "";
	const char *default_value;
	const struct mg_option *options;
	int i, id;

	fprintf(fp, "%s", config_file_top_comment);
	options = mg_get_valid_options();
	for (i = 0; options[i].name != NULL; i++) {
		id = ID_CONTROLS + i;
		if (options[i].type == CONFIG_TYPE_BOOLEAN) {
			snprintf(value,
			         sizeof(value) - 1,
			         "%s",
			         IsDlgButtonChecked(hDlg, id) ? "yes" : "no");
			value[sizeof(value) - 1] = 0;
		} else {
			GetDlgItemText(hDlg, id, value, sizeof(value));
		}
		default_value =
		    options[i].default_value == NULL ? "" : options[i].default_value;
		/* If value is the same as default, skip it */
		if (strcmp(value, default_value) != 0) {
			fprintf(fp, "%s %s\n", options[i].name, value);
		}
	}
}


static INT_PTR CALLBACK
SettingsDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	FILE *fp;
	int i, j;
	const char *name, *value;
	const struct mg_option *default_options = mg_get_valid_options();
	char *file_options[MAX_OPTIONS * 2 + 1] = {0};
	char *title;
	(void)lParam;

	switch (msg) {

	case WM_CLOSE:
		DestroyWindow(hDlg);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case ID_SAVE:
			EnableWindow(GetDlgItem(hDlg, ID_SAVE), FALSE);
			if ((fp = fopen(g_config_file_name, "w+")) != NULL) {
				save_config(hDlg, fp);
				fclose(fp);
				stop_civetweb();
				start_civetweb(__argc, __argv);
			}
			EnableWindow(GetDlgItem(hDlg, ID_SAVE), TRUE);
			break;

		case ID_RESET_DEFAULTS:
			for (i = 0; default_options[i].name != NULL; i++) {
				name = default_options[i].name;
				value = default_options[i].default_value == NULL
				            ? ""
				            : default_options[i].default_value;
				if (default_options[i].type == CONFIG_TYPE_BOOLEAN) {
					CheckDlgButton(hDlg,
					               ID_CONTROLS + i,
					               !strcmp(value, "yes") ? BST_CHECKED
					                                     : BST_UNCHECKED);
				} else {
					SetWindowText(GetDlgItem(hDlg, ID_CONTROLS + i), value);
				}
			}
			break;

		case ID_RESET_FILE:
			read_config_file(g_config_file_name, file_options);
			for (i = 0; default_options[i].name != NULL; i++) {
				name = default_options[i].name;
				value = default_options[i].default_value;
				for (j = 0; file_options[j * 2] != NULL; j++) {
					if (!strcmp(name, file_options[j * 2])) {
						value = file_options[j * 2 + 1];
					}
				}
				if (value == NULL) {
					value = "";
				}
				if (default_options[i].type == CONFIG_TYPE_BOOLEAN) {
					CheckDlgButton(hDlg,
					               ID_CONTROLS + i,
					               !strcmp(value, "yes") ? BST_CHECKED
					                                     : BST_UNCHECKED);
				} else {
					SetWindowText(GetDlgItem(hDlg, ID_CONTROLS + i), value);
				}
			}
			for (i = 0; i < MAX_OPTIONS; i++) {
				free(file_options[2 * i]);
				free(file_options[2 * i + 1]);
			}
			break;

		case ID_RESET_ACTIVE:
			for (i = 0; default_options[i].name != NULL; i++) {
				name = default_options[i].name;
				value = mg_get_option(g_ctx, name);
				if (default_options[i].type == CONFIG_TYPE_BOOLEAN) {
					CheckDlgButton(hDlg,
					               ID_CONTROLS + i,
					               !strcmp(value, "yes") ? BST_CHECKED
					                                     : BST_UNCHECKED);
				} else {
					SetDlgItemText(hDlg,
					               ID_CONTROLS + i,
					               value == NULL ? "" : value);
				}
			}
			break;
		}

		for (i = 0; default_options[i].name != NULL; i++) {
			name = default_options[i].name;
			if (((default_options[i].type == CONFIG_TYPE_FILE)
			     || (default_options[i].type == CONFIG_TYPE_DIRECTORY))
			    && LOWORD(wParam) == ID_CONTROLS + i + ID_FILE_BUTTONS_DELTA) {
				OPENFILENAME of;
				BROWSEINFO bi;
				char path[PATH_MAX] = "";

				memset(&of, 0, sizeof(of));
				of.lStructSize = sizeof(of);
				of.hwndOwner = (HWND)hDlg;
				of.lpstrFile = path;
				of.nMaxFile = sizeof(path);
				of.lpstrInitialDir = mg_get_option(g_ctx, "document_root");
				of.Flags =
				    OFN_CREATEPROMPT | OFN_NOCHANGEDIR | OFN_HIDEREADONLY;

				memset(&bi, 0, sizeof(bi));
				bi.hwndOwner = (HWND)hDlg;
				bi.lpszTitle = "Choose WWW root directory:";
				bi.ulFlags = BIF_RETURNONLYFSDIRS;

				if (default_options[i].type == CONFIG_TYPE_DIRECTORY) {
					SHGetPathFromIDList(SHBrowseForFolder(&bi), path);
				} else {
					GetOpenFileName(&of);
				}

				if (path[0] != '\0') {
					SetWindowText(GetDlgItem(hDlg, ID_CONTROLS + i), path);
				}
			}
		}
		break;

	case WM_INITDIALOG:
		/* Store hWnd in a parameter accessible by the parent, so we can
		 * bring this window to front if required. */
		*((HWND *)lParam) = hDlg;
		/* Initialize the dialog elements */
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
		title = malloc(strlen(g_server_name) + 16);
		if (title) {
			strcpy(title, g_server_name);
			strcat(title, " settings");
			SetWindowText(hDlg, title);
			free(title);
		}
		SetFocus(GetDlgItem(hDlg, ID_SAVE));

		/* Init dialog with active settings */
		SendMessage(hDlg, WM_COMMAND, ID_RESET_ACTIVE, 0);
		/* alternative: SendMessage(hDlg, WM_COMMAND, ID_RESET_FILE, 0); */
		break;

	default:
		break;
	}

	return FALSE;
}


struct tstring_input_buf {
	unsigned buflen;
	char *buffer;
};


static INT_PTR CALLBACK
InputDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lP)
{
	static struct tstring_input_buf *inBuf = 0;
	WORD ctrlId;

	switch (msg) {
	case WM_CLOSE:
		inBuf = 0;
		DestroyWindow(hDlg);
		break;

	case WM_COMMAND:
		ctrlId = LOWORD(wParam);
		if (ctrlId == IDOK) {
			/* Add user */
			GetWindowText(GetDlgItem(hDlg, ID_INPUT_LINE),
			              inBuf->buffer,
			              (int)inBuf->buflen);
			if (strlen(inBuf->buffer) > 0) {
				EndDialog(hDlg, IDOK);
			}
		} else if (ctrlId == IDCANCEL) {
			EndDialog(hDlg, IDCANCEL);
		}
		break;

	case WM_INITDIALOG:
		inBuf = (struct tstring_input_buf *)lP;
		assert(inBuf != NULL);
		assert((inBuf->buffer != NULL) && (inBuf->buflen != 0));
		assert(strlen(inBuf->buffer) < inBuf->buflen);
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
		SendDlgItemMessage(
		    hDlg, ID_INPUT_LINE, EM_LIMITTEXT, inBuf->buflen - 1, 0);
		SetWindowText(GetDlgItem(hDlg, ID_INPUT_LINE), inBuf->buffer);
		SetWindowText(hDlg, "Modify password");
		SetFocus(GetDlgItem(hDlg, ID_INPUT_LINE));
		break;

	default:
		break;
	}

	return FALSE;
}


static void
suggest_passwd(char *passwd)
{
	unsigned u;
	char *p;
	union {
		FILETIME ft;
		LARGE_INTEGER li;
	} num;

	/* valid characters are 32 to 126 */
	GetSystemTimeAsFileTime(&num.ft);
	num.li.HighPart |= (LONG)GetCurrentProcessId();
	p = passwd;
	while (num.li.QuadPart) {
		u = (unsigned)(num.li.QuadPart % 95);
		num.li.QuadPart -= u;
		num.li.QuadPart /= 95;
		*p = (char)(u + 32);
		p++;
	}
}


static void add_control(unsigned char **mem,
                        DLGTEMPLATE *dia,
                        WORD type,
                        WORD id,
                        DWORD style,
                        short x,
                        short y,
                        short cx,
                        short cy,
                        const char *caption);


static int
get_password(const char *user,
             const char *realm,
             char *passwd,
             unsigned passwd_len)
{
#define HEIGHT (15)
#define WIDTH (280)
#define LABEL_WIDTH (90)

	unsigned char mem[4096], *p;
	DLGTEMPLATE *dia = (DLGTEMPLATE *)mem;
	int ok;
	short y;
	struct tstring_input_buf dlgprms;

	static struct {
		DLGTEMPLATE template; /* 18 bytes */
		WORD menu, class;
		wchar_t caption[1];
		WORD fontsiz;
		wchar_t fontface[7];
	} dialog_header = {{WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_VISIBLE
	                        | DS_SETFONT | WS_DLGFRAME,
	                    WS_EX_TOOLWINDOW,
	                    0,
	                    200,
	                    200,
	                    WIDTH,
	                    0},
	                   0,
	                   0,
	                   L"",
	                   8,
	                   L"Tahoma"};

	dlgprms.buffer = passwd;
	dlgprms.buflen = passwd_len;

	assert((user != NULL) && (realm != NULL) && (passwd != NULL));

	if (sGuard < 100) {
		sGuard += 100;
	} else {
		return 0;
	}

	/* Create a password suggestion */
	memset(passwd, 0, passwd_len);
	suggest_passwd(passwd);

	/* Create the dialog */
	(void)memset(mem, 0, sizeof(mem));
	(void)memcpy(mem, &dialog_header, sizeof(dialog_header));
	p = mem + sizeof(dialog_header);

	y = HEIGHT;
	add_control(&p,
	            dia,
	            0x82,
	            ID_STATIC,
	            WS_VISIBLE | WS_CHILD,
	            10,
	            y,
	            LABEL_WIDTH,
	            HEIGHT,
	            "User:");
	add_control(&p,
	            dia,
	            0x81,
	            ID_CONTROLS + 1,
	            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
	                | WS_DISABLED,
	            15 + LABEL_WIDTH,
	            y,
	            WIDTH - LABEL_WIDTH - 25,
	            HEIGHT,
	            user);

	y += HEIGHT;
	add_control(&p,
	            dia,
	            0x82,
	            ID_STATIC,
	            WS_VISIBLE | WS_CHILD,
	            10,
	            y,
	            LABEL_WIDTH,
	            HEIGHT,
	            "Realm:");
	add_control(&p,
	            dia,
	            0x81,
	            ID_CONTROLS + 2,
	            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
	                | WS_DISABLED,
	            15 + LABEL_WIDTH,
	            y,
	            WIDTH - LABEL_WIDTH - 25,
	            HEIGHT,
	            realm);

	y += HEIGHT;
	add_control(&p,
	            dia,
	            0x82,
	            ID_STATIC,
	            WS_VISIBLE | WS_CHILD,
	            10,
	            y,
	            LABEL_WIDTH,
	            HEIGHT,
	            "Password:");
	add_control(&p,
	            dia,
	            0x81,
	            ID_INPUT_LINE,
	            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_TABSTOP,
	            15 + LABEL_WIDTH,
	            y,
	            WIDTH - LABEL_WIDTH - 25,
	            HEIGHT,
	            "");

	y += (WORD)(HEIGHT * 2);
	add_control(&p,
	            dia,
	            0x80,
	            IDOK,
	            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
	            80,
	            y,
	            55,
	            12,
	            "Ok");
	add_control(&p,
	            dia,
	            0x80,
	            IDCANCEL,
	            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
	            140,
	            y,
	            55,
	            12,
	            "Cancel");

	assert((intptr_t)p - (intptr_t)mem < (intptr_t)sizeof(mem));

	dia->cy = y + (WORD)(HEIGHT * 1.5);

	ok = (IDOK == DialogBoxIndirectParam(
	                  NULL, dia, NULL, InputDlgProc, (LPARAM)&dlgprms));

	sGuard -= 100;

	return ok;

#undef HEIGHT
#undef WIDTH
#undef LABEL_WIDTH
}


static INT_PTR CALLBACK
PasswordDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lP)
{
	static const char *passfile = 0;
	char domain[256], user[256], password[256];
	WORD ctrlId;

	switch (msg) {
	case WM_CLOSE:
		passfile = 0;
		DestroyWindow(hDlg);
		break;

	case WM_COMMAND:
		ctrlId = LOWORD(wParam);
		if (ctrlId == ID_ADD_USER) {
			/* Add user */
			GetWindowText(GetDlgItem(hDlg, ID_ADD_USER_NAME),
			              user,
			              sizeof(user));
			GetWindowText(GetDlgItem(hDlg, ID_ADD_USER_REALM),
			              domain,
			              sizeof(domain));
			if (get_password(user, domain, password, sizeof(password))) {
				mg_modify_passwords_file(passfile, domain, user, password);
				EndDialog(hDlg, IDOK);
			}
		} else if ((ctrlId >= (ID_CONTROLS + ID_FILE_BUTTONS_DELTA * 3))
		           && (ctrlId < (ID_CONTROLS + ID_FILE_BUTTONS_DELTA * 4))) {
			/* Modify password */
			GetWindowText(GetDlgItem(hDlg, ctrlId - ID_FILE_BUTTONS_DELTA * 3),
			              user,
			              sizeof(user));
			GetWindowText(GetDlgItem(hDlg, ctrlId - ID_FILE_BUTTONS_DELTA * 2),
			              domain,
			              sizeof(domain));
			if (get_password(user, domain, password, sizeof(password))) {
				mg_modify_passwords_file(passfile, domain, user, password);
				EndDialog(hDlg, IDOK);
			}
		} else if ((ctrlId >= (ID_CONTROLS + ID_FILE_BUTTONS_DELTA * 2))
		           && (ctrlId < (ID_CONTROLS + ID_FILE_BUTTONS_DELTA * 3))) {
			/* Remove user */
			GetWindowText(GetDlgItem(hDlg, ctrlId - ID_FILE_BUTTONS_DELTA * 2),
			              user,
			              sizeof(user));
			GetWindowText(GetDlgItem(hDlg, ctrlId - ID_FILE_BUTTONS_DELTA),
			              domain,
			              sizeof(domain));
			mg_modify_passwords_file(passfile, domain, user, NULL);
			EndDialog(hDlg, IDOK);
		}
		break;

	case WM_INITDIALOG:
		passfile = (const char *)lP;
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
		SetWindowText(hDlg, passfile);
		SetFocus(GetDlgItem(hDlg, ID_ADD_USER_NAME));
		break;

	default:
		break;
	}

	return FALSE;
}


static void
add_control(unsigned char **mem,
            DLGTEMPLATE *dia,
            WORD type,
            WORD id,
            DWORD style,
            short x,
            short y,
            short cx,
            short cy,
            const char *caption)
{
	DLGITEMTEMPLATE *tp;
	LPWORD p;

	dia->cdit++;

	*mem = align(*mem, 3);
	tp = (DLGITEMTEMPLATE *)*mem;

	tp->id = id;
	tp->style = style;
	tp->dwExtendedStyle = 0;
	tp->x = x;
	tp->y = y;
	tp->cx = cx;
	tp->cy = cy;

	p = align(*mem + sizeof(*tp), 1);
	*p++ = 0xffff;
	*p++ = type;

	while (*caption != '\0') {
		*p++ = (WCHAR)*caption++;
	}
	*p++ = 0;
	p = align(p, 1);

	*p++ = 0;
	*mem = (unsigned char *)p;
}


static void
show_settings_dialog()
{
#define HEIGHT (15)
#define WIDTH (460)
#define LABEL_WIDTH (90)

	unsigned char mem[16 * 1024], *p;
	const struct mg_option *options;
	DWORD style;
	DLGTEMPLATE *dia = (DLGTEMPLATE *)mem;
	WORD i, cl, nelems = 0;
	short width, x, y;
	static HWND sDlgHWnd;

	static struct {
		DLGTEMPLATE template; /* 18 bytes */
		WORD menu, class;
		wchar_t caption[1];
		WORD fontsiz;
		wchar_t fontface[7];
	} dialog_header = {{WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_VISIBLE
	                        | DS_SETFONT | WS_DLGFRAME,
	                    WS_EX_TOOLWINDOW,
	                    0,
	                    200,
	                    200,
	                    WIDTH,
	                    0},
	                   0,
	                   0,
	                   L"",
	                   8,
	                   L"Tahoma"};

	if (sGuard == 0) {
		sGuard++;
	} else {
		SetForegroundWindow(sDlgHWnd);
		return;
	}

	(void)memset(mem, 0, sizeof(mem));
	(void)memcpy(mem, &dialog_header, sizeof(dialog_header));
	p = mem + sizeof(dialog_header);

	options = mg_get_valid_options();
	for (i = 0; options[i].name != NULL; i++) {
		style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
		x = 10 + (WIDTH / 2) * (nelems % 2);
		y = (nelems / 2 + 1) * HEIGHT + 5;
		width = WIDTH / 2 - 20 - LABEL_WIDTH;
		if (options[i].type == CONFIG_TYPE_NUMBER) {
			style |= ES_NUMBER;
			cl = 0x81;
			style |= WS_BORDER | ES_AUTOHSCROLL;
		} else if (options[i].type == CONFIG_TYPE_BOOLEAN) {
			cl = 0x80;
			style |= BS_AUTOCHECKBOX;
		} else if ((options[i].type == CONFIG_TYPE_FILE)
		           || (options[i].type == CONFIG_TYPE_DIRECTORY)) {
			style |= WS_BORDER | ES_AUTOHSCROLL;
			width -= 20;
			cl = 0x81;
			add_control(&p,
			            dia,
			            0x80,
			            ID_CONTROLS + i + ID_FILE_BUTTONS_DELTA,
			            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			            x + width + LABEL_WIDTH + 5,
			            y,
			            15,
			            12,
			            "...");
		} else {
			cl = 0x81;
			style |= WS_BORDER | ES_AUTOHSCROLL;
		}
		add_control(&p,
		            dia,
		            0x82,
		            ID_STATIC,
		            WS_VISIBLE | WS_CHILD,
		            x,
		            y,
		            LABEL_WIDTH,
		            HEIGHT,
		            options[i].name);
		add_control(&p,
		            dia,
		            cl,
		            ID_CONTROLS + i,
		            style,
		            x + LABEL_WIDTH,
		            y,
		            width,
		            12,
		            "");
		nelems++;

		assert(((intptr_t)p - (intptr_t)mem) < (intptr_t)sizeof(mem));
	}

	y = (((nelems + 1) / 2 + 1) * HEIGHT + 5);
	add_control(&p,
	            dia,
	            0x80,
	            ID_GROUP,
	            WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
	            5,
	            5,
	            WIDTH - 10,
	            y,
	            " Settings ");
	y += 10;
	add_control(&p,
	            dia,
	            0x80,
	            ID_SAVE,
	            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
	            WIDTH - 70,
	            y,
	            65,
	            12,
	            "Save Settings");
	add_control(&p,
	            dia,
	            0x80,
	            ID_RESET_DEFAULTS,
	            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
	            WIDTH - 140,
	            y,
	            65,
	            12,
	            "Reset to defaults");
	add_control(&p,
	            dia,
	            0x80,
	            ID_RESET_FILE,
	            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
	            WIDTH - 210,
	            y,
	            65,
	            12,
	            "Reload from file");
	add_control(&p,
	            dia,
	            0x80,
	            ID_RESET_ACTIVE,
	            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
	            WIDTH - 280,
	            y,
	            65,
	            12,
	            "Reload active");
	add_control(&p,
	            dia,
	            0x82,
	            ID_STATIC,
	            WS_CHILD | WS_VISIBLE | WS_DISABLED,
	            5,
	            y,
	            100,
	            12,
	            g_server_base_name);

	assert(((intptr_t)p - (intptr_t)mem) < (intptr_t)sizeof(mem));

	dia->cy = ((nelems + 1) / 2 + 1) * HEIGHT + 30;
	DialogBoxIndirectParam(NULL, dia, NULL, SettingsDlgProc, (LPARAM)&sDlgHWnd);
	sGuard--;
	sDlgHWnd = NULL;

#undef HEIGHT
#undef WIDTH
#undef LABEL_WIDTH
}


static void
change_password_file()
{
#define HEIGHT (15)
#define WIDTH (320)
#define LABEL_WIDTH (90)

	OPENFILENAME of;
	char path[PATH_MAX] = PASSWORDS_FILE_NAME;
	char strbuf[256], u[256], d[256];
	HWND hDlg = NULL;
	FILE *f;
	short y, nelems;
	unsigned char mem[4096], *p;
	DLGTEMPLATE *dia = (DLGTEMPLATE *)mem;
	const char *domain = mg_get_option(g_ctx, "authentication_domain");

	static struct {
		DLGTEMPLATE template; /* 18 bytes */
		WORD menu, class;
		wchar_t caption[1];
		WORD fontsiz;
		wchar_t fontface[7];
	} dialog_header = {{WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_VISIBLE
	                        | DS_SETFONT | WS_DLGFRAME,
	                    WS_EX_TOOLWINDOW,
	                    0,
	                    200,
	                    200,
	                    WIDTH,
	                    0},
	                   0,
	                   0,
	                   L"",
	                   8,
	                   L"Tahoma"};

	if (sGuard == 0) {
		sGuard++;
	} else {
		return;
	}

	memset(&of, 0, sizeof(of));
	of.lStructSize = sizeof(of);
	of.hwndOwner = (HWND)hDlg;
	of.lpstrFile = path;
	of.nMaxFile = sizeof(path);
	of.lpstrInitialDir = mg_get_option(g_ctx, "document_root");
	of.Flags = OFN_CREATEPROMPT | OFN_NOCHANGEDIR | OFN_HIDEREADONLY;

	if (IDOK != GetSaveFileName(&of)) {
		sGuard--;
		return;
	}

	f = fopen(path, "a+");
	if (f) {
		fclose(f);
	} else {
		MessageBox(NULL, path, "Can not open file", MB_ICONERROR);
		sGuard--;
		return;
	}

	do {
		(void)memset(mem, 0, sizeof(mem));
		(void)memcpy(mem, &dialog_header, sizeof(dialog_header));
		p = mem + sizeof(dialog_header);

		f = fopen(path, "r+");
		if (!f) {
			MessageBox(NULL, path, "Can not open file", MB_ICONERROR);
			sGuard--;
			return;
		}

		nelems = 0;
		while (fgets(strbuf, sizeof(strbuf), f)) {
			if (sscanf(strbuf, "%255[^:]:%255[^:]:%*s", u, d) != 2) {
				continue;
			}
			u[255] = 0;
			d[255] = 0;
			y = (nelems + 1) * HEIGHT + 5;
			add_control(&p,
			            dia,
			            0x80,
			            ID_CONTROLS + nelems + ID_FILE_BUTTONS_DELTA * 3,
			            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
			            10,
			            y,
			            65,
			            12,
			            "Modify password");
			add_control(&p,
			            dia,
			            0x80,
			            ID_CONTROLS + nelems + ID_FILE_BUTTONS_DELTA * 2,
			            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
			            80,
			            y,
			            55,
			            12,
			            "Remove user");
			add_control(&p,
			            dia,
			            0x81,
			            ID_CONTROLS + nelems + ID_FILE_BUTTONS_DELTA,
			            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
			                | WS_DISABLED,
			            245,
			            y,
			            60,
			            12,
			            d);
			add_control(&p,
			            dia,
			            0x81,
			            ID_CONTROLS + nelems,
			            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
			                | WS_DISABLED,
			            140,
			            y,
			            100,
			            12,
			            u);

			nelems++;
			assert(((intptr_t)p - (intptr_t)mem) < (intptr_t)sizeof(mem));
		}
		fclose(f);

		y = (nelems + 1) * HEIGHT + 10;
		add_control(&p,
		            dia,
		            0x80,
		            ID_ADD_USER,
		            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		            80,
		            y,
		            55,
		            12,
		            "Add user");
		add_control(&p,
		            dia,
		            0x81,
		            ID_ADD_USER_NAME,
		            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
		                | WS_TABSTOP,
		            140,
		            y,
		            100,
		            12,
		            "");
		add_control(&p,
		            dia,
		            0x81,
		            ID_ADD_USER_REALM,
		            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
		                | WS_TABSTOP,
		            245,
		            y,
		            60,
		            12,
		            domain);

		y = (nelems + 2) * HEIGHT + 10;
		add_control(&p,
		            dia,
		            0x80,
		            ID_GROUP,
		            WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		            5,
		            5,
		            WIDTH - 10,
		            y,
		            " Users ");

		y += HEIGHT;
		add_control(&p,
		            dia,
		            0x82,
		            ID_STATIC,
		            WS_CHILD | WS_VISIBLE | WS_DISABLED,
		            5,
		            y,
		            100,
		            12,
		            g_server_base_name);

		assert(((intptr_t)p - (intptr_t)mem) < (intptr_t)sizeof(mem));

		dia->cy = y + 20;
	} while ((IDOK == DialogBoxIndirectParam(
	                      NULL, dia, NULL, PasswordDlgProc, (LPARAM)path))
	         && (!g_exit_flag));

	sGuard--;

#undef HEIGHT
#undef WIDTH
#undef LABEL_WIDTH
}


static int
manage_service(int action)
{
	const char *service_name = g_server_name;
	SC_HANDLE hSCM = NULL, hService = NULL;
	SERVICE_DESCRIPTION descr;
	char path[PATH_MAX + 20] = ""; /* Path to executable plus magic argument */
	int success = 1;

	descr.lpDescription = (LPSTR)g_server_name;

	if ((hSCM = OpenSCManager(NULL,
	                          NULL,
	                          action == ID_INSTALL_SERVICE ? GENERIC_WRITE
	                                                       : GENERIC_READ))
	    == NULL) {
		success = 0;
		show_error();
	} else if (action == ID_INSTALL_SERVICE) {
		path[sizeof(path) - 1] = 0;
		GetModuleFileName(NULL, path, sizeof(path) - 1);
		strncat(path, " ", sizeof(path) - 1);
		strncat(path, service_magic_argument, sizeof(path) - 1);
		hService = CreateService(hSCM,
		                         service_name,
		                         service_name,
		                         SERVICE_ALL_ACCESS,
		                         SERVICE_WIN32_OWN_PROCESS,
		                         SERVICE_AUTO_START,
		                         SERVICE_ERROR_NORMAL,
		                         path,
		                         NULL,
		                         NULL,
		                         NULL,
		                         NULL,
		                         NULL);
		if (hService) {
			ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &descr);
		} else {
			show_error();
		}
	} else if (action == ID_REMOVE_SERVICE) {
		if ((hService = OpenService(hSCM, service_name, DELETE)) == NULL
		    || !DeleteService(hService)) {
			show_error();
		}
	} else if ((hService =
	                OpenService(hSCM, service_name, SERVICE_QUERY_STATUS))
	           == NULL) {
		success = 0;
	}

	if (hService)
		CloseServiceHandle(hService);
	if (hSCM)
		CloseServiceHandle(hSCM);

	return success;
}


static LRESULT CALLBACK
WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SERVICE_TABLE_ENTRY service_table[2];
	int service_installed;
	char buf[200], *service_argv[2];
	POINT pt;
	HMENU hMenu;
	static UINT s_uTaskbarRestart; /* for taskbar creation */

	service_argv[0] = __argv[0];
	service_argv[1] = NULL;

	memset(service_table, 0, sizeof(service_table));
	service_table[0].lpServiceName = (LPSTR)g_server_name;
	service_table[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	switch (msg) {

	case WM_CREATE:
		if (__argv[1] != NULL && !strcmp(__argv[1], service_magic_argument)) {
			start_civetweb(1, service_argv);
			StartServiceCtrlDispatcher(service_table);
			exit(EXIT_SUCCESS);
		} else {
			start_civetweb(__argc, __argv);
			s_uTaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_QUIT:
			stop_civetweb();
			Shell_NotifyIcon(NIM_DELETE, &TrayIcon);
			g_exit_flag = 1;
			PostQuitMessage(0);
			return 0;
		case ID_SETTINGS:
			show_settings_dialog();
			break;
		case ID_PASSWORD:
			change_password_file();
			break;
		case ID_INSTALL_SERVICE:
		case ID_REMOVE_SERVICE:
			manage_service(LOWORD(wParam));
			break;
		case ID_CONNECT:
			fprintf(stdout, "[%s]\n", get_url_to_first_open_port(g_ctx));
			ShellExecute(NULL,
			             "open",
			             get_url_to_first_open_port(g_ctx),
			             NULL,
			             NULL,
			             SW_SHOW);
			break;
		}
		break;

	case WM_USER:
		switch (lParam) {
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
			hMenu = CreatePopupMenu();
			AppendMenu(hMenu,
			           MF_STRING | MF_GRAYED,
			           ID_SEPARATOR,
			           g_server_name);
			AppendMenu(hMenu, MF_SEPARATOR, ID_SEPARATOR, "");
			service_installed = manage_service(0);
			snprintf(buf,
			         sizeof(buf) - 1,
			         "NT service: %s installed",
			         service_installed ? "" : "not");
			buf[sizeof(buf) - 1] = 0;
			AppendMenu(hMenu, MF_STRING | MF_GRAYED, ID_SEPARATOR, buf);
			AppendMenu(hMenu,
			           MF_STRING | (service_installed ? MF_GRAYED : 0),
			           ID_INSTALL_SERVICE,
			           "Install service");
			AppendMenu(hMenu,
			           MF_STRING | (!service_installed ? MF_GRAYED : 0),
			           ID_REMOVE_SERVICE,
			           "Deinstall service");
			AppendMenu(hMenu, MF_SEPARATOR, ID_SEPARATOR, "");
			AppendMenu(hMenu, MF_STRING, ID_CONNECT, "Start browser");
			AppendMenu(hMenu, MF_STRING, ID_SETTINGS, "Edit settings");
			AppendMenu(hMenu, MF_STRING, ID_PASSWORD, "Modify password file");
			AppendMenu(hMenu, MF_SEPARATOR, ID_SEPARATOR, "");
			AppendMenu(hMenu, MF_STRING, ID_QUIT, "Exit");
			GetCursorPos(&pt);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hMenu, 0, pt.x, pt.y, 0, hWnd, NULL);
			PostMessage(hWnd, WM_NULL, 0, 0);
			DestroyMenu(hMenu);
			break;
		}
		break;

	case WM_CLOSE:
		stop_civetweb();
		Shell_NotifyIcon(NIM_DELETE, &TrayIcon);
		g_exit_flag = 1;
		PostQuitMessage(0);
		return 0; /* We've just sent our own quit message, with proper hwnd. */

	default:
		if (msg == s_uTaskbarRestart)
			Shell_NotifyIcon(NIM_ADD, &TrayIcon);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


static int
MakeConsole(void)
{
	DWORD err;
	int ok = (GetConsoleWindow() != NULL);
	if (!ok) {
		if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
			FreeConsole();
			if (!AllocConsole()) {
				err = GetLastError();
				if (err == ERROR_ACCESS_DENIED) {
					MessageBox(NULL,
					           "Insufficient rights to create a console window",
					           "Error",
					           MB_ICONERROR);
				}
			}
			AttachConsole(GetCurrentProcessId());
		}

		ok = (GetConsoleWindow() != NULL);
		if (ok) {
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);
		}
	}

	if (ok) {
		SetConsoleTitle(g_server_name);
	}

	return ok;
}


int WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdline, int show)
{
	WNDCLASS cls;
	HWND hWnd;
	MSG msg;

#if defined(DEBUG)
	(void)MakeConsole();
#endif

	(void)hInst;
	(void)hPrev;
	(void)cmdline;
	(void)show;

	init_server_name((int)__argc, (const char **)__argv);
	memset(&cls, 0, sizeof(cls));
	cls.lpfnWndProc = (WNDPROC)WindowProc;
	cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	cls.lpszClassName = g_server_base_name;

	RegisterClass(&cls);
	hWnd = CreateWindow(cls.lpszClassName,
	                    g_server_name,
	                    WS_OVERLAPPEDWINDOW,
	                    0,
	                    0,
	                    0,
	                    0,
	                    NULL,
	                    NULL,
	                    NULL,
	                    NULL);
	ShowWindow(hWnd, SW_HIDE);

	if (g_icon_name) {
		hIcon =
		    LoadImage(NULL, g_icon_name, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	} else {
		hIcon = LoadImage(GetModuleHandle(NULL),
		                  MAKEINTRESOURCE(ID_ICON),
		                  IMAGE_ICON,
		                  16,
		                  16,
		                  0);
	}

	TrayIcon.cbSize = sizeof(TrayIcon);
	TrayIcon.uID = ID_ICON;
	TrayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	TrayIcon.hIcon = hIcon;
	TrayIcon.hWnd = hWnd;
	snprintf(TrayIcon.szTip, sizeof(TrayIcon.szTip), "%s", g_server_name);
	TrayIcon.uCallbackMessage = WM_USER;
	Shell_NotifyIcon(NIM_ADD, &TrayIcon);

	while (GetMessage(&msg, hWnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/* Return the WM_QUIT value. */
	return (int)msg.wParam;
}


int
main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	return WinMain(0, 0, 0, 0);
}


#elif defined(USE_COCOA)
#import <Cocoa/Cocoa.h>

@interface Civetweb : NSObject <NSApplicationDelegate>
- (void)openBrowser;
- (void)shutDown;
@end

@implementation Civetweb
- (void)openBrowser
{
	[[NSWorkspace sharedWorkspace]
	    openURL:[NSURL URLWithString:[NSString stringWithUTF8String:
	                                               get_url_to_first_open_port(
	                                                   g_ctx)]]];
}
- (void)editConfig
{
	create_config_file(g_ctx, g_config_file_name);
	NSString *path = [NSString stringWithUTF8String:g_config_file_name];
	if (![[NSWorkspace sharedWorkspace] openFile:path
	                             withApplication:@"TextEdit"]) {
		NSAlert *alert = [[[NSAlert alloc] init] autorelease];
		[alert setAlertStyle:NSWarningAlertStyle];
		[alert setMessageText:NSLocalizedString(@"Unable to open config file.",
		                                        "")];
		[alert setInformativeText:path];
		(void)[alert runModal];
	}
}
- (void)shutDown
{
	[NSApp terminate:nil];
}
@end

int
main(int argc, char *argv[])
{
	init_server_name(argc, (const char **)argv);
	start_civetweb(argc, argv);

	[NSAutoreleasePool new];
	[NSApplication sharedApplication];

	/* Add delegate to process menu item actions */
	Civetweb *myDelegate = [[Civetweb alloc] autorelease];
	[NSApp setDelegate:myDelegate];

	/* Run this app as agent */
	ProcessSerialNumber psn = {0, kCurrentProcess};
	TransformProcessType(&psn, kProcessTransformToBackgroundApplication);
	SetFrontProcess(&psn);

	/* Add status bar menu */
	id menu = [[NSMenu new] autorelease];

	/* Add version menu item */
	[menu
	    addItem:
	        [[[NSMenuItem alloc]
	            /*initWithTitle:[NSString stringWithFormat:@"%s", server_name]*/
	            initWithTitle:[NSString stringWithUTF8String:g_server_name]
	                   action:@selector(noexist)
	            keyEquivalent:@""] autorelease]];

	/* Add configuration menu item */
	[menu addItem:[[[NSMenuItem alloc] initWithTitle:@"Edit configuration"
	                                          action:@selector(editConfig)
	                                   keyEquivalent:@""] autorelease]];

	/* Add connect menu item */
	[menu
	    addItem:[[[NSMenuItem alloc] initWithTitle:@"Open web root in a browser"
	                                        action:@selector(openBrowser)
	                                 keyEquivalent:@""] autorelease]];

	/* Separator */
	[menu addItem:[NSMenuItem separatorItem]];

	/* Add quit menu item */
	[menu addItem:[[[NSMenuItem alloc] initWithTitle:@"Quit"
	                                          action:@selector(shutDown)
	                                   keyEquivalent:@"q"] autorelease]];

	/* Attach menu to the status bar */
	id item = [[[NSStatusBar systemStatusBar]
	    statusItemWithLength:NSVariableStatusItemLength] retain];
	[item setHighlightMode:YES];
	[item setImage:[NSImage imageNamed:@"civetweb_22x22.png"]];
	[item setMenu:menu];

	/* Run the app */
	[NSApp activateIgnoringOtherApps:YES];
	[NSApp run];

	stop_civetweb();

	return EXIT_SUCCESS;
}

#else

int
main(int argc, char *argv[])
{
	init_server_name(argc, (const char **)argv);
	start_civetweb(argc, argv);
	fprintf(stdout,
	        "%s started on port(s) %s with web root [%s]\n",
	        g_server_name,
	        mg_get_option(g_ctx, "listening_ports"),
	        mg_get_option(g_ctx, "document_root"));
	while (g_exit_flag == 0) {
		sleep(1);
	}
	fprintf(stdout,
	        "Exiting on signal %d, waiting for all threads to finish...",
	        g_exit_flag);
	fflush(stdout);
	stop_civetweb();
	fprintf(stdout, "%s", " done.\n");

	return EXIT_SUCCESS;
}
#endif /* _WIN32 */
