
Examples
=====

The ([embedded_c](https://github.com/civetweb/civetweb/tree/master/examples/embedded_c)) example shows
how to embed civetweb into a C or C++ application.

The ([embedded_cpp](https://github.com/civetweb/civetweb/tree/master/examples/embedded_cpp)) example
demonstrates embedding in a C++ application. It uses the C++ wrapper of the full C interface `civetweb.h`.
The C++ wrapper only offers a limited subset of the full C API.
Thus, the C example is more complete than the C++ example.

These examples were not designed with security in mind, but to show how the API can be used in principle.
For more information, see the [documentation](https://github.com/civetweb/civetweb/tree/master/docs).
Some examples can also be found in the [test](https://github.com/civetweb/civetweb/tree/master/test) folder,
but they are less documented and adapted to some special needs of the test frameworks.

The ([https](https://github.com/civetweb/civetweb/tree/master/examples/https)) example shows how to configure
a HTTPS server with improved security settings.
It does not hold any source, but only a configuration file and some documentation how to use it.

The [multidomain](https://github.com/civetweb/civetweb/tree/master/examples/multidomain) example demonstrates 
how to host multiple domains with different HTTPS certificates. 
It uses the standalone server (civetweb.c + main.c) and existing certificates.

The [ws_client](https://github.com/civetweb/civetweb/tree/master/examples/ws_client) example shows 
how to use the websocket client interface to communicate with an (external) websocket server. 
It uses the "echo demo" of [websocket.org](http://websocket.org/echo.html) and will work only if this server is reachable.

All examples are subject to the MIT license (unless noted otherwise) - they come without warranty of any kind.

Note that the examples are only meant as a demonstration how to use CivetWeb.
The example codes are omitting some error checking and input validation for better readability of the source.
They are not as actively maintained and continuously developed as main source of the project 
(include/civetweb.h, src/civetweb.c, src/*.inl).  
Example codes undergo less quality management than the main source files of this project.
Examples are not checked and updated with every new version.
There are no repeated security assessment for the example codes, settings are not updated on a regular basis.

Contributions to examples code are welcome, under the same conditions as contributions to the 
main source code ([Contribution.md](https://github.com/civetweb/civetweb/blob/master/Contribution.md)).
