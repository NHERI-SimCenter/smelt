from conans import ConanFile, CMake, tools

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
    build_policy = "missing"    
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

    def build_id(self):
        self.info_build.settings.build_type = "Any"            

    def package(self):
        if self.settings.build_type == "Debug":
            self.copy("*.h", dst="include", src="include")

            if self.options.shared == "True":
                self.copy("*.dll", dst="bin", keep_path=False)
                self.copy("*.so", dst="lib", keep_path=False)
                self.copy("*.dylib", dst="lib", keep_path=False)
            else:
                self.copy("*.a", dst="lib", keep_path=False)
                self.copy("*.lib", dst="lib", keep_path=False)

        else:
            self.copy("*.h", dst="include", src="include")

            if self.options.shared == "True":
                self.copy("*.dll", dst="bin", keep_path=False)
                self.copy("*.so", dst="lib", keep_path=False)
                self.copy("*.dylib", dst="lib", keep_path=False)
            else:
                self.copy("*.a", dst="lib", keep_path=False)
                self.copy("*.lib", dst="lib", keep_path=False)            

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
