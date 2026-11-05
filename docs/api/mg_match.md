# Civetweb API Reference

### `struct mg_match_element;`
### `struct mg_match_context;`
### `mg_match( pat, str, mcx );`

### Parameters of `mg_match()`

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`pat`**|`const char *`|A pattern string to search for.|
|**`str`**|`const char *`|The pattern will be searched in this string.|
|**`mcx`**|`struct mg_match_context *`|Pointer to a "match context" structure. Optional: can be NULL.|

### Fields of `struct mg_match_context`

| Field | Type | Description |
| :--- | :--- | :--- |
|**`case_sensitive`**|`int`| Input: 1 for case sensitive or for 0 insensitive match. |
|**`num_matches`**|`size_t`| Output: Number of wildcard matches found in `str`. |
|**`match`**|`struct mg_match_element[]`| Output: This array is filled with matches. |

In case `mcx` is not provided to `mg_match()` (a NULL pointer is provided), case insensitive pattern matching will be performed. The wildcard matches will not be stored.

The size of the `match` array is defined by the compile time constant `MG_MATCH_CONTEXT_MAX_MATCHES`.

### Fields of `struct mg_match_element`

| Field | Type | Description |
| :--- | :--- | :--- |
|**`str`**|`const char *`| Pointer to the begin of a wildcard match in `str`. |
|**`len`**|`size_t`| Length of the wildcard match. |

### Return Value

| Type | Description |
| :--- | :--- |
|`ptrdiff_t`| Number of characters matched, counting from the beginning or `str`. If the pattern does not match -1 is returned. Note that 0 may be a valid match for some patterns. |

### Description

Pattern matching function. Try to find characters matching pattern `pat` in string `str`.
A definition of valid pattern can be found in the [User Manual](../UserManual.md).

E.g.:
`mg_match("**.cgi$", "anywhere/anyname.cgi", NULL)` will return 20 (`strlen(str)`).

`mg_match("**.cgi$", "anywhere/anyname.cgi", &mcx)` will return 20 (`strlen(str)`). 
Furthermore will set `mcx.match[0].str` to point to the first match "anywhere/anyname" (that is `str+0`)
and `mcx.match[0].len` to 16 (`strlen("anywhere/anyname")`), but it will not terminate the string.
The value of `mcx.num_matches` is 1, since the only wildcard pattern `**` matches everything.

`mg_match("a?*/?*g", "ABC/DEFG", &mcx)` with `mcx.case_sensitive = 0` will return 8 (`strlen(str)`).
The value of `mcx.num_matches` is 2, one for each `?*` wildcard.
The first match `mcx.match[0].str` will point to "BC" (`str+1`) and `mcx.match[0].len == 2`.
The second match `mcx.match[1].str` will point to "DEF" (`str+4`) and `mcx.match[1].len == 3`.

Note: This is an experimental interface. 
Structure definitions may change in future versions.

### See Also

* Pattern Matching in the [User Manual](../UserManual.md).
