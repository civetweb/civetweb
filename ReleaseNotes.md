
Release Notes v1.1 
===
### Objectives: *Build, Documentation, License Improvements*
The objective of this release is to establish a maintable code base, ensure MIT license rights and improve usability and documention.

Changes
-------

- Added fallback configuration file path for Linux systems.
    + Good for having a system wide default configuration /etc/civetweb/civetweb.conf
- Added new C++ abstraction class cpp/CivetServer
- Added thread safety for and fixed websocket defects (Morgan McGuire)
- Created PKGBUILD to use Arch distribution (Daniel Oaks)
- Created new documentation on Embeddeding, Building.
- Updated License file to include all licenses.
- Replaced MD5 implementation due to questionable license.
     + This requires two new source files md5.c, md5.h
- Changed UNIX/OSX build to conform to common practices.
     + Supports build, install and clean rules.
     + Supports cross compiling
     + Features can be chosen in make options
- Moved Cocoa/OSX build and packaging to a separate file.
     + This actually a second build variant for OSX.
     + Removed yaSSL from the OSX build, not needed.
- Added new Visual Studio projects for Windows builds.
     + Removed Windows support from Makefiles
     + Provided additional, examples with LUA, and another with yaSSL. 
- Changed Zombie Reaping policy to not ignore SIGCHLD.
     + The previous method caused trouble in applciations that spawn children.

Known Issues
-----

- Build support for VS6 and some other has been deprecated.
    + This does not impact embedded programs, just the stand-alone build.
    + The old Makefile was renamed to Makefile.deprecated.
    + This is partcially do to lack fo testing. 
    + Need to find out what is actually in demand.
- Build changes may impact current users.
    + As with any change of this type, changes may impact some users.

Release Notes v1.0
===

### Objectives: *MIT License Preservation, Rebranding*
The objective of this release is to establish a version of the Mongoose software distribution thate still retains the MIT license.

Changes
-------

- Renamed Mongoose to Civetweb in the code and documentation.
- Replaced copyrighted images with bew images
- Created a new code respository at https://github.com/sunsetbrew/civetweb
- Created a distribution site at https://sourceforge.net/projects/civetweb/
- Basic build testing
