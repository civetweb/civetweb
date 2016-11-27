# Civetweb API Reference

### `struct mg_request_info;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`request_method`**|`const char *`| The request method used by the client for the connection this can be **GET**, **POST** or one of the other common HTTP request methods |
|**`request_uri`**|`const char *`| The absolute or URL-encoded URI as it was sent in the request |
|**`local_uri`**|`const char *`| The relative URL-encoded URI as it references the local resource. If the request URI does not reference a resource on the local server, this field is NULL |
|~~`uri`~~|`const char *`| *Deprecated. Use* `local_uri` *instead* |
|**`http_version`**|`const char *`| The HTTP version as mentioned in the client request. This can be "1.0", "1.1", etc. |
|**`remote_user`**|`const char *`| The name of the authenticated remote user, or NULL if no authentication was used |
|**`remote addr`**|`char[48]`| The IP address of the remote client as a string. This can either represent an IPv4 or an IPv6 address. |
|~~`remote_ip`~~|`long`| *Deprecated. Use* `remote_addr` *instead* |
|**`content_length`**|`long long`| The content length of the request body. This value can be -1 if no content length was provided. |
|**`remote_port`**|`int`| The port number at the client's side |
|**`is_ssl`**|`int`| 1 if the connection is over SSL, and 0 if it is a plain connection |
|**`user_data`**|`void *`| A pointer to the `user_data` information which was provided as a parameter to `mg_start()`. |
|**`conn_data`**|`void *`| A pointer to connection specific user data |
|**`num_headers`**|`int`| The number of HTTP request headers sent by the client |
|**`http_headers`**|`struct mg_header[64]`| Array of structures with the HTTP request headers sent by the client |
|**`client_cert`**|`struct client_cert *`| Pointer to the client certificate information, when available |

### Description

The `mg_request_info` structure contains the client information of an existing connection.

### See Also

* [`struct client_cert;`](client_cert.md)
* [`struct mg_header;`](mg_header.md)
* [`mg_get_request_info();`](mg_get_request_info.md)
