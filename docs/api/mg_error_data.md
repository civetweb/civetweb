# Civetweb API Reference

### `struct mg_error_data;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`code`**|`unsigned *`| Error code (see `MG_ERROR_DATA_CODE_*`). |
|**`code_sub`**|`unsigned *`| Error sub code, depending on error code. |
|**`text`**|`char *`| A text buffer to store the error text. |
|**`text_buffer_size`**|`size_t`| Size of the text buffer. |

### Description

The structure `mg_error_data` is used to return error information.
The `code` number will be set to one of the `MG_ERROR_DATA_CODE_*` values defined in civetweb.h.

The meaning of the `code_sub` number will depend on the value of `code`.
The `code_sub` member is experimental and may change in future versions.

The optional pointer `text` can be used to provide storage for a textual error message.
The size of the provided `text` pointer must be set in `text_buffer_size`.
If no textual error message is required and no buffer is probided, `text_buffer_size` must be set to 0.

Currently `struct mg_error_data` is used by the functions [`mg_start2()`](mg_start.md), [`mg_start_domain2();`](mg_start_domain2.md), [`mg_connect_client2();`](mg_connect_client2.md) and [`mg_get_response2();`](mg_get_response2.md).

### See Also

* [`mg_start2();`](mg_start2.md)
* [`mg_start_domain2();`](mg_start_domain2.md)
* [`mg_connect_client2();`](mg_connect_client2.md)
* [`mg_get_response2();`](mg_get_response2.md)
* [`struct mg_init_data;`](mg_init_data.md)
