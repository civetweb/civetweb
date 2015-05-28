Testing
=======

C API
-----

The unit tests leverage the CTest and Check frameworks to provide a easy
environment to build up unit tests. They are split into Public and Private
test suites reflecting the public and internal API functions of civetweb.

When adding new functionality to civetweb tests should be written so that the
new functionality will be tested across the continuous build servers. There
are various levels of the unit tests:

  * Tests are included in
  * Test Cases which are there are multiple in
  * Test Suites which are ran by the check framework by
  * `civetweb-unit-tests` which is driven using the `--suite` and
    `--test-case` arguments by
  * CTest via `add_test` in `CMakeLists.txt`

Each test suite and test case is ran individually by CTest so that it provides
good feedback to the continuous integration servers and also CMake. Adding a
new test case or suite will require the corresponding `add_test` driver to be
added to `CMakeLists.txt`
