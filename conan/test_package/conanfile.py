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
        bin_path = os.path.join("bin", "test_package")
        run_vars = RunEnvironment(self).vars
        with tools.environment_append(run_vars):
            if self.settings.os == "Macos":
                run_vars["DYLD_LIBRARY_PATH"] = os.environ.get('DYLD_LIBRARY_PATH', '')
            process = subprocess.Popen([bin_path], shell=True, env=run_vars)
            time.sleep(3)
            response = requests.get("http://localhost:8080/example")
            assert response.ok
            process.kill()
            print("Finish Conan test package - SUCCESS!")
