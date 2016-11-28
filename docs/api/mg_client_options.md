# Civetweb API Reference

### `struct mg_client_options;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`host`**|`const char *`|The hostname or IP address to connect to|
|**`port`**|`int`|The port on the server|
|**`client_cert`**|`const char *`|Pointer to client certificate|
|**`server_cert`**|`const char *`|Pointer to a server certificate|

### Description

The the `mgclient_options` structure contains host and security information to connect
as a client to another host. A parameter of this type is used in the call to the
function [`mg_connect_client_secure();`](mg_connect_client_secure.md).

### See Also

* [`mg_connect_client_secure();`](mg_connect_client_secure.md)
