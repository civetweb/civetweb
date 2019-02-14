#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
from cpt.packager import ConanMultiPackager
from cpt.ci_manager import CIManager
from cpt.printer import Printer
from cpt import tools


class BuilderSettings(object):

    def __init__(self):
        self._ci_manager = CIManager(Printer(None))

    @property
    def branch(self):
        """ Get branch name
        """
        return self._ci_manager.get_branch()

    @property
    def is_tag(self):
        """ Is the current branch a tag
        """
        return self._ci_manager.is_tag()

    @property
    def commit_id(self):
        """ Get current commit hash
        """
        return self._ci_manager.get_commit_id()[:7]

    @property
    def channel(self):
        """ Get channel name
        """
        return "stable" if self.is_tag else "testing"

    @property
    def username(self):
        """ Set civetweb as package's owner
        """
        return os.getenv("CONAN_USERNAME", "civetweb")

    @property
    def upload(self):
        """ Set civetweb repository to be used on upload.
            The upload server address could be customized by env var
            CONAN_UPLOAD. If not defined, the method will check if the
            current branch is a tag.
        """
        if os.getenv("CONAN_UPLOAD", None) is not None:
            return os.getenv("CONAN_UPLOAD")

        if self.is_tag:
            return "https://api.bintray.com/conan/civetweb/civetweb"

        return None

    @property
    def upload_only_when_stable(self):
        """ Force to upload when match stable pattern branch
        """
        return os.getenv("CONAN_UPLOAD_ONLY_WHEN_STABLE", "true").lower() in ["true", "1", "y", "yes"]

    @property
    def version(self):
        """ Return the package version based on branch name or
            the commit id if it is not a tag.
        """
        if self.is_tag:
            return re.match(r"v(\d+\.\d+)", self.branch).group(1)
        return self.commit_id

    @property
    def reference(self):
        """ Read project version from branch name to create Conan referece
        """
        return os.getenv("CONAN_REFERENCE", "civetweb/{}".format(self.version))

if __name__ == "__main__":
    settings = BuilderSettings()
    builder = ConanMultiPackager(
        reference=settings.reference,
        username=settings.username,
        upload=settings.upload,
        upload_only_when_stable=settings.upload_only_when_stable,
        channel=settings.channel,
        test_folder=os.path.join("conan", "test_package"))
    builder.add_common_builds(pure_c=False)
    builder.run()
