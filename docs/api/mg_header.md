# Civetweb API Reference

### `struct mg_header;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`name`**|`const char *`| The name of the client request header |
|**`value`**|`const char *`| The value of the client request header |

### Description

The structure `mg_header` is used as a sub-structure in the `mg_request_info` structure to
store the name and value of one HTTP request header as sent by the client.

### See Also
