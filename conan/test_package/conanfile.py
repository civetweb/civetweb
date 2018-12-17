#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import subprocess
import requests
import time
from conans import ConanFile, CMake, tools, RunEnvironment


class TestPackageConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        assert os.path.isfile(os.path.join(self.deps_cpp_info["civetweb"].rootpath, "licenses", "LICENSE.md"))
        bin_path = os.path.join(os.getcwd(), "bin", "test_package")
        with tools.environment_append(RunEnvironment(self).vars):
            process = subprocess.Popen([bin_path], shell=True)
            time.sleep(2)
            response = requests.get("http://localhost:8080/example")
            assert response.ok
            process.kill()
