# Security Policy

## Supported Versions

The current "head" version ("master" branch) undergoes some automatic tests, but may have defects and vulnerabilities.
All development branches may be in an intermediate, untested state.

For released versions, additional tests are performed, including manual tests, static source code analysis and fuzz testing.
Except for features marked as "experimental". Experimental features are deactivated in the default configuration.

Defects will be fixed in the current head version.
Selected, critical defects are fixed in the latest release as well.

Note that different security policies apply to different files/folders in this project:
- The core components are include/civetweb.h, src/civetweb.c and src/*.inl.  These files are part of every server instance in production. Therefore they have to undergo the most intensive security tests and reviews.
- The src/main.c file is used by the standalone server. It is used in various tests in combination with the aforementioned core components. Applications embedding civetweb will not use main.c and, thus, do not suffer from any vulnerabilities therein.
- The example folders contain different usage examples in different maintenance state. This is explained in detail in the README file there.
- The content of all test folders (test, unittest, fuzztest) is used to test the server functionality. These tests are not designed with security in mind - on the contrary, some tests contain scripts or settings that even introduce security leaks on purpose. All tests are only meant to be run in a test environment. You should not use the content of any test folder in production. Also certificates in "resources/cert" are only meant to be used in test environments and must never be used in production.


## Reporting a Vulnerability

Please send vulnerability reports by email to bel2125 at gmail com.
Vulnerability with low severity can be sent directly by email.

For high severity vulnerabilities, you can get an individual gpg key to encrypt your detailed description of vulnerabilities you want to report.

If you do not get any response within one week, your email might have been lost (e.g., deleted as false positive by a spam filter). In this case, please open a GitHub issue.

