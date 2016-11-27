# Civetweb API Reference

### `mg_connect_client( host, port, use_ssl, error_buffer, error_buffer_size );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`host`**|`const char *`||
|**`port`**|`int`||
|**`use_ssl`**|`int`||
|**`error_buffer`**|`char *`||
|**`error_buffer_size`**|`size_t`||

### Return Value

| Type | Description |
| :--- | :--- |
|`struct mg_connection *`||

### Description

### See Also

* [`mg_connect_client_secure();`](mg_connect_client_secure.md)
* [`mg_connect_websocket_client();`](mg_connect_websocket_client.md)
