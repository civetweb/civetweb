# Civetweb API Reference

### `mg_connect_websocket_client( host, port, use_ssl, error_buffer, error_buffer_size, path, origin, data_func, close_func, user-data);`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`host`**|`const char *`||
|**`port`**|`int`||
|**`use_ssl`**|`int`||
|**`error_buffer`**|`char *`||
|**`error_buffer_size`**|`size_t`||
|**`path`**|`const char *`||
|**`origin`**|`const char *`||
|**`data_func`**|`mg_websocket_data_handler`||
|**`close_func`**|`mg_websocket_close_handler`||
|**`user_data`**|`void *`||

### Return Value

| Type | Description |
| :--- | :--- |
|`struct mg_connection *`||

### Description

### See Also

* [`mg_connect_client();`](mg_connect_client.md)
* [`mg_connect_client_secure();`](mg_connect_client_secure.md)
