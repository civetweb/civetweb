# Civetweb API Reference

### `mg_get_system_info( buffer, buflen );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`buffer**|`char *`|A string buffer to store the information|
|**`buflen**|`int`|Size of the string buffer (including space for a terminating 0)|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|Available system information in bytes (excluding the terminating 0)|

### Description

The function `mg_get_system_info()` returns information collected for the system (operating system, compiler, version, ...). The string is in a human readable format - changes in the format are possible in future versions. This string should be included for support requests.

### See Also

* [`mg_get_context_info();`](mg_get_context_info.md)

