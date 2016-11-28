# Civetweb API Reference

### `mg_connect_client( host, port, use_ssl, error_buffer, error_buffer_size );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`host`**|`const char *`|hostname or IP address of the server|
|**`port`**|`int`|The port to connect to on the server|
|**`use_ssl`**|`int`|Connects using SSL of this value is not zero|
|**`error_buffer`**|`char *`|Buffer to store an error message|
|**`error_buffer_size`**|`size_t`|Maximum size of the error buffer including the NUL terminator|

### Return Value

| Type | Description |
| :--- | :--- |
|`struct mg_connection *`||

### Description

The function `mg_connect_client()` connects to a TCP server as a client. This server
can be a HTTP server but this is not necessary. The function returns a poinyer to
a connection structure when the connection is established, and NULL otherwise.

### See Also

* [`mg_connect_client_secure();`](mg_connect_client_secure.md)
* [`mg_connect_websocket_client();`](mg_connect_websocket_client.md)
