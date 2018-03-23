# Interface changes

## Proposed interface changes for 1.10

Status: To be discussed

### Server interface

#### mg\_start / mg\_init\_library

Calling mg\_init\_library is recommended before calling mg\_start.

Compatibility:
Initially, mg\_init\_library will be called implicitly if it has 
not been called before mg\_start.
If mg\_init\_library was not called, mg\_stop may leave memory leaks.

#### mg\_websocket\_write functions

Calling mg\_lock\_connection is no longer called implicitly
in mg\_websocket\_write functions. 
If you use websocket write functions them from two threads,
you must call mg\_lock\_connection explicitly, just like for any
other connection.

This is an API harmonization issue.

Compatibility:
If a websocket connection was used in only one thread, there is
no incompatibility. If a websocket connection was used in multiple
threads, the user has to add the mg\_lock\_connection before and
the mg\_unlock\_connection after the websocket write call.

#### open\_file member of mg\_callbacks

This member is going to be removed.
It is superseeded by mg\_add\_request\_handler.

Compatibility:
Current code using open\_file needs to be changed.
Instructions how to do this will be provided.


### Client interface


#### mg\_init\_library

Calling mg\_init\_library is required before calling any client
function. In particular, the TLS initialization must be done
before using mg\_connect\_client\_secure.

Compatibility:
Some parts of the client interface did not work, if mg\_start
was not called before. Now server and client become independent.

#### mg\_connect\_client (family)

mg_connect_client needs several new parameters (options).

Details are to be defined.

mg_connect_client and mg_download should return a different kind of
mg_connection than used in server callbacks. At least, there should
be a function mg_get_response_info, instead of using 
mg_get_request_info, and getting the HTTP response code from the
server by looking into the uri member of struct mg_request_info.


### `size_t` in all interface

Having `size_t` in interfaces while building for 32 and 64 bit
complicates maintenance in an unnecessary way 
(see [498](https://github.com/civetweb/civetweb/issues/498)).

Replace all data sizes by 64 bit integers.


### Pattern definition

The current definition of pattern matching is problematic
(see [499](https://github.com/civetweb/civetweb/issues/499)).

Find and implement a new definition.


