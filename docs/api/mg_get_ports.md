# Civetweb API Reference

### ~~`mg_get_ports( ctx, size, ports, ssl );`~~

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`ctx`**|`const struct mg_context *`||
|**`size`**|`size_t`||
|**`ports`**|`int *`||
|**`ssl`**|`int *`||

### Return Value

| Type | Description |
| :--- | :--- |
|`size_t`||

### Description

This function is deprecated. Use [`mg_get_server_ports()`](mg_get_server_ports.md)
instead.

### See Also

* [`struct mg_server_ports;`](mg_server_ports.md)
* [`mg_get_server_ports();`](mg_get_server_ports.md)
