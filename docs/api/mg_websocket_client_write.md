# Civetweb API Reference

### `mg_websocket_client_write( conn, opcode, data, data_len );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`||
|**`opcode`**|`int`||
|**`data const`**|`char *`||
|**`data_len`**|`size_t`||

### Return Value

| Type | Description |
| :--- | :--- |
|`int`||

### Description

### See Also

* [`mg_lock_connection();`](mg_lock_connection.md)
* [`mg_printf();`](mg_printf.md)
* [`mg_unlock_connection();`](mg_unlock_connection.md)
* [`mg_websocket_write();`](mg_websocket_write.md)
* [`mg_write();`](mg_write.md)
