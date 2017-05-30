# Civetweb API Reference

### `mg_set_user_connection_data( conn, data );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`|connection to add the user data|
|**`data`**|`void *`|Pointer to the user data|

### Return Value

*none*

### Description

The function `mg_set_user_connection_data()` can be used to add or change the user data pointer attached to a connection. Using the value NULL in the call will remove a previously assigned user data pointer.

### See Also

* [`mg_get_user_connection_data();`](mg_user_connection_data.md)
