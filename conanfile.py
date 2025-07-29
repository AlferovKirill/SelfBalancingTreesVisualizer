from conan import ConanFile
from conan.tools.cmake import cmake_layout

class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    def layout(self):
        self.folders.generators = "conan"

    def requirements(self):
        self.requires("fmt/11.0.2")
        self.requires("qt/5.15.16")

    def configure(self):
        self.options["qt"].qtdeclarative = True
        self.options["qt"].qtquickcontrols = True
        self.options["qt"].qtquickcontrols2 = True
        self.options["qt"].qtgraphicaleffects = True

    def build_requirements(self):
        self.test_requires("catch2/3.7.1")
