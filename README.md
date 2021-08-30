![CivetWeb](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/civetweb_64x64.png "CivetWeb") CivetWeb
=======

**The official home of CivetWeb is [https://github.com/civetweb/civetweb](https://github.com/civetweb/civetweb)**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![GitHub contributors](https://img.shields.io/github/contributors/civetweb/civetweb.svg)](https://github.com/civetweb/civetweb/blob/master/CREDITS.md)

Continuous integration for Linux and macOS ([Travis CI](https://app.travis-ci.com/github/civetweb/civetweb)):

[![Travis Build Status](https://api.travis-ci.com/civetweb/civetweb.svg?branch=master)](https://app.travis-ci.com/github/civetweb/civetweb)

Continuous integration for Windows ([AppVeyor](https://ci.appveyor.com/project/civetweb/civetweb)):

[![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/github/civetweb/civetweb?svg=true)](https://ci.appveyor.com/project/civetweb/civetweb/branch/master)

Test coverage check ([coveralls](https://coveralls.io/github/civetweb/civetweb), [codecov](https://codecov.io/gh/civetweb/civetweb/branch/master)) (using different tools/settings):

[![Coveralls](https://img.shields.io/coveralls/civetweb/civetweb.svg?maxAge=3600)]()
[![Coverage Status](https://coveralls.io/repos/github/civetweb/civetweb/badge.svg?branch=master)](https://coveralls.io/github/civetweb/civetweb?branch=master)

[![codecov](https://codecov.io/gh/civetweb/civetweb/branch/master/graph/badge.svg)](https://codecov.io/gh/civetweb/civetweb)



Static source code analysis ([Coverity](https://scan.coverity.com/projects/5784)):

[![Coverity Scan Build Status](https://scan.coverity.com/projects/5784/badge.svg)](https://scan.coverity.com/projects/5784)



Project Mission
-----------------

Project mission is to provide easy to use, powerful, C (C/C++) embeddable web server with optional CGI, SSL and Lua support.
CivetWeb has a MIT license so you can innovate without restrictions.

CivetWeb can be used by developers as a library, to add web server functionality to an existing application.

It can also be used by end users as a stand-alone web server running on a Windows or Linux PC. It is available as single executable, no installation is required.


Where to find the official version?
-----------------------------------

End users can download CivetWeb binaries / releases from SourceForge
[https://sourceforge.net/projects/civetweb/](https://sourceforge.net/projects/civetweb/)

Developers can contribute to CivetWeb via GitHub
[https://github.com/civetweb/civetweb](https://github.com/civetweb/civetweb)

Due to a [bug in Git for Windows V2.24](https://github.com/git-for-windows/git/issues/2435)
CivetWeb must be used with an earlier or later version (see also [here](https://github.com/civetweb/civetweb/issues/812)).

Trouble tickets should be filed on GitHub
[https://github.com/civetweb/civetweb/issues](https://github.com/civetweb/civetweb/issues)

New releases are announced at Google Groups
[https://groups.google.com/d/forum/civetweb](https://groups.google.com/d/forum/civetweb)

Formerly some support question and discussion threads have been at [Google groups](https://groups.google.com/d/forum/civetweb).
Recent questions and discussions use [GitHub issues](https://github.com/civetweb/civetweb/issues).

Source releases can be found on GitHub
[https://github.com/civetweb/civetweb/releases](https://github.com/civetweb/civetweb/releases)

A very brief overview can be found on GitHub Pages
[http://civetweb.github.io/civetweb/](http://civetweb.github.io/civetweb/)


Getting The Source
------------------
Download the source code by running the following code in your command prompt:

$ git clone https://github.com/civetweb/civetweb.git
or simply grab a copy of the source code as a ZIP or TGZ file.


Quick start documentation
--------------------------

- [docs/Installing.md](https://github.com/civetweb/civetweb/blob/master/docs/Installing.md) - Install Guide (for end users using pre-built binaries)
- [docs/UserManual.md](https://github.com/civetweb/civetweb/blob/master/docs/UserManual.md) - End User Guide
- [docs/Building.md](https://github.com/civetweb/civetweb/blob/master/docs/Building.md) - Building the Server (quick start guide)
- [docs/Embedding.md](https://github.com/civetweb/civetweb/blob/master/docs/Embedding.md) - Embedding (how to add HTTP support to an existing application)
- [docs/OpenSSL.md](https://github.com/civetweb/civetweb/blob/master/docs/OpenSSL.md) - Adding HTTPS (SSL/TLS) support using OpenSSL.
- [API documentation](https://github.com/civetweb/civetweb/tree/master/docs/api) - Additional documentation on the civetweb application programming interface ([civetweb.h](https://github.com/civetweb/civetweb/blob/master/include/civetweb.h)).
- [RELEASE_NOTES.md](https://github.com/civetweb/civetweb/blob/master/RELEASE_NOTES.md) - Release Notes
- [SECURITY.md](https://github.com/civetweb/civetweb/blob/master/SECURITY.md) - Security Policy
- [LICENSE.md](https://github.com/civetweb/civetweb/blob/master/LICENSE.md) - Copyright License


Overview
--------

CivetWeb keeps the balance between functionality and
simplicity by a carefully selected list of features:

- Liberal, commercial-friendly, permissive,
  [MIT license](http://en.wikipedia.org/wiki/MIT_License)
- Free from copy-left licenses, like GPL, because you should innovate without
  restrictions.
- Forked from [Mongoose](https://code.google.com/p/mongoose/) in 2013, before
  it changed the licence from MIT to commercial + GPL. A lot of enhancements
  have been added since that time, see
  [RELEASE_NOTES.md](https://github.com/civetweb/civetweb/blob/master/RELEASE_NOTES.md).
- Works on Windows, Mac, Linux, UNIX, iPhone, Android, Buildroot, and many
  other platforms.
- Scripting and database support (CGI, SQLite database, Lua Server Pages,
  Server side Lua scripts, Server side JavaScript).
  This provides a ready to go, powerful web development platform in a one
  single-click executable with **no dependencies**.0
- Support for CGI, SSI, HTTP digest (MD5) authorization, WebSocket,
  WebDAV.
- HTTPS (SSL/TLS) support using [OpenSSL](https://www.openssl.org/).
- Optional support for authentication using client side X.509 certificates.
- Resumed download, URL rewrite, file blacklist, IP-based ACL.
- May run as Windows service.
- Download speed limit based on client subnet or URI pattern.
- Simple and clean embedding API.
- The source is in single file to make things easy.
- Embedding examples included.
- HTTP client capable of sending arbitrary HTTP/HTTPS requests.
- Websocket client functionality available (WS/WSS).


### Optionally included software

[![Lua](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/lua-logo.jpg "Lua Logo")](http://lua.org)

[![Sqlite3](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/sqlite3-logo.jpg "Sqlite3 Logo")](http://sqlite.org)

[![LuaFileSystem](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/luafilesystem-logo.jpg "LuaFileSystem Logo")](http://keplerproject.github.io/luafilesystem/)

[![LuaSQLite3](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/luasqlite-logo.jpg "LuaSQLite3 Logo")](http://lua.sqlite.org/index.cgi/index)

[![LuaXML](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/luaxml-logo.jpg "LuaXML Logo")](https://github.com/n1tehawk/LuaXML)

[![Duktape](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/duktape-logo.png "Duktape Logo")](http://duktape.org)


### Optional depencencies

[![zlib](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/zlib3d-b1.png "zlib Logo")](https://zlib.net)

[![OpenSSL](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/OpenSSL_logo.png "OpenSSL Logo")](https://www.openssl.org/)

[![Mbed TLS](https://raw.githubusercontent.com/civetweb/civetweb/master/resources/mbedTLS_logo.png "mbedTLS Logo")](https://github.com/ARMmbed/mbedtls)



Support
-------

This project is very easy to install and use.
Please read the [documentation](https://github.com/civetweb/civetweb/blob/master/docs/)
and have a look at the [examples](https://github.com/civetweb/civetweb/blob/master/examples/).

Recent questions and discussions usually use [GitHub issues](https://github.com/civetweb/civetweb/issues).
Some old information may be found on the [mailing list](https://groups.google.com/d/forum/civetweb), 
but this information may be outdated.

Feel free to create a GitHub issue for bugs, feature requests, questions, suggestions or if you want to share tips and tricks.
When creating an issues for a bug, add enough description to reproduce the issue - at least add CivetWeb version and operating system.
Please see also the guidelines for [Contributions](https://github.com/civetweb/civetweb/blob/master/docs/Contribution.md) and the [Security Policy](https://github.com/civetweb/civetweb/blob/master/SECURITY.md)

Note: We do not take any liability or warranty for any linked contents.  Visit these pages and try the community support suggestions at your own risk.
Any link provided in this project (including source and documentation) is provided in the hope that this information will be helpful.
However, we cannot accept any responsibility for any content on an external page.



Contributions
-------------

Contributions are welcome provided all contributions carry the MIT license.

DO NOT APPLY fixes copied from Mongoose to this project to prevent GPL tainting.
Since 2013, CivetWeb and Mongoose are developed independently.
By now the code base differs, so patches cannot be safely transferred in either direction.

Some guidelines can be found in [docs/Contribution.md](https://github.com/civetweb/civetweb/blob/master/docs/Contribution.md).


Authors
-------

CivetWeb has been forked from the last MIT version of Mongoose in 2013.
Since then, CivetWeb has seen many improvements from various authors
(Copyright (c) 2013-2021 the CivetWeb developers, MIT license).

A list of authors can be found in [CREDITS.md](https://github.com/civetweb/civetweb/blob/master/CREDITS.md).

CivetWeb is based on the Mongoose project.  The original author of Mongoose was
Sergey Lyubka (Copyright (c) 2004-2013 Sergey Lyubka, MIT license).
However, on August 16, 2013, the [license of Mongoose has been changed](https://groups.google.com/forum/#!topic/mongoose-users/aafbOnHonkI)
after writing and distributing the original code this project is based on.
The license change and CivetWeb used to be mentioned on the Mongoose
[Wikipedia](https://en.wikipedia.org/wiki/Mongoose_(web_server))
page as well, but it's getting deleted (and added again) there every
now and then.

Using the CivetWeb project ensures the MIT licenses terms are applied and
GPL cannot be imposed on any of this code, as long as it is sourced from
here. This code will remain free with the MIT license protection.

