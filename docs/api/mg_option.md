# Civetweb API Reference

### `struct mg_option;`

### Fields

| Field | Type | Description |
| :--- | :--- | :--- |
|**`name`**|`const char *`||
|**`type`**|`int`||
|**`default_value`**|`const char *`||

### Description

Type can be one of the following values:

`CONFIG_TYPE_UNKNOWN`
`CONFIG_TYPE_NUMBER`
`CONFIG_TYPE_STRING`
`CONFIG_TYPE_FILE`
`CONFIG_TYPE_DIRECTORY`
`CONFIG_TYPE_BOOLEAN`
`CONFIG_TYPE_EXT_PATTERN`

### See Also

* [`mg_get_valid_options();`](mg_get_valid_options.md)
