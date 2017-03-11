# Civetweb API Reference

### `mg_send_chunk( conn, buf, len );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct mg_connection *`| A pointer to the connection to be used to send data |
|**`chunk`**|`const void *`| A pointer to the data to be sent |
|**`chunk_len`**|`size_t`| The number of bytes to be sent |

### Return Value

| Type | Description |
| :--- | :--- |
|`int`| An integer indicating the amount of bytes sent, or failure |

### Description

The function `mg_send_chunk()` can be used to send a block of data, if chunked transfer encoding is used. Only use this function after sending a complete HTTP request or response header with the "Transfer-Encoding: chunked" header field set.
The function returns the amount of bytes sent in case of success, or **-1** in case of an error.

### See Also

* [`mg_write();`](mg_write.md)
* [`mg_printf();`](mg_print.md)
