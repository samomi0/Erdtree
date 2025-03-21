import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake

class Erdtree(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
      self.requires("yaml-cpp/0.8.0")
      self.requires("spdlog/1.15.1")
      self.requires("fmt/11.1.3")
      

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def layout(self):
        self.folders.source = "."
        self.folders.build = os.path.join("build", str(self.settings.build_type))
        self.folders.generators = os.path.join(self.folders.build, "generators")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()