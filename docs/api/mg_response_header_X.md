# Civetweb API Reference

### `mg_response_header_start( conn, status );`
### `mg_response_header_add( conn, header, value, value_len );`
### `mg_response_header_add_lines( conn, http1_headers );`
### `mg_response_header_send( conn );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`|The server is sending a response to this connection|
|**`status`**|`int`|HTTP status code to send (100-599)|
|**`header`**|`const char *`|HTTP header name|
|**`value`**|`const char *`|HTTP header value|
|**`value_len`**|`int`|Length of HTTP header value. -1 will use the entire value string.|
|**`http1_headers`**|`const char *`|String in format "Key: Value\r\nKey2: Value2"|

### Return Value

| Type | Description |
|`int`| An integer indicating success (>=0) or failure (<0). |
|`-1`| Error: Invalid parameter (e.g., NULL pointer or value out of range) |
|`-2`| Error: Invalid connection type. These functions must only be used in web server request handlers. |
|`-3`| Error: Invalid connection status. These functions must be used before `mg_send()` or `mg_printf()` functions. |
|`-4`| Error: Too many headers. The total number of header lines is limited to MG_MAX_HEADERS (default: 64). |
|`-5`| Error: Out of memory. |


### Description

The `mg_response_header_*` family of functions can be used to send HTTP response headers from a web server handler function.
Do not use it for HTTP clients or websockets. When using `mg_response_header_*` functions, they must be used to send all HTTP headers - combining `mg_response_header_*` functions and `mg_send_*`/`mg_write`/`mg_printf` to send HTTP headers is not possible.
After sending HTTP headers, HTTP body data is still sent using `mg_write`/`mg_printf`.

All `mg_response_header_*` functions must be used prior to any call to `mg_write`/`mg_printf`, in the following order:

1) `mg_response_header_start` (once)
2) `mg_response_header_add` (0 - 64 times), alternatively `mg_response_header_add_lines`. The preferred function is `mg_response_header_add`. The function `mg_response_header_add_lines` can process HTTP/1.x formatted header lines and is used for compatibility.
3) `mg_response_header_send` (once)
4) `mg_write` or `mg_printf` (multiple) to send the HTTP body

Using `mg_response_header_*` functions will allow a request handler to process HTTP/1.x and HTTP/2 requests, in contrast to sending HTTP headers directly using `mg_printf`/`mg_write`.


### See Also

* [`mg_send_http_ok();`](mg_send_http_ok.md)
* [`mg_send_http_error();`](mg_send_http_error.md)
* [`mg_send_http_redirect();`](mg_send_http_redirect.md)
* [`mg_write();`](mg_write.md)
* [`mg_send_chunk();`](mg_send_chunk.md)

