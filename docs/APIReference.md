# CivetWeb API Reference

CivetWeb is often used as HTTP and HTTPS library inside a larger application. An API is available to integrate the CivetWeb functionality in a larger codebase. This document describes the API. Basic usage examples of the API can be found in [Embedding.md](Embedding.md).

## Structures

Work in progress...

## Functions

### `mg_check_feature( feature )`

#### Parameters

| Type | Parameter | Description |
| :--- | :--- | :--- |
| `unsigned` | `feature` | A value indicating the feature to be checked |

#### Returns

| Type | Description |
| :--- | :--- |
| `unsigned` | A value indicating if a feature is available. A positive value indicates available, while 0 is returned for an unavailable feature |

##### Description

The function `mg_check_feature()` can be called from an application program to check of specific features have been compiled in the civetweb version which the application has been linked to. The feature to check is provided as an unsigned integer parameter. If the function is available in the currently linked library version, a value > 0 is returned. Otherwise the function mg_check_feature() returns the value 0.

The following parameter values can be used:

| Value | Compilation option | Description |
| :---: | :--- | :--- |
| **1** | NO_FILES | *Able to serve files*.  If this feature is available, the webserver is able to serve files directly from a directory tree. |
| **2** | NO_SSL | *Support for HTTPS*. If this feature is available, the webserver van use encryption in the client-server connection. SSLv2, SSLv3, TLSv1.0, TLSv1.1 and TLSv1.2 are supported depending on the SSL library CivetWeb has been compiled with, but which protocols are used effectively when the server is running is dependent on the options used when the server is started. |
| **4** | NO_CGI | *Support for CGI*. If this feature is available, external CGI scripts can be called by the webserver. |
| **8** | USE_IPV6 | *Support IPv6*. The CivetWeb library is capable of communicating over both IPv4 and IPv6, but IPv6 support is only available if it has been enabled at compile time. |
| **16** | USE_WEBSOCKET | Support for web sockets. WebSockets support is available in the CivetWeb library if the proper options has been used during cimpile time. |
| **32** | USE_LUA | *Support for Lua scripts and Lua server pages*. CivetWeb supports server side scripting through the Lua language, if that has been enabled at compile time. Lua is an efficient scripting language which is less resource heavy than for example PHP. |
| **64** | USE_DUKTAPE | *Support for server side JavaScript*. Server side JavaScript can be used for dynamic page generation if the proper options have been set at compile time. Please note that client side JavaScript execution is always available if it has been enabled in the connecting browser. |
| **128** | NO_CACHING | *Support for caching*. The webserver will support caching, if it has not been disabled while compiling the library. |

Parameter values other than the values mentioned above will give undefined results.

### `mg_get_header( conn, name )`

#### Parameters

  - `struct mg_connection *conn` - a pointer referencing the connection
  - `const char *name` - the name of the request header

#### Returns

  - `const char *` - a pointer to the value of the request header, or NULL of no matching header count be found

##### Description

HTTP and HTTPS clients can send request headers to the server to provide details about the communication. These request headers can for example specify the preferred language in which the server should respond and the supported compression algorithms. The function `mg_get_header()` can be called to return the contents of a specific request header. The function will return a pointer to the value text of the header when succesful, and NULL of no matching request header from the client could be found.

### `mg_get_option( context, name )`

#### Parameters

  - `const struct mg_context *` - a pointer to the webserver contect
  - `const char *name` - the name of the option to query

#### Returns

  - `const char *` - a pointer to the option value in text, or NULL if an error occured

##### Description

When starting the CivetWeb webserver, options are provided to set the wanted behaviour of the server. The options which were used during startup can be queried through the `mg_get_option()` function. Options are read-only and cannot be changed while the webserver is running. The function returns a pointer to a text string containing the value of the queried option, or NULL if an error occured. It is guaranteed however that if a valid option name is provided as a parameter to this function, that a pointer to a string is returned and not NULL. In case an option was empty or NULL during initialisation, `mg_get_option()` will return a pointer to an empty string.


### `mg_get_response_code_text( conn, response_code )`

#### Parameters

  - `struct mg_connection *conn` - a pointer referencing the connection
  - `int response_code` - response code who's text is queried

#### Returns

  - `const char *` - a pointer to a human readable text explaining the response code

##### Description

The function `mg_get_response_code_text()` returns a pointer to a human readable text describing the HTTP response code which was provided as a parameter.


### `mg_read( conn, buf, len )`

#### Parametera

  - `struct mg_connection *conn` - a pointer referencing the connection
  - `void *buf` - a pointer to the location where the received data can be stored
  - `size_t len` - the maximum number of bytes to be stored in the buffer

#### Returns

  - `int` - The number of read bytes, or a status indication

##### Description

The function `mg_read()` receives data over an existing connection. The data is handled as binary and is stored in a buffer whose address has been provided as a parameter. The function returns the number of read bytes when successful, the value 0 when the connection has been closed by peer and a negative value when no more data could be read from the connection.

### `mg_start( callbacks, user_data, options )`

#### Parameters

  - `const struct mg_callbacks *callbacks` - a structure with optional callback functions to process requests from the web server.
  - `void *user_data` - a pointer to optional user data
  - `char **options` - a list of options used to initialize the web server

#### Returns

  - `struct mg_context *` - a pointer to a context structure when successful, or NULL in case of failure

##### Description

The function `mg_start()` is the only function needed to call to initialize the webserver. After the function returns and a pointer to a contect structure is provided, it is guaranteed that the server has started and is listening on the designated ports. In case of failure a NULL pointer is returned. The behaviour of the web server is controlled by a list of callback functions and a list of options. The callback functions can do application specific processing of events which are encountered by the webserver. If a specific callback function is not provided, the webserver uses their default callback routines. The options list controls how the webserver should be started and contains settings for for example the ports to listen on, the maximum number of threads created to handle requests in parallel and if settings for SSL encryption.

### `mg_stop( context )`

#### Parameters

  - `struct mg_context *context` - a pointer to the current webserver context

#### Returns

Nothing

#### Description

The function `mg_stop()` is used to stop and cleanup a running webserver. A pointer to the context of the running webserver is provided as a parameter. The execution of this function may take some time because it waits until all threads have stopped and returns all memory to the heap. After the function returns, the location the context pointer points to is invalid. The function does not return a return value and it is therefore not possible to know if stopping the webserver succeeded or not.

### `mg_version()`

#### Parameters

None

#### Returns

 - `const char *` - a pointer to a text with the current CivetWeb version

##### Description

The function `mg_version()` can be used to return the current CivetWeb version. function returns a pointer to a string with the current major and minor version number, separated with a dot. For example "1.9".


### `mg_write( conn, buf, len )`

#### Parameters

  - `struct mg_connection *conn` - a pointer to the connection to be used to send data
  - `const void *buf` - a pointer to the blob of information to be sent
  - `size_t len` - the amount of bytes to be sent

#### Returns

  - `int` - An integer indicating the amount of bytes sent, or failure

#### Description

The function `mg_write()` can be used to send a blob of arbitrary data over a connection. The size of the data is provided as a parameter. The only length limitation on this function is MAX_INT, because the return value of this function will turn negative with larger blocks of data, although they may have been sent correctly. The function returns the amount of bytes sent in case of success, the value 0 when the connection has been closed, and -1 in case of an error.

