# Civetweb API Reference

### `mg_connect_websocket_client2( client_options, use_ssl, error_buffer, error_buffer_size, path, origin, data_func, close_func, user-data);`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`client_options`**|`const struct mg_client_options *`|The client connection opions including mandatory hostname/IP address of server and port on server
|**`use_ssl`**|`int`|Use SSL if this parameter is not equal to zero|
|**`error_buffer`**|`char *`|Buffer to store an error message|
|**`error_buffer_size`**|`size_t`|Size of the error message buffer including the NUL terminator|
|**`path`**|`const char *`|The server path to connect to, for example `/app` if you want to connect to `localhost/app`|
|**`origin`**|`const char *`|The value of the `Origin` HTTP header|
|**`data_func`**|`mg_websocket_data_handler`|Callback which is used to process data coming back from the server|
|**`close_func`**|`mg_websocket_close_handler`|Callback which is called when the connection is to be closed|
|**`user_data`**|`void *`|User supplied argument|

### Return Value

| Type | Description |
| :--- | :--- |
|`struct mg_connection *`|A pointer to the connection structure, or NULL if connecting failed|

### Description

The `mg_connect_websocket_client2()` works exactly like [`mg_connect_websocket_client()`](mg_connect_websocket_client.md) but allows to set additional client options, like client or server SSL/TLS certificates or request timeout.  See the [`mg_connect_websocket_client()`](mg_connect_websocket_client.md) for description.

### See Also

* [`mg_check_feature();`](mg_check_feature.md)
* [`mg_connect_client();`](mg_connect_client.md)
* [`mg_connect_client_secure();`](mg_connect_client_secure.md)
* [`mg_connect_websocket_client();`](mg_connect_websocket_client.md)
