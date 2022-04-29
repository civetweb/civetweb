# Civetweb API Reference

### `struct mg_init_data;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`callbacks`**|`const struct mg_callbacks *`| A structure with optional callback functions to process requests from the web server |
|**`user_data`**|`void *`| A pointer to optional user data |
|**`configuration_options`**|`const char **`| A list of options used to initialize the web server. The list consists of an NULL terminated list of option-value string pairs. |

### Description

The structure `mg_init_data` is used in [`mg_start2()`](mg_start.md) and [`mg_connect_client2();`](mg_connect_client2.md).
It holds the same parameters supplied to [`mg_start();`](mg_start.md) as individual arguments in one structure.

### See Also

* [`struct mg_callbacks;`](mg_callbacks.md)
* [`mg_start();`](mg_start.md)
* [`mg_start2();`](mg_start2.md)
* [`mg_connect_client2();`](mg_connect_client2.md)
* [`struct mg_error_data;`](mg_error_data.md)
