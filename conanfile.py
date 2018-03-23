from conans import ConanFile, tools, CMake
import os

class civetwebConan(ConanFile):
    name = "civetweb"
    version = "1.10"
    license = "MIT"
    url = "https://github.com/civetweb/civetweb"
    description = "Embedded C/C++ web server"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*"
    requires = "OpenSSL/1.0.2@conan/stable"
    generators = "cmake"
    options = {
        "shared"            : [True, False],
        "enable_ssl"        : [True, False],
        "enable_websockets" : [True, False],
        "enable_cxx"        : [True, False]
    }
    default_options = (
        "shared=False",
        "enable_ssl=True",
        "enable_websockets=True",
        "enable_cxx=True",
    )

    def optionBool(self, b):
        if b:
            return "ON"
        else:
            return "OFF"

    def parseOptionsToCMake(self):
        cmakeOpts = {
            "CIVETWEB_BUILD_TESTING" : "OFF",
            "CIVETWEB_ENABLE_LUA" : "OFF",
            "CIVETWEB_ENABLE_SERVER_EXECUTABLE" : "OFF",
            "CIVETWEB_INSTALL_EXECUTABLE" : "OFF",
            "CIVETWEB_ENABLE_ASAN" : "OFF"
        }

        cmakeOpts["BUILD_SHARED_LIBS"] = self.optionBool(self.options.shared)
        cmakeOpts["CIVETWEB_ENABLE_SSL"] = self.optionBool(self.options.enable_ssl)
        cmakeOpts["CIVETWEB_ENABLE_WEBSOCKETS"] = self.optionBool(self.options.enable_websockets)
        cmakeOpts["CIVETWEB_ENABLE_CXX"] = self.optionBool(self.options.enable_cxx)

        return cmakeOpts

    def build(self):
        cmake = CMake(self)
        os.makedirs("./buildit")
        cmake.configure(defs=self.parseOptionsToCMake(), build_dir="./buildit")
        cmake.build()
        cmake.install()

    def package(self):
        # nothing to do here now
        pass

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
