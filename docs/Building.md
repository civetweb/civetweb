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


Building on Android
---------

This is a small guide to help you run civetweb on Android. Currently it is
tested on the HTC Wildfire. If you have managed to run it on other devices
as well, please comment or drop an email in the mailing list.
Note : You dont need root access to run civetweb on Android.

- Download the source from the Downloads page.
- Download the Android NDK from [http://developer.android.com/tools/sdk/ndk/index.html](http://developer.android.com/tools/sdk/ndk/index.html)
- Run `/path-to-ndk/ndk-build -C /path-to-civetweb/build`
  That should generate civetweb/lib/armeabi/civetweb
- Using the adb tool (you need to have Android SDK installed for that),
  push the generated civetweb binary to `/data/local` folder on device.
- From adb shell, navigate to `/data/local` and execute `./civetweb`.
- To test if the server is running fine, visit your web-browser and
  navigate to `http://127.0.0.1:8080` You should see the `Index of /` page.

![screenshot](https://a248.e.akamai.net/camo.github.com/b88428bf009a2b6141000937ab684e04cc8586af/687474703a2f2f692e696d6775722e636f6d2f62676f6b702e706e67)


Notes:

- `jni` stands for Java Native Interface. Read up on Android NDK if you want
  to know how to interact with the native C functions of civetweb in Android
  Java applications.
- TODO: A Java application that interacts with the native binary or a
  shared library.


