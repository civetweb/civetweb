Building Civetweb
=========

This guide covers the build instructions for stand-alone web server.  
See [Embedding.md](https://github.com/sunsetbrew/civetweb/blob/master/docs/Embedding.md) for information on extending an application.

#### Where to get the source code?
https://github.com/sunsetbrew/civetweb


Building for Windows
---------

Open the *build/VS2012/civetweb/civetweb.sln* in Visual Studio.


Building for Linux, BSD, and OSX
---------

## Using Make

```
make help
```
Get help

```
make build
```
compile the code

```
make install
```
Install on the system, Linux only.

```
make clean
```
Clean up the mess

## Setting build options

Make options can be set on the command line with the make command like so.
```
make build WITH_LUA=1
```


| Make Options              | Description                          |
| ------------------------- | ------------------------------------ |
| WITH_LUA=1                | build with LUA support               |
| WITH_DEBUG=1              | build with GDB debug support         |
| WITH_IPV6=1               | with IPV6 support                    |
| WITH_WEBSOCKET=1          | build with web socket support        |
| CONFIG_FILE=file          | use 'file' as the config file        |
| CONFIG_FILE2=file         | use 'file' as the backup config file |
| SSL_LIB=libssl.so.0       | use versioned SSL library            |
| CRYPTO_LIB=libcrypto.so.0 | system versioned CRYPTO library      |
| INSTALL_DIR=/             | sets the install directory           |
| COPT='-DNO_SSL'           | method to insert compile flags       |

## Setting compile flags

Compile flags can be set using the *COPT* make option like so.
```
make build COPT="-DNDEBUG -DNO_CGI"
```

| Compile Flags             | Description                          |
| ------------------------- | ------------------------------------ |
| NDEBUG                    | strip off all debug code             |
| DEBUG                     | build debug version (very noisy)     |
| NO_CGI                    | disable CGI support                  |
| NO_SSL                    | disable SSL functionality            |
| NO_SSL_DL                 | link against system libssl library   |

## Advanced Cross Compiling

Take total control with *CC* and *CFLAGS* as make options.
```
make build CC="mycc" CFLAGS="-gprof -DNDEBUG -DNO_CGI"
```

## Cocoa DMG Packaging (OSX Only)

Use the alternate *Makefile.osx* to do the build.  The entire build has
to be done using *Makefile.osx* because additional compile and link options
are required.  This Makefile has all the same options as the other one plus
one additional *package* rule.

```
make -f Makefile.osx package
```



