# Civetweb API Reference

### `mg_connect_client_secure( client_options, error_buffer, error_buffer_size );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`client_options`**|`const struct mg_client_options *`||
|**`error_buffer`**|`char *`||
|**`error_buffer_size`**|`size_t`||

### Return Value

| Type | Description |
| :--- | :--- |
|`struct mg_connection *`||

### Description

### See Also

* [`struct mg_client_options;`](mg_client_options.md)
* [`mg_connect_client();`](mg_connect_client.md)
* [`mg_connect_websocket_client();`](mg_connect_websocket_client.md)
