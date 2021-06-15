# Civetweb API Reference

### `mg_modify_passwords_file_ha1( passwords_file_name, domain, user, ha1 );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`passwords_file_name`**|`const char *`|The path to the passwords file|
|**`realm`**|`const char *`|The authentication realm (domain) of the user record|
|**`user`**|`const char *`|Username of the record to be added, changed or deleted|
|**`ha1`**|`const char *`|HA1 hash of "user:realm:password"|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|Success or error code|

### Description

The function `mg_modify_passwords_file_ha1()` is similar to `mg_modify_passwords_file()`, but the password is not specified in plain text and thus is not revealed to the civetweb library. Instead of the password, a hash ("HA1") is specified which is constructed by the caller as the MD5 checksum (in lower-case hex digits) of the string `user:realm:password`.

For example, if the user name is `myuser`, the realm is `myrealm`, and the password is `secret`, then the HA1 is `e67fd3248b58975c3e89ff18ecb75e2f`:

```
$ echo -n "myuser:myrealm:secret" | md5sum
e67fd3248b58975c3e89ff18ecb75e2f  -
```

The function returns 1 when successful and 0 if an error occurs.

### See Also

* [`mg_modify_passwords_file();`](mg_modify_passwords_file.md)


