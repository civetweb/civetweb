# Civetweb API Reference

### `mg_base64_encode( src, src_len, dst, dst_len );`
### `mg_base64_decode( src, src_len, dst, dst_len );`

### Parameters `base64_encode`

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`src`**|`const unsigned char *`|Source buffer containing bytes to be encoded into BASE-64 code.|
|**`src_len`**|`size_t`|Number of bytes in source buffer to be encoded.|
|**`dst`**|`char *`|Destination string buffer.|
|**`dst_len`**|`size_t *`|Pointer to a variable containing the available size of the destination buffer in bytes.|

### Parameters `base64_decode`

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`src`**|`const char *`|Source string containing BASE-64 encoded data.|
|**`src_len`**|`size_t`|Number of bytes in the source buffer to be decoded.|
|**`dst`**|`unsigned char *`|Destination byte buffer.|
|**`dst_len`**|`size_t *`|Pointer to a variable containing the available size of the destination buffer in bytes.|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|Error information. -1 indicates success.|

### Description

The function `mg_base64_encode()` encodes the source buffer into the destination buffer using BASE-64 encoding.
The function `mg_base64_decode()` reads a BASE-64 encoded source buffer and decodes it into the destination buffer.
Both functions return -1 on success.
If the destination buffer is not big enough, the functions return 0.
If the source buffer supplied to `mg_base64_decode()` contains invalid characters, the return value is the position of this character. 
