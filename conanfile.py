from conans import ConanFile, CMake

class smeltConan(ConanFile):
    name = "smelt"
    version = "1.1"
    description = "Stochastic, Modular, and Extensible Library for Time histories"
    license = "BSD 2-Clause"
    author = "Michael Gardner mhgardner@berkeley.edu"
    url = "https://github.com/NHERI-SimCenter/smelt"
    settings = "os", "arch", "compiler", "build_type"
    options = {"shared": [True, False]}
    default_options = {"shared": False}    
    generators = "cmake"
    exports_sources = "src/*", "include/*", "CMakeLists.txt", "cmake/*", "test/*", "external/*"
    requires = "mkl-include/2019.4@simcenter/stable", \
               "mkl-shared/2019.4@simcenter/stable", \
               "mkl-static/2019.4@simcenter/stable", \
               "ipp-include/2019.4@simcenter/stable", \
               "ipp-shared/2019.4@simcenter/stable", \
               "ipp-static/2019.4@simcenter/stable", \
               "intel-openmp/2019.4@simcenter/stable"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["smelt"]
