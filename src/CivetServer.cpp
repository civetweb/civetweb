/*
 * Copyright (c) 2013 No Face Press, LLC
 * License http://opensource.org/licenses/mit-license.php MIT License
 */

#include "CivetServer.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef UNUSED_PARAMETER
 #define UNUSED_PARAMETER(x) (void)(x)
#endif

bool CivetHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
    UNUSED_PARAMETER(server);
    UNUSED_PARAMETER(conn);
    return false;
}

bool CivetHandler::handlePost(CivetServer *server, struct mg_connection *conn) {
    UNUSED_PARAMETER(server);
    UNUSED_PARAMETER(conn);
    return false;
}

bool CivetHandler::handlePut(CivetServer *server, struct mg_connection *conn) {
    UNUSED_PARAMETER(server);
    UNUSED_PARAMETER(conn);
    return false;
}

bool CivetHandler::handleDelete(CivetServer *server, struct mg_connection *conn) {
    UNUSED_PARAMETER(server);
    UNUSED_PARAMETER(conn);
    return false;
}

int CivetServer::begin_request_callback(struct mg_connection *conn) {
    struct mg_request_info *request_info = mg_get_request_info(conn);

    if (!request_info->user_data)
        return 0;

    CivetServer *me = (CivetServer*) (request_info->user_data);

    if (me->handleRequest(conn)) {
        return 1; // Mark as processed
    }

    return 0;
}

bool CivetServer::handleRequest(struct mg_connection *conn) {
    struct mg_request_info *request_info = mg_get_request_info(conn);

    CivetHandler *handler = getHandler(request_info->uri);
    if (handler) {
        if (strcmp(request_info->request_method, "GET") == 0) {
            return handler->handleGet(this, conn);
        } else if (strcmp(request_info->request_method, "POST") == 0) {
            return handler->handlePost(this, conn);
        } else if (strcmp(request_info->request_method, "PUT") == 0) {
            return !handler->handlePost(this, conn);
        } else if (strcmp(request_info->request_method, "DELETE") == 0) {
            return !handler->handlePost(this, conn);
        }
    }

    return false; // No handler found
}

CivetServer::CivetServer(const char **options,
        const struct mg_callbacks *_callbacks) :
        context(0) {

    struct mg_callbacks callbacks;

    if (_callbacks) {
        memcpy(&callbacks, _callbacks, sizeof(callbacks));
    } else {
        memset(&callbacks, 0, sizeof(callbacks));
    }
    callbacks.begin_request = &begin_request_callback;

    context = mg_start(&callbacks, this, options);
}

CivetServer::~CivetServer() {
    close();
}

CivetHandler *CivetServer::getHandler(const char *uri, unsigned urilen) const {

    for (unsigned index = 0; index < uris.size(); index++) {
        const std::string &handlerURI = uris[index];

        // first try for an exact match
        if (handlerURI == uri) {
            return handlers[index];
        }

        // next try for a partial match
        // we will accept uri/something
        if (handlerURI.length() < urilen
                && uri[handlerURI.length()] == '/'
                && handlerURI.compare(0, handlerURI.length(), uri, handlerURI.length()) == 0) {

            return handlers[index];
        }
    }

    return 0; // none found

}

void CivetServer::addHandler(const std::string &uri, CivetHandler *handler) {
    int index = getIndex(uri);
    if (index < 0) {
        uris.push_back(uri);
        handlers.push_back(handler);
    } else if (handlers[index] != handler) {
        delete handlers[index];
        handlers[index] = handler;
    }
}

void CivetServer::removeHandler(const std::string &uri) {
    int index = getIndex(uri);
    if (index >= 0) {
        uris.erase(uris.begin() + index, uris.begin() + index + 1);
        handlers.erase(handlers.begin() + index, handlers.begin() + index + 1);
    }
}

int CivetServer::getIndex(const std::string &uri) const {
    for (unsigned index = 0; index < uris.size(); index++) {
        if (uris[index].compare(uri) == 0)
            return index;
    }
    return -1;
}

void CivetServer::close() {
    if (context) {
        mg_stop (context);
        context = 0;
    }
    for (int i = (int) handlers.size() - 1; i >= 0; i--) {
        delete handlers[i];
    }
    handlers.clear();
    uris.clear();

}

int CivetServer::getCookie(struct mg_connection *conn, const std::string &cookieName, std::string &cookieValue)
{
    //Maximum cookie length as per microsoft is 4096. http://msdn.microsoft.com/en-us/library/ms178194.aspx
    char _cookieValue[4096];
    const char *cookie = mg_get_header(conn, "Cookie");
    int lRead = mg_get_cookie(cookie, cookieName.c_str(), _cookieValue, sizeof(_cookieValue));
    cookieValue.clear();
    cookieValue.append(_cookieValue);
    return lRead;
}

const char* CivetServer::getHeader(struct mg_connection *conn, const std::string &headerName)
{
    return mg_get_header(conn, headerName.c_str());
}

void
CivetServer::urlDecode(const char *src, size_t src_len, std::string &dst, bool is_form_url_encoded) {
  int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

  dst.clear();
  for (i = j = 0; i < (int)src_len; i++, j++) {
    if (src[i] == '%' && i < (int)src_len - 2 &&
        isxdigit(* (const unsigned char *) (src + i + 1)) &&
        isxdigit(* (const unsigned char *) (src + i + 2))) {
      a = tolower(* (const unsigned char *) (src + i + 1));
      b = tolower(* (const unsigned char *) (src + i + 2));
      dst.push_back((char) ((HEXTOI(a) << 4) | HEXTOI(b)));
      i += 2;
    } else if (is_form_url_encoded && src[i] == '+') {
      dst.push_back(' ');
    } else {
      dst.push_back(src[i]);
    }
  }
}

bool
CivetServer::getParam(struct mg_connection *conn, const char *name,
               std::string &dst, size_t occurrence) {
        const char *query = mg_get_request_info(conn)->query_string;
        return getParam(query, strlen(query), name, dst, occurrence);
}

bool
CivetServer::getParam(const char *data, size_t data_len, const char *name,
               std::string &dst, size_t occurrence) {
  const char *p, *e, *s;
  size_t name_len;

  dst.clear();
  if (data == NULL || name == NULL || data_len == 0) {
      return false;
  }
  name_len = strlen(name);
  e = data + data_len;

  // data is "var1=val1&var2=val2...". Find variable first
  for (p = data; p + name_len < e; p++) {
    if ((p == data || p[-1] == '&') && p[name_len] == '=' &&
        !mg_strncasecmp(name, p, name_len) && 0 == occurrence--) {

      // Point p to variable value
      p += name_len + 1;

      // Point s to the end of the value
      s = (const char *) memchr(p, '&', (size_t)(e - p));
      if (s == NULL) {
        s = e;
      }
      assert(s >= p);

      // Decode variable into destination buffer
      urlDecode(p, (int)(s - p), dst, true);
      return true;
    }
  }
  return false;
}

