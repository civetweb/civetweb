Contributing to CivetWeb
====

Contributions to CivetWeb are welcome, provided all contributions carry the MIT license.

- Please report [issues](https://github.com/civetweb/civetweb/issues) on GitHub. If the issue you want to report is already reported there, add a comment with your specific details to that issue. In case of doubt, please create a new issue.
- If you know how to fix the issue, please create a pull request on GitHub. Please take care your modifications pass the continuous integration checks. These checks are performed automatically when you create a pull request, but it may take some hours until all tests are completed. Please provide a description for every pull request (see below).
- Alternatively, you can post a patch or describe the required modifications in a GitHub issue. However, a pull request would be preferred.

- Improvements to documentation, tests and examples are welcome as well.

- Contributor names are listed in [CREDITS.md](https://github.com/civetweb/civetweb/blob/master/CREDITS.md), unless you explicitly state you don't want your name to be listed there. This file is occasionally updated, adding new contributors, using author names from git commits and GitHub comments.

- In case your modifications either
  1. modify or extend the API,
  2. affect multi-threading,
  3. imply structural changes,
  or
  4. have significant influence on maintenance,

  please first create an issue on GitHub to discuss the planned changed.

- In case you think you found a security issue that should be evaluated and fixed before public disclosure, feel free to write an email.  Although CivetWeb is a fork from Mongoose from 2013, the code bases are different now, so security vulnerabilities of Mongoose usually do not affect CivetWeb. See also [SECURITY.md](https://github.com/civetweb/civetweb/blob/master/SECURITY.md).



Closing Issues
---

Feel free to create a GitHub issue also for questions, discussions or support requests.
When your question is anwered, please close your issue again - so I know your request is handled.
In some cases I will have to query you for additional information.
If there is no acticity for a question/discussion/support issue for some weeks, I will close this issues.
Issues created for bugs or enhancement requests will not be closed only because some time has passed.



Why does a pull request need a description?
---

I'm asking for this, because I review most pull requests.
The first thing I check is: "What is the intention of the fix **according to the description**?" and "Does the code really fix it?".
Second: "Do I except side effects?".
Third: "Is there a better way to fix the issue **explained in the description**?"
I don't like to "reverse engineer" the intention of the fix from the diff (although it may be obvious to the author of the PR, sometimes it's not for others).
Writing a description will also help you to get early feedback if your changes are not doing what you expect, or if there is a much more effective way to reach the same goal.
Finally it will help all other users, since it allows to write better release notes.
