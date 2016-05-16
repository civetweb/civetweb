![CivetWeb](https://raw.github.com/civetweb/civetweb/master/resources/civetweb_64x64.png "CivetWeb") CivetWeb
=======

**The official home of CivetWeb is [https://github.com/civetweb/civetweb](https://github.com/civetweb/civetweb)**


Continuous integration for Linux and OSX ([Travis CI](https://travis-ci.org/civetweb/civetweb)):

[![Travis Build Status](https://travis-ci.org/civetweb/civetweb.svg?branch=master)](https://travis-ci.org/civetweb/civetweb)

Continuous integration for Windows ([AppVeyor](https://ci.appveyor.com/project/civetweb/civetweb)) (currently in a setup phase):

[![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/github/civetweb/civetweb?svg=true)](https://ci.appveyor.com/project/civetweb/civetweb/branch/master)

Test coverage check ([coveralls](https://coveralls.io/github/civetweb/civetweb)) (currently in a setup phase):

[![Coverage Status](https://coveralls.io/repos/github/civetweb/civetweb/badge.svg?branch=master)](https://coveralls.io/github/civetweb/civetweb?branch=master)

Static source code analysis ([Coverity](https://scan.coverity.com/projects/5784)): 

[![Coverity Scan Build Status](https://scan.coverity.com/projects/5784/badge.svg)](https://scan.coverity.com/projects/5784)




Project Mission
-----------------

Project mission is to provide easy to use, powerful, C/C++ embeddable web
server with optional CGI, SSL and Lua support.
CivetWeb has a MIT license so you can innovate without restrictions.

CivetWeb can be used by developers as a library, to add web server functionality to an existing application.
It can also be used by end users as a stand-alone web server. It is available as single executable, no installation is required.


Where to find the official version?
-----------------------------------

End users can download CivetWeb at SourceForge
[https://sourceforge.net/projects/civetweb/](https://sourceforge.net/projects/civetweb/)

Developers can contribute to CivetWeb via GitHub
[https://github.com/civetweb/civetweb](https://github.com/civetweb/civetweb)

Trouble tickets should be filed on GitHub
[https://github.com/civetweb/civetweb/issues](https://github.com/civetweb/civetweb/issues)

Discussion/support group and announcements are at Google Groups
[https://groups.google.com/d/forum/civetweb](https://groups.google.com/d/forum/civetweb)


Quick start documentation
--------------------------

- [docs/Installing.md](https://github.com/civetweb/civetweb/blob/master/docs/Installing.md) - Install Guide (for end users using pre-built binaries)
- [docs/UserManual.md](https://github.com/civetweb/civetweb/blob/master/docs/UserManual.md) - End User Guide
- [docs/Building.md](https://github.com/civetweb/civetweb/blob/master/docs/Building.md) - Building the Server (quick start guide)
- [docs/Embedding.md](https://github.com/civetweb/civetweb/blob/master/docs/Embedding.md) - Embedding (how to add HTTP support to an existing application)
- [docs/OpenSSL.md](https://github.com/civetweb/civetweb/blob/master/docs/OpenSSL.md) - Adding HTTPS (SSL/TLS) support using OpenSSL.
- [RELEASE_NOTES.md](https://github.com/civetweb/civetweb/blob/master/RELEASE_NOTES.md) - Release Notes
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
  it changed the licence from MIT to commercial + GPL. A lot of enchancements
  have been added since that time, see
  [RELEASE_NOTES.md](https://github.com/civetweb/civetweb/blob/master/RELEASE_NOTES.md).
- Works on Windows, Mac, Linux, UNIX, iPhone, Android, Buildroot, and many
  other platforms.
- Scripting and database support (Lua scipts, Lua Server Pages, CGI + SQLite
  database, Server side javascript).
  This provides a ready to go, powerful web development platform in a one
  single-click executable with **no dependencies**.
- Support for CGI, HTTPS (SSL/TLS), SSI, HTTP digest (MD5) authorization, Websocket,
  WEbDAV.
- Optional support for authentication using client side X.509 certificates.
- Resumed download, URL rewrite, file blacklist, IP-based ACL, Windows service.
- Download speed limit based on client subnet or URI pattern.
- Simple and clean embedding API.
- The source is in single file to make things easy.
- Embedding examples included.
- HTTP client capable of sending arbitrary HTTP/HTTPS requests.
- Websocket client functionality available (WS/WSS).


### Optionally included software

<a href="http://lua.org">
![Lua](https://raw.github.com/civetweb/civetweb/master/resources/lua-logo.jpg "Lua Logo")
</a>
<a href="http://sqlite.org">
![Sqlite3](https://raw.github.com/civetweb/civetweb/master/resources/sqlite3-logo.jpg "Sqlite3 Logo")
</a>
<a href="http://keplerproject.github.io/luafilesystem/">
![LuaFileSystem](https://raw.github.com/civetweb/civetweb/master/resources/luafilesystem-logo.jpg "LuaFileSystem Logo")
</a>
<a href="http://lua.sqlite.org/index.cgi/index">
![LuaSQLite3](https://raw.github.com/civetweb/civetweb/master/resources/luasqlite-logo.jpg "LuaSQLite3 Logo")
</a>
<a href="http://viremo.eludi.net/LuaXML/index.html">
![LuaXML](https://raw.github.com/civetweb/civetweb/master/resources/luaxml-logo.jpg "LuaXML Logo")
</a>
<a href="http://duktape.org">
![Duktape](https://raw.github.com/civetweb/civetweb/master/resources/duktape-logo.png "Duktape Logo")
</a>


Support
-------

This project is very easy to install and use. Please read the [documentation](https://github.com/civetweb/civetweb/blob/master/docs/)
and have a look at the [examples] (https://github.com/civetweb/civetweb/blob/master/examples/).
More information may be found on the [mailing list](https://groups.google.com/d/forum/civetweb).


Contributions
---------------

Contributions are welcome provided all contributions carry the MIT license.

DO NOT APPLY fixes copied from Mongoose to this project to prevent GPL tainting.
Since 2013 CivetWeb and Mongoose are developed independently. By now the code base differs, so patches cannot be safely transfered in either direction.

Some guidelines can be found in [docs/Contribution.md](https://github.com/civetweb/civetweb/blob/master/docs/Contribution.md).


### Authors

CivetWeb is based on the Mongoose project.  The original author of Mongoose was
Sergey Lyubka (Copyright (c) 2004-2013 Sergey Lyubka, MIT license).

However, in August 16, 2013, the [license of Mongoose has been changed](https://groups.google.com/forum/#!topic/mongoose-users/aafbOnHonkI)
after writing and distributing the original code this project is based on.
The license change used to be described on the Mongoose Wikipedia page as well, but it's getting deleted there regularly.

CivetWeb has been forked from the last MIT version of Mongoose. 
Since 2013, CivetWeb has seen many improvements from various authors 
(Copyright (c) 2013-2016 the CivetWeb developers, MIT license).
A list of authors can be found in [CREDITS.md](https://github.com/civetweb/civetweb/blob/master/CREDITS.md).

Using the CivetWeb project ensures the MIT licenses terms are applied and
GPL cannot be imposed on any of this code as long as it is sourced from
here. This code will remain free with the MIT license protection.

