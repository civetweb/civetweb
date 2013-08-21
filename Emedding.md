Embedding Civetweb
=========

Civetweb is primarily designed so applications can easily add HTTP server functionality.  For example, an application server could use Civetweb to enable a web service interface for automation or remote control.

Files
------

There is no library, it is just a small set of files to compile in to the application.

#### Required Files

  1. HTTP Server API
    - civetweb.c
    - civetweb.h
  2. MD5 API
    - md5.h
    - md5.c
  3. C++ Wrapper (Optional)
    - cpp/CivetServer.cpp
    - cpp/CivetServer.h

#### Other Files

  1. Reference C Server
    - main.c
  2. Reference C++ Server
    - cpp/example.cpp

Quick Start
------

By default, the server will automatically serve up files liek a normal HTTP server.  An embedded server is most likely going to overload this functionality.

### C
  - Use mg_start() to start the server.
  - Use mg_stop() to stop the server.
  - Use mg_start() options to select the port and document root among other things.
  - Use mg_start() callbacks to add your own hooks.  The *begin_request* callback is almost always what is needed.

### C++
  - Create CivetHandlers for each URI.
  - Register the handlers with CivertServer::addHandler()
  - CivetServer starts on contruction and stops on destruction.
  - Use mg_stop() to stop the server.
  - Use contructor options to select the port and document root among other things.
  - Use constructor callbacks to add your own hooks.

LUA Support
------

LUA is a server side include functionality.  Files ending in .la will be processed with LUA.

##### Add the following CFLAGS

  - -DLUA_COMPAT_ALL
  - -DUSE_LUA
  - -DUSE_LUA_SQLITE3

##### Add the following sources

  - mod_lua.c
  - lua-5.2.1/src
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
  - build/sqlite3.c
  - build/sqlite3.h
  - build/lsqlite3.c
