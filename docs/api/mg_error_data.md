# Civetweb API Reference

### `struct mg_error_data;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`code`**|`unsigned *`| A pointer to an `unsigned` variable, to store the error code. |
|**`text`**|`char *`| A text buffer to store the error text. |
|**`text_buffer_size`**|`size_t`| Size of the text buffer. |

### Description

The structure `mg_error_data` is used in [`mg_start2()`](mg_start.md), [`mg_start_domain2();`](mg_start_domain2.md), [`mg_connect_client2();`](mg_connect_client2.md) and [`mg_get_response2();`](mg_get_response2.md) to return error information.

### See Also

* [`mg_start2();`](mg_start2.md)
* [`mg_start_domain2();`](mg_start_domain2.md)
* [`mg_connect_client2();`](mg_connect_client2.md)
* [`mg_get_response2();`](mg_get_response2.md)
* [`struct mg_init_data;`](mg_init_data.md)
