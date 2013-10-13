<<<<<<< HEAD
// Copyright (c) 2004-2013 Sergey Lyubka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#if defined(_WIN32)
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS // Disable deprecation warning in VS2005
#endif
#else
#ifdef __linux__
#define _XOPEN_SOURCE 600     // For flockfile() on Linux
#endif
#define _LARGEFILE_SOURCE     // Enable 64-bit file offsets
#define __STDC_FORMAT_MACROS  // <inttypes.h> wants this for C++
#define __STDC_LIMIT_MACROS   // C++ wants that for INT64_MAX
#endif

#if defined (_MSC_VER)
// 'type cast' : conversion from 'int' to 'HANDLE' of greater size
#pragma warning (disable : 4306 )
// conditional expression is constant: introduced by FD_SET(..)
#pragma warning (disable : 4127)
// non-constant aggregate initializer: issued due to missing C99 support
#pragma warning (disable : 4204)
#endif

// Disable WIN32_LEAN_AND_MEAN.
// This makes windows.h always include winsock2.h
=======
/* Copyright (c) 2004-2013 Sergey Lyubka
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
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS /* Disable deprecation warning in VS2005 */
#endif
#else
#ifdef __linux__
#define _XOPEN_SOURCE 600     /* For flockfile() on Linux */
#endif
#define _LARGEFILE_SOURCE     /* Enable 64-bit file offsets */
#define __STDC_FORMAT_MACROS  /* <inttypes.h> wants this for C++ */
#define __STDC_LIMIT_MACROS   /* C++ wants that for INT64_MAX */
#endif

#if defined (_MSC_VER)
/* 'type cast' : conversion from 'int' to 'HANDLE' of greater size */
#pragma warning (disable : 4306 )
/* conditional expression is constant: introduced by FD_SET(..) */
#pragma warning (disable : 4127)
/* non-constant aggregate initializer: issued due to missing C99 support */
#pragma warning (disable : 4204)
#endif

/* Disable WIN32_LEAN_AND_MEAN.
   This makes windows.h always include winsock2.h */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#if defined(WIN32_LEAN_AND_MEAN) && (_MSC_VER <= 1400)
#undef WIN32_LEAN_AND_MEAN
#endif

#if defined USE_IPV6 && defined(_WIN32)
#include <ws2tcpip.h>
#endif

#if defined(__SYMBIAN32__)
<<<<<<< HEAD
#define NO_SSL // SSL is not supported
#define NO_CGI // CGI is not supported
#define PATH_MAX FILENAME_MAX
#endif // __SYMBIAN32__
=======
#define NO_SSL /* SSL is not supported */
#define NO_CGI /* CGI is not supported */
#define PATH_MAX FILENAME_MAX
#endif /* __SYMBIAN32__ */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

#ifndef IGNORE_UNUSED_RESULT
#define IGNORE_UNUSED_RESULT(a) (void)((a) && 1)
#endif

<<<<<<< HEAD
#ifndef _WIN32_WCE // Some ANSI #includes are not available on Windows CE
=======
#ifndef _WIN32_WCE /* Some ANSI #includes are not available on Windows CE */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
<<<<<<< HEAD
#endif // !_WIN32_WCE
=======
#endif /* !_WIN32_WCE */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

<<<<<<< HEAD
#if defined(_WIN32) && !defined(__SYMBIAN32__) // Windows specific
#if defined(_MSC_VER) && _MSC_VER <= 1400
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0400 // To make it link in VS2005
=======
#if defined(_WIN32) && !defined(__SYMBIAN32__) /* Windows specific */
#if defined(_MSC_VER) && _MSC_VER <= 1400
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0400 /* To make it link in VS2005 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#endif
#include <windows.h>

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#ifndef _WIN32_WCE
#include <process.h>
#include <direct.h>
#include <io.h>
<<<<<<< HEAD
#else // _WIN32_WCE
#define NO_CGI // WinCE has no pipes
=======
#else /* _WIN32_WCE */
#define NO_CGI /* WinCE has no pipes */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

typedef long off_t;

#define errno   GetLastError()
#define strerror(x)  _ultoa(x, (char *) _alloca(sizeof(x) *3 ), 10)
<<<<<<< HEAD
#endif // _WIN32_WCE

#define MAKEUQUAD(lo, hi) ((uint64_t)(((uint32_t)(lo)) | \
      ((uint64_t)((uint32_t)(hi))) << 32))
#define RATE_DIFF 10000000 // 100 nsecs
=======
#endif /* _WIN32_WCE */

#define MAKEUQUAD(lo, hi) ((uint64_t)(((uint32_t)(lo)) | \
      ((uint64_t)((uint32_t)(hi))) << 32))
#define RATE_DIFF 10000000 /* 100 nsecs */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#define EPOCH_DIFF MAKEUQUAD(0xd53e8000, 0x019db1de)
#define SYS2UNIX_TIME(lo, hi) \
  (time_t) ((MAKEUQUAD((lo), (hi)) - EPOCH_DIFF) / RATE_DIFF)

<<<<<<< HEAD
// Visual Studio 6 does not know __func__ or __FUNCTION__
// The rest of MS compilers use __FUNCTION__, not C99 __func__
// Also use _strtoui64 on modern M$ compilers
=======
/* Visual Studio 6 does not know __func__ or __FUNCTION__
   The rest of MS compilers use __FUNCTION__, not C99 __func__
   Also use _strtoui64 on modern M$ compilers */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#if defined(_MSC_VER) && _MSC_VER < 1300
#define STRX(x) #x
#define STR(x) STRX(x)
#define __func__ __FILE__ ":" STR(__LINE__)
#define strtoull(x, y, z) (unsigned __int64) _atoi64(x)
#define strtoll(x, y, z) _atoi64(x)
#else
#define __func__  __FUNCTION__
#define strtoull(x, y, z) _strtoui64(x, y, z)
#define strtoll(x, y, z) _strtoi64(x, y, z)
<<<<<<< HEAD
#endif // _MSC_VER
=======
#endif /* _MSC_VER */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

#define ERRNO   GetLastError()
#define NO_SOCKLEN_T
#define SSL_LIB   "ssleay32.dll"
#define CRYPTO_LIB  "libeay32.dll"
#define O_NONBLOCK  0
#if !defined(EWOULDBLOCK)
#define EWOULDBLOCK  WSAEWOULDBLOCK
<<<<<<< HEAD
#endif // !EWOULDBLOCK
=======
#endif /* !EWOULDBLOCK */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#define _POSIX_
#define INT64_FMT  "I64d"

#define WINCDECL __cdecl
#define SHUT_WR 1
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define mg_sleep(x) Sleep(x)

#define pipe(x) _pipe(x, MG_BUF_LEN, _O_BINARY)
#ifndef popen
#define popen(x, y) _popen(x, y)
#endif
#ifndef pclose
#define pclose(x) _pclose(x)
#endif
#define close(x) _close(x)
#define dlsym(x,y) GetProcAddress((HINSTANCE) (x), (y))
#define RTLD_LAZY  0
#define fseeko(x, y, z) _lseeki64(_fileno(x), (y), (z))
#define fdopen(x, y) _fdopen((x), (y))
#define write(x, y, z) _write((x), (y), (unsigned) z)
#define read(x, y, z) _read((x), (y), (unsigned) z)
#define flockfile(x) EnterCriticalSection(&global_log_file_lock)
#define funlockfile(x) LeaveCriticalSection(&global_log_file_lock)
#define sleep(x) Sleep((x) * 1000)
#define rmdir(x) _rmdir(x)

#if !defined(va_copy)
#define va_copy(x, y) x = y
<<<<<<< HEAD
#endif // !va_copy MINGW #defines va_copy

#if !defined(fileno)
#define fileno(x) _fileno(x)
#endif // !fileno MINGW #defines fileno
=======
#endif /* !va_copy MINGW #defines va_copy */

#if !defined(fileno)
#define fileno(x) _fileno(x)
#endif /* !fileno MINGW #defines fileno */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

typedef HANDLE pthread_mutex_t;
typedef struct {
    HANDLE signal, broadcast;
} pthread_cond_t;
typedef HANDLE pthread_t;
<<<<<<< HEAD
#define pid_t HANDLE // MINGW typedefs pid_t to int. Using #define here.
=======
#define pid_t HANDLE /* MINGW typedefs pid_t to int. Using #define here. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static int pthread_mutex_lock(pthread_mutex_t *);
static int pthread_mutex_unlock(pthread_mutex_t *);
static void to_unicode(const char *path, wchar_t *wbuf, size_t wbuf_len);
struct file;
static char *mg_fgets(char *buf, size_t size, struct file *filep, char **p);

#if defined(HAVE_STDINT)
#include <stdint.h>
#else
typedef unsigned int  uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned __int64 uint64_t;
typedef __int64   int64_t;
#define INT64_MAX  9223372036854775807
<<<<<<< HEAD
#endif // HAVE_STDINT

// POSIX dirent interface
=======
#endif /* HAVE_STDINT */

/* POSIX dirent interface */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
struct dirent {
    char d_name[PATH_MAX];
};

typedef struct DIR {
    HANDLE   handle;
    WIN32_FIND_DATAW info;
    struct dirent  result;
} DIR;

<<<<<<< HEAD
#ifndef USE_IPV6 && defined(_WIN32)
=======
#if !defined(USE_IPV6) && defined(_WIN32)
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#ifndef HAVE_POLL
struct pollfd {
    SOCKET fd;
    short events;
    short revents;
};
#define POLLIN 1
#endif
#endif

<<<<<<< HEAD
// Mark required libraries
=======
/* Mark required libraries */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif

<<<<<<< HEAD
#else    // UNIX  specific
=======
#else    /* UNIX  specific */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdint.h>
#include <inttypes.h>
#include <netdb.h>

#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#if !defined(NO_SSL_DL) && !defined(NO_SSL)
#include <dlfcn.h>
#endif
#include <pthread.h>
#if defined(__MACH__)
#define SSL_LIB   "libssl.dylib"
#define CRYPTO_LIB  "libcrypto.dylib"
#else
#if !defined(SSL_LIB)
#define SSL_LIB   "libssl.so"
#endif
#if !defined(CRYPTO_LIB)
#define CRYPTO_LIB  "libcrypto.so"
#endif
#endif
#ifndef O_BINARY
#define O_BINARY  0
<<<<<<< HEAD
#endif // O_BINARY
=======
#endif /* O_BINARY */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#define closesocket(a) close(a)
#define mg_mkdir(x, y) mkdir(x, y)
#define mg_remove(x) remove(x)
#define mg_sleep(x) usleep((x) * 1000)
#define ERRNO errno
#define INVALID_SOCKET (-1)
#define INT64_FMT PRId64
typedef int SOCKET;
#define WINCDECL

<<<<<<< HEAD
#endif // End of Windows and UNIX specific includes
=======
#endif /* End of Windows and UNIX specific includes */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

#include "civetweb.h"

#define CIVETWEB_VERSION "1.5"
#define PASSWORDS_FILE_NAME ".htpasswd"
#define CGI_ENVIRONMENT_SIZE 4096
#define MAX_CGI_ENVIR_VARS 64
#define MG_BUF_LEN 8192
#ifndef MAX_REQUEST_SIZE
#define MAX_REQUEST_SIZE 16384
#endif
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#ifdef _WIN32
static CRITICAL_SECTION global_log_file_lock;
static DWORD pthread_self(void)
{
    return GetCurrentThreadId();
}
<<<<<<< HEAD
#endif // _WIN32
=======
#endif /* _WIN32 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

#define MD5_STATIC static
#include "md5.inl"

#ifdef DEBUG_TRACE
#undef DEBUG_TRACE
#define DEBUG_TRACE(x)
#else
#if defined(DEBUG)
#define DEBUG_TRACE(x) do { \
  flockfile(stdout); \
  printf("*** %lu.%p.%s.%d: ", \
         (unsigned long) time(NULL), (void *) pthread_self(), \
         __func__, __LINE__); \
  printf x; \
  putchar('\n'); \
  fflush(stdout); \
  funlockfile(stdout); \
} while (0)
#else
#define DEBUG_TRACE(x)
<<<<<<< HEAD
#endif // DEBUG
#endif // DEBUG_TRACE

// Darwin prior to 7.0 and Win32 do not have socklen_t
#ifdef NO_SOCKLEN_T
typedef int socklen_t;
#endif // NO_SOCKLEN_T
#define _DARWIN_UNLIMITED_SELECT

#define IP_ADDR_STR_LEN 50  // IPv6 hex string is 46 chars
=======
#endif /* DEBUG */
#endif /* DEBUG_TRACE */

/* Darwin prior to 7.0 and Win32 do not have socklen_t */
#ifdef NO_SOCKLEN_T
typedef int socklen_t;
#endif /* NO_SOCKLEN_T */
#define _DARWIN_UNLIMITED_SELECT

#define IP_ADDR_STR_LEN 50  /* IPv6 hex string is 46 chars */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

#if !defined(MSG_NOSIGNAL)
#define MSG_NOSIGNAL 0
#endif

#if !defined(SOMAXCONN)
#define SOMAXCONN 100
#endif

#if !defined(PATH_MAX)
#define PATH_MAX 4096
#endif

<<<<<<< HEAD
// Size of the accepted socket queue
=======
/* Size of the accepted socket queue */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#if !defined(MGSQLEN)
#define MGSQLEN 20
#endif

static const char *http_500_error = "Internal Server Error";

#if defined(NO_SSL_DL)
#include <openssl/ssl.h>
#include <openssl/err.h>
#else
<<<<<<< HEAD
// SSL loaded dynamically from DLL.
// I put the prototypes here to be independent from OpenSSL source installation.
=======
/* SSL loaded dynamically from DLL.
   I put the prototypes here to be independent from OpenSSL source
   installation. */

>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
typedef struct ssl_st SSL;
typedef struct ssl_method_st SSL_METHOD;
typedef struct ssl_ctx_st SSL_CTX;

struct ssl_func {
<<<<<<< HEAD
    const char *name;   // SSL function name
    void  (*ptr)(void); // Function pointer
=======
    const char *name;   /* SSL function name */
    void  (*ptr)(void); /* Function pointer */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
};

#define SSL_free (* (void (*)(SSL *)) ssl_sw[0].ptr)
#define SSL_accept (* (int (*)(SSL *)) ssl_sw[1].ptr)
#define SSL_connect (* (int (*)(SSL *)) ssl_sw[2].ptr)
#define SSL_read (* (int (*)(SSL *, void *, int)) ssl_sw[3].ptr)
#define SSL_write (* (int (*)(SSL *, const void *,int)) ssl_sw[4].ptr)
#define SSL_get_error (* (int (*)(SSL *, int)) ssl_sw[5].ptr)
#define SSL_set_fd (* (int (*)(SSL *, SOCKET)) ssl_sw[6].ptr)
#define SSL_new (* (SSL * (*)(SSL_CTX *)) ssl_sw[7].ptr)
#define SSL_CTX_new (* (SSL_CTX * (*)(SSL_METHOD *)) ssl_sw[8].ptr)
#define SSLv23_server_method (* (SSL_METHOD * (*)(void)) ssl_sw[9].ptr)
#define SSL_library_init (* (int (*)(void)) ssl_sw[10].ptr)
#define SSL_CTX_use_PrivateKey_file (* (int (*)(SSL_CTX *, \
        const char *, int)) ssl_sw[11].ptr)
#define SSL_CTX_use_certificate_file (* (int (*)(SSL_CTX *, \
        const char *, int)) ssl_sw[12].ptr)
#define SSL_CTX_set_default_passwd_cb \
  (* (void (*)(SSL_CTX *, mg_callback_t)) ssl_sw[13].ptr)
#define SSL_CTX_free (* (void (*)(SSL_CTX *)) ssl_sw[14].ptr)
#define SSL_load_error_strings (* (void (*)(void)) ssl_sw[15].ptr)
#define SSL_CTX_use_certificate_chain_file \
  (* (int (*)(SSL_CTX *, const char *)) ssl_sw[16].ptr)
#define SSLv23_client_method (* (SSL_METHOD * (*)(void)) ssl_sw[17].ptr)
#define SSL_pending (* (int (*)(SSL *)) ssl_sw[18].ptr)
#define SSL_CTX_set_verify (* (void (*)(SSL_CTX *, int, int)) ssl_sw[19].ptr)
#define SSL_shutdown (* (int (*)(SSL *)) ssl_sw[20].ptr)

#define CRYPTO_num_locks (* (int (*)(void)) crypto_sw[0].ptr)
#define CRYPTO_set_locking_callback \
  (* (void (*)(void (*)(int, int, const char *, int))) crypto_sw[1].ptr)
#define CRYPTO_set_id_callback \
  (* (void (*)(unsigned long (*)(void))) crypto_sw[2].ptr)
#define ERR_get_error (* (unsigned long (*)(void)) crypto_sw[3].ptr)
#define ERR_error_string (* (char * (*)(unsigned long,char *)) crypto_sw[4].ptr)

<<<<<<< HEAD
// set_ssl_option() function updates this array.
// It loads SSL library dynamically and changes NULLs to the actual addresses
// of respective functions. The macros above (like SSL_connect()) are really
// just calling these functions indirectly via the pointer.
=======
/* set_ssl_option() function updates this array.
   It loads SSL library dynamically and changes NULLs to the actual addresses
   of respective functions. The macros above (like SSL_connect()) are really
   just calling these functions indirectly via the pointer. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static struct ssl_func ssl_sw[] = {
    {"SSL_free",   NULL},
    {"SSL_accept",   NULL},
    {"SSL_connect",   NULL},
    {"SSL_read",   NULL},
    {"SSL_write",   NULL},
    {"SSL_get_error",  NULL},
    {"SSL_set_fd",   NULL},
    {"SSL_new",   NULL},
    {"SSL_CTX_new",   NULL},
    {"SSLv23_server_method", NULL},
    {"SSL_library_init",  NULL},
    {"SSL_CTX_use_PrivateKey_file", NULL},
    {"SSL_CTX_use_certificate_file",NULL},
    {"SSL_CTX_set_default_passwd_cb",NULL},
    {"SSL_CTX_free",  NULL},
    {"SSL_load_error_strings", NULL},
    {"SSL_CTX_use_certificate_chain_file", NULL},
    {"SSLv23_client_method", NULL},
    {"SSL_pending", NULL},
    {"SSL_CTX_set_verify", NULL},
    {"SSL_shutdown",   NULL},
    {NULL,    NULL}
};

<<<<<<< HEAD
// Similar array as ssl_sw. These functions could be located in different lib.
=======
/* Similar array as ssl_sw. These functions could be located in different
   lib. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#if !defined(NO_SSL)
static struct ssl_func crypto_sw[] = {
    {"CRYPTO_num_locks",  NULL},
    {"CRYPTO_set_locking_callback", NULL},
    {"CRYPTO_set_id_callback", NULL},
    {"ERR_get_error",  NULL},
    {"ERR_error_string", NULL},
    {NULL,    NULL}
};
<<<<<<< HEAD
#endif // NO_SSL
#endif // NO_SSL_DL
=======
#endif /* NO_SSL */
#endif /* NO_SSL_DL */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static const char *month_names[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

<<<<<<< HEAD
// Unified socket address. For IPv6 support, add IPv6 address structure
// in the union u.
=======
/* Unified socket address. For IPv6 support, add IPv6 address structure
   in the union u. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
union usa {
    struct sockaddr sa;
    struct sockaddr_in sin;
#if defined(USE_IPV6)
    struct sockaddr_in6 sin6;
#endif
};

<<<<<<< HEAD
// Describes a string (chunk of memory).
=======
/* Describes a string (chunk of memory). */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
struct vec {
    const char *ptr;
    size_t len;
};

struct file {
    int is_directory;
    time_t modification_time;
    int64_t size;
    FILE *fp;
<<<<<<< HEAD
    const char *membuf;   // Non-NULL if file data is in memory
    // set to 1 if the content is gzipped
    // in which case we need a content-encoding: gzip header
=======
    const char *membuf;   /* Non-NULL if file data is in memory */
    /* set to 1 if the content is gzipped
       in which case we need a content-encoding: gzip header */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    int gzipped;
};
#define STRUCT_FILE_INITIALIZER {0, 0, 0, NULL, NULL, 0}

<<<<<<< HEAD
// Describes listening socket, or socket which was accept()-ed by the master
// thread and queued for future handling by the worker thread.
struct socket {
    SOCKET sock;          // Listening socket
    union usa lsa;        // Local socket address
    union usa rsa;        // Remote socket address
    unsigned is_ssl:1;    // Is port SSL-ed
    unsigned ssl_redir:1; // Is port supposed to redirect everything to SSL port
};

// NOTE(lsm): this enum shoulds be in sync with the config_options below.
=======
/* Describes listening socket, or socket which was accept()-ed by the master
   thread and queued for future handling by the worker thread. */
struct socket {
    SOCKET sock;          /* Listening socket */
    union usa lsa;        /* Local socket address */
    union usa rsa;        /* Remote socket address */
    unsigned is_ssl:1;    /* Is port SSL-ed */
    unsigned ssl_redir:1; /* Is port supposed to redirect everything to SSL
                             port */
};

/* NOTE(lsm): this enum shoulds be in sync with the config_options below. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
enum {
    CGI_EXTENSIONS, CGI_ENVIRONMENT, PUT_DELETE_PASSWORDS_FILE, CGI_INTERPRETER,
    PROTECT_URI, AUTHENTICATION_DOMAIN, SSI_EXTENSIONS, THROTTLE,
    ACCESS_LOG_FILE, ENABLE_DIRECTORY_LISTING, ERROR_LOG_FILE,
    GLOBAL_PASSWORDS_FILE, INDEX_FILES, ENABLE_KEEP_ALIVE, ACCESS_CONTROL_LIST,
    EXTRA_MIME_TYPES, LISTENING_PORTS, DOCUMENT_ROOT, SSL_CERTIFICATE,
    NUM_THREADS, RUN_AS_USER, REWRITE, HIDE_FILES, REQUEST_TIMEOUT,
    NUM_OPTIONS
};

static const char *config_options[] = {
    "cgi_pattern", "**.cgi$|**.pl$|**.php$",
    "cgi_environment", NULL,
    "put_delete_auth_file", NULL,
    "cgi_interpreter", NULL,
    "protect_uri", NULL,
    "authentication_domain", "mydomain.com",
    "ssi_pattern", "**.shtml$|**.shtm$",
    "throttle", NULL,
    "access_log_file", NULL,
    "enable_directory_listing", "yes",
    "error_log_file", NULL,
    "global_auth_file", NULL,
    "index_files",
    "index.html,index.htm,index.cgi,index.shtml,index.php,index.lp",
    "enable_keep_alive", "no",
    "access_control_list", NULL,
    "extra_mime_types", NULL,
    "listening_ports", "8080",
    "document_root",  NULL,
    "ssl_certificate", NULL,
    "num_threads", "50",
    "run_as_user", NULL,
    "url_rewrite_patterns", NULL,
    "hide_files_patterns", NULL,
    "request_timeout_ms", "30000",
    NULL
};

struct mg_request_handler_info {
    char *uri;
    size_t uri_len;
    mg_request_handler handler;
    void *cbdata;
    struct mg_request_handler_info *next;
};

struct mg_context {
<<<<<<< HEAD
    volatile int stop_flag;         // Should we stop event loop
    void *ssllib_dll_handle;        // Store the ssl library handle.
    void *cryptolib_dll_handle;     // Store the crypto library handle.
    SSL_CTX *ssl_ctx;               // SSL context
    char *config[NUM_OPTIONS];      // Civetweb configuration parameters
    struct mg_callbacks callbacks;  // User-defined callback function
    void *user_data;                // User-defined data
=======
    volatile int stop_flag;         /* Should we stop event loop */
    void *ssllib_dll_handle;        /* Store the ssl library handle. */
    void *cryptolib_dll_handle;     /* Store the crypto library handle. */
    SSL_CTX *ssl_ctx;               /* SSL context */
    char *config[NUM_OPTIONS];      /* Civetweb configuration parameters */
    struct mg_callbacks callbacks;  /* User-defined callback function */
    void *user_data;                /* User-defined data */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

    struct socket *listening_sockets;
    int num_listening_sockets;

<<<<<<< HEAD
    volatile int num_threads;  // Number of threads
    pthread_mutex_t mutex;     // Protects (max|num)_threads
    pthread_cond_t  cond;      // Condvar for tracking workers terminations

    struct socket queue[MGSQLEN];   // Accepted sockets
    volatile int sq_head;      // Head of the socket queue
    volatile int sq_tail;      // Tail of the socket queue
    pthread_cond_t sq_full;    // Signaled when socket is produced
    pthread_cond_t sq_empty;   // Signaled when socket is consumed
    pthread_t masterthreadid;  // The master thread ID.
    int workerthreadcount;     // The amount of worker threads.
    pthread_t *workerthreadids;// The worker thread IDs.

    // linked list of uri handlers
=======
    volatile int num_threads;  /* Number of threads */
    pthread_mutex_t mutex;     /* Protects (max|num)_threads */
    pthread_cond_t  cond;      /* Condvar for tracking workers terminations */

    struct socket queue[MGSQLEN];   /* Accepted sockets */
    volatile int sq_head;      /* Head of the socket queue */
    volatile int sq_tail;      /* Tail of the socket queue */
    pthread_cond_t sq_full;    /* Signaled when socket is produced */
    pthread_cond_t sq_empty;   /* Signaled when socket is consumed */
    pthread_t masterthreadid;  /* The master thread ID. */
    int workerthreadcount;     /* The amount of worker threads. */
    pthread_t *workerthreadids;/* The worker thread IDs. */

    /* linked list of uri handlers */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    struct mg_request_handler_info *request_handlers;
};

struct mg_connection {
    struct mg_request_info request_info;
    struct mg_context *ctx;
<<<<<<< HEAD
    SSL *ssl;                   // SSL descriptor
    SSL_CTX *client_ssl_ctx;    // SSL context for client connections
    struct socket client;       // Connected client
    time_t birth_time;          // Time when request was received
    int64_t num_bytes_sent;     // Total bytes sent to client
    int64_t content_len;        // Content-Length header value
    int64_t consumed_content;   // How many bytes of content have been read
    char *buf;                  // Buffer for received data
    char *path_info;            // PATH_INFO part of the URL
    int must_close;             // 1 if connection must be closed
    int buf_size;               // Buffer size
    int request_len;            // Size of the request + headers in a buffer
    int data_len;               // Total size of data in a buffer
    int status_code;            // HTTP reply status code, e.g. 200
    int throttle;               // Throttling, bytes/sec. <= 0 means no throttle
    time_t last_throttle_time;  // Last time throttled data was sent
    int64_t last_throttle_bytes;// Bytes sent this second
    pthread_mutex_t mutex;      // Used by mg_lock/mg_unlock to ensure atomic transmissions for websockets
};

// Directory entry
=======
    SSL *ssl;                   /* SSL descriptor */
    SSL_CTX *client_ssl_ctx;    /* SSL context for client connections */
    struct socket client;       /* Connected client */
    time_t birth_time;          /* Time when request was received */
    int64_t num_bytes_sent;     /* Total bytes sent to client */
    int64_t content_len;        /* Content-Length header value */
    int64_t consumed_content;   /* How many bytes of content have been read */
    char *buf;                  /* Buffer for received data */
    char *path_info;            /* PATH_INFO part of the URL */
    int must_close;             /* 1 if connection must be closed */
    int buf_size;               /* Buffer size */
    int request_len;            /* Size of the request + headers in a buffer */
    int data_len;               /* Total size of data in a buffer */
    int status_code;            /* HTTP reply status code, e.g. 200 */
    int throttle;               /* Throttling, bytes/sec. <= 0 means no
                                   throttle */
    time_t last_throttle_time;  /* Last time throttled data was sent */
    int64_t last_throttle_bytes;/* Bytes sent this second */
    pthread_mutex_t mutex;      /* Used by mg_lock/mg_unlock to ensure atomic
                                   transmissions for websockets */
};

/* Directory entry */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
struct de {
    struct mg_connection *conn;
    char *file_name;
    struct file file;
};

const char **mg_get_valid_option_names(void)
{
    return config_options;
}

static int is_file_in_memory(struct mg_connection *conn, const char *path,
                             struct file *filep)
{
    size_t size = 0;
    if ((filep->membuf = conn->ctx->callbacks.open_file == NULL ? NULL :
                         conn->ctx->callbacks.open_file(conn, path, &size)) != NULL) {
<<<<<<< HEAD
        // NOTE: override filep->size only on success. Otherwise, it might break
        // constructs like if (!mg_stat() || !mg_fopen()) ...
=======
        /* NOTE: override filep->size only on success. Otherwise, it might
           break constructs like if (!mg_stat() || !mg_fopen()) ... */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        filep->size = size;
    }
    return filep->membuf != NULL;
}

static int is_file_opened(const struct file *filep)
{
    return filep->membuf != NULL || filep->fp != NULL;
}

static int mg_fopen(struct mg_connection *conn, const char *path,
                    const char *mode, struct file *filep)
{
    if (!is_file_in_memory(conn, path, filep)) {
#ifdef _WIN32
        wchar_t wbuf[PATH_MAX], wmode[20];
        to_unicode(path, wbuf, ARRAY_SIZE(wbuf));
        MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, ARRAY_SIZE(wmode));
        filep->fp = _wfopen(wbuf, wmode);
#else
        filep->fp = fopen(path, mode);
#endif
    }

    return is_file_opened(filep);
}

static void mg_fclose(struct file *filep)
{
    if (filep != NULL && filep->fp != NULL) {
        fclose(filep->fp);
    }
}

static int get_option_index(const char *name)
{
    int i;

    for (i = 0; config_options[i * 2] != NULL; i++) {
        if (strcmp(config_options[i * 2], name) == 0) {
            return i;
        }
    }
    return -1;
}

const char *mg_get_option(const struct mg_context *ctx, const char *name)
{
    int i;
    if ((i = get_option_index(name)) == -1) {
        return NULL;
    } else if (ctx->config[i] == NULL) {
        return "";
    } else {
        return ctx->config[i];
    }
}

static void sockaddr_to_string(char *buf, size_t len,
                               const union usa *usa)
{
    buf[0] = '\0';
#if defined(USE_IPV6)
    inet_ntop(usa->sa.sa_family, usa->sa.sa_family == AF_INET ?
              (void *) &usa->sin.sin_addr :
              (void *) &usa->sin6.sin6_addr, buf, len);
#elif defined(_WIN32)
<<<<<<< HEAD
    // Only Windoze Vista (and newer) have inet_ntop()
=======
    /* Only Windoze Vista (and newer) have inet_ntop() */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    strncpy(buf, inet_ntoa(usa->sin.sin_addr), len);
#else
    inet_ntop(usa->sa.sa_family, (void *) &usa->sin.sin_addr, buf, len);
#endif
}

<<<<<<< HEAD
// Print error message to the opened error log stream.
=======
/* Print error message to the opened error log stream. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
void mg_cry(struct mg_connection *conn, const char *fmt, ...)
{
    char buf[MG_BUF_LEN], src_addr[IP_ADDR_STR_LEN];
    va_list ap;
    FILE *fp;
    time_t timestamp;

    va_start(ap, fmt);
    IGNORE_UNUSED_RESULT(vsnprintf(buf, sizeof(buf), fmt, ap));
    va_end(ap);

<<<<<<< HEAD
    // Do not lock when getting the callback value, here and below.
    // I suppose this is fine, since function cannot disappear in the
    // same way string option can.
=======
    /* Do not lock when getting the callback value, here and below.
       I suppose this is fine, since function cannot disappear in the
       same way string option can. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (conn->ctx->callbacks.log_message == NULL ||
        conn->ctx->callbacks.log_message(conn, buf) == 0) {
        fp = conn->ctx->config[ERROR_LOG_FILE] == NULL ? NULL :
             fopen(conn->ctx->config[ERROR_LOG_FILE], "a+");

        if (fp != NULL) {
            flockfile(fp);
            timestamp = time(NULL);

            sockaddr_to_string(src_addr, sizeof(src_addr), &conn->client.rsa);
            fprintf(fp, "[%010lu] [error] [client %s] ", (unsigned long) timestamp,
                    src_addr);

            if (conn->request_info.request_method != NULL) {
                fprintf(fp, "%s %s: ", conn->request_info.request_method,
                        conn->request_info.uri);
            }

            fprintf(fp, "%s", buf);
            fputc('\n', fp);
            funlockfile(fp);
            fclose(fp);
        }
    }
}

<<<<<<< HEAD
// Return fake connection structure. Used for logging, if connection
// is not applicable at the moment of logging.
=======
/* Return fake connection structure. Used for logging, if connection
   is not applicable at the moment of logging. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static struct mg_connection *fc(struct mg_context *ctx)
{
    static struct mg_connection fake_connection;
    fake_connection.ctx = ctx;
    return &fake_connection;
}

const char *mg_version(void)
{
    return CIVETWEB_VERSION;
}

struct mg_request_info *mg_get_request_info(struct mg_connection *conn)
{
    return &conn->request_info;
}

static void mg_strlcpy(register char *dst, register const char *src, size_t n)
{
    for (; *src != '\0' && n > 1; n--) {
        *dst++ = *src++;
    }
    *dst = '\0';
}

static int lowercase(const char *s)
{
    return tolower(* (const unsigned char *) s);
}

int mg_strncasecmp(const char *s1, const char *s2, size_t len)
{
    int diff = 0;

    if (len > 0)
        do {
            diff = lowercase(s1++) - lowercase(s2++);
        } while (diff == 0 && s1[-1] != '\0' && --len > 0);

    return diff;
}

static int mg_strcasecmp(const char *s1, const char *s2)
{
    int diff;

    do {
        diff = lowercase(s1++) - lowercase(s2++);
    } while (diff == 0 && s1[-1] != '\0');

    return diff;
}

static char * mg_strndup(const char *ptr, size_t len)
{
    char *p;

    if ((p = (char *) malloc(len + 1)) != NULL) {
        mg_strlcpy(p, ptr, len + 1);
    }

    return p;
}

static char * mg_strdup(const char *str)
{
    return mg_strndup(str, strlen(str));
}

static const char *mg_strcasestr(const char *big_str, const char *small_str)
{
    int i, big_len = (int)strlen(big_str), small_len = (int)strlen(small_str);

    for (i = 0; i <= big_len - small_len; i++) {
        if (mg_strncasecmp(big_str + i, small_str, small_len) == 0) {
            return big_str + i;
        }
    }

    return NULL;
}

<<<<<<< HEAD
// Like snprintf(), but never returns negative value, or a value
// that is larger than a supplied buffer.
// Thanks to Adam Zeldis to pointing snprintf()-caused vulnerability
// in his audit report.
=======
/* Like snprintf(), but never returns negative value, or a value
   that is larger than a supplied buffer.
   Thanks to Adam Zeldis to pointing snprintf()-caused vulnerability
   in his audit report. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int mg_vsnprintf(struct mg_connection *conn, char *buf, size_t buflen,
                        const char *fmt, va_list ap)
{
    int n;

    if (buflen == 0)
        return 0;

    n = vsnprintf(buf, buflen, fmt, ap);

    if (n < 0) {
        mg_cry(conn, "vsnprintf error");
        n = 0;
    } else if (n >= (int) buflen) {
        mg_cry(conn, "truncating vsnprintf buffer: [%.*s]",
               n > 200 ? 200 : n, buf);
        n = (int) buflen - 1;
    }
    buf[n] = '\0';

    return n;
}

static int mg_snprintf(struct mg_connection *conn, char *buf, size_t buflen,
                       PRINTF_FORMAT_STRING(const char *fmt), ...)
PRINTF_ARGS(4, 5);

static int mg_snprintf(struct mg_connection *conn, char *buf, size_t buflen,
                       const char *fmt, ...)
{
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = mg_vsnprintf(conn, buf, buflen, fmt, ap);
    va_end(ap);

    return n;
}

<<<<<<< HEAD
// Skip the characters until one of the delimiters characters found.
// 0-terminate resulting word. Skip the delimiter and following whitespaces.
// Advance pointer to buffer to the next word. Return found 0-terminated word.
// Delimiters can be quoted with quotechar.
=======
/* Skip the characters until one of the delimiters characters found.
   0-terminate resulting word. Skip the delimiter and following whitespaces.
   Advance pointer to buffer to the next word. Return found 0-terminated word.
   Delimiters can be quoted with quotechar. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static char *skip_quoted(char **buf, const char *delimiters,
                         const char *whitespace, char quotechar)
{
    char *p, *begin_word, *end_word, *end_whitespace;

    begin_word = *buf;
    end_word = begin_word + strcspn(begin_word, delimiters);

<<<<<<< HEAD
    // Check for quotechar
    if (end_word > begin_word) {
        p = end_word - 1;
        while (*p == quotechar) {
            // If there is anything beyond end_word, copy it
=======
    /* Check for quotechar */
    if (end_word > begin_word) {
        p = end_word - 1;
        while (*p == quotechar) {
            /* If there is anything beyond end_word, copy it */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            if (*end_word == '\0') {
                *p = '\0';
                break;
            } else {
                size_t end_off = strcspn(end_word + 1, delimiters);
                memmove (p, end_word, end_off + 1);
<<<<<<< HEAD
                p += end_off; // p must correspond to end_word - 1
=======
                p += end_off; /* p must correspond to end_word - 1 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                end_word += end_off + 1;
            }
        }
        for (p++; p < end_word; p++) {
            *p = '\0';
        }
    }

    if (*end_word == '\0') {
        *buf = end_word;
    } else {
        end_whitespace = end_word + 1 + strspn(end_word + 1, whitespace);

        for (p = end_word; p < end_whitespace; p++) {
            *p = '\0';
        }

        *buf = end_whitespace;
    }

    return begin_word;
}

<<<<<<< HEAD
// Simplified version of skip_quoted without quote char
// and whitespace == delimiters
=======
/* Simplified version of skip_quoted without quote char
   and whitespace == delimiters */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static char *skip(char **buf, const char *delimiters)
{
    return skip_quoted(buf, delimiters, delimiters, 0);
}


<<<<<<< HEAD
// Return HTTP header value, or NULL if not found.
=======
/* Return HTTP header value, or NULL if not found. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static const char *get_header(const struct mg_request_info *ri,
                              const char *name)
{
    int i;

    for (i = 0; i < ri->num_headers; i++)
        if (!mg_strcasecmp(name, ri->http_headers[i].name))
            return ri->http_headers[i].value;

    return NULL;
}

const char *mg_get_header(const struct mg_connection *conn, const char *name)
{
    return get_header(&conn->request_info, name);
}

<<<<<<< HEAD
// A helper function for traversing a comma separated list of values.
// It returns a list pointer shifted to the next value, or NULL if the end
// of the list found.
// Value is stored in val vector. If value has form "x=y", then eq_val
// vector is initialized to point to the "y" part, and val vector length
// is adjusted to point only to "x".
=======
/* A helper function for traversing a comma separated list of values.
   It returns a list pointer shifted to the next value, or NULL if the end
   of the list found.
   Value is stored in val vector. If value has form "x=y", then eq_val
   vector is initialized to point to the "y" part, and val vector length
   is adjusted to point only to "x". */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static const char *next_option(const char *list, struct vec *val,
                               struct vec *eq_val)
{
    if (list == NULL || *list == '\0') {
<<<<<<< HEAD
        // End of the list
=======
        /* End of the list */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        list = NULL;
    } else {
        val->ptr = list;
        if ((list = strchr(val->ptr, ',')) != NULL) {
<<<<<<< HEAD
            // Comma found. Store length and shift the list ptr
            val->len = list - val->ptr;
            list++;
        } else {
            // This value is the last one
=======
            /* Comma found. Store length and shift the list ptr */
            val->len = list - val->ptr;
            list++;
        } else {
            /* This value is the last one */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            list = val->ptr + strlen(val->ptr);
            val->len = list - val->ptr;
        }

        if (eq_val != NULL) {
<<<<<<< HEAD
            // Value has form "x=y", adjust pointers and lengths
            // so that val points to "x", and eq_val points to "y".
            eq_val->len = 0;
            eq_val->ptr = (const char *) memchr(val->ptr, '=', val->len);
            if (eq_val->ptr != NULL) {
                eq_val->ptr++;  // Skip over '=' character
=======
            /* Value has form "x=y", adjust pointers and lengths
               so that val points to "x", and eq_val points to "y". */
            eq_val->len = 0;
            eq_val->ptr = (const char *) memchr(val->ptr, '=', val->len);
            if (eq_val->ptr != NULL) {
                eq_val->ptr++;  /* Skip over '=' character */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                eq_val->len = val->ptr + val->len - eq_val->ptr;
                val->len = (eq_val->ptr - val->ptr) - 1;
            }
        }
    }

    return list;
}

<<<<<<< HEAD
// Perform case-insensitive match of string against pattern
=======
/* Perform case-insensitive match of string against pattern */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int match_prefix(const char *pattern, int pattern_len, const char *str)
{
    const char *or_str;
    int i, j, len, res;

    if ((or_str = (const char *) memchr(pattern, '|', pattern_len)) != NULL) {
        res = match_prefix(pattern, (int)(or_str - pattern), str);
        return res > 0 ? res :
               match_prefix(or_str + 1, (int)((pattern + pattern_len) - (or_str + 1)), str);
    }

    i = j = 0;
    res = -1;
    for (; i < pattern_len; i++, j++) {
        if (pattern[i] == '?' && str[j] != '\0') {
            continue;
        } else if (pattern[i] == '$') {
            return str[j] == '\0' ? j : -1;
        } else if (pattern[i] == '*') {
            i++;
            if (pattern[i] == '*') {
                i++;
                len = (int) strlen(str + j);
            } else {
                len = (int) strcspn(str + j, "/");
            }
            if (i == pattern_len) {
                return j + len;
            }
            do {
                res = match_prefix(pattern + i, pattern_len - i, str + j + len);
            } while (res == -1 && len-- > 0);
            return res == -1 ? -1 : j + res + len;
        } else if (lowercase(&pattern[i]) != lowercase(&str[j])) {
            return -1;
        }
    }
    return j;
}

<<<<<<< HEAD
// HTTP 1.1 assumes keep alive if "Connection:" header is not set
// This function must tolerate situations when connection info is not
// set up, for example if request parsing failed.
=======
/* HTTP 1.1 assumes keep alive if "Connection:" header is not set
   This function must tolerate situations when connection info is not
   set up, for example if request parsing failed. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int should_keep_alive(const struct mg_connection *conn)
{
    const char *http_version = conn->request_info.http_version;
    const char *header = mg_get_header(conn, "Connection");
    if (conn->must_close ||
        conn->status_code == 401 ||
        mg_strcasecmp(conn->ctx->config[ENABLE_KEEP_ALIVE], "yes") != 0 ||
        (header != NULL && mg_strcasecmp(header, "keep-alive") != 0) ||
        (header == NULL && http_version && strcmp(http_version, "1.1"))) {
        return 0;
    }
    return 1;
}

static const char *suggest_connection_header(const struct mg_connection *conn)
{
    return should_keep_alive(conn) ? "keep-alive" : "close";
}

static void send_http_error(struct mg_connection *, int, const char *,
                            PRINTF_FORMAT_STRING(const char *fmt), ...)
PRINTF_ARGS(4, 5);


static void send_http_error(struct mg_connection *conn, int status,
                            const char *reason, const char *fmt, ...)
{
    char buf[MG_BUF_LEN];
    va_list ap;
    int len = 0;

    conn->status_code = status;
    if (conn->ctx->callbacks.http_error == NULL ||
        conn->ctx->callbacks.http_error(conn, status)) {
        buf[0] = '\0';

<<<<<<< HEAD
        // Errors 1xx, 204 and 304 MUST NOT send a body
=======
        /* Errors 1xx, 204 and 304 MUST NOT send a body */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (status > 199 && status != 204 && status != 304) {
            len = mg_snprintf(conn, buf, sizeof(buf), "Error %d: %s", status, reason);
            buf[len++] = '\n';

            va_start(ap, fmt);
            len += mg_vsnprintf(conn, buf + len, sizeof(buf) - len, fmt, ap);
            va_end(ap);
        }
        DEBUG_TRACE(("[%s]", buf));

        mg_printf(conn, "HTTP/1.1 %d %s\r\n"
                  "Content-Length: %d\r\n"
                  "Connection: %s\r\n\r\n", status, reason, len,
                  suggest_connection_header(conn));
        conn->num_bytes_sent += mg_printf(conn, "%s", buf);
    }
}

#if defined(_WIN32) && !defined(__SYMBIAN32__)
static int pthread_mutex_init(pthread_mutex_t *mutex, void *unused)
{
    (void) unused;
    *mutex = CreateMutex(NULL, FALSE, NULL);
    return *mutex == NULL ? -1 : 0;
}

static int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    return CloseHandle(*mutex) == 0 ? -1 : 0;
}

static int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

static int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    return ReleaseMutex(*mutex) == 0 ? -1 : 0;
}

static int pthread_cond_init(pthread_cond_t *cv, const void *unused)
{
    (void) unused;
    cv->signal = CreateEvent(NULL, FALSE, FALSE, NULL);
    cv->broadcast = CreateEvent(NULL, TRUE, FALSE, NULL);
    return cv->signal != NULL && cv->broadcast != NULL ? 0 : -1;
}

static int pthread_cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex)
{
    HANDLE handles[] = {cv->signal, cv->broadcast};
    ReleaseMutex(*mutex);
    WaitForMultipleObjects(2, handles, FALSE, INFINITE);
    return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

static int pthread_cond_signal(pthread_cond_t *cv)
{
    return SetEvent(cv->signal) == 0 ? -1 : 0;
}

static int pthread_cond_broadcast(pthread_cond_t *cv)
{
<<<<<<< HEAD
    // Implementation with PulseEvent() has race condition, see
    // http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
=======
    /* Implementation with PulseEvent() has race condition, see
       http://www.cs.wustl.edu/~schmidt/win32-cv-1.html */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    return PulseEvent(cv->broadcast) == 0 ? -1 : 0;
}

static int pthread_cond_destroy(pthread_cond_t *cv)
{
    return CloseHandle(cv->signal) && CloseHandle(cv->broadcast) ? 0 : -1;
}

<<<<<<< HEAD
// For Windows, change all slashes to backslashes in path names.
=======
/* For Windows, change all slashes to backslashes in path names. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void change_slashes_to_backslashes(char *path)
{
    int i;

    for (i = 0; path[i] != '\0'; i++) {
        if (path[i] == '/')
            path[i] = '\\';
<<<<<<< HEAD
        // i > 0 check is to preserve UNC paths, like \\server\file.txt
=======
        /* i > 0 check is to preserve UNC paths, like \\server\file.txt */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (path[i] == '\\' && i > 0)
            while (path[i + 1] == '\\' || path[i + 1] == '/')
                (void) memmove(path + i + 1,
                               path + i + 2, strlen(path + i + 1));
    }
}

<<<<<<< HEAD
// Encode 'path' which is assumed UTF-8 string, into UNICODE string.
// wbuf and wbuf_len is a target buffer and its length.
=======
/* Encode 'path' which is assumed UTF-8 string, into UNICODE string.
   wbuf and wbuf_len is a target buffer and its length. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void to_unicode(const char *path, wchar_t *wbuf, size_t wbuf_len)
{
    char buf[PATH_MAX], buf2[PATH_MAX];

    mg_strlcpy(buf, path, sizeof(buf));
    change_slashes_to_backslashes(buf);

<<<<<<< HEAD
    // Convert to Unicode and back. If doubly-converted string does not
    // match the original, something is fishy, reject.
=======
    /* Convert to Unicode and back. If doubly-converted string does not
       match the original, something is fishy, reject. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int) wbuf_len);
    WideCharToMultiByte(CP_UTF8, 0, wbuf, (int) wbuf_len, buf2, sizeof(buf2),
                        NULL, NULL);
    if (strcmp(buf, buf2) != 0) {
        wbuf[0] = L'\0';
    }
}

#if defined(_WIN32_WCE)
static time_t time(time_t *ptime)
{
    time_t t;
    SYSTEMTIME st;
    FILETIME ft;

    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);
    t = SYS2UNIX_TIME(ft.dwLowDateTime, ft.dwHighDateTime);

    if (ptime != NULL) {
        *ptime = t;
    }

    return t;
}

static struct tm *localtime(const time_t *ptime, struct tm *ptm)
{
    int64_t t = ((int64_t) *ptime) * RATE_DIFF + EPOCH_DIFF;
    FILETIME ft, lft;
    SYSTEMTIME st;
    TIME_ZONE_INFORMATION tzinfo;

    if (ptm == NULL) {
        return NULL;
    }

    * (int64_t *) &ft = t;
    FileTimeToLocalFileTime(&ft, &lft);
    FileTimeToSystemTime(&lft, &st);
    ptm->tm_year = st.wYear - 1900;
    ptm->tm_mon = st.wMonth - 1;
    ptm->tm_wday = st.wDayOfWeek;
    ptm->tm_mday = st.wDay;
    ptm->tm_hour = st.wHour;
    ptm->tm_min = st.wMinute;
    ptm->tm_sec = st.wSecond;
<<<<<<< HEAD
    ptm->tm_yday = 0; // hope nobody uses this
=======
    ptm->tm_yday = 0; /* hope nobody uses this */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    ptm->tm_isdst =
        GetTimeZoneInformation(&tzinfo) == TIME_ZONE_ID_DAYLIGHT ? 1 : 0;

    return ptm;
}

static struct tm *gmtime(const time_t *ptime, struct tm *ptm)
{
<<<<<<< HEAD
    // FIXME(lsm): fix this.
=======
    /* FIXME(lsm): fix this. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    return localtime(ptime, ptm);
}

static size_t strftime(char *dst, size_t dst_size, const char *fmt,
                       const struct tm *tm)
{
    (void) snprintf(dst, dst_size, "implement strftime() for WinCE");
    return 0;
}
#endif

<<<<<<< HEAD
// Windows happily opens files with some garbage at the end of file name.
// For example, fopen("a.cgi    ", "r") on Windows successfully opens
// "a.cgi", despite one would expect an error back.
// This function returns non-0 if path ends with some garbage.
=======
/* Windows happily opens files with some garbage at the end of file name.
   For example, fopen("a.cgi    ", "r") on Windows successfully opens
   "a.cgi", despite one would expect an error back.
   This function returns non-0 if path ends with some garbage. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int path_cannot_disclose_cgi(const char *path)
{
    static const char *allowed_last_characters = "_-";
    int last = path[strlen(path) - 1];
    return isalnum(last) || strchr(allowed_last_characters, last) != NULL;
}

static int mg_stat(struct mg_connection *conn, const char *path,
                   struct file *filep)
{
    wchar_t wbuf[PATH_MAX];
    WIN32_FILE_ATTRIBUTE_DATA info;

    if (!is_file_in_memory(conn, path, filep)) {
        to_unicode(path, wbuf, ARRAY_SIZE(wbuf));
        if (GetFileAttributesExW(wbuf, GetFileExInfoStandard, &info) != 0) {
            filep->size = MAKEUQUAD(info.nFileSizeLow, info.nFileSizeHigh);
            filep->modification_time = SYS2UNIX_TIME(
                                           info.ftLastWriteTime.dwLowDateTime,
                                           info.ftLastWriteTime.dwHighDateTime);
            filep->is_directory = info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
<<<<<<< HEAD
            // If file name is fishy, reset the file structure and return error.
            // Note it is important to reset, not just return the error, cause
            // functions like is_file_opened() check the struct.
=======
            /* If file name is fishy, reset the file structure and return
               error.
               Note it is important to reset, not just return the error, cause
               functions like is_file_opened() check the struct. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            if (!filep->is_directory && !path_cannot_disclose_cgi(path)) {
                memset(filep, 0, sizeof(*filep));
            }
        }
    }

    return filep->membuf != NULL || filep->modification_time != 0;
}

static int mg_remove(const char *path)
{
    wchar_t wbuf[PATH_MAX];
    to_unicode(path, wbuf, ARRAY_SIZE(wbuf));
    return DeleteFileW(wbuf) ? 0 : -1;
}

static int mg_mkdir(const char *path, int mode)
{
    char buf[PATH_MAX];
    wchar_t wbuf[PATH_MAX];

    (void) mode;
    mg_strlcpy(buf, path, sizeof(buf));
    change_slashes_to_backslashes(buf);

    (void) MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, ARRAY_SIZE(wbuf));

    return CreateDirectoryW(wbuf, NULL) ? 0 : -1;
}

<<<<<<< HEAD
// Implementation of POSIX opendir/closedir/readdir for Windows.
=======
/* Implementation of POSIX opendir/closedir/readdir for Windows. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static DIR * opendir(const char *name)
{
    DIR *dir = NULL;
    wchar_t wpath[PATH_MAX];
    DWORD attrs;

    if (name == NULL) {
        SetLastError(ERROR_BAD_ARGUMENTS);
    } else if ((dir = (DIR *) malloc(sizeof(*dir))) == NULL) {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    } else {
        to_unicode(name, wpath, ARRAY_SIZE(wpath));
        attrs = GetFileAttributesW(wpath);
        if (attrs != 0xFFFFFFFF &&
            ((attrs & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)) {
            (void) wcscat(wpath, L"\\*");
            dir->handle = FindFirstFileW(wpath, &dir->info);
            dir->result.d_name[0] = '\0';
        } else {
            free(dir);
            dir = NULL;
        }
    }

    return dir;
}

static int closedir(DIR *dir)
{
    int result = 0;

    if (dir != NULL) {
        if (dir->handle != INVALID_HANDLE_VALUE)
            result = FindClose(dir->handle) ? 0 : -1;

        free(dir);
    } else {
        result = -1;
        SetLastError(ERROR_BAD_ARGUMENTS);
    }

    return result;
}

static struct dirent *readdir(DIR *dir)
{
    struct dirent *result = 0;

    if (dir) {
        if (dir->handle != INVALID_HANDLE_VALUE) {
            result = &dir->result;
            (void) WideCharToMultiByte(CP_UTF8, 0,
                                       dir->info.cFileName, -1, result->d_name,
                                       sizeof(result->d_name), NULL, NULL);

            if (!FindNextFileW(dir->handle, &dir->info)) {
                (void) FindClose(dir->handle);
                dir->handle = INVALID_HANDLE_VALUE;
            }

        } else {
            SetLastError(ERROR_FILE_NOT_FOUND);
        }
    } else {
        SetLastError(ERROR_BAD_ARGUMENTS);
    }

    return result;
}

#ifndef HAVE_POLL
static int poll(struct pollfd *pfd, int n, int milliseconds)
{
    struct timeval tv;
    fd_set set;
    int i, result;
    SOCKET maxfd = 0;

    tv.tv_sec = milliseconds / 1000;
    tv.tv_usec = (milliseconds % 1000) * 1000;
    FD_ZERO(&set);

    for (i = 0; i < n; i++) {
        FD_SET((SOCKET) pfd[i].fd, &set);
        pfd[i].revents = 0;

        if (pfd[i].fd > maxfd) {
            maxfd = pfd[i].fd;
        }
    }

    if ((result = select((int)maxfd + 1, &set, NULL, NULL, &tv)) > 0) {
        for (i = 0; i < n; i++) {
            if (FD_ISSET(pfd[i].fd, &set)) {
                pfd[i].revents = POLLIN;
            }
        }
    }

    return result;
}
<<<<<<< HEAD
#endif // HAVE_POLL
=======
#endif /* HAVE_POLL */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static void set_close_on_exec(SOCKET sock, struct mg_connection *conn /* may be null */)
{
    (void) conn; /* Unused. */
    (void) SetHandleInformation((HANDLE) sock, HANDLE_FLAG_INHERIT, 0);
}

int mg_start_thread(mg_thread_func_t f, void *p)
{
    return (long)_beginthread((void (__cdecl *)(void *)) f, 0, p) == -1L ? -1 : 0;
}

/* Start a thread storing the thread context. */

static int mg_start_thread_with_id(unsigned (__stdcall *f)(void *), void *p,
                                   pthread_t *threadidptr)
{
    uintptr_t uip;
    HANDLE threadhandle;
    int result;

    uip = _beginthreadex(NULL, 0, (unsigned (__stdcall *)(void *)) f, p, 0,
                         NULL);
    threadhandle = (HANDLE) uip;
    if (threadidptr != NULL) {
        *threadidptr = threadhandle;
    }
    result = (threadhandle == NULL) ? -1 : 0;

    return result;
}

/* Wait for a thread to finish. */

static int mg_join_thread(pthread_t threadid)
{
    int result;
    DWORD dwevent;

    result = -1;
    dwevent = WaitForSingleObject(threadid, INFINITE);
    if (dwevent == WAIT_FAILED) {
        int err;

        err = GetLastError();
        DEBUG_TRACE(("WaitForSingleObject() failed, error %d", err));
    } else {
        if (dwevent == WAIT_OBJECT_0) {
            CloseHandle(threadid);
            result = 0;
        }
    }

    return result;
}

static HANDLE dlopen(const char *dll_name, int flags)
{
    wchar_t wbuf[PATH_MAX];
    (void) flags;
    to_unicode(dll_name, wbuf, ARRAY_SIZE(wbuf));
    return LoadLibraryW(wbuf);
}

static int dlclose(void *handle)
{
    int result;

    if (FreeLibrary(handle) != 0) {
        result = 0;
    } else {
        result = -1;
    }

    return result;
}

#if !defined(NO_CGI)
#define SIGKILL 0
static int kill(pid_t pid, int sig_num)
{
    (void) TerminateProcess(pid, sig_num);
    (void) CloseHandle(pid);
    return 0;
}

static void trim_trailing_whitespaces(char *s)
{
    char *e = s + strlen(s) - 1;
    while (e > s && isspace(* (unsigned char *) e)) {
        *e-- = '\0';
    }
}

static pid_t spawn_process(struct mg_connection *conn, const char *prog,
                           char *envblk, char *envp[], int fdin,
                           int fdout, const char *dir)
{
    HANDLE me;
    char *p, *interp, full_interp[PATH_MAX], full_dir[PATH_MAX],
         cmdline[PATH_MAX], buf[PATH_MAX];
    struct file file = STRUCT_FILE_INITIALIZER;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi = { 0 };

    (void) envp;

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);

<<<<<<< HEAD
    // TODO(lsm): redirect CGI errors to the error log file
=======
    /* TODO(lsm): redirect CGI errors to the error log file */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    me = GetCurrentProcess();
    DuplicateHandle(me, (HANDLE) _get_osfhandle(fdin), me,
                    &si.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
    DuplicateHandle(me, (HANDLE) _get_osfhandle(fdout), me,
                    &si.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);

<<<<<<< HEAD
    // If CGI file is a script, try to read the interpreter line
=======
    /* If CGI file is a script, try to read the interpreter line */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    interp = conn->ctx->config[CGI_INTERPRETER];
    if (interp == NULL) {
        buf[0] = buf[1] = '\0';

<<<<<<< HEAD
        // Read the first line of the script into the buffer
=======
        /* Read the first line of the script into the buffer */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        snprintf(cmdline, sizeof(cmdline), "%s%c%s", dir, '/', prog);
        if (mg_fopen(conn, cmdline, "r", &file)) {
            p = (char *) file.membuf;
            mg_fgets(buf, sizeof(buf), &file, &p);
            mg_fclose(&file);
            buf[sizeof(buf) - 1] = '\0';
        }

        if (buf[0] == '#' && buf[1] == '!') {
            trim_trailing_whitespaces(buf + 2);
        } else {
            buf[2] = '\0';
        }
        interp = buf + 2;
    }

    if (interp[0] != '\0') {
        GetFullPathNameA(interp, sizeof(full_interp), full_interp, NULL);
        interp = full_interp;
    }
    GetFullPathNameA(dir, sizeof(full_dir), full_dir, NULL);

    mg_snprintf(conn, cmdline, sizeof(cmdline), "%s%s\"%s\\%s\"",
                interp, interp[0] == '\0' ? "" : " ", full_dir, prog);

    DEBUG_TRACE(("Running [%s]", cmdline));
    if (CreateProcessA(NULL, cmdline, NULL, NULL, TRUE,
                       CREATE_NEW_PROCESS_GROUP, envblk, NULL, &si, &pi) == 0) {
        mg_cry(conn, "%s: CreateProcess(%s): %ld",
               __func__, cmdline, ERRNO);
        pi.hProcess = (pid_t) -1;
    }

    (void) CloseHandle(si.hStdOutput);
    (void) CloseHandle(si.hStdInput);
    if (pi.hThread != NULL)
        (void) CloseHandle(pi.hThread);

    return (pid_t) pi.hProcess;
}
<<<<<<< HEAD
#endif // !NO_CGI
=======
#endif /* !NO_CGI */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static int set_non_blocking_mode(SOCKET sock)
{
    unsigned long on = 1;
    return ioctlsocket(sock, FIONBIO, &on);
}

#else
static int mg_stat(struct mg_connection *conn, const char *path,
                   struct file *filep)
{
    struct stat st;

    if (!is_file_in_memory(conn, path, filep) && !stat(path, &st)) {
        filep->size = st.st_size;
        filep->modification_time = st.st_mtime;
        filep->is_directory = S_ISDIR(st.st_mode);
    } else {
        filep->modification_time = (time_t) 0;
    }

    return filep->membuf != NULL || filep->modification_time != (time_t) 0;
}

static void set_close_on_exec(int fd, struct mg_connection *conn /* may be null */)
{
    if (fcntl(fd, F_SETFD, FD_CLOEXEC) != 0) {
        if (conn)
            mg_cry(conn, "%s: fcntl(F_SETFD FD_CLOEXEC) failed: %s",
                   __func__, strerror(ERRNO));
    }
}

int mg_start_thread(mg_thread_func_t func, void *param)
{
    pthread_t thread_id;
    pthread_attr_t attr;
    int result;

    (void) pthread_attr_init(&attr);
    (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if defined(USE_STACK_SIZE) && USE_STACK_SIZE > 1
<<<<<<< HEAD
    // Compile-time option to control stack size, e.g. -DUSE_STACK_SIZE=16384
=======
    /* Compile-time option to control stack size,
       e.g. -DUSE_STACK_SIZE=16384 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    (void) pthread_attr_setstacksize(&attr, USE_STACK_SIZE);
#endif /* defined(USE_STACK_SIZE) && USE_STACK_SIZE > 1 */

    result = pthread_create(&thread_id, &attr, func, param);
    pthread_attr_destroy(&attr);

    return result;
}

/* Start a thread storing the thread context. */

static int mg_start_thread_with_id(mg_thread_func_t func, void *param,
                                   pthread_t *threadidptr)
{
    pthread_t thread_id;
    pthread_attr_t attr;
    int result;

    (void) pthread_attr_init(&attr);

#if defined(USE_STACK_SIZE) && USE_STACK_SIZE > 1
<<<<<<< HEAD
    // Compile-time option to control stack size, e.g. -DUSE_STACK_SIZE=16384
=======
    /* Compile-time option to control stack size,
       e.g. -DUSE_STACK_SIZE=16384 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    (void) pthread_attr_setstacksize(&attr, USE_STACK_SIZE);
#endif /* defined(USE_STACK_SIZE) && USE_STACK_SIZE > 1 */

    result = pthread_create(&thread_id, &attr, func, param);
    pthread_attr_destroy(&attr);
    if (threadidptr != NULL) {
        *threadidptr = thread_id;
    }
    return result;
}

/* Wait for a thread to finish. */

static int mg_join_thread(pthread_t threadid)
{
    int result;

    result = pthread_join(threadid, NULL);
    return result;
}

#ifndef NO_CGI
static pid_t spawn_process(struct mg_connection *conn, const char *prog,
                           char *envblk, char *envp[], int fdin,
                           int fdout, const char *dir)
{
    pid_t pid;
    const char *interp;

    (void) envblk;

    if ((pid = fork()) == -1) {
<<<<<<< HEAD
        // Parent
        send_http_error(conn, 500, http_500_error, "fork(): %s", strerror(ERRNO));
    } else if (pid == 0) {
        // Child
=======
        /* Parent */
        send_http_error(conn, 500, http_500_error, "fork(): %s", strerror(ERRNO));
    } else if (pid == 0) {
        /* Child */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (chdir(dir) != 0) {
            mg_cry(conn, "%s: chdir(%s): %s", __func__, dir, strerror(ERRNO));
        } else if (dup2(fdin, 0) == -1) {
            mg_cry(conn, "%s: dup2(%d, 0): %s", __func__, fdin, strerror(ERRNO));
        } else if (dup2(fdout, 1) == -1) {
            mg_cry(conn, "%s: dup2(%d, 1): %s", __func__, fdout, strerror(ERRNO));
        } else {
<<<<<<< HEAD
            // Not redirecting stderr to stdout, to avoid output being littered
            // with the error messages.
            (void) close(fdin);
            (void) close(fdout);

            // After exec, all signal handlers are restored to their default values,
            // with one exception of SIGCHLD. According to POSIX.1-2001 and Linux's
            // implementation, SIGCHLD's handler will leave unchanged after exec
            // if it was set to be ignored. Restore it to default action.
=======
            /* Not redirecting stderr to stdout, to avoid output being littered
               with the error messages. */
            (void) close(fdin);
            (void) close(fdout);

            /* After exec, all signal handlers are restored to their default
               values, with one exception of SIGCHLD. According to
               POSIX.1-2001 and Linux's implementation, SIGCHLD's handler will
               leave unchanged after exec if it was set to be ignored. Restore
               it to default action. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            signal(SIGCHLD, SIG_DFL);

            interp = conn->ctx->config[CGI_INTERPRETER];
            if (interp == NULL) {
                (void) execle(prog, prog, NULL, envp);
                mg_cry(conn, "%s: execle(%s): %s", __func__, prog, strerror(ERRNO));
            } else {
                (void) execle(interp, interp, prog, NULL, envp);
                mg_cry(conn, "%s: execle(%s %s): %s", __func__, interp, prog,
                       strerror(ERRNO));
            }
        }
        exit(EXIT_FAILURE);
    }

    return pid;
}
<<<<<<< HEAD
#endif // !NO_CGI
=======
#endif /* !NO_CGI */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static int set_non_blocking_mode(SOCKET sock)
{
    int flags;

    flags = fcntl(sock, F_GETFL, 0);
    (void) fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    return 0;
}
<<<<<<< HEAD
#endif // _WIN32

// Write data to the IO channel - opened file descriptor, socket or SSL
// descriptor. Return number of bytes written.
=======
#endif /* _WIN32 */

/* Write data to the IO channel - opened file descriptor, socket or SSL
   descriptor. Return number of bytes written. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int64_t push(FILE *fp, SOCKET sock, SSL *ssl, const char *buf,
                    int64_t len)
{
    int64_t sent;
    int n, k;

<<<<<<< HEAD
    (void) ssl;  // Get rid of warning
    sent = 0;
    while (sent < len) {

        // How many bytes we send in this iteration
=======
    (void) ssl;  /* Get rid of warning */
    sent = 0;
    while (sent < len) {

        /* How many bytes we send in this iteration */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        k = len - sent > INT_MAX ? INT_MAX : (int) (len - sent);

#ifndef NO_SSL
        if (ssl != NULL) {
            n = SSL_write(ssl, buf + sent, k);
        } else
#endif
            if (fp != NULL) {
                n = (int) fwrite(buf + sent, 1, (size_t) k, fp);
                if (ferror(fp))
                    n = -1;
            } else {
                n = send(sock, buf + sent, (size_t) k, MSG_NOSIGNAL);
            }

        if (n <= 0)
            break;

        sent += n;
    }

    return sent;
}

<<<<<<< HEAD
// Read from IO channel - opened file descriptor, socket, or SSL descriptor.
// Return negative value on error, or number of bytes read on success.
=======
/* Read from IO channel - opened file descriptor, socket, or SSL descriptor.
   Return negative value on error, or number of bytes read on success. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int pull(FILE *fp, struct mg_connection *conn, char *buf, int len)
{
    int nread;

    if (fp != NULL) {
<<<<<<< HEAD
        // Use read() instead of fread(), because if we're reading from the CGI
        // pipe, fread() may block until IO buffer is filled up. We cannot afford
        // to block and must pass all read bytes immediately to the client.
=======
        /* Use read() instead of fread(), because if we're reading from the
           CGI pipe, fread() may block until IO buffer is filled up. We cannot
           afford to block and must pass all read bytes immediately to the
           client. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        nread = read(fileno(fp), buf, (size_t) len);
#ifndef NO_SSL
    } else if (conn->ssl != NULL) {
        nread = SSL_read(conn->ssl, buf, len);
#endif
    } else {
        nread = recv(conn->client.sock, buf, (size_t) len, 0);
    }

    return conn->ctx->stop_flag ? -1 : nread;
}

static int pull_all(FILE *fp, struct mg_connection *conn, char *buf, int len)
{
    int n, nread = 0;

    while (len > 0 && conn->ctx->stop_flag == 0) {
        n = pull(fp, conn, buf + nread, len);
        if (n < 0) {
<<<<<<< HEAD
            nread = n;  // Propagate the error
            break;
        } else if (n == 0) {
            break;  // No more data to read
=======
            nread = n;  /* Propagate the error */
            break;
        } else if (n == 0) {
            break;  /* No more data to read */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        } else {
            conn->consumed_content += n;
            nread += n;
            len -= n;
        }
    }

    return nread;
}

int mg_read(struct mg_connection *conn, void *buf, size_t len)
{
    int n, buffered_len, nread;
    const char *body;

<<<<<<< HEAD
    // If Content-Length is not set, read until socket is closed
=======
    /* If Content-Length is not set, read until socket is closed */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (conn->consumed_content == 0 && conn->content_len == 0) {
        conn->content_len = INT64_MAX;
        conn->must_close = 1;
    }

    nread = 0;
    if (conn->consumed_content < conn->content_len) {
<<<<<<< HEAD
        // Adjust number of bytes to read.
=======
        /* Adjust number of bytes to read. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        int64_t to_read = conn->content_len - conn->consumed_content;
        if (to_read < (int64_t) len) {
            len = (size_t) to_read;
        }

<<<<<<< HEAD
        // Return buffered data
=======
        /* Return buffered data */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        body = conn->buf + conn->request_len + conn->consumed_content;
        buffered_len = (int)(&conn->buf[conn->data_len] - body);
        if (buffered_len > 0) {
            if (len < (size_t) buffered_len) {
                buffered_len = (int) len;
            }
            memcpy(buf, body, (size_t) buffered_len);
            len -= buffered_len;
            conn->consumed_content += buffered_len;
            nread += buffered_len;
            buf = (char *) buf + buffered_len;
        }

<<<<<<< HEAD
        // We have returned all buffered data. Read new data from the remote socket.
=======
        /* We have returned all buffered data. Read new data from the remote
           socket. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        n = pull_all(NULL, conn, (char *) buf, (int) len);
        nread = n >= 0 ? nread + n : n;
    }
    return nread;
}

int mg_write(struct mg_connection *conn, const void *buf, size_t len)
{
    time_t now;
    int64_t n, total, allowed;

    if (conn->throttle > 0) {
        if ((now = time(NULL)) != conn->last_throttle_time) {
            conn->last_throttle_time = now;
            conn->last_throttle_bytes = 0;
        }
        allowed = conn->throttle - conn->last_throttle_bytes;
        if (allowed > (int64_t) len) {
            allowed = len;
        }
        if ((total = push(NULL, conn->client.sock, conn->ssl, (const char *) buf,
                          (int64_t) allowed)) == allowed) {
            buf = (char *) buf + total;
            conn->last_throttle_bytes += total;
            while (total < (int64_t) len && conn->ctx->stop_flag == 0) {
                allowed = conn->throttle > (int64_t) len - total ?
                          (int64_t) len - total : conn->throttle;
                if ((n = push(NULL, conn->client.sock, conn->ssl, (const char *) buf,
                              (int64_t) allowed)) != allowed) {
                    break;
                }
                sleep(1);
                conn->last_throttle_bytes = allowed;
                conn->last_throttle_time = time(NULL);
                buf = (char *) buf + n;
                total += n;
            }
        }
    } else {
        total = push(NULL, conn->client.sock, conn->ssl, (const char *) buf,
                     (int64_t) len);
    }
    return (int) total;
}

<<<<<<< HEAD
// Alternative alloc_vprintf() for non-compliant C runtimes
=======
/* Alternative alloc_vprintf() for non-compliant C runtimes */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int alloc_vprintf2(char **buf, const char *fmt, va_list ap)
{
    va_list ap_copy;
    int size = MG_BUF_LEN;
    int len = -1;

    *buf = NULL;
    while (len == -1) {
        if (*buf) free(*buf);
        *buf = (char *)malloc(size *= 4);
        if (!*buf) break;
        va_copy(ap_copy, ap);
        len = vsnprintf(*buf, size, fmt, ap_copy);
        va_end(ap_copy);
    }

    return len;
}

<<<<<<< HEAD
// Print message to buffer. If buffer is large enough to hold the message,
// return buffer. If buffer is to small, allocate large enough buffer on heap,
// and return allocated buffer.
=======
/* Print message to buffer. If buffer is large enough to hold the message,
   return buffer. If buffer is to small, allocate large enough buffer on heap,
   and return allocated buffer. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int alloc_vprintf(char **buf, size_t size, const char *fmt, va_list ap)
{
    va_list ap_copy;
    int len;

<<<<<<< HEAD
    // Windows is not standard-compliant, and vsnprintf() returns -1 if
    // buffer is too small. Also, older versions of msvcrt.dll do not have
    // _vscprintf().  However, if size is 0, vsnprintf() behaves correctly.
    // Therefore, we make two passes: on first pass, get required message length.
    // On second pass, actually print the message.
=======
    /* Windows is not standard-compliant, and vsnprintf() returns -1 if
       buffer is too small. Also, older versions of msvcrt.dll do not have
       _vscprintf().  However, if size is 0, vsnprintf() behaves correctly.
       Therefore, we make two passes: on first pass, get required message
       length.
       On second pass, actually print the message. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    va_copy(ap_copy, ap);
    len = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);

    if (len < 0) {
<<<<<<< HEAD
        // C runtime is not standard compliant, vsnprintf() returned -1.
        // Switch to alternative code path that uses incremental allocations.
=======
        /* C runtime is not standard compliant, vsnprintf() returned -1.
           Switch to alternative code path that uses incremental allocations.
        */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        va_copy(ap_copy, ap);
        len = alloc_vprintf2(buf, fmt, ap);
        va_end(ap_copy);
    } else if (len > (int) size &&
               (size = len + 1) > 0 &&
               (*buf = (char *) malloc(size)) == NULL) {
<<<<<<< HEAD
        len = -1;  // Allocation failed, mark failure
=======
        len = -1;  /* Allocation failed, mark failure */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    } else {
        va_copy(ap_copy, ap);
        IGNORE_UNUSED_RESULT(vsnprintf(*buf, size, fmt, ap_copy));
        va_end(ap_copy);
    }

    return len;
}

int mg_vprintf(struct mg_connection *conn, const char *fmt, va_list ap);

int mg_vprintf(struct mg_connection *conn, const char *fmt, va_list ap)
{
    char mem[MG_BUF_LEN], *buf = mem;
    int len;

    if ((len = alloc_vprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
        len = mg_write(conn, buf, (size_t) len);
    }
    if (buf != mem && buf != NULL) {
        free(buf);
    }

    return len;
}

int mg_printf(struct mg_connection *conn, const char *fmt, ...)
{
    va_list ap;
    int result;

    va_start(ap, fmt);
    result = mg_vprintf(conn, fmt, ap);
    va_end(ap);

    return result;
}

int mg_url_decode(const char *src, int src_len, char *dst,
                  int dst_len, int is_form_url_encoded)
{
    int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

    for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
        if (src[i] == '%' && i < src_len - 2 &&
            isxdigit(* (const unsigned char *) (src + i + 1)) &&
            isxdigit(* (const unsigned char *) (src + i + 2))) {
            a = tolower(* (const unsigned char *) (src + i + 1));
            b = tolower(* (const unsigned char *) (src + i + 2));
            dst[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
            i += 2;
        } else if (is_form_url_encoded && src[i] == '+') {
            dst[j] = ' ';
        } else {
            dst[j] = src[i];
        }
    }

<<<<<<< HEAD
    dst[j] = '\0'; // Null-terminate the destination
=======
    dst[j] = '\0'; /* Null-terminate the destination */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

    return i >= src_len ? j : -1;
}

int mg_get_var(const char *data, size_t data_len, const char *name,
               char *dst, size_t dst_len)
{
    return mg_get_var2(data,data_len,name,dst,dst_len,0);
}

int mg_get_var2(const char *data, size_t data_len, const char *name,
                char *dst, size_t dst_len, size_t occurrence)
{
    const char *p, *e, *s;
    size_t name_len;
    int len;

    if (dst == NULL || dst_len == 0) {
        len = -2;
    } else if (data == NULL || name == NULL || data_len == 0) {
        len = -1;
        dst[0] = '\0';
    } else {
        name_len = strlen(name);
        e = data + data_len;
        len = -1;
        dst[0] = '\0';

<<<<<<< HEAD
        // data is "var1=val1&var2=val2...". Find variable first
=======
        /* data is "var1=val1&var2=val2...". Find variable first */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        for (p = data; p + name_len < e; p++) {
            if ((p == data || p[-1] == '&') && p[name_len] == '=' &&
                !mg_strncasecmp(name, p, name_len) && 0 == occurrence--) {

<<<<<<< HEAD
                // Point p to variable value
                p += name_len + 1;

                // Point s to the end of the value
=======
                /* Point p to variable value */
                p += name_len + 1;

                /* Point s to the end of the value */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                s = (const char *) memchr(p, '&', (size_t)(e - p));
                if (s == NULL) {
                    s = e;
                }
                assert(s >= p);

<<<<<<< HEAD
                // Decode variable into destination buffer
                len = mg_url_decode(p, (int)(s - p), dst, (int)dst_len, 1);

                // Redirect error code from -1 to -2 (destination buffer too small).
=======
                /* Decode variable into destination buffer */
                len = mg_url_decode(p, (int)(s - p), dst, (int)dst_len, 1);

                /* Redirect error code from -1 to -2 (destination buffer too
                   small). */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                if (len == -1) {
                    len = -2;
                }
                break;
            }
        }
    }

    return len;
}

int mg_get_cookie(const char *cookie_header, const char *var_name,
                  char *dst, size_t dst_size)
{
    const char *s, *p, *end;
    int name_len, len = -1;

    if (dst == NULL || dst_size == 0) {
        len = -2;
    } else if (var_name == NULL || (s = cookie_header) == NULL) {
        len = -1;
        dst[0] = '\0';
    } else {
        name_len = (int) strlen(var_name);
        end = s + strlen(s);
        dst[0] = '\0';

        for (; (s = mg_strcasestr(s, var_name)) != NULL; s += name_len) {
            if (s[name_len] == '=') {
                s += name_len + 1;
                if ((p = strchr(s, ' ')) == NULL)
                    p = end;
                if (p[-1] == ';')
                    p--;
                if (*s == '"' && p[-1] == '"' && p > s + 1) {
                    s++;
                    p--;
                }
                if ((size_t) (p - s) < dst_size) {
                    len = (int)(p - s);
                    mg_strlcpy(dst, s, (size_t) len + 1);
                } else {
                    len = -3;
                }
                break;
            }
        }
    }
    return len;
}

static void convert_uri_to_file_name(struct mg_connection *conn, char *buf,
                                     size_t buf_len, struct file *filep)
{
    struct vec a, b;
    const char *rewrite, *uri = conn->request_info.uri,
                          *root = conn->ctx->config[DOCUMENT_ROOT];
    char *p;
    int match_len;
    char gz_path[PATH_MAX];
    char const* accept_encoding;

<<<<<<< HEAD
    // Using buf_len - 1 because memmove() for PATH_INFO may shift part
    // of the path one byte on the right.
    // If document_root is NULL, leave the file empty.
=======
    /* Using buf_len - 1 because memmove() for PATH_INFO may shift part
       of the path one byte on the right.
       If document_root is NULL, leave the file empty. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    mg_snprintf(conn, buf, buf_len - 1, "%s%s",
                root == NULL ? "" : root,
                root == NULL ? "" : uri);

    rewrite = conn->ctx->config[REWRITE];
    while ((rewrite = next_option(rewrite, &a, &b)) != NULL) {
        if ((match_len = match_prefix(a.ptr, (int) a.len, uri)) > 0) {
            mg_snprintf(conn, buf, buf_len - 1, "%.*s%s", (int) b.len, b.ptr,
                        uri + match_len);
            break;
        }
    }

    if (mg_stat(conn, buf, filep)) return;

<<<<<<< HEAD
    // if we can't find the actual file, look for the file
    // with the same name but a .gz extension. If we find it,
    // use that and set the gzipped flag in the file struct
    // to indicate that the response need to have the content-
    // encoding: gzip header
    // we can only do this if the browser declares support
=======
    /* if we can't find the actual file, look for the file
       with the same name but a .gz extension. If we find it,
       use that and set the gzipped flag in the file struct
       to indicate that the response need to have the content-
       encoding: gzip header
       we can only do this if the browser declares support */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if ((accept_encoding = mg_get_header(conn, "Accept-Encoding")) != NULL) {
        if (strstr(accept_encoding,"gzip") != NULL) {
            snprintf(gz_path, sizeof(gz_path), "%s.gz", buf);
            if (mg_stat(conn, gz_path, filep)) {
                filep->gzipped = 1;
                return;
            }
        }
    }

<<<<<<< HEAD
    // Support PATH_INFO for CGI scripts.
=======
    /* Support PATH_INFO for CGI scripts. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    for (p = buf + strlen(buf); p > buf + 1; p--) {
        if (*p == '/') {
            *p = '\0';
            if (match_prefix(conn->ctx->config[CGI_EXTENSIONS],
                             (int)strlen(conn->ctx->config[CGI_EXTENSIONS]), buf) > 0 &&
                mg_stat(conn, buf, filep)) {
<<<<<<< HEAD
                // Shift PATH_INFO block one character right, e.g.
                //  "/x.cgi/foo/bar\x00" => "/x.cgi\x00/foo/bar\x00"
                // conn->path_info is pointing to the local variable "path" declared
                // in handle_request(), so PATH_INFO is not valid after
                // handle_request returns.
                conn->path_info = p + 1;
                memmove(p + 2, p + 1, strlen(p + 1) + 1);  // +1 is for trailing \0
=======
                /* Shift PATH_INFO block one character right, e.g.
                    "/x.cgi/foo/bar\x00" => "/x.cgi\x00/foo/bar\x00"
                   conn->path_info is pointing to the local variable "path"
                   declared in handle_request(), so PATH_INFO is not valid
                   after handle_request returns. */
                conn->path_info = p + 1;
                memmove(p + 2, p + 1, strlen(p + 1) + 1);  /* +1 is for
                                                              trailing \0 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                p[1] = '/';
                break;
            } else {
                *p = '/';
            }
        }
    }
}

<<<<<<< HEAD
// Check whether full request is buffered. Return:
//   -1  if request is malformed
//    0  if request is not yet fully buffered
//   >0  actual request length, including last \r\n\r\n
=======
/* Check whether full request is buffered. Return:
     -1  if request is malformed
      0  if request is not yet fully buffered
     >0  actual request length, including last \r\n\r\n */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int get_request_len(const char *buf, int buflen)
{
    const char *s, *e;
    int len = 0;

    for (s = buf, e = s + buflen - 1; len <= 0 && s < e; s++)
<<<<<<< HEAD
        // Control characters are not allowed but >=128 is.
        if (!isprint(* (const unsigned char *) s) && *s != '\r' &&
            *s != '\n' && * (const unsigned char *) s < 128) {
            len = -1;
            break;  // [i_a] abort scan as soon as one malformed character is found;
            // don't let subsequent \r\n\r\n win us over anyhow
=======
        /* Control characters are not allowed but >=128 is. */
        if (!isprint(* (const unsigned char *) s) && *s != '\r' &&
            *s != '\n' && * (const unsigned char *) s < 128) {
            len = -1;
            break;  /* [i_a] abort scan as soon as one malformed character is
                       found; */
            /* don't let subsequent \r\n\r\n win us over anyhow */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        } else if (s[0] == '\n' && s[1] == '\n') {
            len = (int) (s - buf) + 2;
        } else if (s[0] == '\n' && &s[1] < e &&
                   s[1] == '\r' && s[2] == '\n') {
            len = (int) (s - buf) + 3;
        }

    return len;
}

<<<<<<< HEAD
// Convert month to the month number. Return -1 on error, or month number
=======
/* Convert month to the month number. Return -1 on error, or month number */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int get_month_index(const char *s)
{
    size_t i;

    for (i = 0; i < ARRAY_SIZE(month_names); i++)
        if (!strcmp(s, month_names[i]))
            return (int) i;

    return -1;
}

static int num_leap_years(int year)
{
    return year / 4 - year / 100 + year / 400;
}

<<<<<<< HEAD
// Parse UTC date-time string, and return the corresponding time_t value.
=======
/* Parse UTC date-time string, and return the corresponding time_t value. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static time_t parse_date_string(const char *datetime)
{
    static const unsigned short days_before_month[] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };
    char month_str[32];
    int second, minute, hour, day, month, year, leap_days, days;
    time_t result = (time_t) 0;

    if (((sscanf(datetime, "%d/%3s/%d %d:%d:%d",
                 &day, month_str, &year, &hour, &minute, &second) == 6) ||
         (sscanf(datetime, "%d %3s %d %d:%d:%d",
                 &day, month_str, &year, &hour, &minute, &second) == 6) ||
         (sscanf(datetime, "%*3s, %d %3s %d %d:%d:%d",
                 &day, month_str, &year, &hour, &minute, &second) == 6) ||
         (sscanf(datetime, "%d-%3s-%d %d:%d:%d",
                 &day, month_str, &year, &hour, &minute, &second) == 6)) &&
        year > 1970 &&
        (month = get_month_index(month_str)) != -1) {
        leap_days = num_leap_years(year) - num_leap_years(1970);
        year -= 1970;
        days = year * 365 + days_before_month[month] + (day - 1) + leap_days;
        result = (time_t) days * 24 * 3600 + (time_t) hour * 3600 +
                 minute * 60 + second;
    }

    return result;
}

<<<<<<< HEAD
// Protect against directory disclosure attack by removing '..',
// excessive '/' and '\' characters
=======
/* Protect against directory disclosure attack by removing '..',
   excessive '/' and '\' characters */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void remove_double_dots_and_double_slashes(char *s)
{
    char *p = s;

    while (*s != '\0') {
        *p++ = *s++;
        if (s[-1] == '/' || s[-1] == '\\') {
<<<<<<< HEAD
            // Skip all following slashes, backslashes and double-dots
=======
            /* Skip all following slashes, backslashes and double-dots */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            while (s[0] != '\0') {
                if (s[0] == '/' || s[0] == '\\') {
                    s++;
                } else if (s[0] == '.' && s[1] == '.') {
                    s += 2;
                } else {
                    break;
                }
            }
        }
    }
    *p = '\0';
}

static const struct {
    const char *extension;
    size_t ext_len;
    const char *mime_type;
} builtin_mime_types[] = {
<<<<<<< HEAD
    {".html", 5, "text/html"},
    {".htm", 4, "text/html"},
    {".shtm", 5, "text/html"},
    {".shtml", 6, "text/html"},
    {".css", 4, "text/css"},
    {".js",  3, "application/x-javascript"},
    {".ico", 4, "image/x-icon"},
    {".gif", 4, "image/gif"},
    {".jpg", 4, "image/jpeg"},
    {".jpeg", 5, "image/jpeg"},
    {".png", 4, "image/png"},
    {".svg", 4, "image/svg+xml"},
    {".txt", 4, "text/plain"},
    {".torrent", 8, "application/x-bittorrent"},
    {".wav", 4, "audio/x-wav"},
    {".mp3", 4, "audio/x-mp3"},
    {".mid", 4, "audio/mid"},
    {".m3u", 4, "audio/x-mpegurl"},
    {".ogg", 4, "audio/ogg"},
    {".ram", 4, "audio/x-pn-realaudio"},
    {".xml", 4, "text/xml"},
    {".json",  5, "text/json"},
    {".xslt", 5, "application/xml"},
    {".xsl", 4, "application/xml"},
    {".ra",  3, "audio/x-pn-realaudio"},
    {".doc", 4, "application/msword"},
    {".exe", 4, "application/octet-stream"},
    {".zip", 4, "application/x-zip-compressed"},
    {".xls", 4, "application/excel"},
    {".tgz", 4, "application/x-tar-gz"},
    {".tar", 4, "application/x-tar"},
    {".gz",  3, "application/x-gunzip"},
    {".arj", 4, "application/x-arj-compressed"},
    {".rar", 4, "application/x-arj-compressed"},
    {".rtf", 4, "application/rtf"},
    {".pdf", 4, "application/pdf"},
    {".swf", 4, "application/x-shockwave-flash"},
    {".mpg", 4, "video/mpeg"},
    {".webm", 5, "video/webm"},
    {".mpeg", 5, "video/mpeg"},
    {".mov", 4, "video/quicktime"},
    {".mp4", 4, "video/mp4"},
    {".m4v", 4, "video/x-m4v"},
    {".asf", 4, "video/x-ms-asf"},
    {".avi", 4, "video/x-msvideo"},
    {".bmp", 4, "image/bmp"},
    {".ttf", 4, "application/x-font-ttf"},
    {NULL,  0, NULL}
=======
    /* IANA registered MIME types (http://www.iana.org/assignments/media-types)
       application types */
    {".doc",     4, "application/msword"},
    {".eps",     4, "application/postscript"},
    {".exe",     4, "application/octet-stream"},
    {".js",      3, "application/javascript"},
    {".json",    5, "application/json"},
    {".pdf",     4, "application/pdf"},
    {".ps",      3, "application/postscript"},
    {".rtf",     4, "application/rtf"},
    {".xhtml",   6, "application/xhtml+xml"},
    {".xsl",     4, "application/xml"},
    {".xslt",    5, "application/xml"},

    /* audio */
    {".mp3",     4, "audio/mpeg"},
    {".oga",     4, "audio/ogg"},
    {".ogg",     4, "audio/ogg"},

    /* image */
    {".gif",     4, "image/gif"},
    {".ief",     4, "image/ief"},
    {".jpeg",    5, "image/jpeg"},
    {".jpg",     4, "image/jpeg"},
    {".jpm",     4, "image/jpm"},
    {".jpx",     4, "image/jpx"},
    {".png",     4, "image/png"},
    {".svg",     4, "image/svg+xml"},
    {".tif",     4, "image/tiff"},
    {".tiff",    5, "image/tiff"},

    /* model */
    {".wrl",     4, "model/vrml"},

    /* text */
    {".css",     4, "text/css"},
    {".csv",     4, "text/csv"},
    {".htm",     4, "text/html"},
    {".html",    5, "text/html"},
    {".sgm",     4, "text/sgml"},
    {".shtm",    5, "text/html"},
    {".shtml",   6, "text/html"},
    {".txt",     4, "text/plain"},
    {".xml",     4, "text/xml"},

    /* video */
    {".mov",     4, "video/quicktime"},
    {".mp4",     4, "video/mp4"},
    {".mpeg",    5, "video/mpeg"},
    {".mpg",     4, "video/mpeg"},
    {".ogv",     4, "video/ogg"},
    {".qt",      3, "video/quicktime"},

    /* not registered types
       (http://reference.sitepoint.com/html/mime-types-full,
        http://www.hansenb.pdx.edu/DMKB/dict/tutorials/mime_typ.php, ..) */
    {".arj",     4, "application/x-arj-compressed"},
    {".gz",      3, "application/x-gunzip"},
    {".rar",     4, "application/x-arj-compressed"},
    {".swf",     4, "application/x-shockwave-flash"},
    {".tar",     4, "application/x-tar"},
    {".tgz",     4, "application/x-tar-gz"},
    {".torrent", 8, "application/x-bittorrent"},
    {".ppt",     4, "application/x-mspowerpoint"},
    {".xls",     4, "application/x-msexcel"},
    {".zip",     4, "application/x-zip-compressed"},
    {".aac",     4, "audio/aac"}, /* http://en.wikipedia.org/wiki/Advanced_Audio_Coding */
    {".aif",     4, "audio/x-aif"},
    {".m3u",     4, "audio/x-mpegurl"},
    {".mid",     4, "audio/x-midi"},
    {".ra",      3, "audio/x-pn-realaudio"},
    {".ram",     4, "audio/x-pn-realaudio"},
    {".wav",     4, "audio/x-wav"},
    {".bmp",     4, "image/bmp"},
    {".ico",     4, "image/x-icon"},
    {".pct",     4, "image/x-pct"},
    {".pict",    5, "image/pict"},
    {".rgb",     4, "image/x-rgb"},
    {".webm",    5, "video/webm"}, /* http://en.wikipedia.org/wiki/WebM */
    {".asf",     4, "video/x-ms-asf"},
    {".avi",     4, "video/x-msvideo"},
    {".m4v",     4, "video/x-m4v"},
    {NULL, 0, NULL}
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
};

const char *mg_get_builtin_mime_type(const char *path)
{
    const char *ext;
    size_t i, path_len;

    path_len = strlen(path);

    for (i = 0; builtin_mime_types[i].extension != NULL; i++) {
        ext = path + (path_len - builtin_mime_types[i].ext_len);
        if (path_len > builtin_mime_types[i].ext_len &&
            mg_strcasecmp(ext, builtin_mime_types[i].extension) == 0) {
            return builtin_mime_types[i].mime_type;
        }
    }

    return "text/plain";
}

<<<<<<< HEAD
// Look at the "path" extension and figure what mime type it has.
// Store mime type in the vector.
=======
/* Look at the "path" extension and figure what mime type it has.
   Store mime type in the vector. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void get_mime_type(struct mg_context *ctx, const char *path,
                          struct vec *vec)
{
    struct vec ext_vec, mime_vec;
    const char *list, *ext;
    size_t path_len;

    path_len = strlen(path);

<<<<<<< HEAD
    // Scan user-defined mime types first, in case user wants to
    // override default mime types.
    list = ctx->config[EXTRA_MIME_TYPES];
    while ((list = next_option(list, &ext_vec, &mime_vec)) != NULL) {
        // ext now points to the path suffix
=======
    /* Scan user-defined mime types first, in case user wants to
       override default mime types. */
    list = ctx->config[EXTRA_MIME_TYPES];
    while ((list = next_option(list, &ext_vec, &mime_vec)) != NULL) {
        /* ext now points to the path suffix */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        ext = path + path_len - ext_vec.len;
        if (mg_strncasecmp(ext, ext_vec.ptr, ext_vec.len) == 0) {
            *vec = mime_vec;
            return;
        }
    }

    vec->ptr = mg_get_builtin_mime_type(path);
    vec->len = strlen(vec->ptr);
}

<<<<<<< HEAD
// Stringify binary data. Output buffer must be twice as big as input,
// because each byte takes 2 bytes in string representation
=======
/* Stringify binary data. Output buffer must be twice as big as input,
   because each byte takes 2 bytes in string representation */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void bin2str(char *to, const unsigned char *p, size_t len)
{
    static const char *hex = "0123456789abcdef";

    for (; len--; p++) {
        *to++ = hex[p[0] >> 4];
        *to++ = hex[p[0] & 0x0f];
    }
    *to = '\0';
}

<<<<<<< HEAD
// Return stringified MD5 hash for list of strings. Buffer must be 33 bytes.
=======
/* Return stringified MD5 hash for list of strings. Buffer must be 33 bytes. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
char *mg_md5(char buf[33], ...)
{
    md5_byte_t hash[16];
    const char *p;
    va_list ap;
    md5_state_t ctx;

    md5_init(&ctx);

    va_start(ap, buf);
    while ((p = va_arg(ap, const char *)) != NULL) {
        md5_append(&ctx, (const md5_byte_t *) p, (int) strlen(p));
    }
    va_end(ap);

    md5_finish(&ctx, hash);
    bin2str(buf, hash, sizeof(hash));
    return buf;
}

<<<<<<< HEAD
// Check the user's password, return 1 if OK
=======
/* Check the user's password, return 1 if OK */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int check_password(const char *method, const char *ha1, const char *uri,
                          const char *nonce, const char *nc, const char *cnonce,
                          const char *qop, const char *response)
{
    char ha2[32 + 1], expected_response[32 + 1];

<<<<<<< HEAD
    // Some of the parameters may be NULL
=======
    /* Some of the parameters may be NULL */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (method == NULL || nonce == NULL || nc == NULL || cnonce == NULL ||
        qop == NULL || response == NULL) {
        return 0;
    }

<<<<<<< HEAD
    // NOTE(lsm): due to a bug in MSIE, we do not compare the URI
    // TODO(lsm): check for authentication timeout
    if (// strcmp(dig->uri, c->ouri) != 0 ||
        strlen(response) != 32
        // || now - strtoul(dig->nonce, NULL, 10) > 3600
=======
    /* NOTE(lsm): due to a bug in MSIE, we do not compare the URI */
    /* TODO(lsm): check for authentication timeout */
    if (/* strcmp(dig->uri, c->ouri) != 0 || */
        strlen(response) != 32
        /* || now - strtoul(dig->nonce, NULL, 10) > 3600 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    ) {
        return 0;
    }

    mg_md5(ha2, method, ":", uri, NULL);
    mg_md5(expected_response, ha1, ":", nonce, ":", nc,
           ":", cnonce, ":", qop, ":", ha2, NULL);

    return mg_strcasecmp(response, expected_response) == 0;
}

<<<<<<< HEAD
// Use the global passwords file, if specified by auth_gpass option,
// or search for .htpasswd in the requested directory.
=======
/* Use the global passwords file, if specified by auth_gpass option,
   or search for .htpasswd in the requested directory. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void open_auth_file(struct mg_connection *conn, const char *path,
                           struct file *filep)
{
    char name[PATH_MAX];
    const char *p, *e, *gpass = conn->ctx->config[GLOBAL_PASSWORDS_FILE];
    struct file file = STRUCT_FILE_INITIALIZER;

    if (gpass != NULL) {
<<<<<<< HEAD
        // Use global passwords file
=======
        /* Use global passwords file */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (!mg_fopen(conn, gpass, "r", filep)) {
#ifdef DEBUG
            mg_cry(conn, "fopen(%s): %s", gpass, strerror(ERRNO));
#endif
        }
<<<<<<< HEAD
        // Important: using local struct file to test path for is_directory flag.
        // If filep is used, mg_stat() makes it appear as if auth file was opened.
=======
        /* Important: using local struct file to test path for is_directory
           flag.
           If filep is used, mg_stat() makes it appear as if auth file was
           opened. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    } else if (mg_stat(conn, path, &file) && file.is_directory) {
        mg_snprintf(conn, name, sizeof(name), "%s%c%s",
                    path, '/', PASSWORDS_FILE_NAME);
        if (!mg_fopen(conn, name, "r", filep)) {
#ifdef DEBUG
            mg_cry(conn, "fopen(%s): %s", name, strerror(ERRNO));
#endif
        }
    } else {
<<<<<<< HEAD
        // Try to find .htpasswd in requested directory.
=======
        /* Try to find .htpasswd in requested directory. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        for (p = path, e = p + strlen(p) - 1; e > p; e--)
            if (e[0] == '/')
                break;
        mg_snprintf(conn, name, sizeof(name), "%.*s%c%s",
                    (int) (e - p), p, '/', PASSWORDS_FILE_NAME);
        if (!mg_fopen(conn, name, "r", filep)) {
#ifdef DEBUG
            mg_cry(conn, "fopen(%s): %s", name, strerror(ERRNO));
#endif
        }
    }
}

<<<<<<< HEAD
// Parsed Authorization header
=======
/* Parsed Authorization header */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
struct ah {
    char *user, *uri, *cnonce, *response, *qop, *nc, *nonce;
};

<<<<<<< HEAD
// Return 1 on success. Always initializes the ah structure.
=======
/* Return 1 on success. Always initializes the ah structure. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int parse_auth_header(struct mg_connection *conn, char *buf,
                             size_t buf_size, struct ah *ah)
{
    char *name, *value, *s;
    const char *auth_header;

    (void) memset(ah, 0, sizeof(*ah));
    if ((auth_header = mg_get_header(conn, "Authorization")) == NULL ||
        mg_strncasecmp(auth_header, "Digest ", 7) != 0) {
        return 0;
    }

<<<<<<< HEAD
    // Make modifiable copy of the auth header
    (void) mg_strlcpy(buf, auth_header + 7, buf_size);
    s = buf;

    // Parse authorization header
    for (;;) {
        // Gobble initial spaces
=======
    /* Make modifiable copy of the auth header */
    (void) mg_strlcpy(buf, auth_header + 7, buf_size);
    s = buf;

    /* Parse authorization header */
    for (;;) {
        /* Gobble initial spaces */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        while (isspace(* (unsigned char *) s)) {
            s++;
        }
        name = skip_quoted(&s, "=", " ", 0);
<<<<<<< HEAD
        // Value is either quote-delimited, or ends at first comma or space.
=======
        /* Value is either quote-delimited, or ends at first comma or space. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (s[0] == '\"') {
            s++;
            value = skip_quoted(&s, "\"", " ", '\\');
            if (s[0] == ',') {
                s++;
            }
        } else {
<<<<<<< HEAD
            value = skip_quoted(&s, ", ", " ", 0);  // IE uses commas, FF uses spaces
=======
            value = skip_quoted(&s, ", ", " ", 0);  /* IE uses commas, FF uses
                                                       spaces */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        }
        if (*name == '\0') {
            break;
        }

        if (!strcmp(name, "username")) {
            ah->user = value;
        } else if (!strcmp(name, "cnonce")) {
            ah->cnonce = value;
        } else if (!strcmp(name, "response")) {
            ah->response = value;
        } else if (!strcmp(name, "uri")) {
            ah->uri = value;
        } else if (!strcmp(name, "qop")) {
            ah->qop = value;
        } else if (!strcmp(name, "nc")) {
            ah->nc = value;
        } else if (!strcmp(name, "nonce")) {
            ah->nonce = value;
        }
    }

<<<<<<< HEAD
    // CGI needs it as REMOTE_USER
=======
    /* CGI needs it as REMOTE_USER */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (ah->user != NULL) {
        conn->request_info.remote_user = mg_strdup(ah->user);
    } else {
        return 0;
    }

    return 1;
}

static char *mg_fgets(char *buf, size_t size, struct file *filep, char **p)
{
    char *eof;
    size_t len;
    char *memend;

    if (filep->membuf != NULL && *p != NULL) {
        memend = (char *) &filep->membuf[filep->size];
<<<<<<< HEAD
        eof = (char *) memchr(*p, '\n', memend - *p); // Search for \n from p till the end of stream
        if (eof != NULL) {
            eof += 1; // Include \n
        } else {
            eof = memend; // Copy remaining data
=======
        eof = (char *) memchr(*p, '\n', memend - *p); /* Search for \n from p
                                                         till the end of
                                                         stream */
        if (eof != NULL) {
            eof += 1; /* Include \n */
        } else {
            eof = memend; /* Copy remaining data */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        }
        len = (size_t) (eof - *p) > size - 1 ? size - 1 : (size_t) (eof - *p);
        memcpy(buf, *p, len);
        buf[len] = '\0';
        *p += len;
        return len ? eof : NULL;
    } else if (filep->fp != NULL) {
        return fgets(buf, (int)size, filep->fp);
    } else {
        return NULL;
    }
}

<<<<<<< HEAD
// Authorize against the opened passwords file. Return 1 if authorized.
=======
/* Authorize against the opened passwords file. Return 1 if authorized. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int authorize(struct mg_connection *conn, struct file *filep)
{
    struct ah ah;
    char line[256], f_user[256] = "", ha1[256] = "", f_domain[256] = "", buf[MG_BUF_LEN], *p;

    if (!parse_auth_header(conn, buf, sizeof(buf), &ah)) {
        return 0;
    }

<<<<<<< HEAD
    // Loop over passwords file
=======
    /* Loop over passwords file */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    p = (char *) filep->membuf;
    while (mg_fgets(line, sizeof(line), filep, &p) != NULL) {
        if (sscanf(line, "%[^:]:%[^:]:%s", f_user, f_domain, ha1) != 3) {
            continue;
        }

        if (!strcmp(ah.user, f_user) &&
            !strcmp(conn->ctx->config[AUTHENTICATION_DOMAIN], f_domain))
            return check_password(conn->request_info.request_method, ha1, ah.uri,
                                  ah.nonce, ah.nc, ah.cnonce, ah.qop, ah.response);
    }

    return 0;
}

<<<<<<< HEAD
// Return 1 if request is authorised, 0 otherwise.
=======
/* Return 1 if request is authorised, 0 otherwise. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int check_authorization(struct mg_connection *conn, const char *path)
{
    char fname[PATH_MAX];
    struct vec uri_vec, filename_vec;
    const char *list;
    struct file file = STRUCT_FILE_INITIALIZER;
    int authorized = 1;

    list = conn->ctx->config[PROTECT_URI];
    while ((list = next_option(list, &uri_vec, &filename_vec)) != NULL) {
        if (!memcmp(conn->request_info.uri, uri_vec.ptr, uri_vec.len)) {
            mg_snprintf(conn, fname, sizeof(fname), "%.*s",
                        (int) filename_vec.len, filename_vec.ptr);
            if (!mg_fopen(conn, fname, "r", &file)) {
                mg_cry(conn, "%s: cannot open %s: %s", __func__, fname, strerror(errno));
            }
            break;
        }
    }

    if (!is_file_opened(&file)) {
        open_auth_file(conn, path, &file);
    }

    if (is_file_opened(&file)) {
        authorized = authorize(conn, &file);
        mg_fclose(&file);
    }

    return authorized;
}

static void send_authorization_request(struct mg_connection *conn)
{
    conn->status_code = 401;
    mg_printf(conn,
              "HTTP/1.1 401 Unauthorized\r\n"
              "Content-Length: 0\r\n"
              "WWW-Authenticate: Digest qop=\"auth\", "
              "realm=\"%s\", nonce=\"%lu\"\r\n\r\n",
              conn->ctx->config[AUTHENTICATION_DOMAIN],
              (unsigned long) time(NULL));
}

static int is_authorized_for_put(struct mg_connection *conn)
{
    struct file file = STRUCT_FILE_INITIALIZER;
    const char *passfile = conn->ctx->config[PUT_DELETE_PASSWORDS_FILE];
    int ret = 0;

    if (passfile != NULL && mg_fopen(conn, passfile, "r", &file)) {
        ret = authorize(conn, &file);
        mg_fclose(&file);
    }

    return ret;
}

int mg_modify_passwords_file(const char *fname, const char *domain,
                             const char *user, const char *pass)
{
    int found;
    char line[512], u[512] = "", d[512] ="", ha1[33], tmp[PATH_MAX+1];
    FILE *fp, *fp2;

    found = 0;
    fp = fp2 = NULL;

<<<<<<< HEAD
    // Regard empty password as no password - remove user record.
=======
    /* Regard empty password as no password - remove user record. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (pass != NULL && pass[0] == '\0') {
        pass = NULL;
    }

    (void) snprintf(tmp, sizeof(tmp) - 1, "%s.tmp", fname);
    tmp[sizeof(tmp) - 1] = 0;

<<<<<<< HEAD
    // Create the file if does not exist
=======
    /* Create the file if does not exist */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if ((fp = fopen(fname, "a+")) != NULL) {
        (void) fclose(fp);
    }

<<<<<<< HEAD
    // Open the given file and temporary file
=======
    /* Open the given file and temporary file */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if ((fp = fopen(fname, "r")) == NULL) {
        return 0;
    } else if ((fp2 = fopen(tmp, "w+")) == NULL) {
        fclose(fp);
        return 0;
    }

<<<<<<< HEAD
    // Copy the stuff to temporary file
=======
    /* Copy the stuff to temporary file */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%[^:]:%[^:]:%*s", u, d) != 2) {
            continue;
        }

        if (!strcmp(u, user) && !strcmp(d, domain)) {
            found++;
            if (pass != NULL) {
                mg_md5(ha1, user, ":", domain, ":", pass, NULL);
                fprintf(fp2, "%s:%s:%s\n", user, domain, ha1);
            }
        } else {
            fprintf(fp2, "%s", line);
        }
    }

<<<<<<< HEAD
    // If new user, just add it
=======
    /* If new user, just add it */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (!found && pass != NULL) {
        mg_md5(ha1, user, ":", domain, ":", pass, NULL);
        fprintf(fp2, "%s:%s:%s\n", user, domain, ha1);
    }

<<<<<<< HEAD
    // Close files
    fclose(fp);
    fclose(fp2);

    // Put the temp file in place of real file
=======
    /* Close files */
    fclose(fp);
    fclose(fp2);

    /* Put the temp file in place of real file */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    IGNORE_UNUSED_RESULT(remove(fname));
    IGNORE_UNUSED_RESULT(rename(tmp, fname));

    return 1;
}

static SOCKET conn2(struct mg_context *ctx  /* may be null */, const char *host, int port,
                    int use_ssl, char *ebuf, size_t ebuf_len)
{
    struct sockaddr_in sain;
    struct hostent *he;
    SOCKET sock = INVALID_SOCKET;

    if (host == NULL) {
        snprintf(ebuf, ebuf_len, "%s", "NULL host");
    } else if (use_ssl && SSLv23_client_method == NULL) {
        snprintf(ebuf, ebuf_len, "%s", "SSL is not initialized");
<<<<<<< HEAD
        // TODO(lsm): use something threadsafe instead of gethostbyname()
=======
        /* TODO(lsm): use something threadsafe instead of gethostbyname() */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    } else if ((he = gethostbyname(host)) == NULL) {
        snprintf(ebuf, ebuf_len, "gethostbyname(%s): %s", host, strerror(ERRNO));
    } else if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        snprintf(ebuf, ebuf_len, "socket(): %s", strerror(ERRNO));
    } else {
        set_close_on_exec(sock, fc(ctx));
        memset(&sain, '\0', sizeof(sain));
        sain.sin_family = AF_INET;
        sain.sin_port = htons((uint16_t) port);
        sain.sin_addr = * (struct in_addr *) he->h_addr_list[0];
        if (connect(sock, (struct sockaddr *) &sain, sizeof(sain)) != 0) {
            snprintf(ebuf, ebuf_len, "connect(%s:%d): %s",
                     host, port, strerror(ERRNO));
            closesocket(sock);
            sock = INVALID_SOCKET;
        }
    }
    return sock;
}

int mg_url_encode(const char *src, char *dst, size_t dst_len)
{
    static const char *dont_escape = "._-$,;~()";
    static const char *hex = "0123456789abcdef";
    char *pos = dst;
    const char *end = dst + dst_len - 1;

    for (; *src != '\0' && pos < end; src++, pos++) {
        if (isalnum(*(const unsigned char *) src) ||
            strchr(dont_escape, * (const unsigned char *) src) != NULL) {
            *pos = *src;
        } else if (pos + 2 < end) {
            pos[0] = '%';
            pos[1] = hex[(* (const unsigned char *) src) >> 4];
            pos[2] = hex[(* (const unsigned char *) src) & 0xf];
            pos += 2;
        } else {
            return -1;
        }
    }

    *pos = '\0';
    return (*src == '\0') ? (int)(pos - dst) : -1;
}

static void print_dir_entry(struct de *de)
{
    char size[64], mod[64], href[PATH_MAX];
    struct tm *tm;

    if (de->file.is_directory) {
        mg_snprintf(de->conn, size, sizeof(size), "%s", "[DIRECTORY]");
    } else {
<<<<<<< HEAD
        // We use (signed) cast below because MSVC 6 compiler cannot
        // convert unsigned __int64 to double. Sigh.
=======
        /* We use (signed) cast below because MSVC 6 compiler cannot
           convert unsigned __int64 to double. Sigh. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (de->file.size < 1024) {
            mg_snprintf(de->conn, size, sizeof(size), "%d", (int) de->file.size);
        } else if (de->file.size < 0x100000) {
            mg_snprintf(de->conn, size, sizeof(size),
                        "%.1fk", (double) de->file.size / 1024.0);
        } else if (de->file.size < 0x40000000) {
            mg_snprintf(de->conn, size, sizeof(size),
                        "%.1fM", (double) de->file.size / 1048576);
        } else {
            mg_snprintf(de->conn, size, sizeof(size),
                        "%.1fG", (double) de->file.size / 1073741824);
        }
    }
    tm = localtime(&de->file.modification_time);
    if (tm != NULL) {
        strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M", tm);
    } else {
        strncpy(mod, "01-Jan-1970 00:00", sizeof(mod));
        mod[sizeof(mod) - 1] = '\0';
    }
    mg_url_encode(de->file_name, href, sizeof(href));
    de->conn->num_bytes_sent += mg_printf(de->conn,
                                          "<tr><td><a href=\"%s%s%s\">%s%s</a></td>"
                                          "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
                                          de->conn->request_info.uri, href, de->file.is_directory ? "/" : "",
                                          de->file_name, de->file.is_directory ? "/" : "", mod, size);
}

<<<<<<< HEAD
// This function is called from send_directory() and used for
// sorting directory entries by size, or name, or modification time.
// On windows, __cdecl specification is needed in case if project is built
// with __stdcall convention. qsort always requires __cdels callback.
=======
/* This function is called from send_directory() and used for
   sorting directory entries by size, or name, or modification time.
   On windows, __cdecl specification is needed in case if project is built
   with __stdcall convention. qsort always requires __cdels callback. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int WINCDECL compare_dir_entries(const void *p1, const void *p2)
{
    const struct de *a = (const struct de *) p1, *b = (const struct de *) p2;
    const char *query_string = a->conn->request_info.query_string;
    int cmp_result = 0;

    if (query_string == NULL) {
        query_string = "na";
    }

    if (a->file.is_directory && !b->file.is_directory) {
<<<<<<< HEAD
        return -1;  // Always put directories on top
    } else if (!a->file.is_directory && b->file.is_directory) {
        return 1;   // Always put directories on top
=======
        return -1;  /* Always put directories on top */
    } else if (!a->file.is_directory && b->file.is_directory) {
        return 1;   /* Always put directories on top */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    } else if (*query_string == 'n') {
        cmp_result = strcmp(a->file_name, b->file_name);
    } else if (*query_string == 's') {
        cmp_result = a->file.size == b->file.size ? 0 :
                     a->file.size > b->file.size ? 1 : -1;
    } else if (*query_string == 'd') {
        cmp_result = a->file.modification_time == b->file.modification_time ? 0 :
                     a->file.modification_time > b->file.modification_time ? 1 : -1;
    }

    return query_string[1] == 'd' ? -cmp_result : cmp_result;
}

static int must_hide_file(struct mg_connection *conn, const char *path)
{
    const char *pw_pattern = "**" PASSWORDS_FILE_NAME "$";
    const char *pattern = conn->ctx->config[HIDE_FILES];
    return match_prefix(pw_pattern, (int)strlen(pw_pattern), path) > 0 ||
           (pattern != NULL && match_prefix(pattern, (int)strlen(pattern), path) > 0);
}

static int scan_directory(struct mg_connection *conn, const char *dir,
                          void *data, void (*cb)(struct de *, void *))
{
    char path[PATH_MAX];
    struct dirent *dp;
    DIR *dirp;
    struct de de;

    if ((dirp = opendir(dir)) == NULL) {
        return 0;
    } else {
        de.conn = conn;

        while ((dp = readdir(dirp)) != NULL) {
<<<<<<< HEAD
            // Do not show current dir and hidden files
=======
            /* Do not show current dir and hidden files */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            if (!strcmp(dp->d_name, ".") ||
                !strcmp(dp->d_name, "..") ||
                must_hide_file(conn, dp->d_name)) {
                continue;
            }

            mg_snprintf(conn, path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);

<<<<<<< HEAD
            // If we don't memset stat structure to zero, mtime will have
            // garbage and strftime() will segfault later on in
            // print_dir_entry(). memset is required only if mg_stat()
            // fails. For more details, see
            // http://code.google.com/p/mongoose/issues/detail?id=79
=======
            /* If we don't memset stat structure to zero, mtime will have
               garbage and strftime() will segfault later on in
               print_dir_entry(). memset is required only if mg_stat()
               fails. For more details, see
               http://code.google.com/p/mongoose/issues/detail?id=79 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            memset(&de.file, 0, sizeof(de.file));
            if (!mg_stat(conn, path, &de.file)) {
                mg_cry(conn, "%s: mg_stat(%s) failed: %s",
                       __func__, path, strerror(ERRNO));
            }

            de.file_name = dp->d_name;
            cb(&de, data);
        }
        (void) closedir(dirp);
    }
    return 1;
}

static int remove_directory(struct mg_connection *conn, const char *dir)
{
    char path[PATH_MAX];
    struct dirent *dp;
    DIR *dirp;
    struct de de;

    if ((dirp = opendir(dir)) == NULL) {
        return 0;
    } else {
        de.conn = conn;

        while ((dp = readdir(dirp)) != NULL) {
<<<<<<< HEAD
            // Do not show current dir (but show hidden files as they will also be removed)
=======
            /* Do not show current dir (but show hidden files as they will
               also be removed) */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            if (!strcmp(dp->d_name, ".") ||
                !strcmp(dp->d_name, "..")) {
                continue;
            }

            mg_snprintf(conn, path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);

<<<<<<< HEAD
            // If we don't memset stat structure to zero, mtime will have
            // garbage and strftime() will segfault later on in
            // print_dir_entry(). memset is required only if mg_stat()
            // fails. For more details, see
            // http://code.google.com/p/mongoose/issues/detail?id=79
=======
            /* If we don't memset stat structure to zero, mtime will have
               garbage and strftime() will segfault later on in
               print_dir_entry(). memset is required only if mg_stat()
               fails. For more details, see
               http://code.google.com/p/mongoose/issues/detail?id=79 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            memset(&de.file, 0, sizeof(de.file));
            if (!mg_stat(conn, path, &de.file)) {
                mg_cry(conn, "%s: mg_stat(%s) failed: %s",
                       __func__, path, strerror(ERRNO));
            }
            if(de.file.modification_time) {
                if(de.file.is_directory) {
                    remove_directory(conn, path);
                } else {
                    mg_remove(path);
                }
            }

        }
        (void) closedir(dirp);

        IGNORE_UNUSED_RESULT(rmdir(dir));
    }

    return 1;
}

struct dir_scan_data {
    struct de *entries;
    int num_entries;
    int arr_size;
};

<<<<<<< HEAD
// Behaves like realloc(), but frees original pointer on failure
=======
/* Behaves like realloc(), but frees original pointer on failure */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void *realloc2(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        free(ptr);
    }
    return new_ptr;
}

static void dir_scan_callback(struct de *de, void *data)
{
    struct dir_scan_data *dsd = (struct dir_scan_data *) data;

    if (dsd->entries == NULL || dsd->num_entries >= dsd->arr_size) {
        dsd->arr_size *= 2;
        dsd->entries = (struct de *) realloc2(dsd->entries, dsd->arr_size *
                                              sizeof(dsd->entries[0]));
    }
    if (dsd->entries == NULL) {
<<<<<<< HEAD
        // TODO(lsm): propagate an error to the caller
=======
        /* TODO(lsm): propagate an error to the caller */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        dsd->num_entries = 0;
    } else {
        dsd->entries[dsd->num_entries].file_name = mg_strdup(de->file_name);
        dsd->entries[dsd->num_entries].file = de->file;
        dsd->entries[dsd->num_entries].conn = de->conn;
        dsd->num_entries++;
    }
}

static void handle_directory_request(struct mg_connection *conn,
                                     const char *dir)
{
    int i, sort_direction;
    struct dir_scan_data data = { NULL, 0, 128 };

    if (!scan_directory(conn, dir, &data, dir_scan_callback)) {
        send_http_error(conn, 500, "Cannot open directory",
                        "Error: opendir(%s): %s", dir, strerror(ERRNO));
        return;
    }

    sort_direction = conn->request_info.query_string != NULL &&
                     conn->request_info.query_string[1] == 'd' ? 'a' : 'd';

    conn->must_close = 1;
    mg_printf(conn, "%s",
              "HTTP/1.1 200 OK\r\n"
              "Connection: close\r\n"
              "Content-Type: text/html; charset=utf-8\r\n\r\n");

    conn->num_bytes_sent += mg_printf(conn,
                                      "<html><head><title>Index of %s</title>"
                                      "<style>th {text-align: left;}</style></head>"
                                      "<body><h1>Index of %s</h1><pre><table cellpadding=\"0\">"
                                      "<tr><th><a href=\"?n%c\">Name</a></th>"
                                      "<th><a href=\"?d%c\">Modified</a></th>"
                                      "<th><a href=\"?s%c\">Size</a></th></tr>"
                                      "<tr><td colspan=\"3\"><hr></td></tr>",
                                      conn->request_info.uri, conn->request_info.uri,
                                      sort_direction, sort_direction, sort_direction);

<<<<<<< HEAD
    // Print first entry - link to a parent directory
=======
    /* Print first entry - link to a parent directory */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    conn->num_bytes_sent += mg_printf(conn,
                                      "<tr><td><a href=\"%s%s\">%s</a></td>"
                                      "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
                                      conn->request_info.uri, "..", "Parent directory", "-", "-");

<<<<<<< HEAD
    // Sort and print directory entries
=======
    /* Sort and print directory entries */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (data.entries != NULL) {
        qsort(data.entries, (size_t) data.num_entries,
              sizeof(data.entries[0]), compare_dir_entries);
        for (i = 0; i < data.num_entries; i++) {
            print_dir_entry(&data.entries[i]);
            free(data.entries[i].file_name);
        }
        free(data.entries);
    }

    conn->num_bytes_sent += mg_printf(conn, "%s", "</table></body></html>");
    conn->status_code = 200;
}

<<<<<<< HEAD
// Send len bytes from the opened file to the client.
=======
/* Send len bytes from the opened file to the client. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void send_file_data(struct mg_connection *conn, struct file *filep,
                           int64_t offset, int64_t len)
{
    char buf[MG_BUF_LEN];
    int to_read, num_read, num_written;

<<<<<<< HEAD
    // Sanity check the offset
=======
    /* Sanity check the offset */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    offset = offset < 0 ? 0 : offset > filep->size ? filep->size : offset;

    if (len > 0 && filep->membuf != NULL && filep->size > 0) {
        if (len > filep->size - offset) {
            len = filep->size - offset;
        }
        mg_write(conn, filep->membuf + offset, (size_t) len);
    } else if (len > 0 && filep->fp != NULL) {
        if (offset > 0 && fseeko(filep->fp, offset, SEEK_SET) != 0) {
            mg_cry(conn, "%s: fseeko() failed: %s",
                   __func__, strerror(ERRNO));
        }
        while (len > 0) {
<<<<<<< HEAD
            // Calculate how much to read from the file in the buffer
=======
            /* Calculate how much to read from the file in the buffer */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            to_read = sizeof(buf);
            if ((int64_t) to_read > len) {
                to_read = (int) len;
            }

<<<<<<< HEAD
            // Read from file, exit the loop on error
=======
            /* Read from file, exit the loop on error */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            if ((num_read = (int) fread(buf, 1, (size_t) to_read, filep->fp)) <= 0) {
                break;
            }

<<<<<<< HEAD
            // Send read bytes to the client, exit the loop on error
=======
            /* Send read bytes to the client, exit the loop on error */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            if ((num_written = mg_write(conn, buf, (size_t) num_read)) != num_read) {
                break;
            }

<<<<<<< HEAD
            // Both read and were successful, adjust counters
=======
            /* Both read and were successful, adjust counters */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            conn->num_bytes_sent += num_written;
            len -= num_written;
        }
    }
}

static int parse_range_header(const char *header, int64_t *a, int64_t *b)
{
    return sscanf(header, "bytes=%" INT64_FMT "-%" INT64_FMT, a, b);
}

static void gmt_time_string(char *buf, size_t buf_len, time_t *t)
{
    struct tm *tm;

    tm = gmtime(t);
    if (tm != NULL) {
        strftime(buf, buf_len, "%a, %d %b %Y %H:%M:%S GMT", tm);
    } else {
        strncpy(buf, "Thu, 01 Jan 1970 00:00:00 GMT", buf_len);
        buf[buf_len - 1] = '\0';
    }
}

static void construct_etag(char *buf, size_t buf_len,
                           const struct file *filep)
{
    snprintf(buf, buf_len, "\"%lx.%" INT64_FMT "\"",
             (unsigned long) filep->modification_time, filep->size);
}

static void fclose_on_exec(struct file *filep, struct mg_connection *conn)
{
    if (filep != NULL && filep->fp != NULL) {
#ifdef _WIN32
        (void) conn; /* Unused. */
#else
        if (fcntl(fileno(filep->fp), F_SETFD, FD_CLOEXEC) != 0) {
            mg_cry(conn, "%s: fcntl(F_SETFD FD_CLOEXEC) failed: %s",
                   __func__, strerror(ERRNO));
        }
#endif
    }
}

static void handle_file_request(struct mg_connection *conn, const char *path,
                                struct file *filep)
{
    char date[64], lm[64], etag[64], range[64];
    const char *msg = "OK", *hdr;
    time_t curtime = time(NULL);
    int64_t cl, r1, r2;
    struct vec mime_vec;
    int n;
    char gz_path[PATH_MAX];
    char const* encoding = "";

    get_mime_type(conn->ctx, path, &mime_vec);
    cl = filep->size;
    conn->status_code = 200;
    range[0] = '\0';

<<<<<<< HEAD
    // if this file is in fact a pre-gzipped file, rewrite its filename
    // it's important to rewrite the filename after resolving
    // the mime type from it, to preserve the actual file's type
=======
    /* if this file is in fact a pre-gzipped file, rewrite its filename
       it's important to rewrite the filename after resolving
       the mime type from it, to preserve the actual file's type */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (filep->gzipped) {
        snprintf(gz_path, sizeof(gz_path), "%s.gz", path);
        path = gz_path;
        encoding = "Content-Encoding: gzip\r\n";
    }

    if (!mg_fopen(conn, path, "rb", filep)) {
        send_http_error(conn, 500, http_500_error,
                        "fopen(%s): %s", path, strerror(ERRNO));
        return;
    }

    fclose_on_exec(filep, conn);

<<<<<<< HEAD
    // If Range: header specified, act accordingly
=======
    /* If Range: header specified, act accordingly */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    r1 = r2 = 0;
    hdr = mg_get_header(conn, "Range");
    if (hdr != NULL && (n = parse_range_header(hdr, &r1, &r2)) > 0 &&
        r1 >= 0 && r2 >= 0) {
<<<<<<< HEAD
        // actually, range requests don't play well with a pre-gzipped
        // file (since the range is specified in the uncompressed space)
=======
        /* actually, range requests don't play well with a pre-gzipped
           file (since the range is specified in the uncompressed space) */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (filep->gzipped) {
            send_http_error(conn, 501, "Not Implemented", "range requests in gzipped files are not supported");
            mg_fclose(filep);
            return;
        }
        conn->status_code = 206;
cl = n == 2 ? (r2 > cl ? cl : r2) - r1 + 1: cl - r1;
        mg_snprintf(conn, range, sizeof(range),
                    "Content-Range: bytes "
                    "%" INT64_FMT "-%"
                    INT64_FMT "/%" INT64_FMT "\r\n",
                    r1, r1 + cl - 1, filep->size);
        msg = "Partial Content";
    }

<<<<<<< HEAD
    // Prepare Etag, Date, Last-Modified headers. Must be in UTC, according to
    // http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3
=======
    /* Prepare Etag, Date, Last-Modified headers. Must be in UTC, according to
       http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    gmt_time_string(date, sizeof(date), &curtime);
    gmt_time_string(lm, sizeof(lm), &filep->modification_time);
    construct_etag(etag, sizeof(etag), filep);

    (void) mg_printf(conn,
                     "HTTP/1.1 %d %s\r\n"
                     "Date: %s\r\n"
                     "Last-Modified: %s\r\n"
                     "Etag: %s\r\n"
                     "Content-Type: %.*s\r\n"
                     "Content-Length: %" INT64_FMT "\r\n"
                     "Connection: %s\r\n"
                     "Accept-Ranges: bytes\r\n"
                     "%s%s\r\n",
                     conn->status_code, msg, date, lm, etag, (int) mime_vec.len,
                     mime_vec.ptr, cl, suggest_connection_header(conn), range, encoding);

    if (strcmp(conn->request_info.request_method, "HEAD") != 0) {
        send_file_data(conn, filep, r1, cl);
    }
    mg_fclose(filep);
}

void mg_send_file(struct mg_connection *conn, const char *path)
{
    struct file file = STRUCT_FILE_INITIALIZER;
    if (mg_stat(conn, path, &file)) {
        handle_file_request(conn, path, &file);
    } else {
        send_http_error(conn, 404, "Not Found", "%s", "File not found");
    }
}


<<<<<<< HEAD
// Parse HTTP headers from the given buffer, advance buffer to the point
// where parsing stopped.
=======
/* Parse HTTP headers from the given buffer, advance buffer to the point
   where parsing stopped. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void parse_http_headers(char **buf, struct mg_request_info *ri)
{
    int i;

    for (i = 0; i < (int) ARRAY_SIZE(ri->http_headers); i++) {
        ri->http_headers[i].name = skip_quoted(buf, ":", " ", 0);
        ri->http_headers[i].value = skip(buf, "\r\n");
        if (ri->http_headers[i].name[0] == '\0')
            break;
        ri->num_headers = i + 1;
    }
}

static int is_valid_http_method(const char *method)
{
    return !strcmp(method, "GET") || !strcmp(method, "POST") ||
           !strcmp(method, "HEAD") || !strcmp(method, "CONNECT") ||
           !strcmp(method, "PUT") || !strcmp(method, "DELETE") ||
           !strcmp(method, "OPTIONS") || !strcmp(method, "PROPFIND")
           || !strcmp(method, "MKCOL")
           ;
}

<<<<<<< HEAD
// Parse HTTP request, fill in mg_request_info structure.
// This function modifies the buffer by NUL-terminating
// HTTP request components, header names and header values.
=======
/* Parse HTTP request, fill in mg_request_info structure.
   This function modifies the buffer by NUL-terminating
   HTTP request components, header names and header values. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int parse_http_message(char *buf, int len, struct mg_request_info *ri)
{
    int is_request, request_length = get_request_len(buf, len);
    if (request_length > 0) {
<<<<<<< HEAD
        // Reset attributes. DO NOT TOUCH is_ssl, remote_ip, remote_port
=======
        /* Reset attributes. DO NOT TOUCH is_ssl, remote_ip, remote_port */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        ri->remote_user = ri->request_method = ri->uri = ri->http_version = NULL;
        ri->num_headers = 0;

        buf[request_length - 1] = '\0';

<<<<<<< HEAD
        // RFC says that all initial whitespaces should be ingored
=======
        /* RFC says that all initial whitespaces should be ingored */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        while (*buf != '\0' && isspace(* (unsigned char *) buf)) {
            buf++;
        }
        ri->request_method = skip(&buf, " ");
        ri->uri = skip(&buf, " ");
        ri->http_version = skip(&buf, "\r\n");

<<<<<<< HEAD
        // HTTP message could be either HTTP request or HTTP response, e.g.
        // "GET / HTTP/1.0 ...." or  "HTTP/1.0 200 OK ..."
=======
        /* HTTP message could be either HTTP request or HTTP response, e.g.
           "GET / HTTP/1.0 ...." or  "HTTP/1.0 200 OK ..." */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        is_request = is_valid_http_method(ri->request_method);
        if ((is_request && memcmp(ri->http_version, "HTTP/", 5) != 0) ||
            (!is_request && memcmp(ri->request_method, "HTTP/", 5) != 0)) {
            request_length = -1;
        } else {
            if (is_request) {
                ri->http_version += 5;
            }
            parse_http_headers(&buf, ri);
        }
    }
    return request_length;
}

<<<<<<< HEAD
// Keep reading the input (either opened file descriptor fd, or socket sock,
// or SSL descriptor ssl) into buffer buf, until \r\n\r\n appears in the
// buffer (which marks the end of HTTP request). Buffer buf may already
// have some data. The length of the data is stored in nread.
// Upon every read operation, increase nread by the number of bytes read.
=======
/* Keep reading the input (either opened file descriptor fd, or socket sock,
   or SSL descriptor ssl) into buffer buf, until \r\n\r\n appears in the
   buffer (which marks the end of HTTP request). Buffer buf may already
   have some data. The length of the data is stored in nread.
   Upon every read operation, increase nread by the number of bytes read. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int read_request(FILE *fp, struct mg_connection *conn,
                        char *buf, int bufsiz, int *nread)
{
    int request_len, n = 0;

    request_len = get_request_len(buf, *nread);
    while (conn->ctx->stop_flag == 0 &&
           *nread < bufsiz && request_len == 0 &&
           (n = pull(fp, conn, buf + *nread, bufsiz - *nread)) > 0) {
        *nread += n;
        assert(*nread <= bufsiz);
        request_len = get_request_len(buf, *nread);
    }

    return request_len <= 0 && n <= 0 ? -1 : request_len;
}

<<<<<<< HEAD
// For given directory path, substitute it to valid index file.
// Return 0 if index file has been found, -1 if not found.
// If the file is found, it's stats is returned in stp.
=======
/* For given directory path, substitute it to valid index file.
   Return 0 if index file has been found, -1 if not found.
   If the file is found, it's stats is returned in stp. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int substitute_index_file(struct mg_connection *conn, char *path,
                                 size_t path_len, struct file *filep)
{
    const char *list = conn->ctx->config[INDEX_FILES];
    struct file file = STRUCT_FILE_INITIALIZER;
    struct vec filename_vec;
    size_t n = strlen(path);
    int found = 0;

<<<<<<< HEAD
    // The 'path' given to us points to the directory. Remove all trailing
    // directory separator characters from the end of the path, and
    // then append single directory separator character.
=======
    /* The 'path' given to us points to the directory. Remove all trailing
       directory separator characters from the end of the path, and
       then append single directory separator character. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    while (n > 0 && path[n - 1] == '/') {
        n--;
    }
    path[n] = '/';

<<<<<<< HEAD
    // Traverse index files list. For each entry, append it to the given
    // path and see if the file exists. If it exists, break the loop
    while ((list = next_option(list, &filename_vec, NULL)) != NULL) {

        // Ignore too long entries that may overflow path buffer
        if (filename_vec.len > path_len - (n + 2))
            continue;

        // Prepare full path to the index file
        mg_strlcpy(path + n + 1, filename_vec.ptr, filename_vec.len + 1);

        // Does it exist?
        if (mg_stat(conn, path, &file)) {
            // Yes it does, break the loop
=======
    /* Traverse index files list. For each entry, append it to the given
       path and see if the file exists. If it exists, break the loop */
    while ((list = next_option(list, &filename_vec, NULL)) != NULL) {

        /* Ignore too long entries that may overflow path buffer */
        if (filename_vec.len > path_len - (n + 2))
            continue;

        /* Prepare full path to the index file */
        mg_strlcpy(path + n + 1, filename_vec.ptr, filename_vec.len + 1);

        /* Does it exist? */
        if (mg_stat(conn, path, &file)) {
            /* Yes it does, break the loop */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            *filep = file;
            found = 1;
            break;
        }
    }

<<<<<<< HEAD
    // If no index file exists, restore directory path
=======
    /* If no index file exists, restore directory path */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (!found) {
        path[n] = '\0';
    }

    return found;
}

<<<<<<< HEAD
// Return True if we should reply 304 Not Modified.
=======
/* Return True if we should reply 304 Not Modified. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int is_not_modified(const struct mg_connection *conn,
                           const struct file *filep)
{
    char etag[64];
    const char *ims = mg_get_header(conn, "If-Modified-Since");
    const char *inm = mg_get_header(conn, "If-None-Match");
    construct_etag(etag, sizeof(etag), filep);
    return (inm != NULL && !mg_strcasecmp(etag, inm)) ||
           (ims != NULL && filep->modification_time <= parse_date_string(ims));
}

static int forward_body_data(struct mg_connection *conn, FILE *fp,
                             SOCKET sock, SSL *ssl)
{
    const char *expect, *body;
    char buf[MG_BUF_LEN];
    int to_read, nread, buffered_len, success = 0;

    expect = mg_get_header(conn, "Expect");
    assert(fp != NULL);

    if (conn->content_len == -1) {
        send_http_error(conn, 411, "Length Required", "%s", "");
    } else if (expect != NULL && mg_strcasecmp(expect, "100-continue")) {
        send_http_error(conn, 417, "Expectation Failed", "%s", "");
    } else {
        if (expect != NULL) {
            (void) mg_printf(conn, "%s", "HTTP/1.1 100 Continue\r\n\r\n");
        }

        body = conn->buf + conn->request_len + conn->consumed_content;
        buffered_len = (int)(&conn->buf[conn->data_len] - body);
        assert(buffered_len >= 0);
        assert(conn->consumed_content == 0);

        if (buffered_len > 0) {
            if ((int64_t) buffered_len > conn->content_len) {
                buffered_len = (int) conn->content_len;
            }
            push(fp, sock, ssl, body, (int64_t) buffered_len);
            conn->consumed_content += buffered_len;
        }

        nread = 0;
        while (conn->consumed_content < conn->content_len) {
            to_read = sizeof(buf);
            if ((int64_t) to_read > conn->content_len - conn->consumed_content) {
                to_read = (int) (conn->content_len - conn->consumed_content);
            }
            nread = pull(NULL, conn, buf, to_read);
            if (nread <= 0 || push(fp, sock, ssl, buf, nread) != nread) {
                break;
            }
            conn->consumed_content += nread;
        }

        if (conn->consumed_content == conn->content_len) {
            success = nread >= 0;
        }

<<<<<<< HEAD
        // Each error code path in this function must send an error
=======
        /* Each error code path in this function must send an error */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (!success) {
            send_http_error(conn, 577, http_500_error, "%s", "");
        }
    }

    return success;
}

#if !defined(NO_CGI)
<<<<<<< HEAD
// This structure helps to create an environment for the spawned CGI program.
// Environment is an array of "VARIABLE=VALUE\0" ASCIIZ strings,
// last element must be NULL.
// However, on Windows there is a requirement that all these VARIABLE=VALUE\0
// strings must reside in a contiguous buffer. The end of the buffer is
// marked by two '\0' characters.
// We satisfy both worlds: we create an envp array (which is vars), all
// entries are actually pointers inside buf.
struct cgi_env_block {
    struct mg_connection *conn;
    char buf[CGI_ENVIRONMENT_SIZE]; // Environment buffer
    int len; // Space taken
    char *vars[MAX_CGI_ENVIR_VARS]; // char **envp
    int nvars; // Number of variables
=======
/* This structure helps to create an environment for the spawned CGI program.
   Environment is an array of "VARIABLE=VALUE\0" ASCIIZ strings,
   last element must be NULL.
   However, on Windows there is a requirement that all these VARIABLE=VALUE\0
   strings must reside in a contiguous buffer. The end of the buffer is
   marked by two '\0' characters.
   We satisfy both worlds: we create an envp array (which is vars), all
   entries are actually pointers inside buf. */
struct cgi_env_block {
    struct mg_connection *conn;
    char buf[CGI_ENVIRONMENT_SIZE]; /* Environment buffer */
    int len; /* Space taken */
    char *vars[MAX_CGI_ENVIR_VARS]; /* char **envp */
    int nvars; /* Number of variables */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
};

static char *addenv(struct cgi_env_block *block,
                    PRINTF_FORMAT_STRING(const char *fmt), ...)
PRINTF_ARGS(2, 3);

<<<<<<< HEAD
// Append VARIABLE=VALUE\0 string to the buffer, and add a respective
// pointer into the vars array.
=======
/* Append VARIABLE=VALUE\0 string to the buffer, and add a respective
   pointer into the vars array. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static char *addenv(struct cgi_env_block *block, const char *fmt, ...)
{
    int n, space;
    char *added;
    va_list ap;

<<<<<<< HEAD
    // Calculate how much space is left in the buffer
    space = sizeof(block->buf) - block->len - 2;
    assert(space >= 0);

    // Make a pointer to the free space int the buffer
    added = block->buf + block->len;

    // Copy VARIABLE=VALUE\0 string into the free space
=======
    /* Calculate how much space is left in the buffer */
    space = sizeof(block->buf) - block->len - 2;
    assert(space >= 0);

    /* Make a pointer to the free space int the buffer */
    added = block->buf + block->len;

    /* Copy VARIABLE=VALUE\0 string into the free space */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    va_start(ap, fmt);
    n = mg_vsnprintf(block->conn, added, (size_t) space, fmt, ap);
    va_end(ap);

<<<<<<< HEAD
    // Make sure we do not overflow buffer and the envp array
    if (n > 0 && n + 1 < space &&
        block->nvars < (int) ARRAY_SIZE(block->vars) - 2) {
        // Append a pointer to the added string into the envp array
        block->vars[block->nvars++] = added;
        // Bump up used length counter. Include \0 terminator
=======
    /* Make sure we do not overflow buffer and the envp array */
    if (n > 0 && n + 1 < space &&
        block->nvars < (int) ARRAY_SIZE(block->vars) - 2) {
        /* Append a pointer to the added string into the envp array */
        block->vars[block->nvars++] = added;
        /* Bump up used length counter. Include \0 terminator */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        block->len += n + 1;
    } else {
        mg_cry(block->conn, "%s: CGI env buffer truncated for [%s]", __func__, fmt);
    }

    return added;
}

static void prepare_cgi_environment(struct mg_connection *conn,
                                    const char *prog,
                                    struct cgi_env_block *blk)
{
    const char *s, *slash;
    struct vec var_vec;
    char *p, src_addr[IP_ADDR_STR_LEN];
    int  i;

    blk->len = blk->nvars = 0;
    blk->conn = conn;
    sockaddr_to_string(src_addr, sizeof(src_addr), &conn->client.rsa);

    addenv(blk, "SERVER_NAME=%s", conn->ctx->config[AUTHENTICATION_DOMAIN]);
    addenv(blk, "SERVER_ROOT=%s", conn->ctx->config[DOCUMENT_ROOT]);
    addenv(blk, "DOCUMENT_ROOT=%s", conn->ctx->config[DOCUMENT_ROOT]);
    addenv(blk, "SERVER_SOFTWARE=%s/%s", "Civetweb", mg_version());

<<<<<<< HEAD
    // Prepare the environment block
    addenv(blk, "%s", "GATEWAY_INTERFACE=CGI/1.1");
    addenv(blk, "%s", "SERVER_PROTOCOL=HTTP/1.1");
    addenv(blk, "%s", "REDIRECT_STATUS=200"); // For PHP

    // TODO(lsm): fix this for IPv6 case
=======
    /* Prepare the environment block */
    addenv(blk, "%s", "GATEWAY_INTERFACE=CGI/1.1");
    addenv(blk, "%s", "SERVER_PROTOCOL=HTTP/1.1");
    addenv(blk, "%s", "REDIRECT_STATUS=200"); /* For PHP */

    /* TODO(lsm): fix this for IPv6 case */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    addenv(blk, "SERVER_PORT=%d", ntohs(conn->client.lsa.sin.sin_port));

    addenv(blk, "REQUEST_METHOD=%s", conn->request_info.request_method);
    addenv(blk, "REMOTE_ADDR=%s", src_addr);
    addenv(blk, "REMOTE_PORT=%d", conn->request_info.remote_port);
    addenv(blk, "REQUEST_URI=%s", conn->request_info.uri);

<<<<<<< HEAD
    // SCRIPT_NAME
=======
    /* SCRIPT_NAME */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    assert(conn->request_info.uri[0] == '/');
    slash = strrchr(conn->request_info.uri, '/');
    if ((s = strrchr(prog, '/')) == NULL)
        s = prog;
    addenv(blk, "SCRIPT_NAME=%.*s%s", (int) (slash - conn->request_info.uri),
           conn->request_info.uri, s);

    addenv(blk, "SCRIPT_FILENAME=%s", prog);
    addenv(blk, "PATH_TRANSLATED=%s", prog);
    addenv(blk, "HTTPS=%s", conn->ssl == NULL ? "off" : "on");

    if ((s = mg_get_header(conn, "Content-Type")) != NULL)
        addenv(blk, "CONTENT_TYPE=%s", s);

    if (conn->request_info.query_string != NULL)
        addenv(blk, "QUERY_STRING=%s", conn->request_info.query_string);

    if ((s = mg_get_header(conn, "Content-Length")) != NULL)
        addenv(blk, "CONTENT_LENGTH=%s", s);

    if ((s = getenv("PATH")) != NULL)
        addenv(blk, "PATH=%s", s);

    if (conn->path_info != NULL) {
        addenv(blk, "PATH_INFO=%s", conn->path_info);
    }

#if defined(_WIN32)
    if ((s = getenv("COMSPEC")) != NULL) {
        addenv(blk, "COMSPEC=%s", s);
    }
    if ((s = getenv("SYSTEMROOT")) != NULL) {
        addenv(blk, "SYSTEMROOT=%s", s);
    }
    if ((s = getenv("SystemDrive")) != NULL) {
        addenv(blk, "SystemDrive=%s", s);
    }
    if ((s = getenv("ProgramFiles")) != NULL) {
        addenv(blk, "ProgramFiles=%s", s);
    }
    if ((s = getenv("ProgramFiles(x86)")) != NULL) {
        addenv(blk, "ProgramFiles(x86)=%s", s);
    }
#else
    if ((s = getenv("LD_LIBRARY_PATH")) != NULL)
        addenv(blk, "LD_LIBRARY_PATH=%s", s);
<<<<<<< HEAD
#endif // _WIN32
=======
#endif /* _WIN32 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

    if ((s = getenv("PERLLIB")) != NULL)
        addenv(blk, "PERLLIB=%s", s);

    if (conn->request_info.remote_user != NULL) {
        addenv(blk, "REMOTE_USER=%s", conn->request_info.remote_user);
        addenv(blk, "%s", "AUTH_TYPE=Digest");
    }

<<<<<<< HEAD
    // Add all headers as HTTP_* variables
=======
    /* Add all headers as HTTP_* variables */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    for (i = 0; i < conn->request_info.num_headers; i++) {
        p = addenv(blk, "HTTP_%s=%s",
                   conn->request_info.http_headers[i].name,
                   conn->request_info.http_headers[i].value);

<<<<<<< HEAD
        // Convert variable name into uppercase, and change - to _
=======
        /* Convert variable name into uppercase, and change - to _ */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        for (; *p != '=' && *p != '\0'; p++) {
            if (*p == '-')
                *p = '_';
            *p = (char) toupper(* (unsigned char *) p);
        }
    }

<<<<<<< HEAD
    // Add user-specified variables
=======
    /* Add user-specified variables */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    s = conn->ctx->config[CGI_ENVIRONMENT];
    while ((s = next_option(s, &var_vec, NULL)) != NULL) {
        addenv(blk, "%.*s", (int) var_vec.len, var_vec.ptr);
    }

    blk->vars[blk->nvars++] = NULL;
    blk->buf[blk->len++] = '\0';

    assert(blk->nvars < (int) ARRAY_SIZE(blk->vars));
    assert(blk->len > 0);
    assert(blk->len < (int) sizeof(blk->buf));
}

static void handle_cgi_request(struct mg_connection *conn, const char *prog)
{
    char *buf;
    size_t buflen;
    int headers_len, data_len, i, fdin[2] = { 0, 0 }, fdout[2] = { 0, 0 };
<<<<<<< HEAD
    const char *status, *status_text;
=======
    const char *status, *status_text, *connection_state;
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    char *pbuf, dir[PATH_MAX], *p;
    struct mg_request_info ri;
    struct cgi_env_block blk;
    FILE *in = NULL, *out = NULL;
    struct file fout = STRUCT_FILE_INITIALIZER;
    pid_t pid = (pid_t) -1;

    buf = NULL;
    buflen = 16384;
    prepare_cgi_environment(conn, prog, &blk);

<<<<<<< HEAD
    // CGI must be executed in its own directory. 'dir' must point to the
    // directory containing executable program, 'p' must point to the
    // executable program name relative to 'dir'.
=======
    /* CGI must be executed in its own directory. 'dir' must point to the
       directory containing executable program, 'p' must point to the
       executable program name relative to 'dir'. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    (void) mg_snprintf(conn, dir, sizeof(dir), "%s", prog);
    if ((p = strrchr(dir, '/')) != NULL) {
        *p++ = '\0';
    } else {
        dir[0] = '.', dir[1] = '\0';
        p = (char *) prog;
    }

    if (pipe(fdin) != 0 || pipe(fdout) != 0) {
        send_http_error(conn, 500, http_500_error,
                        "Cannot create CGI pipe: %s", strerror(ERRNO));
        goto done;
    }

    pid = spawn_process(conn, p, blk.buf, blk.vars, fdin[0], fdout[1], dir);
    if (pid == (pid_t) -1) {
        send_http_error(conn, 500, http_500_error,
                        "Cannot spawn CGI process [%s]: %s", prog, strerror(ERRNO));
        goto done;
    }

<<<<<<< HEAD
    // Make sure child closes all pipe descriptors. It must dup them to 0,1
=======
    /* Make sure child closes all pipe descriptors. It must dup them to 0,1 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    set_close_on_exec(fdin[0], conn);
    set_close_on_exec(fdin[1], conn);
    set_close_on_exec(fdout[0], conn);
    set_close_on_exec(fdout[1], conn);

<<<<<<< HEAD
    // Parent closes only one side of the pipes.
    // If we don't mark them as closed, close() attempt before
    // return from this function throws an exception on Windows.
    // Windows does not like when closed descriptor is closed again.
=======
    /* Parent closes only one side of the pipes.
       If we don't mark them as closed, close() attempt before
       return from this function throws an exception on Windows.
       Windows does not like when closed descriptor is closed again. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    (void) close(fdin[0]);
    (void) close(fdout[1]);
    fdin[0] = fdout[1] = -1;


    if ((in = fdopen(fdin[1], "wb")) == NULL ||
        (out = fdopen(fdout[0], "rb")) == NULL) {
        send_http_error(conn, 500, http_500_error,
                        "fopen: %s", strerror(ERRNO));
        goto done;
    }

    setbuf(in, NULL);
    setbuf(out, NULL);
    fout.fp = out;

<<<<<<< HEAD
    // Send POST data to the CGI process if needed
=======
    /* Send POST data to the CGI process if needed */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (!strcmp(conn->request_info.request_method, "POST") &&
        !forward_body_data(conn, in, INVALID_SOCKET, NULL)) {
        goto done;
    }

<<<<<<< HEAD
    // Close so child gets an EOF.
=======
    /* Close so child gets an EOF. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    fclose(in);
    in = NULL;
    fdin[1] = -1;

<<<<<<< HEAD
    // Now read CGI reply into a buffer. We need to set correct
    // status code, thus we need to see all HTTP headers first.
    // Do not send anything back to client, until we buffer in all
    // HTTP headers.
=======
    /* Now read CGI reply into a buffer. We need to set correct
       status code, thus we need to see all HTTP headers first.
       Do not send anything back to client, until we buffer in all
       HTTP headers. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    data_len = 0;
    buf = malloc(buflen);
    if (buf == NULL) {
        send_http_error(conn, 500, http_500_error,
                        "Not enough memory for buffer (%u bytes)",
                        (unsigned int) buflen);
        goto done;
    }
    headers_len = read_request(out, conn, buf, (int) buflen, &data_len);
    if (headers_len <= 0) {
        send_http_error(conn, 500, http_500_error,
                        "CGI program sent malformed or too big (>%u bytes) "
                        "HTTP headers: [%.*s]",
                        (unsigned) buflen, data_len, buf);
        goto done;
    }
    pbuf = buf;
    buf[headers_len - 1] = '\0';
    parse_http_headers(&pbuf, &ri);

<<<<<<< HEAD
    // Make up and send the status line
=======
    /* Make up and send the status line */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    status_text = "OK";
    if ((status = get_header(&ri, "Status")) != NULL) {
        conn->status_code = atoi(status);
        status_text = status;
        while (isdigit(* (unsigned char *) status_text) || *status_text == ' ') {
            status_text++;
        }
    } else if (get_header(&ri, "Location") != NULL) {
        conn->status_code = 302;
    } else {
        conn->status_code = 200;
    }
<<<<<<< HEAD
    if (get_header(&ri, "Connection") != NULL &&
        !mg_strcasecmp(get_header(&ri, "Connection"), "keep-alive")) {
=======
    connection_state = get_header(&ri, "Connection");
    if (connection_state == NULL ||
        mg_strcasecmp(connection_state, "keep-alive")) {
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        conn->must_close = 1;
    }
    (void) mg_printf(conn, "HTTP/1.1 %d %s\r\n", conn->status_code,
                     status_text);

<<<<<<< HEAD
    // Send headers
=======
    /* Send headers */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    for (i = 0; i < ri.num_headers; i++) {
        mg_printf(conn, "%s: %s\r\n",
                  ri.http_headers[i].name, ri.http_headers[i].value);
    }
    mg_write(conn, "\r\n", 2);

<<<<<<< HEAD
    // Send chunk of data that may have been read after the headers
    conn->num_bytes_sent += mg_write(conn, buf + headers_len,
                                     (size_t)(data_len - headers_len));

    // Read the rest of CGI output and send to the client
=======
    /* Send chunk of data that may have been read after the headers */
    conn->num_bytes_sent += mg_write(conn, buf + headers_len,
                                     (size_t)(data_len - headers_len));

    /* Read the rest of CGI output and send to the client */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    send_file_data(conn, &fout, 0, INT64_MAX);

done:
    if (pid != (pid_t) -1) {
        kill(pid, SIGKILL);
#if !defined(_WIN32)
        {
            int st;
<<<<<<< HEAD
            while (waitpid(pid, &st, 0) != -1);  // clean zombies
=======
            while (waitpid(pid, &st, 0) != -1);  /* clean zombies */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        }
#endif
    }
    if (fdin[0] != -1) {
        close(fdin[0]);
    }
    if (fdout[1] != -1) {
        close(fdout[1]);
    }

    if (in != NULL) {
        fclose(in);
    } else if (fdin[1] != -1) {
        close(fdin[1]);
    }

    if (out != NULL) {
        fclose(out);
    } else if (fdout[0] != -1) {
        close(fdout[0]);
    }
    if (buf != NULL) {
        free(buf);
    }
}
<<<<<<< HEAD
#endif // !NO_CGI

// For a given PUT path, create all intermediate subdirectories
// for given path. Return 0 if the path itself is a directory,
// or -1 on error, 1 if OK.
=======
#endif /* !NO_CGI */

/* For a given PUT path, create all intermediate subdirectories
   for given path. Return 0 if the path itself is a directory,
   or -1 on error, 1 if OK. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int put_dir(struct mg_connection *conn, const char *path)
{
    char buf[PATH_MAX];
    const char *s, *p;
    struct file file = STRUCT_FILE_INITIALIZER;
    int len, res = 1;

    for (s = p = path + 2; (p = strchr(s, '/')) != NULL; s = ++p) {
        len = (int)(p - path);
        if (len >= (int) sizeof(buf)) {
            res = -1;
            break;
        }
        memcpy(buf, path, len);
        buf[len] = '\0';

<<<<<<< HEAD
        // Try to create intermediate directory
=======
        /* Try to create intermediate directory */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        DEBUG_TRACE(("mkdir(%s)", buf));
        if (!mg_stat(conn, buf, &file) && mg_mkdir(buf, 0755) != 0) {
            res = -1;
            break;
        }

<<<<<<< HEAD
        // Is path itself a directory?
=======
        /* Is path itself a directory? */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (p[1] == '\0') {
            res = 0;
        }
    }

    return res;
}

static void mkcol(struct mg_connection *conn, const char *path)
{
    int rc, body_len;
    struct de de;
    memset(&de.file, 0, sizeof(de.file));
    if (!mg_stat(conn, path, &de.file)) {
        mg_cry(conn, "%s: mg_stat(%s) failed: %s",
               __func__, path, strerror(ERRNO));
    }

    if(de.file.modification_time) {
        send_http_error(conn, 405, "Method Not Allowed",
                        "mkcol(%s): %s", path, strerror(ERRNO));
        return;
    }

    body_len = conn->data_len - conn->request_len;
    if(body_len > 0) {
        send_http_error(conn, 415, "Unsupported media type",
                        "mkcol(%s): %s", path, strerror(ERRNO));
        return;
    }

    rc = mg_mkdir(path, 0755);

    if (rc == 0) {
        conn->status_code = 201;
        mg_printf(conn, "HTTP/1.1 %d Created\r\n\r\n", conn->status_code);
    } else if (rc == -1) {
        if(errno == EEXIST)
            send_http_error(conn, 405, "Method Not Allowed",
                            "mkcol(%s): %s", path, strerror(ERRNO));
        else if(errno == EACCES)
            send_http_error(conn, 403, "Forbidden",
                            "mkcol(%s): %s", path, strerror(ERRNO));
        else if(errno == ENOENT)
            send_http_error(conn, 409, "Conflict",
                            "mkcol(%s): %s", path, strerror(ERRNO));
        else
            send_http_error(conn, 500, http_500_error,
                            "fopen(%s): %s", path, strerror(ERRNO));
    }
}

static void put_file(struct mg_connection *conn, const char *path)
{
    struct file file = STRUCT_FILE_INITIALIZER;
    const char *range;
    int64_t r1, r2;
    int rc;

    conn->status_code = mg_stat(conn, path, &file) ? 200 : 201;

    if ((rc = put_dir(conn, path)) == 0) {
        mg_printf(conn, "HTTP/1.1 %d OK\r\n\r\n", conn->status_code);
    } else if (rc == -1) {
        send_http_error(conn, 500, http_500_error,
                        "put_dir(%s): %s", path, strerror(ERRNO));
    } else if (!mg_fopen(conn, path, "wb+", &file) || file.fp == NULL) {
        mg_fclose(&file);
        send_http_error(conn, 500, http_500_error,
                        "fopen(%s): %s", path, strerror(ERRNO));
    } else {
        fclose_on_exec(&file, conn);
        range = mg_get_header(conn, "Content-Range");
        r1 = r2 = 0;
        if (range != NULL && parse_range_header(range, &r1, &r2) > 0) {
            conn->status_code = 206;
            fseeko(file.fp, r1, SEEK_SET);
        }
        if (!forward_body_data(conn, file.fp, INVALID_SOCKET, NULL)) {
            conn->status_code = 500;
        }
        mg_printf(conn, "HTTP/1.1 %d OK\r\nContent-Length: 0\r\n\r\n",
                  conn->status_code);
        mg_fclose(&file);
    }
}

static void send_ssi_file(struct mg_connection *, const char *,
                          struct file *, int);

static void do_ssi_include(struct mg_connection *conn, const char *ssi,
                           char *tag, int include_level)
{
    char file_name[MG_BUF_LEN], path[PATH_MAX], *p;
    struct file file = STRUCT_FILE_INITIALIZER;

<<<<<<< HEAD
    // sscanf() is safe here, since send_ssi_file() also uses buffer
    // of size MG_BUF_LEN to get the tag. So strlen(tag) is always < MG_BUF_LEN.
    if (sscanf(tag, " virtual=\"%[^\"]\"", file_name) == 1) {
        // File name is relative to the webserver root
        (void) mg_snprintf(conn, path, sizeof(path), "%s%c%s",
                           conn->ctx->config[DOCUMENT_ROOT], '/', file_name);
    } else if (sscanf(tag, " abspath=\"%[^\"]\"", file_name) == 1) {
        // File name is relative to the webserver working directory
        // or it is absolute system path
        (void) mg_snprintf(conn, path, sizeof(path), "%s", file_name);
    } else if (sscanf(tag, " file=\"%[^\"]\"", file_name) == 1 ||
               sscanf(tag, " \"%[^\"]\"", file_name) == 1) {
        // File name is relative to the currect document
=======
    /* sscanf() is safe here, since send_ssi_file() also uses buffer
       of size MG_BUF_LEN to get the tag. So strlen(tag) is
       always < MG_BUF_LEN. */
    if (sscanf(tag, " virtual=\"%[^\"]\"", file_name) == 1) {
        /* File name is relative to the webserver root */
        (void) mg_snprintf(conn, path, sizeof(path), "%s%c%s",
                           conn->ctx->config[DOCUMENT_ROOT], '/', file_name);
    } else if (sscanf(tag, " abspath=\"%[^\"]\"", file_name) == 1) {
        /* File name is relative to the webserver working directory
           or it is absolute system path */
        (void) mg_snprintf(conn, path, sizeof(path), "%s", file_name);
    } else if (sscanf(tag, " file=\"%[^\"]\"", file_name) == 1 ||
               sscanf(tag, " \"%[^\"]\"", file_name) == 1) {
        /* File name is relative to the currect document */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        (void) mg_snprintf(conn, path, sizeof(path), "%s", ssi);
        if ((p = strrchr(path, '/')) != NULL) {
            p[1] = '\0';
        }
        (void) mg_snprintf(conn, path + strlen(path),
                           sizeof(path) - strlen(path), "%s", file_name);
    } else {
        mg_cry(conn, "Bad SSI #include: [%s]", tag);
        return;
    }

    if (!mg_fopen(conn, path, "rb", &file)) {
        mg_cry(conn, "Cannot open SSI #include: [%s]: fopen(%s): %s",
               tag, path, strerror(ERRNO));
    } else {
        fclose_on_exec(&file, conn);
        if (match_prefix(conn->ctx->config[SSI_EXTENSIONS],
                         (int)strlen(conn->ctx->config[SSI_EXTENSIONS]), path) > 0) {
            send_ssi_file(conn, path, &file, include_level + 1);
        } else {
            send_file_data(conn, &file, 0, INT64_MAX);
        }
        mg_fclose(&file);
    }
}

#if !defined(NO_POPEN)
static void do_ssi_exec(struct mg_connection *conn, char *tag)
{
    char cmd[MG_BUF_LEN] = "";
    struct file file = STRUCT_FILE_INITIALIZER;

    if (sscanf(tag, " \"%[^\"]\"", cmd) != 1) {
        mg_cry(conn, "Bad SSI #exec: [%s]", tag);
    } else if ((file.fp = popen(cmd, "r")) == NULL) {
        mg_cry(conn, "Cannot SSI #exec: [%s]: %s", cmd, strerror(ERRNO));
    } else {
        send_file_data(conn, &file, 0, INT64_MAX);
        pclose(file.fp);
    }
}
<<<<<<< HEAD
#endif // !NO_POPEN
=======
#endif /* !NO_POPEN */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static int mg_fgetc(struct file *filep, int offset)
{
    if (filep->membuf != NULL && offset >=0 && offset < filep->size) {
        return ((unsigned char *) filep->membuf)[offset];
    } else if (filep->fp != NULL) {
        return fgetc(filep->fp);
    } else {
        return EOF;
    }
}

static void send_ssi_file(struct mg_connection *conn, const char *path,
                          struct file *filep, int include_level)
{
    char buf[MG_BUF_LEN];
    int ch, offset, len, in_ssi_tag;

    if (include_level > 10) {
        mg_cry(conn, "SSI #include level is too deep (%s)", path);
        return;
    }

    in_ssi_tag = len = offset = 0;
    while ((ch = mg_fgetc(filep, offset)) != EOF) {
        if (in_ssi_tag && ch == '>') {
            in_ssi_tag = 0;
            buf[len++] = (char) ch;
            buf[len] = '\0';
            assert(len <= (int) sizeof(buf));
            if (len < 6 || memcmp(buf, "<!--#", 5) != 0) {
<<<<<<< HEAD
                // Not an SSI tag, pass it
=======
                /* Not an SSI tag, pass it */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                (void) mg_write(conn, buf, (size_t) len);
            } else {
                if (!memcmp(buf + 5, "include", 7)) {
                    do_ssi_include(conn, path, buf + 12, include_level);
#if !defined(NO_POPEN)
                } else if (!memcmp(buf + 5, "exec", 4)) {
                    do_ssi_exec(conn, buf + 9);
<<<<<<< HEAD
#endif // !NO_POPEN
=======
#endif /* !NO_POPEN */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                } else {
                    mg_cry(conn, "%s: unknown SSI " "command: \"%s\"", path, buf);
                }
            }
            len = 0;
        } else if (in_ssi_tag) {
            if (len == 5 && memcmp(buf, "<!--#", 5) != 0) {
<<<<<<< HEAD
                // Not an SSI tag
=======
                /* Not an SSI tag */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                in_ssi_tag = 0;
            } else if (len == (int) sizeof(buf) - 2) {
                mg_cry(conn, "%s: SSI tag is too large", path);
                len = 0;
            }
            buf[len++] = ch & 0xff;
        } else if (ch == '<') {
            in_ssi_tag = 1;
            if (len > 0) {
                mg_write(conn, buf, (size_t) len);
            }
            len = 0;
            buf[len++] = ch & 0xff;
        } else {
            buf[len++] = ch & 0xff;
            if (len == (int) sizeof(buf)) {
                mg_write(conn, buf, (size_t) len);
                len = 0;
            }
        }
    }

<<<<<<< HEAD
    // Send the rest of buffered data
=======
    /* Send the rest of buffered data */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (len > 0) {
        mg_write(conn, buf, (size_t) len);
    }
}

static void handle_ssi_file_request(struct mg_connection *conn,
                                    const char *path)
{
    struct file file = STRUCT_FILE_INITIALIZER;

    if (!mg_fopen(conn, path, "rb", &file)) {
        send_http_error(conn, 500, http_500_error, "fopen(%s): %s", path,
                        strerror(ERRNO));
    } else {
        conn->must_close = 1;
        fclose_on_exec(&file, conn);
        mg_printf(conn, "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/html\r\nConnection: %s\r\n\r\n",
                  suggest_connection_header(conn));
        send_ssi_file(conn, path, &file, 0);
        mg_fclose(&file);
    }
}

static void send_options(struct mg_connection *conn)
{
    conn->status_code = 200;

    mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n"
              "Allow: GET, POST, HEAD, CONNECT, PUT, DELETE, OPTIONS, PROPFIND, MKCOL\r\n"
              "DAV: 1\r\n\r\n");
}

<<<<<<< HEAD
// Writes PROPFIND properties for a collection element
=======
/* Writes PROPFIND properties for a collection element */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void print_props(struct mg_connection *conn, const char* uri,
                        struct file *filep)
{
    char mtime[64];
    gmt_time_string(mtime, sizeof(mtime), &filep->modification_time);
    conn->num_bytes_sent += mg_printf(conn,
                                      "<d:response>"
                                      "<d:href>%s</d:href>"
                                      "<d:propstat>"
                                      "<d:prop>"
                                      "<d:resourcetype>%s</d:resourcetype>"
                                      "<d:getcontentlength>%" INT64_FMT "</d:getcontentlength>"
                                      "<d:getlastmodified>%s</d:getlastmodified>"
                                      "</d:prop>"
                                      "<d:status>HTTP/1.1 200 OK</d:status>"
                                      "</d:propstat>"
                                      "</d:response>\n",
                                      uri,
                                      filep->is_directory ? "<d:collection/>" : "",
                                      filep->size,
                                      mtime);
}

static void print_dav_dir_entry(struct de *de, void *data)
{
    char href[PATH_MAX];
    char href_encoded[PATH_MAX];
    struct mg_connection *conn = (struct mg_connection *) data;
    mg_snprintf(conn, href, sizeof(href), "%s%s",
                conn->request_info.uri, de->file_name);
    mg_url_encode(href, href_encoded, PATH_MAX-1);
    print_props(conn, href_encoded, &de->file);
}

static void handle_propfind(struct mg_connection *conn, const char *path,
                            struct file *filep)
{
    const char *depth = mg_get_header(conn, "Depth");

    conn->must_close = 1;
    conn->status_code = 207;
    mg_printf(conn, "HTTP/1.1 207 Multi-Status\r\n"
              "Connection: close\r\n"
              "Content-Type: text/xml; charset=utf-8\r\n\r\n");

    conn->num_bytes_sent += mg_printf(conn,
                                      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                      "<d:multistatus xmlns:d='DAV:'>\n");

<<<<<<< HEAD
    // Print properties for the requested resource itself
    print_props(conn, conn->request_info.uri, filep);

    // If it is a directory, print directory entries too if Depth is not 0
=======
    /* Print properties for the requested resource itself */
    print_props(conn, conn->request_info.uri, filep);

    /* If it is a directory, print directory entries too if Depth is not 0 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (filep->is_directory &&
        !mg_strcasecmp(conn->ctx->config[ENABLE_DIRECTORY_LISTING], "yes") &&
        (depth == NULL || strcmp(depth, "0") != 0)) {
        scan_directory(conn, path, conn, &print_dav_dir_entry);
    }

    conn->num_bytes_sent += mg_printf(conn, "%s\n", "</d:multistatus>");
}

void mg_lock(struct mg_connection* conn)
{
    (void) pthread_mutex_lock(&conn->mutex);
}

void mg_unlock(struct mg_connection* conn)
{
    (void) pthread_mutex_unlock(&conn->mutex);
}

#if defined(USE_WEBSOCKET)

<<<<<<< HEAD
// START OF SHA-1 code
// Copyright(c) By Steve Reid <steve@edmweb.com>
=======
/* START OF SHA-1 code
   Copyright(c) By Steve Reid <steve@edmweb.com> */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#define SHA1HANDSOFF
#if defined(__sun)
#include "solarisfixes.h"
#endif

static int is_big_endian(void)
{
    static const int n = 1;
    return ((char *) &n)[0] == 0;
}

union char64long16 {
    unsigned char c[64];
    uint32_t l[16];
};

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

static uint32_t blk0(union char64long16 *block, int i)
{
<<<<<<< HEAD
    // Forrest: SHA expect BIG_ENDIAN, swap if LITTLE_ENDIAN
=======
    /* Forrest: SHA expect BIG_ENDIAN, swap if LITTLE_ENDIAN */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (!is_big_endian()) {
        block->l[i] = (rol(block->l[i], 24) & 0xFF00FF00) |
                      (rol(block->l[i], 8) & 0x00FF00FF);
    }
    return block->l[i];
}

#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(block, i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;

static void SHA1Transform(uint32_t state[5], const unsigned char buffer[64])
{
    uint32_t a, b, c, d, e;
    union char64long16 block[1];

    memcpy(block, buffer, 64);
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    R0(a,b,c,d,e, 0);
    R0(e,a,b,c,d, 1);
    R0(d,e,a,b,c, 2);
    R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4);
    R0(a,b,c,d,e, 5);
    R0(e,a,b,c,d, 6);
    R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8);
    R0(b,c,d,e,a, 9);
    R0(a,b,c,d,e,10);
    R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12);
    R0(c,d,e,a,b,13);
    R0(b,c,d,e,a,14);
    R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16);
    R1(d,e,a,b,c,17);
    R1(c,d,e,a,b,18);
    R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20);
    R2(e,a,b,c,d,21);
    R2(d,e,a,b,c,22);
    R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24);
    R2(a,b,c,d,e,25);
    R2(e,a,b,c,d,26);
    R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28);
    R2(b,c,d,e,a,29);
    R2(a,b,c,d,e,30);
    R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32);
    R2(c,d,e,a,b,33);
    R2(b,c,d,e,a,34);
    R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36);
    R2(d,e,a,b,c,37);
    R2(c,d,e,a,b,38);
    R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40);
    R3(e,a,b,c,d,41);
    R3(d,e,a,b,c,42);
    R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44);
    R3(a,b,c,d,e,45);
    R3(e,a,b,c,d,46);
    R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48);
    R3(b,c,d,e,a,49);
    R3(a,b,c,d,e,50);
    R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52);
    R3(c,d,e,a,b,53);
    R3(b,c,d,e,a,54);
    R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56);
    R3(d,e,a,b,c,57);
    R3(c,d,e,a,b,58);
    R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60);
    R4(e,a,b,c,d,61);
    R4(d,e,a,b,c,62);
    R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64);
    R4(a,b,c,d,e,65);
    R4(e,a,b,c,d,66);
    R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68);
    R4(b,c,d,e,a,69);
    R4(a,b,c,d,e,70);
    R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72);
    R4(c,d,e,a,b,73);
    R4(b,c,d,e,a,74);
    R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76);
    R4(d,e,a,b,c,77);
    R4(c,d,e,a,b,78);
    R4(b,c,d,e,a,79);
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    a = b = c = d = e = 0;
    memset(block, '\0', sizeof(block));
}

static void SHA1Init(SHA1_CTX* context)
{
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->count[0] = context->count[1] = 0;
}

static void SHA1Update(SHA1_CTX* context, const unsigned char* data,
                       uint32_t len)
{
    uint32_t i, j;

    j = context->count[0];
    if ((context->count[0] += len << 3) < j)
        context->count[1]++;
    context->count[1] += (len>>29);
    j = (j >> 3) & 63;
    if ((j + len) > 63) {
        memcpy(&context->buffer[j], data, (i = 64-j));
        SHA1Transform(context->state, context->buffer);
        for ( ; i + 63 < len; i += 64) {
            SHA1Transform(context->state, &data[i]);
        }
        j = 0;
    } else i = 0;
    memcpy(&context->buffer[j], &data[i], len - i);
}

static void SHA1Final(unsigned char digest[20], SHA1_CTX* context)
{
    unsigned i;
    unsigned char finalcount[8], c;

    for (i = 0; i < 8; i++) {
        finalcount[i] = (unsigned char)((context->count[(i >= 4 ? 0 : 1)]
                                         >> ((3-(i & 3)) * 8) ) & 255);
    }
    c = 0200;
    SHA1Update(context, &c, 1);
    while ((context->count[0] & 504) != 448) {
        c = 0000;
        SHA1Update(context, &c, 1);
    }
    SHA1Update(context, finalcount, 8);
    for (i = 0; i < 20; i++) {
        digest[i] = (unsigned char)
                    ((context->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
    }
    memset(context, '\0', sizeof(*context));
    memset(&finalcount, '\0', sizeof(finalcount));
}
<<<<<<< HEAD
// END OF SHA1 CODE
=======
/* END OF SHA1 CODE */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static void base64_encode(const unsigned char *src, int src_len, char *dst)
{
    static const char *b64 =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int i, j, a, b, c;

    for (i = j = 0; i < src_len; i += 3) {
        a = src[i];
        b = i + 1 >= src_len ? 0 : src[i + 1];
        c = i + 2 >= src_len ? 0 : src[i + 2];

        dst[j++] = b64[a >> 2];
        dst[j++] = b64[((a & 3) << 4) | (b >> 4)];
        if (i + 1 < src_len) {
            dst[j++] = b64[(b & 15) << 2 | (c >> 6)];
        }
        if (i + 2 < src_len) {
            dst[j++] = b64[c & 63];
        }
    }
    while (j % 4 != 0) {
        dst[j++] = '=';
    }
    dst[j++] = '\0';
}

static void send_websocket_handshake(struct mg_connection *conn)
{
    static const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    char buf[100], sha[20], b64_sha[sizeof(sha) * 2];
    SHA1_CTX sha_ctx;

    mg_snprintf(conn, buf, sizeof(buf), "%s%s",
                mg_get_header(conn, "Sec-WebSocket-Key"), magic);
    SHA1Init(&sha_ctx);
    SHA1Update(&sha_ctx, (unsigned char *) buf, (uint32_t)strlen(buf));
    SHA1Final((unsigned char *) sha, &sha_ctx);
    base64_encode((unsigned char *) sha, sizeof(sha), b64_sha);
    mg_printf(conn, "%s%s%s",
              "HTTP/1.1 101 Switching Protocols\r\n"
              "Upgrade: websocket\r\n"
              "Connection: Upgrade\r\n"
              "Sec-WebSocket-Accept: ", b64_sha, "\r\n\r\n");
}

static void read_websocket(struct mg_connection *conn)
{
<<<<<<< HEAD
    // Pointer to the beginning of the portion of the incoming websocket message queue.
    // The original websocket upgrade request is never removed, so the queue begins after it.
    unsigned char *buf = (unsigned char *) conn->buf + conn->request_len;
    int n;

    // body_len is the length of the entire queue in bytes
    // len is the length of the current message
    // data_len is the length of the current message's data payload
    // header_len is the length of the current message's header
    size_t i, len, mask_len, data_len, header_len, body_len;

    // "The masking key is a 32-bit value chosen at random by the client."
    // http://tools.ietf.org/html/draft-ietf-hybi-thewebsocketprotocol-17#section-5
    unsigned char mask[4];

    // data points to the place where the message is stored when passed to the websocket_data
    // callback.  This is either mem on the stack, or a dynamically allocated buffer if it is
    // too large.
    char mem[4096];
    char *data = mem;

    // Loop continuously, reading messages from the socket, invoking the callback,
    // and waiting repeatedly until an error occurs.
=======
    /* Pointer to the beginning of the portion of the incoming websocket
       message queue.
       The original websocket upgrade request is never removed, so the queue
       begins after it. */
    unsigned char *buf = (unsigned char *) conn->buf + conn->request_len;
    int n;

    /* body_len is the length of the entire queue in bytes
       len is the length of the current message
       data_len is the length of the current message's data payload
       header_len is the length of the current message's header */
    size_t i, len, mask_len, data_len, header_len, body_len;

    /* "The masking key is a 32-bit value chosen at random by the client."
       http://tools.ietf.org/html/draft-ietf-hybi-thewebsocketprotocol-17#section-5 */
    unsigned char mask[4];

    /* data points to the place where the message is stored when passed to the
       websocket_data callback.  This is either mem on the stack, or a
       dynamically allocated buffer if it is too large. */
    char mem[4096];
    char *data = mem;

    /* Loop continuously, reading messages from the socket, invoking the
       callback, and waiting repeatedly until an error occurs. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    assert(conn->content_len == 0);
    for (;;) {
        header_len = 0;
        if ((body_len = conn->data_len - conn->request_len) >= 2) {
            len = buf[1] & 127;
            mask_len = buf[1] & 128 ? 4 : 0;
            if (len < 126 && body_len >= mask_len) {
                data_len = len;
                header_len = 2 + mask_len;
            } else if (len == 126 && body_len >= 4 + mask_len) {
                header_len = 4 + mask_len;
                data_len = ((((int) buf[2]) << 8) + buf[3]);
            } else if (body_len >= 10 + mask_len) {
                header_len = 10 + mask_len;
                data_len = (((uint64_t) ntohl(* (uint32_t *) &buf[2])) << 32) +
                           ntohl(* (uint32_t *) &buf[6]);
            }
        }

        if (header_len > 0) {
<<<<<<< HEAD
            // Allocate space to hold websocket payload
=======
            /* Allocate space to hold websocket payload */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            data = mem;
            if (data_len > sizeof(mem)) {
                data = (char *)malloc(data_len);
                if (data == NULL) {
<<<<<<< HEAD
                    // Allocation failed, exit the loop and then close the connection
                    // TODO: notify user about the failure
=======
                    /* Allocation failed, exit the loop and then close the
                       connection */
                    /* TODO: notify user about the failure */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                    break;
                }
            }

<<<<<<< HEAD
            // Copy the mask before we shift the queue and destroy it
=======
            /* Copy the mask before we shift the queue and destroy it */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            if (mask_len > 0) {
                *(uint32_t*)mask = *(uint32_t*)(buf + header_len - mask_len);
            } else {
                *(uint32_t*)mask = 0;
            }

<<<<<<< HEAD
            // Read frame payload from the first message in the queue into data and
            // advance the queue by moving the memory in place.
            assert(body_len >= header_len);
            if (data_len + header_len > body_len) {
                // Overflow case
                len = body_len - header_len;
                memcpy(data, buf + header_len, len);
                // TODO: handle pull error
                pull(NULL, conn, data + len, (int)(data_len - len));
                conn->data_len = 0;
            } else {
                // Length of the message being read at the front of the queue
                len = data_len + header_len;

                // Copy the data payload into the data pointer for the callback
                memcpy(data, buf + header_len, data_len);

                // Move the queue forward len bytes
                memmove(buf, buf + len, body_len - len);

                // Mark the queue as advanced
                conn->data_len -= (int)len;
            }

            // Apply mask if necessary
=======
            /* Read frame payload from the first message in the queue into
               data and advance the queue by moving the memory in place. */
            assert(body_len >= header_len);
            if (data_len + header_len > body_len) {
                /* Overflow case */
                len = body_len - header_len;
                memcpy(data, buf + header_len, len);
                /* TODO: handle pull error */
                pull(NULL, conn, data + len, (int)(data_len - len));
                conn->data_len = 0;
            } else {
                /* Length of the message being read at the front of the
                   queue */
                len = data_len + header_len;

                /* Copy the data payload into the data pointer for the
                   callback */
                memcpy(data, buf + header_len, data_len);

                /* Move the queue forward len bytes */
                memmove(buf, buf + len, body_len - len);

                /* Mark the queue as advanced */
                conn->data_len -= (int)len;
            }

            /* Apply mask if necessary */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            if (mask_len > 0) {
                for (i = 0; i < data_len; ++i) {
                    data[i] ^= mask[i & 3];
                }
            }

<<<<<<< HEAD
            // Exit the loop if callback signalled to exit,
            // or "connection close" opcode received.
            if ((conn->ctx->callbacks.websocket_data != NULL &&
                 !conn->ctx->callbacks.websocket_data(conn, buf[0], data, data_len)) ||
                (buf[0] & 0xf) == WEBSOCKET_OPCODE_CONNECTION_CLOSE) {  // Opcode == 8, connection close
=======
            /* Exit the loop if callback signalled to exit,
               or "connection close" opcode received. */
            if ((conn->ctx->callbacks.websocket_data != NULL &&
                 !conn->ctx->callbacks.websocket_data(conn, buf[0], data, data_len)) ||
                (buf[0] & 0xf) == WEBSOCKET_OPCODE_CONNECTION_CLOSE) {  /* Opcode == 8, connection close */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                break;
            }

            if (data != mem) {
                free(data);
            }
<<<<<<< HEAD
            // Not breaking the loop, process next websocket frame.
        } else {
            // Read from the socket into the next available location in the message queue.
            if ((n = pull(NULL, conn, conn->buf + conn->data_len,
                          conn->buf_size - conn->data_len)) <= 0) {
                // Error, no bytes read
=======
            /* Not breaking the loop, process next websocket frame. */
        } else {
            /* Read from the socket into the next available location in the
               message queue. */
            if ((n = pull(NULL, conn, conn->buf + conn->data_len,
                          conn->buf_size - conn->data_len)) <= 0) {
                /* Error, no bytes read */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                break;
            }
            conn->data_len += n;
        }
    }
}

int mg_websocket_write(struct mg_connection* conn, int opcode, const char* data, size_t dataLen)
{
    unsigned char header[10];
    size_t headerLen = 1;

    int retval = -1;

    header[0] = 0x80 + (opcode & 0xF);

<<<<<<< HEAD
    // Frame format: http://tools.ietf.org/html/rfc6455#section-5.2
    if (dataLen < 126) {
        // inline 7-bit length field
        header[1] = (unsigned char)dataLen;
        headerLen = 2;
    } else if (dataLen <= 0xFFFF) {
        // 16-bit length field
=======
    /* Frame format: http://tools.ietf.org/html/rfc6455#section-5.2 */
    if (dataLen < 126) {
        /* inline 7-bit length field */
        header[1] = (unsigned char)dataLen;
        headerLen = 2;
    } else if (dataLen <= 0xFFFF) {
        /* 16-bit length field */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        header[1] = 126;
        *(uint16_t*)(header + 2) = htons((uint16_t)dataLen);
        headerLen = 4;
    } else {
<<<<<<< HEAD
        // 64-bit length field
=======
        /* 64-bit length field */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        header[1] = 127;
        *(uint32_t*)(header + 2) = htonl((uint64_t)dataLen >> 32);
        *(uint32_t*)(header + 6) = htonl(dataLen & 0xFFFFFFFF);
        headerLen = 10;
    }

<<<<<<< HEAD
    // Note that POSIX/Winsock's send() is threadsafe
    // http://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid
    // but mongoose's mg_printf/mg_write is not (because of the loop in push(), although that is only
    // a problem if the packet is large or outgoing buffer is full).
=======
    /* Note that POSIX/Winsock's send() is threadsafe
       http://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid
       but mongoose's mg_printf/mg_write is not (because of the loop in
       push(), although that is only a problem if the packet is large or
       outgoing buffer is full). */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    (void) mg_lock(conn);
    retval = mg_write(conn, header, headerLen);
    retval = mg_write(conn, data, dataLen);
    mg_unlock(conn);

    return retval;
}

static void handle_websocket_request(struct mg_connection *conn)
{
    const char *version = mg_get_header(conn, "Sec-WebSocket-Version");
    if (version == NULL || strcmp(version, "13") != 0) {
        send_http_error(conn, 426, "Upgrade Required", "%s", "Upgrade Required");
    } else if (conn->ctx->callbacks.websocket_connect != NULL &&
               conn->ctx->callbacks.websocket_connect(conn) != 0) {
<<<<<<< HEAD
        // Callback has returned non-zero, do not proceed with handshake
=======
        /* Callback has returned non-zero, do not proceed with handshake */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    } else {
        send_websocket_handshake(conn);
        if (conn->ctx->callbacks.websocket_ready != NULL) {
            conn->ctx->callbacks.websocket_ready(conn);
        }
        read_websocket(conn);
    }
}

static int is_websocket_request(const struct mg_connection *conn)
{
    const char *host, *upgrade, *connection, *version, *key;

    host = mg_get_header(conn, "Host");
    upgrade = mg_get_header(conn, "Upgrade");
    connection = mg_get_header(conn, "Connection");
    key = mg_get_header(conn, "Sec-WebSocket-Key");
    version = mg_get_header(conn, "Sec-WebSocket-Version");

    return host != NULL && upgrade != NULL && connection != NULL &&
           key != NULL && version != NULL &&
           mg_strcasestr(upgrade, "websocket") != NULL &&
           mg_strcasestr(connection, "Upgrade") != NULL;
}
<<<<<<< HEAD
#endif // !USE_WEBSOCKET
=======
#endif /* !USE_WEBSOCKET */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static int isbyte(int n)
{
    return n >= 0 && n <= 255;
}

static int parse_net(const char *spec, uint32_t *net, uint32_t *mask)
{
    int n, a, b, c, d, slash = 32, len = 0;

    if ((sscanf(spec, "%d.%d.%d.%d/%d%n", &a, &b, &c, &d, &slash, &n) == 5 ||
         sscanf(spec, "%d.%d.%d.%d%n", &a, &b, &c, &d, &n) == 4) &&
        isbyte(a) && isbyte(b) && isbyte(c) && isbyte(d) &&
        slash >= 0 && slash < 33) {
        len = n;
        *net = ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | d;
        *mask = slash ? 0xffffffffU << (32 - slash) : 0;
    }

    return len;
}

static int set_throttle(const char *spec, uint32_t remote_ip, const char *uri)
{
    int throttle = 0;
    struct vec vec, val;
    uint32_t net, mask;
    char mult;
    double v;

    while ((spec = next_option(spec, &vec, &val)) != NULL) {
        mult = ',';
        if (sscanf(val.ptr, "%lf%c", &v, &mult) < 1 || v < 0 ||
            (lowercase(&mult) != 'k' && lowercase(&mult) != 'm' && mult != ',')) {
            continue;
        }
        v *= lowercase(&mult) == 'k' ? 1024 : lowercase(&mult) == 'm' ? 1048576 : 1;
        if (vec.len == 1 && vec.ptr[0] == '*') {
            throttle = (int) v;
        } else if (parse_net(vec.ptr, &net, &mask) > 0) {
            if ((remote_ip & mask) == net) {
                throttle = (int) v;
            }
        } else if (match_prefix(vec.ptr, (int)vec.len, uri) > 0) {
            throttle = (int) v;
        }
    }

    return throttle;
}

static uint32_t get_remote_ip(const struct mg_connection *conn)
{
    return ntohl(* (uint32_t *) &conn->client.rsa.sin.sin_addr);
}

#ifdef USE_LUA
#include "mod_lua.inl"
<<<<<<< HEAD
#endif // USE_LUA
=======
#endif /* USE_LUA */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

int mg_upload(struct mg_connection *conn, const char *destination_dir)
{
    const char *content_type_header, *boundary_start;
    char buf[MG_BUF_LEN], path[PATH_MAX], fname[1024], boundary[100], *s;
    FILE *fp;
    int bl, n, i, j, headers_len, boundary_len, eof,
        len = 0, num_uploaded_files = 0;

<<<<<<< HEAD
    // Request looks like this:
    //
    // POST /upload HTTP/1.1
    // Host: 127.0.0.1:8080
    // Content-Length: 244894
    // Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryRVr
    //
    // ------WebKitFormBoundaryRVr
    // Content-Disposition: form-data; name="file"; filename="accum.png"
    // Content-Type: image/png
    //
    //  <89>PNG
    //  <PNG DATA>
    // ------WebKitFormBoundaryRVr

    // Extract boundary string from the Content-Type header
=======
    /* Request looks like this:

       POST /upload HTTP/1.1
       Host: 127.0.0.1:8080
       Content-Length: 244894
       Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryRVr

       ------WebKitFormBoundaryRVr
       Content-Disposition: form-data; name="file"; filename="accum.png"
       Content-Type: image/png

        <89>PNG
        <PNG DATA>
       ------WebKitFormBoundaryRVr */

    /* Extract boundary string from the Content-Type header */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if ((content_type_header = mg_get_header(conn, "Content-Type")) == NULL ||
        (boundary_start = mg_strcasestr(content_type_header,
                                        "boundary=")) == NULL ||
        (sscanf(boundary_start, "boundary=\"%99[^\"]\"", boundary) == 0 &&
         sscanf(boundary_start, "boundary=%99s", boundary) == 0) ||
        boundary[0] == '\0') {
        return num_uploaded_files;
    }

    boundary_len = (int)strlen(boundary);
<<<<<<< HEAD
    bl = boundary_len + 4;  // \r\n--<boundary>
    for (;;) {
        // Pull in headers
=======
    bl = boundary_len + 4;  /* \r\n--<boundary> */
    for (;;) {
        /* Pull in headers */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        assert(len >= 0 && len <= (int) sizeof(buf));
        while ((n = mg_read(conn, buf + len, sizeof(buf) - len)) > 0) {
            len += n;
        }
        if ((headers_len = get_request_len(buf, len)) <= 0) {
            break;
        }

<<<<<<< HEAD
        // Fetch file name.
=======
        /* Fetch file name. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        fname[0] = '\0';
        for (i = j = 0; i < headers_len; i++) {
            if (buf[i] == '\r' && buf[i + 1] == '\n') {
                buf[i] = buf[i + 1] = '\0';
<<<<<<< HEAD
                // TODO(lsm): don't expect filename to be the 3rd field,
                // parse the header properly instead.
=======
                /* TODO(lsm): don't expect filename to be the 3rd field,
                   parse the header properly instead. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                IGNORE_UNUSED_RESULT(sscanf(&buf[j], "Content-Disposition: %*s %*s filename=\"%1023[^\"]",
                                            fname));
                j = i + 2;
            }
        }

<<<<<<< HEAD
        // Give up if the headers are not what we expect
=======
        /* Give up if the headers are not what we expect */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (fname[0] == '\0') {
            break;
        }

<<<<<<< HEAD
        // Move data to the beginning of the buffer
=======
        /* Move data to the beginning of the buffer */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        assert(len >= headers_len);
        memmove(buf, &buf[headers_len], len - headers_len);
        len -= headers_len;

<<<<<<< HEAD
        // We open the file with exclusive lock held. This guarantee us
        // there is no other thread can save into the same file simultaneously.
        fp = NULL;
        // Construct destination file name. Do not allow paths to have slashes.
=======
        /* We open the file with exclusive lock held. This guarantee us
           there is no other thread can save into the same file
           simultaneously. */
        fp = NULL;
        /* Construct destination file name. Do not allow paths to have
           slashes. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if ((s = strrchr(fname, '/')) == NULL &&
            (s = strrchr(fname, '\\')) == NULL) {
            s = fname;
        }

<<<<<<< HEAD
        // Open file in binary mode. TODO: set an exclusive lock.
=======
        /* Open file in binary mode. TODO: set an exclusive lock. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        snprintf(path, sizeof(path), "%s/%s", destination_dir, s);
        if ((fp = fopen(path, "wb")) == NULL) {
            break;
        }

<<<<<<< HEAD
        // Read POST data, write into file until boundary is found.
=======
        /* Read POST data, write into file until boundary is found. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        eof = n = 0;
        do {
            len += n;
            for (i = 0; i < len - bl; i++) {
                if (!memcmp(&buf[i], "\r\n--", 4) &&
                    !memcmp(&buf[i + 4], boundary, boundary_len)) {
<<<<<<< HEAD
                    // Found boundary, that's the end of file data.
=======
                    /* Found boundary, that's the end of file data. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                    fwrite(buf, 1, i, fp);
                    eof = 1;
                    memmove(buf, &buf[i + bl], len - (i + bl));
                    len -= i + bl;
                    break;
                }
            }
            if (!eof && len > bl) {
                fwrite(buf, 1, len - bl, fp);
                memmove(buf, &buf[len - bl], bl);
                len = bl;
            }
        } while (!eof && (n = mg_read(conn, buf + len, sizeof(buf) - len)) > 0);
        fclose(fp);
        if (eof) {
            num_uploaded_files++;
            if (conn->ctx->callbacks.upload != NULL) {
                conn->ctx->callbacks.upload(conn, path);
            }
        }
    }

    return num_uploaded_files;
}

static int is_put_or_delete_request(const struct mg_connection *conn)
{
    const char *s = conn->request_info.request_method;
    return s != NULL && (!strcmp(s, "PUT") ||
                         !strcmp(s, "DELETE") ||
                         !strcmp(s, "MKCOL"));
}

static int get_first_ssl_listener_index(const struct mg_context *ctx)
{
    int i, idx = -1;
    for (i = 0; idx == -1 && i < ctx->num_listening_sockets; i++) {
        idx = ctx->listening_sockets[i].is_ssl ? i : -1;
    }
    return idx;
}

static void redirect_to_https_port(struct mg_connection *conn, int ssl_index)
{
    char host[1025];
    const char *host_header;
    size_t hostlen;

    host_header = mg_get_header(conn, "Host");
    hostlen = sizeof(host);
    if (host_header != NULL) {
        char *pos;

        strncpy(host, host_header, hostlen);
        host[hostlen - 1] = '\0';
        pos = strchr(host, ':');
        if (pos != NULL) {
            *pos = '\0';
        }
    } else {
<<<<<<< HEAD
        // Cannot get host from the Host: header. Fallback to our IP address.
=======
        /* Cannot get host from the Host: header.
           Fallback to our IP address. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        sockaddr_to_string(host, hostlen, &conn->client.lsa);
    }

    mg_printf(conn, "HTTP/1.1 302 Found\r\nLocation: https://%s:%d%s\r\n\r\n",
              host, (int) ntohs(conn->ctx->listening_sockets[ssl_index].
                                lsa.sin.sin_port), conn->request_info.uri);
}


void mg_set_request_handler(struct mg_context *ctx, const char *uri, mg_request_handler handler, void *cbdata)
{
    struct mg_request_handler_info *tmp_rh, *lastref = 0;
    size_t urilen = strlen(uri);

<<<<<<< HEAD
    // first see it the uri exists
    for (tmp_rh = ctx->request_handlers;
         tmp_rh != NULL && strcmp(uri, tmp_rh->uri);
         lastref = tmp_rh, tmp_rh = tmp_rh->next) {
        // first try for an exact match
        if (urilen == tmp_rh->uri_len && !strcmp(tmp_rh->uri,uri)) {
            // already there...

            if (handler != NULL) {
                // change this entry
                tmp_rh->handler = handler;
                tmp_rh->cbdata = cbdata;
            } else {
                // remove this entry
=======
    /* first see it the uri exists */
    for (tmp_rh = ctx->request_handlers;
         tmp_rh != NULL && strcmp(uri, tmp_rh->uri);
         lastref = tmp_rh, tmp_rh = tmp_rh->next) {
        /* first try for an exact match */
        if (urilen == tmp_rh->uri_len && !strcmp(tmp_rh->uri,uri)) {
            /* already there... */

            if (handler != NULL) {
                /* change this entry */
                tmp_rh->handler = handler;
                tmp_rh->cbdata = cbdata;
            } else {
                /* remove this entry */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                if (lastref != NULL)
                    lastref->next = tmp_rh->next;
                else
                    ctx->request_handlers = tmp_rh->next;
                free(tmp_rh->uri);
                free(tmp_rh);
            }
            return;
        }

<<<<<<< HEAD
        // next try for a partial match, we will accept uri/something
        if (tmp_rh->uri_len < urilen
            && uri[tmp_rh->uri_len] == '/'
            && memcmp(tmp_rh->uri, uri, tmp_rh->uri_len) == 0) {
            // if there is a partical match this new entry MUST go BEFORE
            // the current position otherwise it will never be matched.
=======
        /* next try for a partial match, we will accept uri/something */
        if (tmp_rh->uri_len < urilen
            && uri[tmp_rh->uri_len] == '/'
            && memcmp(tmp_rh->uri, uri, tmp_rh->uri_len) == 0) {
            /* if there is a partical match this new entry MUST go BEFORE
               the current position otherwise it will never be matched. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            break;
        }

    }

    if (handler == NULL) {
<<<<<<< HEAD
        // no handler to set, this was a remove request
=======
        /* no handler to set, this was a remove request */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        return;
    }

    tmp_rh = (struct mg_request_handler_info *)malloc(sizeof(struct mg_request_handler_info));
    if (tmp_rh == NULL) {
        mg_cry(fc(ctx), "%s", "Cannot create new request handler struct, OOM");
        return;
    }
    tmp_rh->uri = mg_strdup(uri);
    tmp_rh->uri_len = strlen(uri);
    tmp_rh->handler = handler;
    tmp_rh->cbdata = cbdata;

    if (lastref == NULL) {
        tmp_rh->next = ctx->request_handlers;
        ctx->request_handlers = tmp_rh;
    } else {
        tmp_rh->next = lastref->next;
        lastref->next = tmp_rh;
    }

}

static int use_request_handler(struct mg_connection *conn)
{
    struct mg_request_info *request_info = mg_get_request_info(conn);
    const char *uri = request_info->uri;
    size_t urilen = strlen(uri);
    struct mg_request_handler_info *tmp_rh = conn->ctx->request_handlers;

    for (; tmp_rh != NULL; tmp_rh = tmp_rh->next) {

<<<<<<< HEAD
        // first try for an exact match
=======
        /* first try for an exact match */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (urilen == tmp_rh->uri_len && !strcmp(tmp_rh->uri,uri)) {
            return tmp_rh->handler(conn, tmp_rh->cbdata);
        }

<<<<<<< HEAD
        // next try for a partial match
        // we will accept uri/something
=======
        /* next try for a partial match */
        /* we will accept uri/something */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (tmp_rh->uri_len < urilen
            && uri[tmp_rh->uri_len] == '/'
            && memcmp(tmp_rh->uri, uri, tmp_rh->uri_len) == 0) {

            return tmp_rh->handler(conn, tmp_rh->cbdata);
        }

    }

<<<<<<< HEAD
    return 0; // none found
}

// This is the heart of the Civetweb's logic.
// This function is called when the request is read, parsed and validated,
// and Civetweb must decide what action to take: serve a file, or
// a directory, or call embedded function, etcetera.
=======
    return 0; /* none found */
}

/* This is the heart of the Civetweb's logic.
   This function is called when the request is read, parsed and validated,
   and Civetweb must decide what action to take: serve a file, or
   a directory, or call embedded function, etcetera. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void handle_request(struct mg_connection *conn)
{
    struct mg_request_info *ri = &conn->request_info;
    char path[PATH_MAX];
    int uri_len, ssl_index;
    struct file file = STRUCT_FILE_INITIALIZER;

    if ((conn->request_info.query_string = strchr(ri->uri, '?')) != NULL) {
        * ((char *) conn->request_info.query_string++) = '\0';
    }
    uri_len = (int) strlen(ri->uri);
    mg_url_decode(ri->uri, uri_len, (char *) ri->uri, uri_len + 1, 0);
    remove_double_dots_and_double_slashes((char *) ri->uri);
    path[0] = '\0';
    convert_uri_to_file_name(conn, path, sizeof(path), &file);
    conn->throttle = set_throttle(conn->ctx->config[THROTTLE],
                                  get_remote_ip(conn), ri->uri);

    DEBUG_TRACE(("%s", ri->uri));
<<<<<<< HEAD
    // Perform redirect and auth checks before calling begin_request() handler.
    // Otherwise, begin_request() would need to perform auth checks and redirects.
=======
    /* Perform redirect and auth checks before calling begin_request() handler.
       Otherwise, begin_request() would need to perform auth checks and
       redirects. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (!conn->client.is_ssl && conn->client.ssl_redir &&
        (ssl_index = get_first_ssl_listener_index(conn->ctx)) > -1) {
        redirect_to_https_port(conn, ssl_index);
    } else if (!is_put_or_delete_request(conn) &&
               !check_authorization(conn, path)) {
        send_authorization_request(conn);
    } else if (conn->ctx->callbacks.begin_request != NULL &&
               conn->ctx->callbacks.begin_request(conn)) {
<<<<<<< HEAD
        // Do nothing, callback has served the request
    } else if (conn->ctx->request_handlers != NULL &&
               use_request_handler(conn)) {
        // Do nothing, callback has served the request
=======
        /* Do nothing, callback has served the request */
    } else if (conn->ctx->request_handlers != NULL &&
               use_request_handler(conn)) {
        /* Do nothing, callback has served the request */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#if defined(USE_WEBSOCKET)
    } else if (is_websocket_request(conn)) {
        handle_websocket_request(conn);
#endif
    } else if (!strcmp(ri->request_method, "OPTIONS")) {
        send_options(conn);
    } else if (conn->ctx->config[DOCUMENT_ROOT] == NULL) {
        send_http_error(conn, 404, "Not Found", "Not Found");
    } else if (is_put_or_delete_request(conn) &&
               (is_authorized_for_put(conn) != 1)) {
        send_authorization_request(conn);
    } else if (!strcmp(ri->request_method, "PUT")) {
        put_file(conn, path);
    } else if (!strcmp(ri->request_method, "MKCOL")) {
        mkcol(conn, path);
    } else if (!strcmp(ri->request_method, "DELETE")) {
        struct de de;
        memset(&de.file, 0, sizeof(de.file));
        if(!mg_stat(conn, path, &de.file)) {
            send_http_error(conn, 404, "Not Found", "%s", "File not found");
        } else {
            if(de.file.modification_time) {
                if(de.file.is_directory) {
                    remove_directory(conn, path);
                    send_http_error(conn, 204, "No Content", "%s", "");
                } else if (mg_remove(path) == 0) {
                    send_http_error(conn, 204, "No Content", "%s", "");
                } else {
                    send_http_error(conn, 423, "Locked", "remove(%s): %s", path,
                                    strerror(ERRNO));
                }
            } else {
                send_http_error(conn, 500, http_500_error, "remove(%s): %s", path,
                                strerror(ERRNO));
            }
        }
    } else if ((file.membuf == NULL && file.modification_time == (time_t) 0) ||
               must_hide_file(conn, path)) {
        send_http_error(conn, 404, "Not Found", "%s", "File not found");
    } else if (file.is_directory && ri->uri[uri_len - 1] != '/') {
        mg_printf(conn, "HTTP/1.1 301 Moved Permanently\r\n"
                  "Location: %s/\r\n\r\n", ri->uri);
    } else if (!strcmp(ri->request_method, "PROPFIND")) {
        handle_propfind(conn, path, &file);
    } else if (file.is_directory &&
               !substitute_index_file(conn, path, sizeof(path), &file)) {
        if (!mg_strcasecmp(conn->ctx->config[ENABLE_DIRECTORY_LISTING], "yes")) {
            handle_directory_request(conn, path);
        } else {
            send_http_error(conn, 403, "Directory Listing Denied",
                            "Directory listing denied");
        }
#ifdef USE_LUA
    } else if (match_prefix("**.lp$", 6, path) > 0) {
        handle_lsp_request(conn, path, &file, NULL);
#endif
#if !defined(NO_CGI)
    } else if (match_prefix(conn->ctx->config[CGI_EXTENSIONS],
                            (int)strlen(conn->ctx->config[CGI_EXTENSIONS]),
                            path) > 0) {
        if (strcmp(ri->request_method, "POST") &&
            strcmp(ri->request_method, "HEAD") &&
            strcmp(ri->request_method, "GET")) {
            send_http_error(conn, 501, "Not Implemented",
                            "Method %s is not implemented", ri->request_method);
        } else {
            handle_cgi_request(conn, path);
        }
<<<<<<< HEAD
#endif // !NO_CGI
=======
#endif /* !NO_CGI */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    } else if (match_prefix(conn->ctx->config[SSI_EXTENSIONS],
                            (int)strlen(conn->ctx->config[SSI_EXTENSIONS]),
                            path) > 0) {
        handle_ssi_file_request(conn, path);
    } else if (is_not_modified(conn, &file)) {
        send_http_error(conn, 304, "Not Modified", "%s", "");
    } else {
        handle_file_request(conn, path, &file);
    }
}

static void close_all_listening_sockets(struct mg_context *ctx)
{
    int i;
    for (i = 0; i < ctx->num_listening_sockets; i++) {
        closesocket(ctx->listening_sockets[i].sock);
    }
    free(ctx->listening_sockets);
}

static int is_valid_port(unsigned int port)
{
    return port < 0xffff;
}

<<<<<<< HEAD
// Valid listening port specification is: [ip_address:]port[s]
// Examples: 80, 443s, 127.0.0.1:3128, 1.2.3.4:8080s
// TODO(lsm): add parsing of the IPv6 address
=======
/* Valid listening port specification is: [ip_address:]port[s]
   Examples: 80, 443s, 127.0.0.1:3128, 1.2.3.4:8080s
   TODO(lsm): add parsing of the IPv6 address */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int parse_port_string(const struct vec *vec, struct socket *so)
{
    unsigned int a, b, c, d, ch, len, port;
#if defined(USE_IPV6)
    char buf[100];
#endif

<<<<<<< HEAD
    // MacOS needs that. If we do not zero it, subsequent bind() will fail.
    // Also, all-zeroes in the socket address means binding to all addresses
    // for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT).
=======
    /* MacOS needs that. If we do not zero it, subsequent bind() will fail.
       Also, all-zeroes in the socket address means binding to all addresses
       for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT). */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    memset(so, 0, sizeof(*so));
    so->lsa.sin.sin_family = AF_INET;

    if (sscanf(vec->ptr, "%u.%u.%u.%u:%u%n", &a, &b, &c, &d, &port, &len) == 5) {
<<<<<<< HEAD
        // Bind to a specific IPv4 address, e.g. 192.168.1.5:8080
=======
        /* Bind to a specific IPv4 address, e.g. 192.168.1.5:8080 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        so->lsa.sin.sin_addr.s_addr = htonl((a << 24) | (b << 16) | (c << 8) | d);
        so->lsa.sin.sin_port = htons((uint16_t) port);
#if defined(USE_IPV6)

    } else if (sscanf(vec->ptr, "[%49[^]]]:%d%n", buf, &port, &len) == 2 &&
               inet_pton(AF_INET6, buf, &so->lsa.sin6.sin6_addr)) {
<<<<<<< HEAD
        // IPv6 address, e.g. [3ffe:2a00:100:7031::1]:8080
=======
        /* IPv6 address, e.g. [3ffe:2a00:100:7031::1]:8080 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        so->lsa.sin6.sin6_family = AF_INET6;
        so->lsa.sin6.sin6_port = htons((uint16_t) port);
#endif
    } else if (sscanf(vec->ptr, "%u%n", &port, &len) == 1) {
<<<<<<< HEAD
        // If only port is specified, bind to IPv4, INADDR_ANY
        so->lsa.sin.sin_port = htons((uint16_t) port);
    } else {
        port = len = 0;   // Parsing failure. Make port invalid.
    }

    ch = vec->ptr[len];  // Next character after the port number
    so->is_ssl = ch == 's';
    so->ssl_redir = ch == 'r';

    // Make sure the port is valid and vector ends with 's', 'r' or ','
=======
        /* If only port is specified, bind to IPv4, INADDR_ANY */
        so->lsa.sin.sin_port = htons((uint16_t) port);
    } else {
        port = len = 0;   /* Parsing failure. Make port invalid. */
    }

    ch = vec->ptr[len];  /* Next character after the port number */
    so->is_ssl = ch == 's';
    so->ssl_redir = ch == 'r';

    /* Make sure the port is valid and vector ends with 's', 'r' or ',' */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    return is_valid_port(port) &&
           (ch == '\0' || ch == 's' || ch == 'r' || ch == ',');
}

static int set_ports_option(struct mg_context *ctx)
{
    const char *list = ctx->config[LISTENING_PORTS];
    int on = 1, success = 1;
#if defined(USE_IPV6)
    int off = 0;
#endif
    struct vec vec;
    struct socket so, *ptr;

    while (success && (list = next_option(list, &vec, NULL)) != NULL) {
        if (!parse_port_string(&vec, &so)) {
            mg_cry(fc(ctx), "%s: %.*s: invalid port spec. Expecting list of: %s",
                   __func__, (int) vec.len, vec.ptr, "[IP_ADDRESS:]PORT[s|r]");
            success = 0;
        } else if (so.is_ssl && ctx->ssl_ctx == NULL) {
            mg_cry(fc(ctx), "Cannot add SSL socket, is -ssl_certificate option set?");
            success = 0;
        } else if ((so.sock = socket(so.lsa.sa.sa_family, SOCK_STREAM, 6)) ==
                   INVALID_SOCKET ||
<<<<<<< HEAD
                   // On Windows, SO_REUSEADDR is recommended only for
                   // broadcast UDP sockets
=======
                   /* On Windows, SO_REUSEADDR is recommended only for
                      broadcast UDP sockets */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                   setsockopt(so.sock, SOL_SOCKET, SO_REUSEADDR,
                              (void *) &on, sizeof(on)) != 0 ||
#if defined(USE_IPV6)
                   (so.lsa.sa.sa_family == AF_INET6 &&
                    setsockopt(so.sock, IPPROTO_IPV6, IPV6_V6ONLY, (void *) &off,
                               sizeof(off)) != 0) ||
#endif
                   bind(so.sock, &so.lsa.sa, so.lsa.sa.sa_family == AF_INET ?
                        sizeof(so.lsa.sin) : sizeof(so.lsa)) != 0 ||
                   listen(so.sock, SOMAXCONN) != 0) {
            mg_cry(fc(ctx), "%s: cannot bind to %.*s: %d (%s)", __func__,
                   (int) vec.len, vec.ptr, ERRNO, strerror(errno));
            if (so.sock != INVALID_SOCKET) {
                closesocket(so.sock);
            }
            success = 0;
        } else if ((ptr = (struct socket *) realloc(ctx->listening_sockets,
                          (ctx->num_listening_sockets + 1) *
                          sizeof(ctx->listening_sockets[0]))) == NULL) {
            closesocket(so.sock);
            success = 0;
        } else {
            set_close_on_exec(so.sock, fc(ctx));
            ctx->listening_sockets = ptr;
            ctx->listening_sockets[ctx->num_listening_sockets] = so;
            ctx->num_listening_sockets++;
        }
    }

    if (!success) {
        close_all_listening_sockets(ctx);
    }

    return success;
}

static void log_header(const struct mg_connection *conn, const char *header,
                       FILE *fp)
{
    const char *header_value;

    if ((header_value = mg_get_header(conn, header)) == NULL) {
        (void) fprintf(fp, "%s", " -");
    } else {
        (void) fprintf(fp, " \"%s\"", header_value);
    }
}

static void log_access(const struct mg_connection *conn)
{
    const struct mg_request_info *ri;
    FILE *fp;
    char date[64], src_addr[IP_ADDR_STR_LEN];
    struct tm *tm;

    fp = conn->ctx->config[ACCESS_LOG_FILE] == NULL ?  NULL :
         fopen(conn->ctx->config[ACCESS_LOG_FILE], "a+");

    if (fp == NULL)
        return;

    tm = localtime(&conn->birth_time);
    if (tm != NULL) {
        strftime(date, sizeof(date), "%d/%b/%Y:%H:%M:%S %z", tm);
    } else {
        strncpy(date, "01/Jan/1970:00:00:00 +0000", sizeof(date));
        date[sizeof(date) - 1] = '\0';
    }

    ri = &conn->request_info;
    flockfile(fp);

    sockaddr_to_string(src_addr, sizeof(src_addr), &conn->client.rsa);
    fprintf(fp, "%s - %s [%s] \"%s %s HTTP/%s\" %d %" INT64_FMT,
            src_addr, ri->remote_user == NULL ? "-" : ri->remote_user, date,
            ri->request_method ? ri->request_method : "-",
            ri->uri ? ri->uri : "-", ri->http_version,
            conn->status_code, conn->num_bytes_sent);
    log_header(conn, "Referer", fp);
    log_header(conn, "User-Agent", fp);
    fputc('\n', fp);
    fflush(fp);

    funlockfile(fp);
    fclose(fp);
}

<<<<<<< HEAD
// Verify given socket address against the ACL.
// Return -1 if ACL is malformed, 0 if address is disallowed, 1 if allowed.
=======
/* Verify given socket address against the ACL.
   Return -1 if ACL is malformed, 0 if address is disallowed, 1 if allowed. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int check_acl(struct mg_context *ctx, uint32_t remote_ip)
{
    int allowed, flag;
    uint32_t net, mask;
    struct vec vec;
    const char *list = ctx->config[ACCESS_CONTROL_LIST];

<<<<<<< HEAD
    // If any ACL is set, deny by default
=======
    /* If any ACL is set, deny by default */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    allowed = list == NULL ? '+' : '-';

    while ((list = next_option(list, &vec, NULL)) != NULL) {
        flag = vec.ptr[0];
        if ((flag != '+' && flag != '-') ||
            parse_net(&vec.ptr[1], &net, &mask) == 0) {
            mg_cry(fc(ctx), "%s: subnet must be [+|-]x.x.x.x[/x]", __func__);
            return -1;
        }

        if (net == (remote_ip & mask)) {
            allowed = flag;
        }
    }

    return allowed == '+';
}

#if !defined(_WIN32)
static int set_uid_option(struct mg_context *ctx)
{
    struct passwd *pw;
    const char *uid = ctx->config[RUN_AS_USER];
    int success = 0;

    if (uid == NULL) {
        success = 1;
    } else {
        if ((pw = getpwnam(uid)) == NULL) {
            mg_cry(fc(ctx), "%s: unknown user [%s]", __func__, uid);
        } else if (setgid(pw->pw_gid) == -1) {
            mg_cry(fc(ctx), "%s: setgid(%s): %s", __func__, uid, strerror(errno));
        } else if (setuid(pw->pw_uid) == -1) {
            mg_cry(fc(ctx), "%s: setuid(%s): %s", __func__, uid, strerror(errno));
        } else {
            success = 1;
        }
    }

    return success;
}
<<<<<<< HEAD
#endif // !_WIN32
=======
#endif /* !_WIN32 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

#if !defined(NO_SSL)
static pthread_mutex_t *ssl_mutexes;

static int sslize(struct mg_connection *conn, SSL_CTX *s, int (*func)(SSL *))
{
    return (conn->ssl = SSL_new(s)) != NULL &&
           SSL_set_fd(conn->ssl, conn->client.sock) == 1 &&
           func(conn->ssl) == 1;
}

<<<<<<< HEAD
// Return OpenSSL error message
=======
/* Return OpenSSL error message */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static const char *ssl_error(void)
{
    unsigned long err;
    err = ERR_get_error();
    return err == 0 ? "" : ERR_error_string(err, NULL);
}

static void ssl_locking_callback(int mode, int mutex_num, const char *file,
                                 int line)
{
    (void) line;
    (void) file;

<<<<<<< HEAD
    if (mode & 1) {  // 1 is CRYPTO_LOCK
=======
    if (mode & 1) {  /* 1 is CRYPTO_LOCK */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        (void) pthread_mutex_lock(&ssl_mutexes[mutex_num]);
    } else {
        (void) pthread_mutex_unlock(&ssl_mutexes[mutex_num]);
    }
}

static unsigned long ssl_id_callback(void)
{
    return (unsigned long) pthread_self();
}

#if !defined(NO_SSL_DL)
static void *load_dll(struct mg_context *ctx, const char *dll_name,
                      struct ssl_func *sw)
{
    union {
        void *p;
        void (*fp)(void);
    } u;
    void  *dll_handle;
    struct ssl_func *fp;

    if ((dll_handle = dlopen(dll_name, RTLD_LAZY)) == NULL) {
        mg_cry(fc(ctx), "%s: cannot load %s", __func__, dll_name);
        return NULL;
    }

    for (fp = sw; fp->name != NULL; fp++) {
#ifdef _WIN32
<<<<<<< HEAD
        // GetProcAddress() returns pointer to function
        u.fp = (void (*)(void)) dlsym(dll_handle, fp->name);
#else
        // dlsym() on UNIX returns void *. ISO C forbids casts of data pointers to
        // function pointers. We need to use a union to make a cast.
        u.p = dlsym(dll_handle, fp->name);
#endif // _WIN32
=======
        /* GetProcAddress() returns pointer to function */
        u.fp = (void (*)(void)) dlsym(dll_handle, fp->name);
#else
        /* dlsym() on UNIX returns void *. ISO C forbids casts of data
           pointers to function pointers. We need to use a union to make a
           cast. */
        u.p = dlsym(dll_handle, fp->name);
#endif /* _WIN32 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (u.fp == NULL) {
            mg_cry(fc(ctx), "%s: %s: cannot find %s", __func__, dll_name, fp->name);
            dlclose(dll_handle);
            return NULL;
        } else {
            fp->ptr = u.fp;
        }
    }

    return dll_handle;
}
<<<<<<< HEAD
#endif // NO_SSL_DL

// Dynamically load SSL library. Set up ctx->ssl_ctx pointer.
=======
#endif /* NO_SSL_DL */

/* Dynamically load SSL library. Set up ctx->ssl_ctx pointer. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int set_ssl_option(struct mg_context *ctx)
{
    int i, size;
    const char *pem;

<<<<<<< HEAD
    // If PEM file is not specified and the init_ssl callback
    // is not specified, skip SSL initialization.
=======
    /* If PEM file is not specified and the init_ssl callback
       is not specified, skip SSL initialization. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if ((pem = ctx->config[SSL_CERTIFICATE]) == NULL &&
        ctx->callbacks.init_ssl == NULL) {
        return 1;
    }

#if !defined(NO_SSL_DL)
    ctx->ssllib_dll_handle = load_dll(ctx, SSL_LIB, ssl_sw);
    ctx->cryptolib_dll_handle = load_dll(ctx, CRYPTO_LIB, crypto_sw);
    if (!ctx->ssllib_dll_handle || !ctx->cryptolib_dll_handle) {
        return 0;
    }
<<<<<<< HEAD
#endif // NO_SSL_DL

    // Initialize SSL library
=======
#endif /* NO_SSL_DL */

    /* Initialize SSL library */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    SSL_library_init();
    SSL_load_error_strings();

    if ((ctx->ssl_ctx = SSL_CTX_new(SSLv23_server_method())) == NULL) {
        mg_cry(fc(ctx), "SSL_CTX_new (server) error: %s", ssl_error());
        return 0;
    }

<<<<<<< HEAD
    // If user callback returned non-NULL, that means that user callback has
    // set up certificate itself. In this case, skip sertificate setting.
=======
    /* If user callback returned non-NULL, that means that user callback has
       set up certificate itself. In this case, skip sertificate setting. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if ((ctx->callbacks.init_ssl == NULL ||
         !ctx->callbacks.init_ssl(ctx->ssl_ctx, ctx->user_data)) &&
        (SSL_CTX_use_certificate_file(ctx->ssl_ctx, pem, 1) == 0 ||
         SSL_CTX_use_PrivateKey_file(ctx->ssl_ctx, pem, 1) == 0)) {
        mg_cry(fc(ctx), "%s: cannot open %s: %s", __func__, pem, ssl_error());
        return 0;
    }

    if (pem != NULL) {
        (void) SSL_CTX_use_certificate_chain_file(ctx->ssl_ctx, pem);
    }

<<<<<<< HEAD
    // Initialize locking callbacks, needed for thread safety.
    // http://www.openssl.org/support/faq.html#PROG1
=======
    /* Initialize locking callbacks, needed for thread safety.
       http://www.openssl.org/support/faq.html#PROG1 */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    size = sizeof(pthread_mutex_t) * CRYPTO_num_locks();
    if ((ssl_mutexes = (pthread_mutex_t *) malloc((size_t)size)) == NULL) {
        mg_cry(fc(ctx), "%s: cannot allocate mutexes: %s", __func__, ssl_error());
        return 0;
    }

    for (i = 0; i < CRYPTO_num_locks(); i++) {
        pthread_mutex_init(&ssl_mutexes[i], NULL);
    }

    CRYPTO_set_locking_callback(&ssl_locking_callback);
    CRYPTO_set_id_callback(&ssl_id_callback);

    return 1;
}

static void uninitialize_ssl(struct mg_context *ctx)
{
    int i;
    if (ctx->ssl_ctx != NULL) {
        CRYPTO_set_locking_callback(NULL);
        for (i = 0; i < CRYPTO_num_locks(); i++) {
            pthread_mutex_destroy(&ssl_mutexes[i]);
        }
        CRYPTO_set_locking_callback(NULL);
        CRYPTO_set_id_callback(NULL);
    }
}
<<<<<<< HEAD
#endif // !NO_SSL
=======
#endif /* !NO_SSL */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

static int set_gpass_option(struct mg_context *ctx)
{
    struct file file = STRUCT_FILE_INITIALIZER;
    const char *path = ctx->config[GLOBAL_PASSWORDS_FILE];
    if (path != NULL && !mg_stat(fc(ctx), path, &file)) {
        mg_cry(fc(ctx), "Cannot open %s: %s", path, strerror(ERRNO));
        return 0;
    }
    return 1;
}

static int set_acl_option(struct mg_context *ctx)
{
    return check_acl(ctx, (uint32_t) 0x7f000001UL) != -1;
}

static void reset_per_request_attributes(struct mg_connection *conn)
{
    conn->path_info = NULL;
    conn->num_bytes_sent = conn->consumed_content = 0;
    conn->status_code = -1;
    conn->must_close = conn->request_len = conn->throttle = 0;
}

static void close_socket_gracefully(struct mg_connection *conn)
{
#if defined(_WIN32)
    char buf[MG_BUF_LEN];
    int n;
#endif
    struct linger linger;

<<<<<<< HEAD
    // Set linger option to avoid socket hanging out after close. This prevent
    // ephemeral port exhaust problem under high QPS.
=======
    /* Set linger option to avoid socket hanging out after close. This prevent
       ephemeral port exhaust problem under high QPS. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    linger.l_onoff = 1;
    linger.l_linger = 1;
    if (setsockopt(conn->client.sock, SOL_SOCKET, SO_LINGER,
                   (char *) &linger, sizeof(linger)) != 0) {
        mg_cry(conn, "%s: setsockopt(SOL_SOCKET SO_LINGER) failed: %s",
               __func__, strerror(ERRNO));
    }

<<<<<<< HEAD
    // Send FIN to the client
=======
    /* Send FIN to the client */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    shutdown(conn->client.sock, SHUT_WR);
    set_non_blocking_mode(conn->client.sock);

#if defined(_WIN32)
<<<<<<< HEAD
    // Read and discard pending incoming data. If we do not do that and close the
    // socket, the data in the send buffer may be discarded. This
    // behaviour is seen on Windows, when client keeps sending data
    // when server decides to close the connection; then when client
    // does recv() it gets no data back.
=======
    /* Read and discard pending incoming data. If we do not do that and close
       the socket, the data in the send buffer may be discarded. This
       behaviour is seen on Windows, when client keeps sending data
       when server decides to close the connection; then when client
       does recv() it gets no data back. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    do {
        n = pull(NULL, conn, buf, sizeof(buf));
    } while (n > 0);
#endif

<<<<<<< HEAD
    // Now we know that our FIN is ACK-ed, safe to close
=======
    /* Now we know that our FIN is ACK-ed, safe to close */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    closesocket(conn->client.sock);
}

static void close_connection(struct mg_connection *conn)
{
<<<<<<< HEAD
    // call the connection_close callback if assigned
=======
    /* call the connection_close callback if assigned */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (conn->ctx->callbacks.connection_close != NULL)
        conn->ctx->callbacks.connection_close(conn);

    mg_lock(conn);

    conn->must_close = 1;

#ifndef NO_SSL
    if (conn->ssl != NULL) {
<<<<<<< HEAD
        // Run SSL_shutdown twice to ensure completly close SSL connection
=======
        /* Run SSL_shutdown twice to ensure completly close SSL connection */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        SSL_shutdown(conn->ssl);
        SSL_free(conn->ssl);
        conn->ssl = NULL;
    }
#endif
    if (conn->client.sock != INVALID_SOCKET) {
        close_socket_gracefully(conn);
        conn->client.sock = INVALID_SOCKET;
    }

    mg_unlock(conn);
}

void mg_close_connection(struct mg_connection *conn)
{
#ifndef NO_SSL
    if (conn->client_ssl_ctx != NULL) {
        SSL_CTX_free((SSL_CTX *) conn->client_ssl_ctx);
    }
#endif
    close_connection(conn);
    (void) pthread_mutex_destroy(&conn->mutex);
    free(conn);
}

struct mg_connection *mg_connect(const char *host, int port, int use_ssl,
                                 char *ebuf, size_t ebuf_len);

struct mg_connection *mg_connect(const char *host, int port, int use_ssl,
                                 char *ebuf, size_t ebuf_len)
{
    static struct mg_context fake_ctx;
    struct mg_connection *conn = NULL;
    SOCKET sock;

    if ((sock = conn2(&fake_ctx, host, port, use_ssl, ebuf,
                      ebuf_len)) == INVALID_SOCKET) {
    } else if ((conn = (struct mg_connection *)
                       calloc(1, sizeof(*conn) + MAX_REQUEST_SIZE)) == NULL) {
        snprintf(ebuf, ebuf_len, "calloc(): %s", strerror(ERRNO));
        closesocket(sock);
#ifndef NO_SSL
    } else if (use_ssl && (conn->client_ssl_ctx =
                               SSL_CTX_new(SSLv23_client_method())) == NULL) {
        snprintf(ebuf, ebuf_len, "SSL_CTX_new error");
        closesocket(sock);
        free(conn);
        conn = NULL;
<<<<<<< HEAD
#endif // NO_SSL
=======
#endif /* NO_SSL */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    } else {
        socklen_t len = sizeof(struct sockaddr);
        conn->buf_size = MAX_REQUEST_SIZE;
        conn->buf = (char *) (conn + 1);
        conn->ctx = &fake_ctx;
        conn->client.sock = sock;
        if (getsockname(sock, &conn->client.rsa.sa, &len) != 0) {
            mg_cry(conn, "%s: getsockname() failed: %s",
                   __func__, strerror(ERRNO));
        }
        conn->client.is_ssl = use_ssl;
        (void) pthread_mutex_init(&conn->mutex, NULL);
#ifndef NO_SSL
        if (use_ssl) {
<<<<<<< HEAD
            // SSL_CTX_set_verify call is needed to switch off server certificate
            // checking, which is off by default in OpenSSL and on in yaSSL.
=======
            /* SSL_CTX_set_verify call is needed to switch off server
               certificate checking, which is off by default in OpenSSL and on
               in yaSSL. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            SSL_CTX_set_verify(conn->client_ssl_ctx, 0, 0);
            sslize(conn, conn->client_ssl_ctx, SSL_connect);
        }
#endif
    }

    return conn;
}

static int is_valid_uri(const char *uri)
{
<<<<<<< HEAD
    // Conform to http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.1.2
    // URI can be an asterisk (*) or should start with slash.
=======
    /* Conform to
       http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.1.2
       URI can be an asterisk (*) or should start with slash. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    return uri[0] == '/' || (uri[0] == '*' && uri[1] == '\0');
}

static int getreq(struct mg_connection *conn, char *ebuf, size_t ebuf_len)
{
    const char *cl;

    ebuf[0] = '\0';
    reset_per_request_attributes(conn);
    conn->request_len = read_request(NULL, conn, conn->buf, conn->buf_size,
                                     &conn->data_len);
    assert(conn->request_len < 0 || conn->data_len >= conn->request_len);

    if (conn->request_len == 0 && conn->data_len == conn->buf_size) {
        snprintf(ebuf, ebuf_len, "%s", "Request Too Large");
    } else if (conn->request_len <= 0) {
        snprintf(ebuf, ebuf_len, "%s", "Client closed connection");
    } else if (parse_http_message(conn->buf, conn->buf_size,
                                  &conn->request_info) <= 0) {
        snprintf(ebuf, ebuf_len, "Bad request: [%.*s]", conn->data_len, conn->buf);
    } else {
<<<<<<< HEAD
        // Request is valid
=======
        /* Request is valid */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if ((cl = get_header(&conn->request_info, "Content-Length")) != NULL) {
            conn->content_len = strtoll(cl, NULL, 10);
        } else if (!mg_strcasecmp(conn->request_info.request_method, "POST") ||
                   !mg_strcasecmp(conn->request_info.request_method, "PUT")) {
            conn->content_len = -1;
        } else {
            conn->content_len = 0;
        }
        conn->birth_time = time(NULL);
    }
    return ebuf[0] == '\0';
}

struct mg_connection *mg_download(const char *host, int port, int use_ssl,
                                  char *ebuf, size_t ebuf_len,
                                  const char *fmt, ...)
{
    struct mg_connection *conn;
    va_list ap;

    va_start(ap, fmt);
    ebuf[0] = '\0';
    if ((conn = mg_connect(host, port, use_ssl, ebuf, ebuf_len)) == NULL) {
    } else if (mg_vprintf(conn, fmt, ap) <= 0) {
        snprintf(ebuf, ebuf_len, "%s", "Error sending request");
    } else {
        getreq(conn, ebuf, ebuf_len);
    }
    if (ebuf[0] != '\0' && conn != NULL) {
        mg_close_connection(conn);
        conn = NULL;
    }
    va_end(ap);

    return conn;
}

static void process_new_connection(struct mg_connection *conn)
{
    struct mg_request_info *ri = &conn->request_info;
    int keep_alive_enabled, keep_alive, discard_len;
    char ebuf[100];

    keep_alive_enabled = !strcmp(conn->ctx->config[ENABLE_KEEP_ALIVE], "yes");
    keep_alive = 0;

<<<<<<< HEAD
    // Important: on new connection, reset the receiving buffer. Credit goes
    // to crule42.
=======
    /* Important: on new connection, reset the receiving buffer. Credit goes
       to crule42. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    conn->data_len = 0;
    do {
        if (!getreq(conn, ebuf, sizeof(ebuf))) {
            send_http_error(conn, 500, "Server Error", "%s", ebuf);
            conn->must_close = 1;
        } else if (!is_valid_uri(conn->request_info.uri)) {
            snprintf(ebuf, sizeof(ebuf), "Invalid URI: [%s]", ri->uri);
            send_http_error(conn, 400, "Bad Request", "%s", ebuf);
        } else if (strcmp(ri->http_version, "1.0") &&
                   strcmp(ri->http_version, "1.1")) {
            snprintf(ebuf, sizeof(ebuf), "Bad HTTP version: [%s]", ri->http_version);
            send_http_error(conn, 505, "Bad HTTP version", "%s", ebuf);
        }

        if (ebuf[0] == '\0') {
            handle_request(conn);
            if (conn->ctx->callbacks.end_request != NULL) {
                conn->ctx->callbacks.end_request(conn, conn->status_code);
            }
            log_access(conn);
        }
        if (ri->remote_user != NULL) {
            free((void *) ri->remote_user);
<<<<<<< HEAD
            // Important! When having connections with and without auth
            // would cause double free and then crash
            ri->remote_user = NULL;
        }

        // NOTE(lsm): order is important here. should_keep_alive() call
        // is using parsed request, which will be invalid after memmove's below.
        // Therefore, memorize should_keep_alive() result now for later use
        // in loop exit condition.
        keep_alive = conn->ctx->stop_flag == 0 && keep_alive_enabled &&
                     conn->content_len >= 0 && should_keep_alive(conn);

        // Discard all buffered data for this request
=======
            /* Important! When having connections with and without auth
               would cause double free and then crash */
            ri->remote_user = NULL;
        }

        /* NOTE(lsm): order is important here. should_keep_alive() call is
           using parsed request, which will be invalid after memmove's below.
           Therefore, memorize should_keep_alive() result now for later use
           in loop exit condition. */
        keep_alive = conn->ctx->stop_flag == 0 && keep_alive_enabled &&
                     conn->content_len >= 0 && should_keep_alive(conn);

        /* Discard all buffered data for this request */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        discard_len = conn->content_len >= 0 && conn->request_len > 0 &&
                      conn->request_len + conn->content_len < (int64_t) conn->data_len ?
                      (int) (conn->request_len + conn->content_len) : conn->data_len;
        assert(discard_len >= 0);
        memmove(conn->buf, conn->buf + discard_len, conn->data_len - discard_len);
        conn->data_len -= discard_len;
        assert(conn->data_len >= 0);
        assert(conn->data_len <= conn->buf_size);
    } while (keep_alive);
}

<<<<<<< HEAD
// Worker threads take accepted socket from the queue
=======
/* Worker threads take accepted socket from the queue */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static int consume_socket(struct mg_context *ctx, struct socket *sp)
{
    (void) pthread_mutex_lock(&ctx->mutex);
    DEBUG_TRACE(("going idle"));

<<<<<<< HEAD
    // If the queue is empty, wait. We're idle at this point.
=======
    /* If the queue is empty, wait. We're idle at this point. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    while (ctx->sq_head == ctx->sq_tail && ctx->stop_flag == 0) {
        pthread_cond_wait(&ctx->sq_full, &ctx->mutex);
    }

<<<<<<< HEAD
    // If we're stopping, sq_head may be equal to sq_tail.
    if (ctx->sq_head > ctx->sq_tail) {
        // Copy socket from the queue and increment tail
=======
    /* If we're stopping, sq_head may be equal to sq_tail. */
    if (ctx->sq_head > ctx->sq_tail) {
        /* Copy socket from the queue and increment tail */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        *sp = ctx->queue[ctx->sq_tail % ARRAY_SIZE(ctx->queue)];
        ctx->sq_tail++;
        DEBUG_TRACE(("grabbed socket %d, going busy", sp->sock));

<<<<<<< HEAD
        // Wrap pointers if needed
=======
        /* Wrap pointers if needed */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        while (ctx->sq_tail > (int) ARRAY_SIZE(ctx->queue)) {
            ctx->sq_tail -= ARRAY_SIZE(ctx->queue);
            ctx->sq_head -= ARRAY_SIZE(ctx->queue);
        }
    }

    (void) pthread_cond_signal(&ctx->sq_empty);
    (void) pthread_mutex_unlock(&ctx->mutex);

    return !ctx->stop_flag;
}

static void *worker_thread_run(void *thread_func_param)
{
    struct mg_context *ctx = (struct mg_context *) thread_func_param;
    struct mg_connection *conn;

    conn = (struct mg_connection *) calloc(1, sizeof(*conn) + MAX_REQUEST_SIZE);
    if (conn == NULL) {
        mg_cry(fc(ctx), "%s", "Cannot create new connection struct, OOM");
    } else {
        conn->buf_size = MAX_REQUEST_SIZE;
        conn->buf = (char *) (conn + 1);
        conn->ctx = ctx;
        conn->request_info.user_data = ctx->user_data;
<<<<<<< HEAD
        // Allocate a mutex for this connection to allow communication both
        // within the request handler and from elsewhere in the application
        (void) pthread_mutex_init(&conn->mutex, NULL);

        // Call consume_socket() even when ctx->stop_flag > 0, to let it signal
        // sq_empty condvar to wake up the master waiting in produce_socket()
        while (consume_socket(ctx, &conn->client)) {
            conn->birth_time = time(NULL);

            // Fill in IP, port info early so even if SSL setup below fails,
            // error handler would have the corresponding info.
            // Thanks to Johannes Winkelmann for the patch.
            // TODO(lsm): Fix IPv6 case
=======
        /* Allocate a mutex for this connection to allow communication both
           within the request handler and from elsewhere in the application */
        (void) pthread_mutex_init(&conn->mutex, NULL);

        /* Call consume_socket() even when ctx->stop_flag > 0, to let it
           signal sq_empty condvar to wake up the master waiting in
           produce_socket() */
        while (consume_socket(ctx, &conn->client)) {
            conn->birth_time = time(NULL);

            /* Fill in IP, port info early so even if SSL setup below fails,
               error handler would have the corresponding info.
               Thanks to Johannes Winkelmann for the patch.
               TODO(lsm): Fix IPv6 case */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
            conn->request_info.remote_port = ntohs(conn->client.rsa.sin.sin_port);
            memcpy(&conn->request_info.remote_ip,
                   &conn->client.rsa.sin.sin_addr.s_addr, 4);
            conn->request_info.remote_ip = ntohl(conn->request_info.remote_ip);
            conn->request_info.is_ssl = conn->client.is_ssl;

            if (!conn->client.is_ssl
#ifndef NO_SSL
                || sslize(conn, conn->ctx->ssl_ctx, SSL_accept)
#endif
               ) {
                process_new_connection(conn);
            }

            close_connection(conn);
        }
        free(conn);
    }

<<<<<<< HEAD
    // Signal master that we're done with connection and exiting
=======
    /* Signal master that we're done with connection and exiting */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    (void) pthread_mutex_lock(&ctx->mutex);
    ctx->num_threads--;
    (void) pthread_cond_signal(&ctx->cond);
    assert(ctx->num_threads >= 0);
    (void) pthread_mutex_unlock(&ctx->mutex);

    DEBUG_TRACE(("exiting"));
    return NULL;
}

/* Threads have different return types on Windows and Unix. */

#ifdef _WIN32
static unsigned __stdcall worker_thread(void *thread_func_param)
{
    worker_thread_run(thread_func_param);
    return 0;
}
#else
static void *worker_thread(void *thread_func_param)
{
    worker_thread_run(thread_func_param);
    return NULL;
}
#endif /* _WIN32 */

<<<<<<< HEAD
// Master thread adds accepted socket to a queue
=======
/* Master thread adds accepted socket to a queue */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
static void produce_socket(struct mg_context *ctx, const struct socket *sp)
{
    (void) pthread_mutex_lock(&ctx->mutex);

<<<<<<< HEAD
    // If the queue is full, wait
=======
    /* If the queue is full, wait */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    while (ctx->stop_flag == 0 &&
           ctx->sq_head - ctx->sq_tail >= (int) ARRAY_SIZE(ctx->queue)) {
        (void) pthread_cond_wait(&ctx->sq_empty, &ctx->mutex);
    }

    if (ctx->sq_head - ctx->sq_tail < (int) ARRAY_SIZE(ctx->queue)) {
<<<<<<< HEAD
        // Copy socket to the queue and increment head
=======
        /* Copy socket to the queue and increment head */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        ctx->queue[ctx->sq_head % ARRAY_SIZE(ctx->queue)] = *sp;
        ctx->sq_head++;
        DEBUG_TRACE(("queued socket %d", sp->sock));
    }

    (void) pthread_cond_signal(&ctx->sq_full);
    (void) pthread_mutex_unlock(&ctx->mutex);
}

static int set_sock_timeout(SOCKET sock, int milliseconds)
{
#ifdef _WIN32
    DWORD t = milliseconds;
#else
    struct timeval t;
    t.tv_sec = milliseconds / 1000;
    t.tv_usec = (milliseconds * 1000) % 1000000;
#endif
    return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (void *) &t, sizeof(t)) ||
           setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (void *) &t, sizeof(t));
}

static void accept_new_connection(const struct socket *listener,
                                  struct mg_context *ctx)
{
    struct socket so;
    char src_addr[IP_ADDR_STR_LEN];
    socklen_t len = sizeof(so.rsa);
    int on = 1;

    if ((so.sock = accept(listener->sock, &so.rsa.sa, &len)) == INVALID_SOCKET) {
    } else if (!check_acl(ctx, ntohl(* (uint32_t *) &so.rsa.sin.sin_addr))) {
        sockaddr_to_string(src_addr, sizeof(src_addr), &so.rsa);
        mg_cry(fc(ctx), "%s: %s is not allowed to connect", __func__, src_addr);
        closesocket(so.sock);
    } else {
<<<<<<< HEAD
        // Put so socket structure into the queue
=======
        /* Put so socket structure into the queue */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        DEBUG_TRACE(("Accepted socket %d", (int) so.sock));
        set_close_on_exec(so.sock, fc(ctx));
        so.is_ssl = listener->is_ssl;
        so.ssl_redir = listener->ssl_redir;
        if (getsockname(so.sock, &so.lsa.sa, &len) != 0) {
            mg_cry(fc(ctx), "%s: getsockname() failed: %s",
                   __func__, strerror(ERRNO));
        }
<<<<<<< HEAD
        // Set TCP keep-alive. This is needed because if HTTP-level keep-alive
        // is enabled, and client resets the connection, server won't get
        // TCP FIN or RST and will keep the connection open forever. With TCP
        // keep-alive, next keep-alive handshake will figure out that the client
        // is down and will close the server end.
        // Thanks to Igor Klopov who suggested the patch.
=======
        /* Set TCP keep-alive. This is needed because if HTTP-level keep-alive
           is enabled, and client resets the connection, server won't get
           TCP FIN or RST and will keep the connection open forever. With TCP
           keep-alive, next keep-alive handshake will figure out that the
           client is down and will close the server end.
           Thanks to Igor Klopov who suggested the patch. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
        if (setsockopt(so.sock, SOL_SOCKET, SO_KEEPALIVE, (void *) &on,
                       sizeof(on)) != 0) {
            mg_cry(fc(ctx),
                   "%s: setsockopt(SOL_SOCKET SO_KEEPALIVE) failed: %s",
                   __func__, strerror(ERRNO));
        }
        set_sock_timeout(so.sock, atoi(ctx->config[REQUEST_TIMEOUT]));
        produce_socket(ctx, &so);
    }
}

static void master_thread_run(void *thread_func_param)
{
    struct mg_context *ctx = (struct mg_context *) thread_func_param;
    struct pollfd *pfd;
    int i;
    int workerthreadcount;

<<<<<<< HEAD
    // Increase priority of the master thread
#if defined(_WIN32)
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
#endif

#if defined(ISSUE_317)
    struct sched_param sched_param;
    sched_param.sched_priority = sched_get_priority_max(SCHED_RR);
    pthread_setschedparam(pthread_self(), SCHED_RR, &sched_param);
=======
    /* Increase priority of the master thread */
#if defined(_WIN32)
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
#elif defined(USE_MASTER_THREAD_PRIORITY)
    int min_prio = sched_get_priority_min(SCHED_RR);
    int max_prio = sched_get_priority_max(SCHED_RR);
    if ((min_prio >=0) && (max_prio >= 0) &&
        ((USE_MASTER_THREAD_PRIORITY) <= max_prio) &&
        ((USE_MASTER_THREAD_PRIORITY) >= min_prio)
       ) {
        struct sched_param sched_param = {0};
        sched_param.sched_priority = (USE_MASTER_THREAD_PRIORITY);
        pthread_setschedparam(pthread_self(), SCHED_RR, &sched_param);
    }
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
#endif

    pfd = (struct pollfd *) calloc(ctx->num_listening_sockets, sizeof(pfd[0]));
    while (pfd != NULL && ctx->stop_flag == 0) {
        for (i = 0; i < ctx->num_listening_sockets; i++) {
            pfd[i].fd = ctx->listening_sockets[i].sock;
            pfd[i].events = POLLIN;
        }

        if (poll(pfd, ctx->num_listening_sockets, 200) > 0) {
            for (i = 0; i < ctx->num_listening_sockets; i++) {
<<<<<<< HEAD
                // NOTE(lsm): on QNX, poll() returns POLLRDNORM after the
                // successfull poll, and POLLIN is defined as (POLLRDNORM | POLLRDBAND)
                // Therefore, we're checking pfd[i].revents & POLLIN, not
                // pfd[i].revents == POLLIN.
=======
                /* NOTE(lsm): on QNX, poll() returns POLLRDNORM after the
                   successful poll, and POLLIN is defined as
                   (POLLRDNORM | POLLRDBAND)
                   Therefore, we're checking pfd[i].revents & POLLIN, not
                   pfd[i].revents == POLLIN. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
                if (ctx->stop_flag == 0 && (pfd[i].revents & POLLIN)) {
                    accept_new_connection(&ctx->listening_sockets[i], ctx);
                }
            }
        }
    }
    free(pfd);
    DEBUG_TRACE(("stopping workers"));

<<<<<<< HEAD
    // Stop signal received: somebody called mg_stop. Quit.
    close_all_listening_sockets(ctx);

    // Wakeup workers that are waiting for connections to handle.
    pthread_cond_broadcast(&ctx->sq_full);

    // Wait until all threads finish
=======
    /* Stop signal received: somebody called mg_stop. Quit. */
    close_all_listening_sockets(ctx);

    /* Wakeup workers that are waiting for connections to handle. */
    pthread_cond_broadcast(&ctx->sq_full);

    /* Wait until all threads finish */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    (void) pthread_mutex_lock(&ctx->mutex);
    while (ctx->num_threads > 0) {
        (void) pthread_cond_wait(&ctx->cond, &ctx->mutex);
    }
    (void) pthread_mutex_unlock(&ctx->mutex);

<<<<<<< HEAD
    // Join all worker threads to avoid leaking threads.
=======
    /* Join all worker threads to avoid leaking threads. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    workerthreadcount = ctx->workerthreadcount;
    for (i = 0; i < workerthreadcount; i++) {
        mg_join_thread(ctx->workerthreadids[i]);
    }

<<<<<<< HEAD
    // All threads exited, no sync is needed. Destroy mutex and condvars
=======
    /* All threads exited, no sync is needed. Destroy mutex and condvars */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    (void) pthread_mutex_destroy(&ctx->mutex);
    (void) pthread_cond_destroy(&ctx->cond);
    (void) pthread_cond_destroy(&ctx->sq_empty);
    (void) pthread_cond_destroy(&ctx->sq_full);

#if !defined(NO_SSL)
    uninitialize_ssl(ctx);
#endif
    DEBUG_TRACE(("exiting"));

<<<<<<< HEAD
    // Signal mg_stop() that we're done.
    // WARNING: This must be the very last thing this
    // thread does, as ctx becomes invalid after this line.
=======
    /* Signal mg_stop() that we're done.
       WARNING: This must be the very last thing this
       thread does, as ctx becomes invalid after this line. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    ctx->stop_flag = 2;
}

/* Threads have different return types on Windows and Unix. */

#ifdef _WIN32
static unsigned __stdcall master_thread(void *thread_func_param)
{
    master_thread_run(thread_func_param);
    return 0;
}
#else
static void *master_thread(void *thread_func_param)
{
    master_thread_run(thread_func_param);
    return NULL;
}
#endif /* _WIN32 */

static void free_context(struct mg_context *ctx)
{
    int i;
    struct mg_request_handler_info *tmp_rh;

    if (ctx == NULL)
        return;

<<<<<<< HEAD
    // Deallocate config parameters
=======
    /* Deallocate config parameters */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    for (i = 0; i < NUM_OPTIONS; i++) {
        if (ctx->config[i] != NULL)
#ifdef WIN32
#pragma warning(suppress: 6001)
#endif
            free(ctx->config[i]);
    }

<<<<<<< HEAD
    // Deallocate request handlers
=======
    /* Deallocate request handlers */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    while (ctx->request_handlers) {
        tmp_rh = ctx->request_handlers;
        ctx->request_handlers = tmp_rh->next;
        free(tmp_rh->uri);
        free(tmp_rh);
    }

#ifndef NO_SSL
<<<<<<< HEAD
    // Deallocate SSL context
=======
    /* Deallocate SSL context */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (ctx->ssl_ctx != NULL) {
        SSL_CTX_free(ctx->ssl_ctx);
    }
    if (ssl_mutexes != NULL) {
        free(ssl_mutexes);
        ssl_mutexes = NULL;
    }
<<<<<<< HEAD
#endif // !NO_SSL

    // Deallocate worker thread ID array
=======
#endif /* !NO_SSL */

    /* Deallocate worker thread ID array */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (ctx->workerthreadids != NULL) {
        free(ctx->workerthreadids);
    }

<<<<<<< HEAD
    // Deallocate context itself
=======
    /* Deallocate context itself */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    free(ctx);
}

void mg_stop(struct mg_context *ctx)
{
    ctx->stop_flag = 1;

<<<<<<< HEAD
    // Wait until mg_fini() stops
=======
    /* Wait until mg_fini() stops */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    while (ctx->stop_flag != 2) {
        (void) mg_sleep(10);
    }
    mg_join_thread(ctx->masterthreadid);
    free_context(ctx);

#if defined(_WIN32) && !defined(__SYMBIAN32__)
    (void) WSACleanup();
<<<<<<< HEAD
#endif // _WIN32
=======
#endif /* _WIN32 && !__SYMBIAN32__ */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
}

struct mg_context *mg_start(const struct mg_callbacks *callbacks,
                            void *user_data,
                            const char **options)
{
    struct mg_context *ctx;
    const char *name, *value, *default_value;
    int i;
    int workerthreadcount;

#if defined(_WIN32) && !defined(__SYMBIAN32__)
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);
#pragma warning(suppress: 28125)
    InitializeCriticalSection(&global_log_file_lock);
<<<<<<< HEAD
#endif // _WIN32

    // Allocate context and initialize reasonable general case defaults.
    // TODO(lsm): do proper error handling here.
=======
#endif /* _WIN32 && !__SYMBIAN32__ */

    /* Allocate context and initialize reasonable general case defaults.
       TODO(lsm): do proper error handling here. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if ((ctx = (struct mg_context *) calloc(1, sizeof(*ctx))) == NULL) {
        return NULL;
    }
    ctx->callbacks = *callbacks;
    ctx->user_data = user_data;
    ctx->request_handlers = 0;

    while (options && (name = *options++) != NULL) {
        if ((i = get_option_index(name)) == -1) {
            mg_cry(fc(ctx), "Invalid option: %s", name);
            free_context(ctx);
            return NULL;
        } else if ((value = *options++) == NULL) {
            mg_cry(fc(ctx), "%s: option value cannot be NULL", name);
            free_context(ctx);
            return NULL;
        }
        if (ctx->config[i] != NULL) {
            mg_cry(fc(ctx), "warning: %s: duplicate option", name);
            free(ctx->config[i]);
        }
        ctx->config[i] = mg_strdup(value);
        DEBUG_TRACE(("[%s] -> [%s]", name, value));
    }

<<<<<<< HEAD
    // Set default value if needed
=======
    /* Set default value if needed */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    for (i = 0; config_options[i * 2] != NULL; i++) {
        default_value = config_options[i * 2 + 1];
        if (ctx->config[i] == NULL && default_value != NULL) {
            ctx->config[i] = mg_strdup(default_value);
        }
    }

<<<<<<< HEAD
    // NOTE(lsm): order is important here. SSL certificates must
    // be initialized before listening ports. UID must be set last.
=======
    /* NOTE(lsm): order is important here. SSL certificates must
       be initialized before listening ports. UID must be set last. */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    if (!set_gpass_option(ctx) ||
#if !defined(NO_SSL)
        !set_ssl_option(ctx) ||
#endif
        !set_ports_option(ctx) ||
#if !defined(_WIN32)
        !set_uid_option(ctx) ||
#endif
        !set_acl_option(ctx)) {
        free_context(ctx);
        return NULL;
    }

#if !defined(_WIN32) && !defined(__SYMBIAN32__)
<<<<<<< HEAD
    // Ignore SIGPIPE signal, so if browser cancels the request, it
    // won't kill the whole process.
    (void) signal(SIGPIPE, SIG_IGN);
#endif // !_WIN32
=======
    /* Ignore SIGPIPE signal, so if browser cancels the request, it
       won't kill the whole process. */
    (void) signal(SIGPIPE, SIG_IGN);
#endif /* !_WIN32 && !__SYMBIAN32__ */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3

    (void) pthread_mutex_init(&ctx->mutex, NULL);
    (void) pthread_cond_init(&ctx->cond, NULL);
    (void) pthread_cond_init(&ctx->sq_empty, NULL);
    (void) pthread_cond_init(&ctx->sq_full, NULL);

    workerthreadcount = atoi(ctx->config[NUM_THREADS]);
    if (workerthreadcount > 0) {
        ctx->workerthreadcount = workerthreadcount;
        ctx->workerthreadids = calloc(workerthreadcount, sizeof(pthread_t));
        if (ctx->workerthreadids == NULL) {
            mg_cry(fc(ctx), "Not enough memory for worker thread ID array");
            free_context(ctx);
            return NULL;
        }
    }

<<<<<<< HEAD
    // Start master (listening) thread
    mg_start_thread_with_id(master_thread, ctx, &ctx->masterthreadid);

    // Start worker threads
=======
    /* Start master (listening) thread */
    mg_start_thread_with_id(master_thread, ctx, &ctx->masterthreadid);

    /* Start worker threads */
>>>>>>> b393b8d2410737a01a0d18987b5dfc185b3c0ef3
    for (i = 0; i < workerthreadcount; i++) {
        (void) pthread_mutex_lock(&ctx->mutex);
        ctx->num_threads++;
        (void) pthread_mutex_unlock(&ctx->mutex);
        if (mg_start_thread_with_id(worker_thread, ctx,
                                    &ctx->workerthreadids[i]) != 0) {
            (void) pthread_mutex_lock(&ctx->mutex);
            ctx->num_threads--;
            (void) pthread_mutex_unlock(&ctx->mutex);
            mg_cry(fc(ctx), "Cannot start worker thread: %ld", (long) ERRNO);
        }
    }

    return ctx;
}
