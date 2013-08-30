/*
 * Copyright (c) 2013 No Face Press, LLC
 * License http://opensource.org/licenses/mit-license.php MIT License
 */

#ifndef _CIVETWEB_SERVER_H_
#define _CIVETWEB_SERVER_H_
#ifdef __cplusplus

#include "civetweb.h"
#include <vector>
#include <string>

class CivetServer; // forward declaration

/**
 * Basic interface for a URI request handler.  Handlers implementations
 * must be reentrant.
 */
class CivetHandler {
public:

    /**
     * Destructor
     */
    virtual ~CivetHandler() {
    }

    /**
     * Callback method for GET request.
     *
     * @param server - the calling server
     * @param conn - the connection information
     * @returns true if implemented, false otherwise
     */
    virtual bool handleGet(CivetServer *server, struct mg_connection *conn);

    /**
     * Callback method for POST request.
     *
     * @param server - the calling server
     * @param conn - the connection information
     * @returns true if implemented, false otherwise
     */
    virtual bool handlePost(CivetServer *server, struct mg_connection *conn);

    /**
     * Callback method for PUT request.
     *
     * @param server - the calling server
     * @param conn - the connection information
     * @returns true if implemented, false otherwise
     */
    virtual bool handlePut(CivetServer *server, struct mg_connection *conn);

    /**
     * Callback method for DELETE request.
     *
     * @param server - the calling server
     * @param conn - the connection information
     * @returns true if implemented, false otherwise
     */
    virtual bool handleDelete(CivetServer *server, struct mg_connection *conn);

};

/**
 * CivetServer
 *
 * Basic class for embedded web server.  This has a URL mapping built-in.
 */
class CivetServer {
public:

    /**
     * Constructor
     *
     * This automatically starts the sever.
     * It is good practice to call getContext() after this in case there
     * were errors starting the server.
     *
     * @param options - the web server options.
     * @param callbacks - optional web server callback methods.
     *    Note that this class overrides begin_request callback.
     */
    CivetServer(const char **options, const struct mg_callbacks *callbacks = 0);

    /**
     * Destructor
     */
    virtual ~CivetServer();

    /**
     * close()
     *
     * Stops server and frees resources.
     */
    void close();

    /**
     * getContext()
     *
     * @return the context or 0 if not running.
     */
    const struct mg_context *getContext() const {
        return context;
    }

    /**
     * addHandler(const std::string &, CivetHandler *)
     *
     * Adds a URI handler.  If there is existing URI handler, it will
     * be replaced with this one.  The handler is "owned" by this server
     * and will be deallocated with it.
     *
     * URI's are ordered and partcial URI's are supported. For example,
     * consider two URIs in order: /a/b and /a; /a matches
     *  /a, /a/b matches /a/b, /a/c matches /a.  Reversing the order to
     *  /a and /a/b; /a matches /a/b, /a/b matches /a. /a/c matches /a.
     *
     *  @param uri - URI to match.
     *  @param handler - handler instance to use.  This will be free'ed
     *      when the server closes and instances cannot be reused.
     */
    void addHandler(const std::string &uri, CivetHandler *handler);

    /**
     * removeHandler(const std::string &)
     *
     * Removes a handler, deleting it if found.
     *
     * @param - the exact URL used in addHandler().
     */
    void removeHandler(const std::string &uri);

    /**
     * getHandler(const std::string &uri)
     *
     * @param uri - the URI
     * @returns the handler that matches the requested URI or 0 if none were found.
     */
    CivetHandler *getHandler(const std::string &uri) const {
        return getHandler(uri.data(), uri.length());
    }

    /**
     * getHandler(const char *uri, unsigned urilen)
     *
     * @param uri - the URI
     * @param urilen - the length of the URI
     * @returns the handler that matches the requested URI or 0 if none were found.
     */
    CivetHandler *getHandler(const char *uri, unsigned urilen) const;
	
	/**
	 * getCookie(struct mg_connection *conn, const std::string &cookieName, std::string &cookieValue)
	 * @param conn - the connection information 
	 * @param cookieName - cookie name to get the value from
	 * @param cookieValue - cookie value is returned using thiis reference
	 * @puts the cookie value string that matches the cookie name in the _cookieValue string.
	 * @returns the size of the cookie value string read.
	*/
	int getCookie(struct mg_connection *conn, const std::string &cookieName, std::string &cookieValue);

	/**
	 * getHeader(struct mg_connection *conn, const std::string &headerName)
	 * @param conn - the connection information 
	 * @param headerName - header name to get the value from
	 * @returns a char array whcih contains the header value as string
	*/
	const char* getHeader(struct mg_connection *conn, const std::string &headerName);
protected:

    /**
     * handleRequest(struct mg_connection *)
     *
     * Handles the incomming request.
     *
     * @param conn - the connection information
     * @returns true if implemented, false otherwise
     */
    virtual bool handleRequest(struct mg_connection *conn);

    /**
     *  Returns the index of the handler that matches the
     *  URI exactly.
     *
     *  @param uri - the url to match
     */
    int getIndex(const std::string &uri) const;

    std::vector<std::string> uris;
    std::vector<CivetHandler *> handlers;
    struct mg_context *context;

private:
    static int begin_request_callback(struct mg_connection *conn);

};

#endif /*  __cplusplus */
#endif /* _CIVETWEB_SERVER_H_ */
