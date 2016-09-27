# CivetWeb API Reference

CivetWeb is often used as HTTP and HTTPS library inside a larger application. An API is available to integrate the CivetWeb functionality in a larger codebase. This document describes the API. Basic usage examples of the API can be found in [Embedding.md](Embedding.md).

## Structures

### `struct client_cert;`

#### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`subject`**|`const char *`| The subject of the certificate |
|**`issuer`**|`const char *`| The issuer of the certificate |
|**`serial`**|`const char *`| The serial number of the certificate |
|**`finger`**|`const char *`| The fingerprint of the certificate |

#### Description

The structure `client_cert` is used as a sub-structure in the `mg_request_info` structure to store information of an optional client supplied certificate.

### `struct mg_callbacks;`

#### Fields

| Field | Description |
| :--- | :--- | :--- | :--- |
|**`begin_request`**|**`int begin_request( struct mg_connection * conn );`**|
| |The `begin_request()` callback function is called when CivetWeb has received a new HTTP request. If the callback function does not process the request, it should return 0. In that case CivetWeb will handle the request with the default callback routine. If the callback function returns a value between 1 and 999, CivetWeb does nothing and the callback function should do all the processing, including sending the proper HTTP headers etc. Starting at CivetWeb version 1.7, the function `begin_request()` is called before any authorization is done. If an authorization check is required, `request_handler()` should be used instead. The return value of the callback function is not only used to signal CivetWeb to not further process the request. The returned value is also stored as HTTP status code in the access log. |
|**`end_request`**|**`void end_request( const struct mg_connection * conn );`**|
| |The callback function `end_request()` is called by CivetWeb when a request has been completely processed. It sends the reply status code which was sent to the client to the application.|
|**`log_message`**|**`int log_message( const struct mg_connection *conn, const char *message );`**|
| |The callback function `log_message()` is called when CivetWeb is about to log a message. If the callback function returns 0, CivetWeb will use the default internal log routines to log the message. If a non-zero value is returned CivetWeb assumes that logging has already been done and no further action is performed.|
|**`log_access`**|**`int log_access( const struct mg_connection *conn, const char *message );`**|
| |The callback function `log_access()` is called when CivetWeb is about to log a message. If the callback function returns 0, CivetWeb will use the default internal access log routines to log the access. If a non-zero value is returned, CivetWeb assumes that access logging has already been done and no further action is performed.|
|**`init_ssl`**|**`int init_ssl( void *ssl_context, void *user_data );`**|
| |The callback function `init_ssl()` is called when CivetWeb initializes the SSL library. The parameter `user_data` contains a pointer to the data which was provided to `mg_start()` when the server was started. The callback function can return 0 to signal that CivetWeb should setup the SSL certificate. With a return value of 1 the callback function signals CivetWeb that the certificate has already been setup and no further processing is necessary. The value -1 should be returned when the SSL initialization fails.|
|~~`websocket_connect`~~|**`int websocket_connect( const struct mg_connection *conn );`**|
| |*Deprecated. Use* `mg_set_websocket_handler()` *instead.*|
|~~`websocket_ready`~~|**`int websocket_ready( struct mg_connection *conn );`**|
| |*Deprecated. Use* `mg_set_websocket_handler()` *instead.*|
|~~`websocket_data`~~|**`int websocket_data( struct mg_connection *conn, int bits, char *data, size_t data_len );`**|
| |*Deprecated. Use* `mg_set_websocket_handler()` *instead.*|
|**`connection_close`**|**`void connection_close( const struct mg_connection *conn );`**|
| |The callback function `connection_close()` is called when CivetWeb is closing a connection. The per-context mutex is locked when the callback function is invoked. The function is primarly useful for noting when a websocket is closing and removing it from any application-maintained list of clients. *Using this callback for websocket connections is deprecated. Use* `mg_set_websocket_handler()` *instead.*|
|**`open_file`**|**`const char *open_file( const struct mg_connection *conn, const char *path, size_t *data_len );`**|
| |The callback function `open_file()` is called when a file is to be opened by CivetWeb. The callback can return a pointer to a memory location and set the memory block size in the variable pointed to by `data_len` to signal CivetWeb that the file should not be loaded from disk, but that instead a cached version in memory should be used. If the callback function returns NULL, CivetWeb will open the file from disk. This callback allows caching to be implemented.|
|**`init_lua`**|**`void init_lua( const struct mg_connection *conn, void *lua_context );`**|
| |The callback function `init_lua()` is called just before a Lua server page is to be served. Lua page serving must have been enabled at compile time for this callback function to be called. The parameter `lua_context` is a `lua_State *` pointer.|
|~~`upload`~~|**`void upload( struct mg_connection * conn, const char *file_name );`**|
| |*Deprecated. Use* `mg_handle_form_request()` *instead.*|
|**`http_error`**|**`int http_error( struct mg_connection *conn, int status );`**|
| |The callback function `http_error()` is called by CivetWeb just before an HTTP error is to be sent to the client. The function allows the application to send a custom error page. The status code of the error is provided as a parameter. If the application sends their own error page, it must return 1 to signal CivetWeb that no further processing is needed. If the returned value is 0, CivetWeb will send a built-in error page to the client.|
|**`init_context`**|**`void init_context( const struct mg_context *ctx );`**|
| |The callback function `init_context()` is called after the CivetWeb server has been started and initialized, but before any requests are served. This allowes the application to perform some initialization activities before the first requests are handled.|
|**`init_thread`**|**`void init_thread( const struct mg_context *ctx, int thread_type );`**|
| |The callback function `init_thread()` is called when a new thread is created by CivetWeb. The `thread_type` parameter indicates which type of thread has been created. following thread types are recognized:|
| |**0** The master thread is created |
| |**1** A worker thread which handles client connections has been created|
| |**2** An internal helper thread (timer thread) has been created|
|**`exit_context`**|**void exit_context( const struct mg_context *ctx );`**|
| |The callback function `exit_context()` is called by CivetWeb when the server is stopped. It allows the application to do some cleanup on the application side.|


#### Description

A pointer to a `mg_callbacks` structure is passed as parameter to the `mg_start()` function to provide links to callback functions which the webserver will call at specific events. If a specific callback function is not supplied, CivetWeb will fallback to default internal callback routines. Callback functions give the application detailed control over how specific events should be handled.

### `struct mg_header;`

#### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`name`**|`const char *`| The name of the client request header |
|**`value`**|`const char *`| The value of the client request header |

#### Description

The structure `mg_header` is used as a sub-structure in the `mg_request_info` structure to store the name and value of one HTTP request header as sent by the client.

### `struct mg_request_info;`

#### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`request_method`**|`const char *`| The request method used by the client for the connection this can be **GET**, **POST** or one of the other common HTTP request methods |
|**`request_uri`**|`const char *`| The absolute or URL-encoded URI as it was sent in the request |
|**`local_uri`**|`const char *`| The relative URL-encoded URI as it references the local resource. If the request URI does not reference a resource on the local server, this field is NULL |
|~~`uri`~~|`const char *`| *Deprecated. Use* `local_uri` *instead* |
|**`http_version`**|`const char *`| The HTTP version as mentioned in the client request. This can be "1.0", "1.1", etc. |
|**`remote_user`**|`const char *`| The name of the authenticated remote user, or NULL if no authentication was used |
|**`remote addr`**|`char[48]`| The IP address of the remote client as a string. This can either represent an IPv4 or an IPv6 address. |
|~~`remote_ip`~~|`long`| *Deprecated. Use* `remote_addr` *instead* |
|**`content_length`**|`long long`| The content length of the request body. This value can be -1 if no content length was provided. |
|**`remote_port`**|`int`| The port number at the client's side |
|**`is_ssl`**|`int`| 1 if the connection is over SSL, and 0 if it is a plain connection |
|**`user_data`**|`void *`| A pointer to the `user_data` information which was provided as a parameter to `mg_start()`. |
|**`conn_data`**|`void *`| A pointer to connection specific user data |
|**`num_headers`**|`int`| The number of HTTP request headers sent by the client |
|**`http_headers`**|`struct mg_header[64]`| Array of structures with the HTTP request headers sent by the client |
|**`client_cert`**|`struct client_cert *`| Pointer to the client certificate information, when available |

#### Description

The `mg_request_info` structure contains the client information of an existing connection.

## Functions

### `mg_check_feature( feature );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`feature`**|`unsigned`| A value indicating the feature to be checked |

#### Returns

| Type | Description |
| :--- | :--- |
|`unsigned`| A value indicating if a feature is available. A positive value indicates available, while 0 is returned for an unavailable feature |

#### Description

The function `mg_check_feature()` can be called from an application program to check of specific features have been compiled in the civetweb version which the application has been linked to. The feature to check is provided as an unsigned integer parameter. If the function is available in the currently linked library version, a value > 0 is returned. Otherwise the function mg_check_feature() returns the value 0.

The following parameter values can be used:

| Value | Compilation option | Description |
| :---: | :---: | :--- |
| **1** | NO_FILES | *Able to serve files*.  If this feature is available, the webserver is able to serve files directly from a directory tree. |
| **2** | NO_SSL | *Support for HTTPS*. If this feature is available, the webserver van use encryption in the client-server connection. SSLv2, SSLv3, TLSv1.0, TLSv1.1 and TLSv1.2 are supported depending on the SSL library CivetWeb has been compiled with, but which protocols are used effectively when the server is running is dependent on the options used when the server is started. |
| **4** | NO_CGI | *Support for CGI*. If this feature is available, external CGI scripts can be called by the webserver. |
| **8** | USE_IPV6 | *Support IPv6*. The CivetWeb library is capable of communicating over both IPv4 and IPv6, but IPv6 support is only available if it has been enabled at compile time. |
| **16** | USE_WEBSOCKET | Support for web sockets. WebSockets support is available in the CivetWeb library if the proper options has been used during cimpile time. |
| **32** | USE_LUA | *Support for Lua scripts and Lua server pages*. CivetWeb supports server side scripting through the Lua language, if that has been enabled at compile time. Lua is an efficient scripting language which is less resource heavy than for example PHP. |
| **64** | USE_DUKTAPE | *Support for server side JavaScript*. Server side JavaScript can be used for dynamic page generation if the proper options have been set at compile time. Please note that client side JavaScript execution is always available if it has been enabled in the connecting browser. |
| **128** | NO_CACHING | *Support for caching*. The webserver will support caching, if it has not been disabled while compiling the library. |

Parameter values other than the values mentioned above will give undefined results.

### `mg_get_header( conn, name );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`| A pointer referencing the connection |
|**`name`**|`const char *`| The name of the request header |

#### Returns

| Type | Description |
| :--- | :--- |
|`const char *`| A pointer to the value of the request header, or NULL of no matching header count be found |

#### Description

HTTP and HTTPS clients can send request headers to the server to provide details about the communication. These request headers can for example specify the preferred language in which the server should respond and the supported compression algorithms. The function `mg_get_header()` can be called to return the contents of a specific request header. The function will return a pointer to the value text of the header when succesful, and NULL of no matching request header from the client could be found.

### `mg_get_option( ctx, name );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`ctx`**|`const struct mg_context *`| A pointer to the webserver context |
|**`name`**|`const char *`| The name of the option to query |

#### Returns

| Type | Description |
| :--- | :--- |
|`const char *`| A pointer to the option value in text, or NULL if an error occured |

#### Description

When starting the CivetWeb webserver, options are provided to set the wanted behaviour of the server. The options which were used during startup can be queried through the `mg_get_option()` function. Options are read-only and cannot be changed while the webserver is running. The function returns a pointer to a text string containing the value of the queried option, or NULL if an error occured. It is guaranteed however that if a valid option name is provided as a parameter to this function, that a pointer to a string is returned and not NULL. In case an option was empty or NULL during initialisation, `mg_get_option()` will return a pointer to an empty string.


### `mg_get_response_code_text( conn, response_code );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`| A pointer referencing the connection |
|**`response_code`**|`int`| Response code for which the text is queried |

#### Returns

| Type | Description |
| :--- | :--- |
|`const char *`| A pointer to a human readable text explaining the response code. |

#### Description

The function `mg_get_response_code_text()` returns a pointer to a human readable text describing the HTTP response code which was provided as a parameter.


### `mg_read( conn, buf, len );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`| A pointer referencing the connection |
|**`buf`**|`void *`| A pointer to the location where the received data can be stored |
|**`len`**|`size_t`| The maximum number of bytes to be stored in the buffer |

#### Returns

| Type | Description |
| :--- | :--- |
|`int`| The number of read bytes, or a status indication |

#### Description

The function `mg_read()` receives data over an existing connection. The data is handled as binary and is stored in a buffer whose address has been provided as a parameter. The function returns the number of read bytes when successful, the value 0 when the connection has been closed by peer and a negative value when no more data could be read from the connection.

### `mg_start( callbacks, user_data, options );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`callbacks`**|`const struct mg_callbacks *`| A structure with optional callback functions to process requests from the web server |
|**`user_data`**|`void *`| A pointer to optional user data |
|**`options`**|`char **`| A list of options used to initialize the web server |

#### Returns

| Type | Description |
| :--- | :--- |
|`struct mg_context *`| A pointer to a context structure when successful, or NULL in case of failure |

#### Description

The function `mg_start()` is the only function needed to call to initialize the webserver. After the function returns and a pointer to a contect structure is provided, it is guaranteed that the server has started and is listening on the designated ports. In case of failure a NULL pointer is returned. The behaviour of the web server is controlled by a list of callback functions and a list of options. The callback functions can do application specific processing of events which are encountered by the webserver. If a specific callback function is not provided, the webserver uses their default callback routines. The options list controls how the webserver should be started and contains settings for for example the ports to listen on, the maximum number of threads created to handle requests in parallel and if settings for SSL encryption.

### `mg_stop( ctx );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`ctx`**|**`struct mg_context *`**| A pointer to the current webserver context |

#### Returns

*nothing*

#### Description

The function `mg_stop()` is used to stop and cleanup a running webserver. A pointer to the context of the running webserver is provided as a parameter. The execution of this function may take some time because it waits until all threads have stopped and returns all memory to the heap. After the function returns, the location the context pointer points to is invalid. The function does not return a return value and it is therefore not possible to know if stopping the webserver succeeded or not.

### `mg_version();`

#### Parameters

*none*

#### Returns

| Type | Description |
| :--- | :--- |
|`const char *`| A pointer to a text with the current CivetWeb version |

#### Description

The function `mg_version()` can be used to return the current CivetWeb version. function returns a pointer to a string with the current major and minor version number, separated with a dot. For example "1.9".


### `mg_write( conn, buf, len );`

#### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`| A pointer to the connection to be used to send data |
|**`buf`**|`const void *`| A pointer to the blob of information to be sent |
|**`len`**|`size_t`| The amount of bytes to be sent |

#### Returns

| Type | Description |
| :--- | :--- |
|`int`| An integer indicating the amount of bytes sent, or failure |

#### Description

The function `mg_write()` can be used to send a blob of arbitrary data over a connection. The size of the data is provided as a parameter. The only length limitation on this function is MAX_INT, because the return value of this function will turn negative with larger blocks of data, although they may have been sent correctly. The function returns the amount of bytes sent in case of success, the value 0 when the connection has been closed, and -1 in case of an error.

