# Civetweb API Reference

### `mg_download2( client_options, use_ssl, error_buffer, error_buffer_size, fmt, ... );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`client_options`**|`const struct mg_client_options *`|The client connection opions including mandatory hostname/IP address of server and port on server
|**`use_ssl`**|`int`|Use SSL if this value is not equal zero|
|**`error_buffer`**|`char *`|Buffer to store an error message|
|**`error_buffer_size`**|`size_t`|Size of the error message buffer including the terminating NUL|
|**`fmt`**|`const char *`|Format string specifying the remote command to execute|
|**`...`**|*various*|Parameters used in the format string|

### Return Value

| Type | Description |
| :--- | :--- |
|`struct mg_connection *`|A pointer to the connection structure if successful and NULL otherwise|

### Description

The `mg_download2()` works exactly like [`mg_download()`](mg_download.md) but allows to set additional client options, like client or server SSL/TLS certificates or request timeout.  See the [`mg_download()`](mg_download.md) for description.

### See Also

* [`mg_check_feature();`](mg_check_feature.md)
* [`mg_close_connection();`](mg_close_connection.md)
* [`mg_download();`](mg_download.md)
