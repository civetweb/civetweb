# Civetweb API Reference

### `mg_set_websocket_handler_with_subprotocols( ctx, uri, subprotocols, connect_handler, ready_handler, data_handler, close_handler, cbdata );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`ctx`**|`mg_context *`|The context in which to add the handlers|
|**`uri`**|`const char *`|The URI for which the handlers should be activated|
|**`subprotocols`**|`struct mg_websocket_subprotocols *`|A list of supported sub-protocols|
|**`connect_handler`**|`mg_websocket_connect_handler`|Handler called when a connect is signaled|
|**`ready_handler`**|`mg_websocket_ready_handler`|Handler called when the connection is ready|
|**`data_handler`**|`mg_websocket_data_handler`|Handler called when data is received|
|**`close_handler`**|`mg_websocket_close_handler`|Handler called when the connection closes|
|**`cbdata`**|`void *`|User defined data|

`int mg_websocket_connect_handler( const struct mg_connection *conn, void *cbdata );`
`int mg_websocket_ready_handler( struct mg_connection *conn, void *cbdata );`
`int mg_websocket_data_handler( struct mg_connection *conn, int opcode, char * buf, size_t buf_len, void *cbdata );`
`int mg_websocket_close_handler( const struct mg_connection *conn,  void *cbdata );`

### Return Value

*none*

### Description

The function `mg_set_websocket_handler_with_subprotocols()` connects callback functions to a websocket URI, just like [`mg_set_websocket_handler();`](mg_set_websocket_handler.md). 
In addition, it allows to specify websocket sub-protocols.
The callback functions are called when a state change is detected on the URI like an incoming connection or data received from a remote peer.

### See Also

* [`struct mg_websocket_subprotocols;`](api/mg_websocket_subprotocols.md)
* [`mg_set_websocket_handler();`](mg_set_websocket_handler.md)
