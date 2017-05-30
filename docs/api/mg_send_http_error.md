# Civetweb API Reference

### `mg_send_http_error( conn, status_code, fmt, ... );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`|The connection over which the file must be sent|
|**`status_code`**|`int`|The HTTP status code to return|
|**`fmt`**|`const char *`|Format string specifying the remote command to execute|
|**`...`**|*various*|Parameters used in the format string|

### Return Value

*none*

### Description

The function `mg_send_http_error()` sends a HTTP reply with the given status code and the content specified by fmt. The textual status code is determined from the numerical status code internally.

### See Also

* [`mg_printf();`](mg_printf.md)
* [`mg_write();`](mg_write.md)
* [`mg_send_file();`](mg_send_file.md)
