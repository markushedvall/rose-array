from conans import ConanFile, CMake, tools


class RoseArrayConan(ConanFile):
    name = "rose-array"
    version = "0.1.0"
    license = "MIT OR Apache-2.0"
    url = "https://github.com/markushedvall/rose-array"
    description = "Single header C++ library for arrays and fixed max sized vectors"
    build_requires = "gtest/1.8.0@bincrafters/stable"
    settings = "os", "compiler", "arch", "build_type"
    generators = "cmake"
    exports_sources = "include/*", "test/*", "CMakeLists.txt"
    no_copy_source = True
    requires = "rose-slice/0.1.0@markushedvall/stable"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
