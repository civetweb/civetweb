# Civetweb API Reference

### `mg_get_cookie( cookie, var_name, buf, buf_len );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`cookie`**|`const char *`||
|**`var_name`**|`const char *`||
|**`buf`**|`char *`|The buffer where to store the contents of the cookie|
|**`buf_len`**|`size_t`|The length of the cookie buffer, including the terminating NUL|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|The length of the cookie or an error code|

### Description

### See Also

* [`mg_get_var();`](mg_get_var.md)
* [`mg_get_var2();`](mg_get_var2.md)
