== Travis CI Tests

Travis is a service which will build your project when you commit or get pull requests on Github.

I have fixed and extended the travis configuration to build on the new sudo-less docker infrastructure.

=== CI Process

* On Check-in or Pull Requests clone the repo
* Run make WITH_LUA=1 WITH_DEBUG=1 WITH_IPV6=1 WITH_WEBSOCKET=1
* Build a standalone lua installation (seperate from civetweb or the OS)
* Build LuaRocks in standalone installation
* Install a few rocks into the standalone installation
* Start the test script

=== test/ci_tests/01_basic/basic_spec.lua

On the initial checkin, there is only one test which demonstrates:

* reliably starting civetweb server on travis infrastructure
* waiting (polling) with lua.socket to establish the server is up and running 
* using libcurl via lua to test that files in the specified docroot are available
* kill the civetweb server process
* waiting (polling) the server port to see that the server has freed it

=== Adding Tests

* Create a directory under ci_tests
* Add a spec file, so now we have ci_tests/02_my_awesome_test/awesome_spec.lua
* Any file under ci_tests which ends in _spec.lua will be automatically run
* Check out the 'busted' and lua-curl3 docs for more info
* https://github.com/Lua-cURL/Lua-cURLv3
* http://olivinelabs.com/busted/

