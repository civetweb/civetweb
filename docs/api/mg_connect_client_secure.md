# Civetweb API Reference

### `mg_connect_client_secure( client_options, error_buffer, error_buffer_size );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`client_options`**|`const struct mg_client_options *`|Settings about the server connection|
|**`error_buffer`**|`char *`|Buffer to store an error message|
|**`error_buffer_size`**|`size_t`|Size of the error message buffer including the NUL terminator|

### Return Value

| Type | Description |
| :--- | :--- |
|`struct mg_connection *`||

### Description

The function `mg_connect_client_secure()` creates a secure connection with a server. The
information about the connection and server is passed in a structure and an error message
may be returned in a local buffer. The function returns a pointer to an `struct mg_connection`
structure when successful and NULL otherwise.

### See Also

* [`struct mg_client_options;`](mg_client_options.md)
* [`mg_connect_client();`](mg_connect_client.md)
* [`mg_connect_websocket_client();`](mg_connect_websocket_client.md)
