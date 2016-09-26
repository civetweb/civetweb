CivetWeb API Reference
=========

CivetWeb is often used as HTTP and HTTPS library inside a larger application. An API is available to integrate the CivetWeb functionality in a larger codebase. This document describes the API. Basic usage examples of the API can be found in [Embedding.md](Embedding.md).

Functions
------

### `mg_start( callbacks, user_data, options )`

#### Parameters

  - `const struct mg_callbacks *callbacks` - a structure with optional callback functions to process requests from the web server.
  - `void *user_data` - a pointer to optional user data
  - `char **options` - a list of options used to initialize the web server

#### Returns

  - `struct mg_context *` - a pointer to a context structure when successful, or NULL in case of failure

##### Description

`mg_start()` is the only function needed to call to initialize the webserver. After the function returns and a pointer to a contect structure is provided, it is guaranteed that the server has started and is listening on the designated ports. In case of failure a NULL pointer is returned. The behaviour of the web server is controlled by a list of callback functions and a list of options. The callback functions can do application specific processing of events which are encountered by the webserver. If a specific callback function is not provided, the webserver uses their default callback routines. The options list controls how the webserver should be started and contains settings for for example the ports to listen on, the maximum number of threads created to handle requests in parallel and if settings for SSL encryption.


#### HTTP Server Source Files

These files constitute the CivetWeb library.  They do not contain a `main` function,
but all functions required to run a HTTP server.

  - HTTP server API
    - include/civetweb.h
  - C implementation
    - src/civetweb.c
    - src/md5.inl (MD5 calculation)
    - src/handle_form.inl (HTML form handling functions)
  - Optional: C++ wrapper
    - include/CivetServer.h (C++ interface)
    - src/CivetServer.cpp (C++ wrapper implementation)
  - Optional: Third party components
    - src/third_party/* (third party components, mainly used for the standalone server)
    - src/mod_*.inl (modules to access third party components from civetweb)

Note: The C++ wrapper uses the official C interface (civetweb.h) and does not add new features to the server. Some features available in the C interface might be missing in the C++ interface.

#### Additional Source Files for Executables

These files can be used to build a server executable. They contain a `main` function
starting the HTTP server.

  - Stand-alone C Server
      - src/main.c
  - Reference embedded C Server
      - examples/embedded_c/embedded_c.c
  - Reference embedded C++ Server
      - examples/embedded_cpp/embedded_cpp.cpp

Note: The "embedded" example is actively maintained, updated, extended and tested. Other examples in the examples/ folder might be outdated and remain there for reference.

Quick Start
------

By default, the server will automatically serve up files like a normal HTTP server.  An embedded server is most likely going to overload this functionality.

### C
  - Include the C interface ```civetweb.h```.
  - Use `mg_start()` to start the server.
      - Use *options* to select the port and document root among other things.
      - Use *callbacks* to add your own hooks.
  - Use `mg_set_request_handler()` to easily add your own request handlers.
  - Use `mg_stop()` to stop the server.

### C++
  - Note that CivetWeb is Clean C, and C++ interface ```CivetServer.h``` is only a wrapper layer around the C interface.
    Not all CivetWeb features available in C are also available in C++.
  - Create CivetHandlers for each URI.
  - Register the handlers with `CivetServer::addHandler()`
  - `CivetServer` starts on contruction and stops on destruction.
  - Use contructor *options* to select the port and document root among other things.
  - Use constructor *callbacks* to add your own hooks.

Lua Support
------

Lua is a server side include functionality.  Files ending in .lua will be processed with Lua.

##### Add the following CFLAGS

  - -DLUA_COMPAT_ALL
  - -DUSE_LUA
  - -DUSE_LUA_SQLITE3
  - -DUSE_LUA_FILE_SYSTEM

##### Add the following sources

  - src/mod_lua.inl
  - src/third_party/lua-5.2.4/src
     + lapi.c
     + lauxlib.c
     + lbaselib.c
     + lbitlib.c
     + lcode.c
     + lcorolib.c
     + lctype.c
     + ldblib.c
     + ldebug.c
     + ldo.c
     + ldump.c
     + lfunc.c
     + lgc.c
     + linit.c
     + liolib.c
     + llex.c
     + lmathlib.c
     + lmem.c
     + loadlib.c
     + lobject.c
     + lopcodes.c
     + loslib.c
     + lparser.c
     + lstate.c
     + lstring.c
     + lstrlib.c
     + ltable.c
     + ltablib.c
     + ltm.c
     + lundump.c
     + lvm.c
     + lzio.c
  - src/third_party/sqlite3.c
  - src/third_party/sqlite3.h
  - src/third_party/lsqlite3.c
  - src/third_party/lfs.c
  - src/third_party/lfs.h

This build is valid for Lua version Lua 5.2. It is also possible to build with Lua 5.1 (including LuaJIT) or Lua 5.3.


JavaScript Support
------

CivetWeb can be built with server side JavaScript support by including the Duktape library.


CivetWeb internals
------

CivetWeb is multithreaded web server. `mg_start()` function allocates
web server context (`struct mg_context`), which holds all information
about web server instance:

- configuration options. Note that CivetWeb makes internal copies of
  passed options.
- SSL context, if any
- user-defined callbacks
- opened listening sockets
- a queue for accepted sockets
- mutexes and condition variables for inter-thread synchronization

When `mg_start()` returns, all initialization is guaranteed to be complete
(e.g. listening ports are opened, SSL is initialized, etc). `mg_start()` starts
some threads: a master thread, that accepts new connections, and several
worker threads, that process accepted connections. The number of worker threads
is configurable via `num_threads` configuration option. That number puts a
limit on number of simultaneous requests that can be handled by CivetWeb.
If you embed CivetWeb into a program that uses SSL outside CivetWeb as well,
you may need to initialize SSL before calling `mg_start()`, and set the pre-
processor define SSL_ALREADY_INITIALIZED. This is not required if SSL is used
only within CivetWeb.

When master thread accepts new a connection, a new accepted socket (described
by `struct socket`) it placed into the accepted sockets queue,
which has size of `MGSQLEN` (default 20).
Any idle worker thread can grab accepted sockets from that queue.
If all worker threads are busy, master thread can accept and queue up to
20 more TCP connections, filling up the queue.
In the attempt to queue even more accepted connection, the master thread blocks
until there is space in the queue. When the master thread is blocked on a
full queue, the operating system can also queue incoming connection.
The number is limited by the `listen()` call parameter,
which is `SOMAXCONN` and depends on the platform.

Worker threads are running in an infinite loop, which in a simplified form
looks something like this:

    static void *worker_thread() {
      while (consume_socket()) {
        process_new_connection();
      }
    }

Function `consume_socket()` gets a new accepted socket from the CivetWeb socket
queue, atomically removing it from the queue. If the queue is empty,
`consume_socket()` blocks and waits until a new socket is placed in the queue
by the master thread.

`process_new_connection()` actually processes the
connection, i.e. reads the request, parses it, and performs appropriate action
depending on the parsed request.

Master thread uses `poll()` and `accept()` to accept new connections on
listening sockets. `poll()` is used to avoid `FD_SETSIZE` limitation of
`select()`. Since there are only a few listening sockets, there is no reason
to use hi-performance alternatives like `epoll()` or `kqueue()`. Worker
threads use blocking IO on accepted sockets for reading and writing data.
All accepted sockets have `SO_RCVTIMEO` and `SO_SNDTIMEO` socket options set
(controlled by the `request_timeout_ms` CivetWeb option, 30 seconds default)
which specifies a read/write timeout on client connections.

