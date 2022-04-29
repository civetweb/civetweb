# Civetweb API Reference

### `mg_split_form_urlencoded( data, form_fields, num_form_fields );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`data`**|`char *`|Form encoded data to be split. This buffer will be modified by `mg_split_form_urlencoded`.|
|**`form_fields`**|`struct mg_header []`|Output buffer for name-value-pairs.|
|**`num_form_fields`**|`unsigned`|Number of elements in form_fields buffer.|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|Number of encoded form fields, or an error code|

### Description

The function `mg_split_form_urlencoded()` can be used to split an x-www-form-urlencoded data field into name-value-pairs. It can split the POST body data of an x-www-form-urlencoded html form, or a query string. The parameter `data` is modified by this function, so the `query_string` member of `struct mg_request_info` must not be passed directly to this function, since `query_string` is a `const char *`. Use a copy (e.g, created by `strdup`) as input for `mg_split_form_urlencoded()`. 
The caller has to provide all required buffers, since the function does not do any memory handling itself. Percent-encoded elements will be decoded.

Example for an x-www-form-urlencoded format encoded input:
`data = "keyName1=value1&keyName2=value2&keyName3=value3"`
The function will return 3 and set `form_fields[0].name = "keyName1"`, `form_fields[0].value = "value1"`, `form_fields[1].name = "keyName2"`, `form_fields[1].value = "value2"`, `form_fields[2].name = "keyName3"`, `form_fields[2].value = "value3"`, and modify `data` to `"keyName1\x00value1\x00keyName2\x00value2\x00keyName3\x00value3"`.


### See Also

* [`struct mg_header`](mg_header.md)
* [`struct mg_request_info`](mg_request_info.md)
* [`mg_get_cookie();`](mg_get_cookie.md)
* [`mg_get_var();`](mg_get_var.md)
* [`mg_get_var2();`](mg_get_var2.md)
* [`mg_handle_form_request();`](mg_handle_form_request.md)
