Embedding Civetweb
=========

Civetweb is primarily designed so applications can easily add HTTP server functionality.  For example, an application server could use Civetweb to enable a web service interface for automation or remote control.

Files
------

There is just a small set of files to compile in to the application,
but if a library is desired, see [Building.md](https://github.com/bel2125/civetweb/blob/master/docs/Building.md)

#### Regarding the INL file extension
The *INL* file extension represents code that is statically included inline in a source file.  Slightly different from C++ where it means "inline" code which is technically not the same as static code. Civetweb overloads this extension for the sake of clearity as opossed to having .c extensions on files that should not be directly compiled.

#### Required Files

  1. HTTP Server API
    - src/civetweb.c
    - include/civetweb.h
  2. MD5 API
    - src/md5.inl
  3. C++ Wrapper (Optional)
    - src/CivetServer.cpp
    - include/CivetServer.h

#### Other Files

  1. Reference C Server
    - src/main.c
  2. Reference C++ Server
    - examples/embedded_cpp/embedded_cpp.cpp

Quick Start
------

By default, the server will automatically serve up files like a normal HTTP server.  An embedded server is most likely going to overload this functionality.

### C
  - Use ```mg_start()``` to start the server.
      - Use *options* to select the port and document root among other things.
      - Use *callbacks* to add your own hooks.
  - Use ```mg_set_request_handler()``` to easily add your own request handlers.
  - Use ```mg_stop()``` to stop the server.

### C++
  - Create CivetHandlers for each URI.
  - Register the handlers with ```CivertServer::addHandler()```
  - ```CivetServer``` starts on contruction and stops on destruction.
  - Use contructor *options* to select the port and document root among other things.
  - Use constructor *callbacks* to add your own hooks.

Lua Support
------

Lua is a server side include functionality.  Files ending in .la will be processed with Lua.

##### Add the following CFLAGS

  - -DLUA_COMPAT_ALL
  - -DUSE_LUA
  - -DUSE_LUA_SQLITE3
  - -DUSE_LUA_FILE_SYSTEM

##### Add the following sources

  - src/mod_lua.inl
  - src/third_party/lua-5.2.3/src
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


Civetweb internals
------

Civetweb is multithreaded web server. `mg_start()` function allocates
web server context (`struct mg_context`), which holds all information
about web server instance:

- configuration options. Note that civetweb makes internal copies of
  passed options.
- SSL context, if any
- user-defined callbacks
- opened listening sockets
- a queue for accepted sockets
- mutexes and condition variables for inter-thread synchronization

When `mg_start()` returns, all initialization is quaranteed to be complete
(e.g. listening ports are opened, SSL is initialized, etc). `mg_start()` starts
two threads: a master thread, that accepts new connections, and several
worker threads, that process accepted connections. The number of worker threads
is configurable via `num_threads` configuration option. That number puts a
limit on number of simultaneous requests that can be handled by civetweb.

When master thread accepts new connection, a new accepted socket (described by
`struct socket`) it placed into the accepted sockets queue,
which has size of 20 (see [code](https://github.com/bel2125/civetweb/blob/3892e0199e6ca9613b160535d9d107ede09daa43/civetweb.c#L486)). Any idle worker thread
can grab accepted sockets from that queue. If all worker threads are busy,
master thread can accept and queue up to 20 more TCP connections,
filling up the queue.
In the attempt to queue next accepted connection, master thread blocks
until there is space in a queue. When master thread is blocked on a
full queue, TCP layer in OS can also queue incoming connection.
The number is limited by the `listen()` call parameter on listening socket,
which is `SOMAXCONN` in case of Civetweb, and depends on a platform.

Worker threads are running in an infinite loop, which in simplified form
looks something like this:

    static void *worker_thread() {
      while (consume_socket()) {
        process_new_connection();
      }
    }

Function `consume_socket()` gets new accepted socket from the civetweb socket
queue, atomically removing it from the queue. If the queue is empty,
`consume_socket()` blocks and waits until new sockets are placed in a queue
by the master thread. `process_new_connection()` actually processes the
connection, i.e. reads the request, parses it, and performs appropriate action
depending on a parsed request.

Master thread uses `poll()` and `accept()` to accept new connections on
listening sockets. `poll()` is used to avoid `FD_SETSIZE` limitation of
`select()`. Since there are only a few listening sockets, there is no reason
to use hi-performance alternatives like `epoll()` or `kqueue()`. Worker
threads use blocking IO on accepted sockets for reading and writing data.
All accepted sockets have `SO_RCVTIMEO` and `SO_SNDTIMEO` socket options set
(controlled by `request_timeout_ms` civetweb option, 30 seconds default) which
specify read/write timeout on client connection.
