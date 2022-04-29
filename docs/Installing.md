CivetWeb Install Guide
====

This guide covers the pre-built binary distributions of CivetWeb.  
The latest source code version is available at [https://github.com/civetweb/civetweb](https://github.com/civetweb/civetweb).


Windows
---

This pre-built version comes pre-built wit Lua support. Libraries for SSL support are not included due to licensing restrictions;
however, users may add an SSL library themselves.
Instructions for adding SSL support can be found in [https://github.com/civetweb/civetweb/tree/master/docs](https://github.com/civetweb/civetweb/tree/master/docs)

1. The "Visual C++ Redistributables" are already installed on most Windows PCs.
   In case they are missing, you will see a "msvcr###.dll missing" error message when starting the server.
   You need to download and install the [Redistributable for Visual Studio 2015](http://www.microsoft.com/en-us/download/details.aspx?id=48145)
   Note: The required version of the Redistributables may vary, depending on the CivetWeb version.
2. Download latest *civetweb-win.zip* from [SourceForge](https://sourceforge.net/projects/civetweb/files/)
3. When started, CivetWeb puts itself into the tray.


Building CivetWeb - Using vcpkg
---

You can download and install CivetWeb using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:

    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install civetweb

The CivetWeb port in vcpkg is kept up to date by Microsoft team members and community contributors. If the version is out of date, please [create an issue or pull request](https://github.com/Microsoft/vcpkg) on the vcpkg repository.


Linux
---

1. Download the latest *civetweb.tar.gz* from [SourceForge](https://sourceforge.net/projects/civetweb/files/) or [GitHub](https://github.com/civetweb/civetweb/releases)
2. Open archive and change to the new directory.
3. make help
4. make
5. make install
6. Run the program ```/usr/local/bin/civetweb```, it will use the configuration file */usr/local/etc/civetweb.conf*.

Most Linux systems support auto completion of command line arguments.  To enable bash auto completion for the CivetWeb stand-alone executable, set *resources/complete.lua* as complete command.  See comments in that file for further instructions.

