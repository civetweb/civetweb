#include <lua.h>
#include <lauxlib.h>
#include <setjmp.h>

#ifdef _WIN32
static void *mmap(void *addr, int64_t len, int prot, int flags, int fd,
    int offset)
{
    HANDLE fh = (HANDLE) _get_osfhandle(fd);
    HANDLE mh = CreateFileMapping(fh, 0, PAGE_READONLY, 0, 0, 0);
    void *p = MapViewOfFile(mh, FILE_MAP_READ, 0, 0, (size_t) len);
    CloseHandle(mh);
    return p;
}

static void munmap(void *addr, int64_t length)
{
    UnmapViewOfFile(addr);
}

#define MAP_FAILED NULL
#define MAP_PRIVATE 0
#define PROT_READ 0
#else
#include <sys/mman.h>
#endif

static const char *LUASOCKET = "luasocket";

/* Forward declarations */
static void handle_request(struct mg_connection *);
static int handle_lsp_request(struct mg_connection *, const char *,
struct file *, struct lua_State *);

static void reg_string(struct lua_State *L, const char *name, const char *val)
{
    if (name!=NULL && val!=NULL) {
        lua_pushstring(L, name);
        lua_pushstring(L, val);
        lua_rawset(L, -3);
    }
}

static void reg_int(struct lua_State *L, const char *name, int val)
{
    if (name!=NULL) {
        lua_pushstring(L, name);
        lua_pushinteger(L, val);
        lua_rawset(L, -3);
    }
}

static void reg_boolean(struct lua_State *L, const char *name, int val)
{
    if (name!=NULL) {
        lua_pushstring(L, name);
        lua_pushboolean(L, val != 0);
        lua_rawset(L, -3);
    }
}

static void reg_function(struct lua_State *L, const char *name,
    lua_CFunction func, struct mg_connection *conn)
{
    if (name!=NULL && func!=NULL) {
        lua_pushstring(L, name);
        lua_pushlightuserdata(L, conn);
        lua_pushcclosure(L, func, 1);
        lua_rawset(L, -3);
    }
}

static int lsp_sock_close(lua_State *L)
{
    int num_args = lua_gettop(L);
    if ((num_args == 1) && lua_istable(L, -1)) {
        lua_getfield(L, -1, "sock");
        closesocket((SOCKET) lua_tonumber(L, -1));
    } else {
        return luaL_error(L, "invalid :close() call");
    }
    return 1;
}

static int lsp_sock_recv(lua_State *L)
{
    int num_args = lua_gettop(L);
    char buf[2000];
    int n;

    if ((num_args == 1) && lua_istable(L, -1)) {
        lua_getfield(L, -1, "sock");
        n = recv((SOCKET) lua_tonumber(L, -1), buf, sizeof(buf), 0);
        if (n <= 0) {
            lua_pushnil(L);
        } else {
            lua_pushlstring(L, buf, n);
        }
    } else {
        return luaL_error(L, "invalid :close() call");
    }
    return 1;
}

static int lsp_sock_send(lua_State *L)
{
    int num_args = lua_gettop(L);
    const char *buf;
    size_t len, sent = 0;
    int n = 0, sock;

    if ((num_args == 2) && lua_istable(L, -2) && lua_isstring(L, -1)) {
        buf = lua_tolstring(L, -1, &len);
        lua_getfield(L, -2, "sock");
        sock = (int) lua_tonumber(L, -1);
        while (sent < len) {
            if ((n = send(sock, buf + sent, (int)(len - sent), 0)) <= 0) {
                break;
            }
            sent += n;
        }
        lua_pushnumber(L, n);
    } else {
        return luaL_error(L, "invalid :close() call");
    }
    return 1;
}

static const struct luaL_Reg luasocket_methods[] = {
    {"close", lsp_sock_close},
    {"send", lsp_sock_send},
    {"recv", lsp_sock_recv},
    {NULL, NULL}
};

static int lsp_connect(lua_State *L)
{
    int num_args = lua_gettop(L);
    char ebuf[100];
    SOCKET sock;

    if ((num_args == 3) && lua_isstring(L, -3) && lua_isnumber(L, -2) && lua_isnumber(L, -1)) {
        sock = conn2(NULL, lua_tostring(L, -3), (int) lua_tonumber(L, -2),
            (int) lua_tonumber(L, -1), ebuf, sizeof(ebuf));
        if (sock == INVALID_SOCKET) {
            return luaL_error(L, ebuf);
        } else {
            lua_newtable(L);
            reg_int(L, "sock", (int) sock);
            reg_string(L, "host", lua_tostring(L, -4));
            luaL_getmetatable(L, LUASOCKET);
            lua_setmetatable(L, -2);
        }
    } else {
        return luaL_error(L, "connect(host,port,is_ssl): invalid parameter given.");
    }
    return 1;
}

static int lsp_error(lua_State *L)
{
    lua_getglobal(L, "mg");
    lua_getfield(L, -1, "onerror");
    lua_pushvalue(L, -3);
    lua_pcall(L, 1, 0, 0);
    return 0;
}

/* Silently stop processing chunks. */
static void lsp_abort(lua_State *L)
{
    int top = lua_gettop(L);
    lua_getglobal(L, "mg");
    lua_pushnil(L);
    lua_setfield(L, -2, "onerror");
    lua_settop(L, top);
    lua_pushstring(L, "aborting");
    lua_error(L);
}

struct lsp_var_reader_data
{
    const char * begin;
    unsigned len;
    unsigned state;
};

static const char * lsp_var_reader(lua_State *L, void *ud, size_t *sz)
{
    struct lsp_var_reader_data * reader = (struct lsp_var_reader_data *)ud;
    const char * ret;

    switch (reader->state) {
    case 0:
        ret = "mg.write(";
        *sz = strlen(ret);
        break;
    case 1:
        ret = reader->begin;
        *sz = reader->len;
        break;
    case 2:
        ret = ")";
        *sz = strlen(ret);
        break;
    default:
        ret = 0;
        *sz = 0;
    }

    reader->state++;
    return ret;
}

static int lsp(struct mg_connection *conn, const char *path,
    const char *p, int64_t len, lua_State *L)
{
    int i, j, pos = 0, lines = 1, lualines = 0, is_var, lua_ok;
    char chunkname[MG_BUF_LEN];
    struct lsp_var_reader_data data;

    for (i = 0; i < len; i++) {
        if (p[i] == '\n') lines++;
        if ((i + 1) < len && p[i] == '<' && p[i + 1] == '?') {

            /* <?= ?> means a variable is enclosed and its value should be printed */
            is_var = ((i + 2) < len && p[i + 2] == '=');

            if (is_var) j = i + 2;
            else j = i + 1;

            while (j < len) {
                if (p[j] == '\n') lualines++;
                if ((j + 1) < len && p[j] == '?' && p[j + 1] == '>') {
                    mg_write(conn, p + pos, i - pos);

                    snprintf(chunkname, sizeof(chunkname), "@%s+%i", path, lines);
                    lua_pushlightuserdata(L, conn);
                    lua_pushcclosure(L, lsp_error, 1);

                    if (is_var) {
                        data.begin = p + (i + 3);
                        data.len = j - (i + 3);
                        data.state = 0;
                        lua_ok = lua_load(L, lsp_var_reader, &data, chunkname, NULL);
                    } else {
                        lua_ok = luaL_loadbuffer(L, p + (i + 2), j - (i + 2), chunkname);
                    }

                    if (lua_ok) {
                        /* Syntax error or OOM. Error message is pushed on stack. */
                        lua_pcall(L, 1, 0, 0);
                    } else {
                        /* Success loading chunk. Call it. */
                        lua_pcall(L, 0, 0, 1);
                    }

                    pos = j + 2;
                    i = pos - 1;
                    break;
                }
                j++;
            }

            if (lualines > 0) {
                lines += lualines;
                lualines = 0;
            }
        }
    }

    if (i > pos) {
        mg_write(conn, p + pos, i - pos);
    }

    return 0;
}

/* mg.write: Send data to the client */
static int lsp_write(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    int num_args = lua_gettop(L);
    const char *str;
    size_t size;
    int i;

    for (i = 1; i <= num_args; i++) {
        if (lua_isstring(L, i)) {
            str = lua_tolstring(L, i, &size);
            mg_write(conn, str, size);
        }
    }

    return 0;
}

/* mg.read: Read data from the client (e.g., from a POST request) */
static int lsp_read(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    char buf[1024];
    int len = mg_read(conn, buf, sizeof(buf));

    if (len <= 0) return 0;
    lua_pushlstring(L, buf, len);

    return 1;
}

/* mg.keep_alive: Allow Lua pages to use the http keep-alive mechanism */
static int lsp_keep_alive(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    int num_args = lua_gettop(L);

    /* This function may be called with one parameter (boolean) to set the keep_alive state.
       Or without a parameter to just query the current keep_alive state. */
    if ((num_args==1) && lua_isboolean(L, 1)) {
        conn->must_close = !lua_toboolean(L, 1);
    } else if (num_args != 0) {
        /* Syntax error */
        return luaL_error(L, "invalid keep_alive() call");
    }

    /* Return the current "keep_alive" state. This may be false, even it keep_alive(true) has been called. */
    lua_pushboolean(L, should_keep_alive(conn));
    return 1;
}

/* mg.include: Include another .lp file */
static int lsp_include(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    int num_args = lua_gettop(L);
    struct file file = STRUCT_FILE_INITIALIZER;
    const char * filename = (num_args == 1) ? lua_tostring(L, 1) : NULL;

    if (filename) {
        if (handle_lsp_request(conn, filename, &file, L)) {
            /* handle_lsp_request returned an error code, meaning an error occured in
            the included page and mg.onerror returned non-zero. Stop processing. */
            lsp_abort(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid include() call");
    }
    return 0;
}

/* mg.cry: Log an error. Default value for mg.onerror. */
static int lsp_cry(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    int num_args = lua_gettop(L);
    const char * text = (num_args == 1) ? lua_tostring(L, 1) : NULL;

    if (text) {
        mg_cry(conn, "%s", lua_tostring(L, -1));
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid cry() call");
    }
    return 0;
}

/* mg.redirect: Redirect the request (internally). */
static int lsp_redirect(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    int num_args = lua_gettop(L);
    const char * target = (num_args == 1) ? lua_tostring(L, 1) : NULL;

    if (target) {
        conn->request_info.uri = target;
        handle_request(conn);
        lsp_abort(L);
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid redirect() call");
    }
    return 0;
}

/* mg.send_file */
static int lsp_send_file(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    int num_args = lua_gettop(L);
    const char * filename = (num_args == 1) ? lua_tostring(L, 1) : NULL;

    if (filename) {
        mg_send_file(conn, filename);
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid send_file() call");
    }
    return 0;
}

/* mg.get_var */
static int lsp_get_var(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    int num_args = lua_gettop(L);
    const char *data, *var_name;
    size_t data_len, occurrence;
    int ret;
    char dst[512];

    if (num_args>=2 && num_args<=3) {
        data = lua_tolstring(L, 1, &data_len);
        var_name = lua_tostring(L, 2);
        occurrence = (num_args>2) ? (long)lua_tonumber(L, 3) : 0;

        ret = mg_get_var2(data, data_len, var_name, dst, sizeof(dst), occurrence);
        if (ret>=0) {
            /* Variable found: return value to Lua */
            lua_pushstring(L, dst);
        } else {
            /* Variable not found (TODO: may be string too long) */
            lua_pushnil(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid get_var() call");
    }
    return 1;
}

/* mg.get_mime_type */
static int lsp_get_mime_type(lua_State *L)
{
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    int num_args = lua_gettop(L);
    struct vec mime_type = {0};
    const char *text;

    if (num_args==1) {
        text = lua_tostring(L, 1);
        if (text) {
            get_mime_type(conn->ctx, text, &mime_type);
            lua_pushlstring(L, mime_type.ptr, mime_type.len);
        } else {
            lua_pushnil(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid get_mime_type() call");
    }
    return 1;
}

/* mg.get_cookie */
static int lsp_get_cookie(lua_State *L)
{
    int num_args = lua_gettop(L);
    struct vec mime_type = {0};
    const char *cookie;
    const char *var_name;
    int ret;
    char dst[512];

    if (num_args==2) {
        cookie = lua_tostring(L, 1);
        var_name = lua_tostring(L, 2);
        if (cookie!=NULL && var_name!=NULL) {
            ret = mg_get_cookie(cookie, var_name, dst, sizeof(dst));
        } else {
            ret = -1;
        }

        if (ret>=0) {
            lua_pushlstring(L, dst, ret);
        } else {
            lua_pushnil(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid get_cookie() call");
    }
    return 1;
}

/* mg.md5 */
static int lsp_md5(lua_State *L)
{
    int num_args = lua_gettop(L);
    const char *text;
    md5_byte_t hash[16];
    md5_state_t ctx;
    size_t text_len;
    char buf[40];

    if (num_args==1) {
        text = lua_tolstring(L, 1, &text_len);
        if (text) {
            md5_init(&ctx);
            md5_append(&ctx, (const md5_byte_t *) text, text_len);
            md5_finish(&ctx, hash);
            bin2str(buf, hash, sizeof(hash));
            lua_pushstring(L, buf);
        } else {
            lua_pushnil(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid md5() call");
    }
    return 1;
}

/* mg.url_encode */
static int lsp_url_encode(lua_State *L)
{
    int num_args = lua_gettop(L);
    const char *text;
    size_t text_len;
    char dst[512];

    if (num_args==1) {
        text = lua_tolstring(L, 1, &text_len);
        if (text) {
            mg_url_encode(text, dst, sizeof(dst));
            lua_pushstring(L, dst);
        } else {
            lua_pushnil(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid url_encode() call");
    }
    return 1;
}

/* mg.url_decode */
static int lsp_url_decode(lua_State *L)
{
    int num_args = lua_gettop(L);
    const char *text;
    size_t text_len;
    int is_form;
    char dst[512];

    if (num_args==1 || (num_args==2 && lua_isboolean(L, 2))) {
        text = lua_tolstring(L, 1, &text_len);
        is_form = (num_args==2) ? lua_isboolean(L, 2) : 0;
        if (text) {
            mg_url_decode(text, text_len, dst, sizeof(dst), is_form);
            lua_pushstring(L, dst);
        } else {
            lua_pushnil(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid url_decode() call");
    }
    return 1;
}

/* mg.base64_encode */
static int lsp_base64_encode(lua_State *L)
{
    int num_args = lua_gettop(L);
    const char *text;
    size_t text_len;
    char *dst;

    if (num_args==1) {
        text = lua_tolstring(L, 1, &text_len);
        if (text) {
            dst = mg_malloc(text_len*8/6+4);
            if (dst) {
                base64_encode(text, text_len, dst);
                lua_pushstring(L, dst);
                mg_free(dst);
            } else {
                return luaL_error(L, "out of memory in base64_encode() call");
            }
        } else {
            lua_pushnil(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid base64_encode() call");
    }
    return 1;
}

/* mg.base64_encode */
static int lsp_base64_decode(lua_State *L)
{
    int num_args = lua_gettop(L);
    const char *text;
    size_t text_len, dst_len;
    int ret;
    char *dst;

    if (num_args==1) {
        text = lua_tolstring(L, 1, &text_len);
        if (text) {
            dst = mg_malloc(text_len);
            if (dst) {
                ret = base64_decode(text, text_len, dst, &dst_len);
                if (ret != -1) {
                    mg_free(dst);
                    return luaL_error(L, "illegal character in lsp_base64_decode() call");
                } else {
                    lua_pushlstring(L, dst, dst_len);
                    mg_free(dst);
                }
            } else {
                return luaL_error(L, "out of memory in lsp_base64_decode() call");
            }
        } else {
            lua_pushnil(L);
        }
    } else {
        /* Syntax error */
        return luaL_error(L, "invalid lsp_base64_decode() call");
    }
    return 1;
}

/* mg.write for websockets */
static int lwebsock_write(lua_State *L)
{
#ifdef USE_WEBSOCKET
    int num_args = lua_gettop(L);
    struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
    const char *str;
    size_t size;
    int opcode = -1;

    if (num_args == 1) {
        if (lua_isstring(L, 1)) {
            str = lua_tolstring(L, 1, &size);
            mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, str, size);
        }
    } else if (num_args == 2) {
        if (lua_isnumber(L, 1)) {
            opcode = (int)lua_tointeger(L, 1);
        } else if (lua_isstring(L,1)) {
            str = lua_tostring(L, 1);
            if (!mg_strncasecmp(str, "text", 4)) opcode = WEBSOCKET_OPCODE_TEXT;
            else if (!mg_strncasecmp(str, "bin", 3)) opcode = WEBSOCKET_OPCODE_BINARY;
            else if (!mg_strncasecmp(str, "close", 5)) opcode = WEBSOCKET_OPCODE_CONNECTION_CLOSE;
            else if (!mg_strncasecmp(str, "ping", 4)) opcode = WEBSOCKET_OPCODE_PING;
            else if (!mg_strncasecmp(str, "pong", 4)) opcode = WEBSOCKET_OPCODE_PONG;
            else if (!mg_strncasecmp(str, "cont", 4)) opcode = WEBSOCKET_OPCODE_CONTINUATION;
        }
        if (opcode>=0 && opcode<16 && lua_isstring(L, 2)) {
            str = lua_tolstring(L, 2, &size);
            mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, str, size);
        }
    }
#endif
    return 0;
}

enum {
    LUA_ENV_TYPE_LUA_SERVER_PAGE = 0,
    LUA_ENV_TYPE_PLAIN_LUA_PAGE = 1,
    LUA_ENV_TYPE_LUA_WEBSOCKET = 2,
};

static void prepare_lua_environment(struct mg_connection *conn, lua_State *L, const char *script_name, int lua_env_type)
{
    const struct mg_request_info *ri = mg_get_request_info(conn);
    char src_addr[IP_ADDR_STR_LEN];
    const char * preload_file = conn->ctx->config[LUA_PRELOAD_FILE];
    int i;

    extern void luaL_openlibs(lua_State *);

    sockaddr_to_string(src_addr, sizeof(src_addr), &conn->client.rsa);

    luaL_openlibs(L);
#ifdef USE_LUA_SQLITE3
    {
        extern int luaopen_lsqlite3(lua_State *);
        luaopen_lsqlite3(L);
    }
#endif
#ifdef USE_LUA_FILE_SYSTEM
    {
        extern int luaopen_lfs(lua_State *);
        luaopen_lfs(L);
    }
#endif

    luaL_newmetatable(L, LUASOCKET);
    lua_pushliteral(L, "__index");
    luaL_newlib(L, luasocket_methods);
    lua_rawset(L, -3);
    lua_pop(L, 1);
    lua_register(L, "connect", lsp_connect);

    if (conn == NULL) {
        /* Do not register any connection specific functions or variables */
        return;
    }

    /* Register mg module */
    lua_newtable(L);

    reg_function(L, "cry", lsp_cry, conn);

    switch (lua_env_type) {
        case LUA_ENV_TYPE_LUA_SERVER_PAGE:
            reg_string(L, "lua_type", "page");
            break;
        case LUA_ENV_TYPE_PLAIN_LUA_PAGE:
            reg_string(L, "lua_type", "script");
            break;
        case LUA_ENV_TYPE_LUA_WEBSOCKET:
            reg_string(L, "lua_type", "websocket");
            break;
    }

    if (lua_env_type==LUA_ENV_TYPE_LUA_SERVER_PAGE || lua_env_type==LUA_ENV_TYPE_PLAIN_LUA_PAGE) {
        reg_function(L, "read", lsp_read, conn);
        reg_function(L, "write", lsp_write, conn);
        reg_function(L, "keep_alive", lsp_keep_alive, conn);
    }

    if (lua_env_type==LUA_ENV_TYPE_LUA_SERVER_PAGE) {
        reg_function(L, "include", lsp_include, conn);
        reg_function(L, "redirect", lsp_redirect, conn);
    }

    if (lua_env_type==LUA_ENV_TYPE_LUA_WEBSOCKET) {
        reg_function(L, "write", lwebsock_write, conn);
    }

    reg_function(L, "send_file", lsp_send_file, conn);
    reg_function(L, "get_var", lsp_get_var, conn);
    reg_function(L, "get_mime_type", lsp_get_mime_type, conn);
    reg_function(L, "get_cookie", lsp_get_cookie, conn);
    reg_function(L, "md5", lsp_md5, conn);
    reg_function(L, "url_encode", lsp_url_encode, conn);
    reg_function(L, "url_decode", lsp_url_decode, conn);
    reg_function(L, "base64_encode", lsp_base64_encode, conn);
    reg_function(L, "base64_decode", lsp_base64_decode, conn);

    reg_string(L, "version", CIVETWEB_VERSION);
    reg_string(L, "document_root", conn->ctx->config[DOCUMENT_ROOT]);
    reg_string(L, "auth_domain", conn->ctx->config[AUTHENTICATION_DOMAIN]);
#if defined(USE_WEBSOCKET)
    reg_string(L, "websocket_root", conn->ctx->config[WEBSOCKET_ROOT]);
#endif

    if (conn->ctx->systemName) {
        reg_string(L, "system", conn->ctx->systemName);
    }

    /* Export request_info */
    lua_pushstring(L, "request_info");
    lua_newtable(L);
    reg_string(L, "request_method", ri->request_method);
    reg_string(L, "uri", ri->uri);
    reg_string(L, "http_version", ri->http_version);
    reg_string(L, "query_string", ri->query_string);
    reg_int(L, "remote_ip", ri->remote_ip); /* remote_ip is deprecated, use remote_addr instead */
    reg_string(L, "remote_addr", src_addr);
    /* TODO: ip version */
    reg_int(L, "remote_port", ri->remote_port);
    reg_int(L, "num_headers", ri->num_headers);
    reg_int(L, "server_port", ntohs(conn->client.lsa.sin.sin_port));

    if (conn->request_info.remote_user != NULL) {
        reg_string(L, "remote_user", conn->request_info.remote_user);
        reg_string(L, "auth_type", "Digest");
    }

    lua_pushstring(L, "http_headers");
    lua_newtable(L);
    for (i = 0; i < ri->num_headers; i++) {
        reg_string(L, ri->http_headers[i].name, ri->http_headers[i].value);
    }
    lua_rawset(L, -3);

    reg_boolean(L, "https", conn->ssl != NULL);
    reg_string(L, "script_name", script_name);

    if (conn->status_code > 0) {
        /* Lua error handler should show the status code */
        reg_int(L, "status", conn->status_code);
    }

    lua_rawset(L, -3);
    lua_setglobal(L, "mg");

    /* Register default mg.onerror function */
    IGNORE_UNUSED_RESULT(luaL_dostring(L, "mg.onerror = function(e) mg.write('\\nLua error:\\n', "
        "debug.traceback(e, 1)) end"));

    /* Preload */
    if ((preload_file != NULL) && (*preload_file != 0)) {
        IGNORE_UNUSED_RESULT(luaL_dofile(L, preload_file));
    }
}

static int lua_error_handler(lua_State *L)
{
    const char *error_msg =  lua_isstring(L, -1) ?  lua_tostring(L, -1) : "?\n";

    lua_getglobal(L, "mg");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "write");   /* call mg.write() */
        lua_pushstring(L, error_msg);
        lua_pushliteral(L, "\n");
        lua_call(L, 2, 0);
        IGNORE_UNUSED_RESULT(luaL_dostring(L, "mg.write(debug.traceback(), '\\n')"));
    } else {
        printf("Lua error: [%s]\n", error_msg);
        IGNORE_UNUSED_RESULT(luaL_dostring(L, "print(debug.traceback(), '\\n')"));
    }
    /* TODO(lsm): leave the stack balanced */

    return 0;
}

static void * lua_allocator(void *ud, void *ptr, size_t osize, size_t nsize) {

    (void)ud; (void)osize; /* not used */

    if (nsize == 0) {
        mg_free(ptr);
        return NULL;
    }
    return mg_realloc(ptr, nsize);
}

void mg_exec_lua_script(struct mg_connection *conn, const char *path,
    const void **exports)
{
    int i;
    lua_State *L;

    /* Assume the script does not support keep_alive. The script may change this by calling mg.keep_alive(true). */
    conn->must_close=1;

    /* Execute a plain Lua script. */
    if (path != NULL && (L = lua_newstate(lua_allocator, NULL)) != NULL) {
        prepare_lua_environment(conn, L, path, LUA_ENV_TYPE_PLAIN_LUA_PAGE);
        lua_pushcclosure(L, &lua_error_handler, 0);

        if (exports != NULL) {
            lua_pushglobaltable(L);
            for (i = 0; exports[i] != NULL && exports[i + 1] != NULL; i += 2) {
                lua_pushstring(L, exports[i]);
                lua_pushcclosure(L, (lua_CFunction) exports[i + 1], 0);
                lua_rawset(L, -3);
            }
        }

        if (luaL_loadfile(L, path) != 0) {
            lua_error_handler(L);
        }
        lua_pcall(L, 0, 0, -2);
        lua_close(L);
    }
}

static void lsp_send_err(struct mg_connection *conn, struct lua_State *L,
    const char *fmt, ...)
{
    char buf[MG_BUF_LEN];
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    if (L == NULL) {
        send_http_error(conn, 500, http_500_error, "%s", buf);
    } else {
        lua_pushstring(L, buf);
        lua_error(L);
    }
}

static int handle_lsp_request(struct mg_connection *conn, const char *path,
struct file *filep, struct lua_State *ls)
{
    void *p = NULL;
    lua_State *L = NULL;
    int error = 1;

    /* Assume the script does not support keep_alive. The script may change this by calling mg.keep_alive(true). */
    conn->must_close=1;

    /* We need both mg_stat to get file size, and mg_fopen to get fd */
    if (!mg_stat(conn, path, filep) || !mg_fopen(conn, path, "r", filep)) {
        lsp_send_err(conn, ls, "File [%s] not found", path);
    } else if (filep->membuf == NULL &&
        (p = mmap(NULL, (size_t) filep->size, PROT_READ, MAP_PRIVATE,
        fileno(filep->fp), 0)) == MAP_FAILED) {
            lsp_send_err(conn, ls, "mmap(%s, %zu, %d): %s", path, (size_t) filep->size,
                fileno(filep->fp), strerror(errno));
    } else if ((L = ls != NULL ? ls : lua_newstate(lua_allocator, NULL)) == NULL) {
        send_http_error(conn, 500, http_500_error, "%s", "luaL_newstate failed");
    } else {
        /* We're not sending HTTP headers here, Lua page must do it. */
        if (ls == NULL) {
            prepare_lua_environment(conn, L, path, LUA_ENV_TYPE_LUA_SERVER_PAGE);
            if (conn->ctx->callbacks.init_lua != NULL) {
                conn->ctx->callbacks.init_lua(conn, L);
            }
        }
        error = lsp(conn, path, filep->membuf == NULL ? p : filep->membuf,
            filep->size, L);
    }

    if (L != NULL && ls == NULL) lua_close(L);
    if (p != NULL) munmap(p, filep->size);
    mg_fclose(filep);
    return error;
}

#ifdef USE_WEBSOCKET
struct lua_websock_data {
    lua_State *main;
    lua_State *thread;
    char * script;
    unsigned shared;
    struct mg_connection *conn;
    pthread_mutex_t mutex;
};

struct mg_shared_lua_websocket {
    struct lua_websock_data *sock;
    struct mg_shared_lua_websocket *next;
};

static void websock_cry(struct mg_connection *conn, int err, lua_State * L, const char * ws_operation, const char * lua_operation)
{
    switch (err) {
        case LUA_OK:
        case LUA_YIELD:
            break;
        case LUA_ERRRUN:
            mg_cry(conn, "%s: %s failed: runtime error: %s", ws_operation, lua_operation, lua_tostring(L, -1));
            break;
        case LUA_ERRSYNTAX:
            mg_cry(conn, "%s: %s failed: syntax error: %s", ws_operation, lua_operation, lua_tostring(L, -1));
            break;
        case LUA_ERRMEM:
            mg_cry(conn, "%s: %s failed: out of memory", ws_operation, lua_operation);
            break;
        case LUA_ERRGCMM:
            mg_cry(conn, "%s: %s failed: error during garbage collection", ws_operation, lua_operation);
            break;
        case LUA_ERRERR:
            mg_cry(conn, "%s: %s failed: error in error handling: %s", ws_operation, lua_operation, lua_tostring(L, -1));
            break;
        default:
            mg_cry(conn, "%s: %s failed: error %i", ws_operation, lua_operation, err);
            break;
    }
}

static void * lua_websocket_new(const char * script, struct mg_connection *conn, int is_shared)
{
    struct lua_websock_data *lws_data;
    struct mg_shared_lua_websocket **shared_websock_list = &(conn->ctx->shared_lua_websockets);
    int ok = 0;
    int found = 0;
    int err, nargs;

    assert(conn->lua_websocket_state == NULL);

    /*
    lock list (mg_context global)
    check if in list
    yes: inc rec counter
    no: create state, add to list
    lock list element
    unlock list (mg_context global)
    call add
    unlock list element
    */

    if (is_shared) {
        (void)pthread_mutex_lock(&conn->ctx->mutex);
        while (*shared_websock_list) {
            if (!strcmp((*shared_websock_list)->sock->script, script)) {
                lws_data = (*shared_websock_list)->sock;
                lws_data->shared++;
                found = 1;
            }
            shared_websock_list = &((*shared_websock_list)->next);
        }
        (void)pthread_mutex_unlock(&conn->ctx->mutex);
    }

    if (!found) {
        lws_data = (struct lua_websock_data *) mg_malloc(sizeof(*lws_data));
    }

    if (lws_data) {
        if (!found) {
            lws_data->shared = is_shared;
            lws_data->conn = conn;
            lws_data->script = mg_strdup(script);
            lws_data->main = lua_newstate(lua_allocator, NULL);
            if (is_shared) {
                (void)pthread_mutex_lock(&conn->ctx->mutex);
                shared_websock_list = &(conn->ctx->shared_lua_websockets);
                while (*shared_websock_list) {
                    shared_websock_list = &((*shared_websock_list)->next);
                }
                *shared_websock_list = (struct mg_shared_lua_websocket *)mg_malloc(sizeof(struct mg_shared_lua_websocket));
                if (*shared_websock_list) {
                    (*shared_websock_list)->sock = lws_data;
                    (*shared_websock_list)->next = 0;
                }
                (void)pthread_mutex_unlock(&conn->ctx->mutex);
            }
        }

        if (lws_data->main) {
            prepare_lua_environment(conn, lws_data->main, script, LUA_ENV_TYPE_LUA_WEBSOCKET);
            if (conn->ctx->callbacks.init_lua != NULL) {
                conn->ctx->callbacks.init_lua(conn, lws_data->main);
            }
            lws_data->thread = lua_newthread(lws_data->main);
            err = luaL_loadfile(lws_data->thread, script);
            if (err==LUA_OK) {
                /* Activate the Lua script. */
                err = lua_resume(lws_data->thread, NULL, 0);
                if (err!=LUA_YIELD) {
                    websock_cry(conn, err, lws_data->thread, __func__, "lua_resume");
                } else {
                    nargs = lua_gettop(lws_data->thread);
                    ok = (nargs==1) && lua_isboolean(lws_data->thread, 1) && lua_toboolean(lws_data->thread, 1);
                }
            } else {
                websock_cry(conn, err, lws_data->thread, __func__, "lua_loadfile");
            }

        } else {
            mg_cry(conn, "%s: luaL_newstate failed", __func__);
        }

        if (!ok) {
            if (lws_data->main) lua_close(lws_data->main);
            mg_free(lws_data->script);
            mg_free(lws_data);
            lws_data=0;
        }
    } else {
        mg_cry(conn, "%s: out of memory", __func__);
    }

    return lws_data;
}

static int lua_websocket_data(struct mg_connection *conn, int bits, char *data, size_t data_len)
{
    struct lua_websock_data *lws_data = (struct lua_websock_data *)(conn->lua_websocket_state);
    int err, nargs, ok=0, retry;
    lua_Number delay;

    assert(lws_data != NULL);
    assert(lws_data->main != NULL);
    assert(lws_data->thread != NULL);

    /*
    lock list element
    call data
    unlock list element
    */

    do {
        retry=0;

        /* Push the data to Lua, then resume the Lua state. */
        /* The data will be available to Lua as the result of the coroutine.yield function. */
        lua_pushboolean(lws_data->thread, 1);
        if (bits >= 0) {
            lua_pushinteger(lws_data->thread, bits);
            if (data) {
                lua_pushlstring(lws_data->thread, data, data_len);
                err = lua_resume(lws_data->thread, NULL, 3);
            } else {
                err = lua_resume(lws_data->thread, NULL, 2);
            }
        } else {
            err = lua_resume(lws_data->thread, NULL, 1);
        }

        /* Check if Lua returned by a call to the coroutine.yield function. */
        if (err!=LUA_YIELD) {
            websock_cry(conn, err, lws_data->thread, __func__, "lua_resume");
        } else {
            nargs = lua_gettop(lws_data->thread);
            ok = (nargs>=1) && lua_isboolean(lws_data->thread, 1) && lua_toboolean(lws_data->thread, 1);
            delay = (nargs>=2) && lua_isnumber(lws_data->thread, 2) ? lua_tonumber(lws_data->thread, 2) : -1.0;
            if (ok && delay>0) {
                fd_set rfds;
                struct timeval tv;

                FD_ZERO(&rfds);
                FD_SET(conn->client.sock, &rfds);

                tv.tv_sec = (unsigned long)delay;
                tv.tv_usec = (unsigned long)(((double)delay - (double)((unsigned long)delay))*1000000.0);
                retry = (0==select(conn->client.sock+1, &rfds, NULL, NULL, &tv));
            }
        }
    } while (retry);

    return ok;
}

static int lua_websocket_ready(struct mg_connection *conn)
{
    return lua_websocket_data(conn, -1, NULL, 0);
}

static void lua_websocket_close(struct mg_connection *conn)
{
    struct lua_websock_data *lws_data = (struct lua_websock_data *)(conn->lua_websocket_state);
    struct mg_shared_lua_websocket **shared_websock_list;
    int err;

    assert(lws_data != NULL);
    assert(lws_data->main != NULL);
    assert(lws_data->thread != NULL);

    /*
    lock list element
    lock list (mg_context global)
    call remove    
    dec ref counter
    if ref counter == 0 close state and remove from list
    unlock list element
    unlock list (mg_context global)
    */


    lua_pushboolean(lws_data->thread, 0);
    err = lua_resume(lws_data->thread, NULL, 1);

    if (lws_data->shared) {
        (void)pthread_mutex_lock(&conn->ctx->mutex);
        lws_data->shared--;
        if (lws_data->shared==0) {
        /*
            shared_websock_list = &(conn->ctx->shared_lua_websockets);
            while (*shared_websock_list) {
                if ((*shared_websock_list)->sock == lws_data) {
                    *shared_websock_list = (*shared_websock_list)->next;
                } else {
                    shared_websock_list = &((*shared_websock_list)->next);
                }
            }

            lua_close(lws_data->main);
            mg_free(lws_data->script);
            lws_data->script=0;
            mg_free(lws_data);
         */
        }
        (void)pthread_mutex_unlock(&conn->ctx->mutex);
    } else {
        lua_close(lws_data->main);
        mg_free(lws_data->script);
        mg_free(lws_data);
    }
    conn->lua_websocket_state = NULL;
}
#endif
