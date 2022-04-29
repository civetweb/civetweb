# Civetweb API Reference

### `struct mg_websocket_subprotocols;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`nb_subprotocols`**|`int`| Number of websocket sub-protocol names following (>=0). |
|**`subprotocols`**|`const char **`| Array of websocket sub-protocol names (nb_subprotocols elements). |

### Description

The structure `mg_websocket_subprotocols` is used to specify websocket sub-protocols supported by a websocket handler function.

### See Also

* [`mg_set_websocket_handler_with_subprotocols();`](mg_set_websocket_handler_with_subprotocols.md)
