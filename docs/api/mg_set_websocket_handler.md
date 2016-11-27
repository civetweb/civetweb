# Civetweb API Reference

### `mg_set_websocket_handler( ctx, uri, connect_handler, ready_handler, data_handler, close_handler, cbdata );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`ctx`**|`mg_context *`||
|**`uri`**|`const char *`||
|**`connect_handler`**|`mg_websocket_connect_handler`||
|**`ready_handler`**|`mg_websocket_ready_handler`||
|**`data_handler`**|`mg_websocket_data_handler`||
|**`close_handler`**|`mg_websocket_close_handler`||
|**`cbdata`**|`void *`||

### Return Value

*none*

### Description

### See Also
