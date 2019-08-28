from conans import ConanFile, CMake, tools
import os

class smeltConan(ConanFile):
    name = "smelt"
    version = "1.1.0"
    description = "Stochastic, Modular, and Extensible Library for Time histories"
    license = "BSD 2-Clause"
    author = "Michael Gardner mhgardner@berkeley.edu"
    url = "https://github.com/NHERI-SimCenter/smelt"
    settings = {"os": None, "build_type": None, "compiler": None, "arch": ["x86_64"]}
    options = {"shared": [True, False]}
    default_options = {"shared": True}    
    generators = "cmake"
    # build_policy = "missing"    
    requires = "mkl-include/2019.4@simcenter/stable", \
               "mkl-shared/2019.4@simcenter/stable", \
               "mkl-static/2019.4@simcenter/stable", \
               "ipp-include/2019.4@simcenter/stable", \
               "ipp-shared/2019.4@simcenter/stable", \
               "ipp-static/2019.4@simcenter/stable", \
               "intel-openmp/2019.4@simcenter/stable"

    # Custom attributes for Bincrafters recipe conventions
    _source_subfolder = "source_subfolder"
    _build_subfolder = "build_subfolder"

    def source(self):
       git = tools.Git(folder="smelt")
       git.clone("https://github.com/shellshocked2003/smelt.git", "stable/1.1.0")        
    
    def configure_cmake(self):
        cmake = CMake(self)
        
        # put definitions here so that they are re-used in cmake between
        # build() and package()
        if self.options.shared == "True":
            cmake.definitions["BUILD_SHARED_LIBS"] = "ON"
            cmake.definitions["BUILD_STATIC_LIBS"] = "OFF"
        else:
            cmake.definitions["BUILD_SHARED_LIBS"] = "OFF"
            cmake.definitions["BUILD_STATIC_LIBS"] = "ON"

        cmake.configure(source_folder="smelt")
        return cmake
    
    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

        if self.settings.os == "Macos":
            if self.options.shared:
                with tools.environment_append({"DYLD_LIBRARY_PATH": [os.getcwd() + "/lib"]}):
                    self.run("DYLD_LIBRARY_PATH=%s ctest --verbose" % os.environ['DYLD_LIBRARY_PATH'])
            else:
                self.run("ctest --verbose")                
        elif self.settings.os == "Windows":
            if self.settings.build_type == "Release":
                self.run("ctest -C Release --verbose")
            else:
                self.run("ctest -C Debug --verbose")
        else:
            self.run("ctest --verbose")

    def package(self):
        self.copy(pattern="LICENSE", dst="licenses", src=self._source_subfolder)
        
        include_folder = os.path.join(self._source_subfolder, "include")       
        self.copy("*.h", dst="include", src=include_folder)        
        if self.settings.build_type == "Release":
            if self.options.shared == "True":
                self.copy("*.dll", dst="bin", keep_path=False)
                self.copy("*.lib", dst="lib", keep_path=False)
                self.copy("*.so", dst="lib", keep_path=False)
                self.copy("*.dylib", dst="lib", keep_path=False)
            else:
                self.copy("*.a", dst="lib", keep_path=False)
                self.copy("*.lib", dst="lib", keep_path=False)

        else:
            if self.options.shared == "True":
                self.copy("*.dll", dst="bin", keep_path=False)
                self.copy("*.lib", dst="lib", keep_path=False)
                self.copy("*.so", dst="lib", keep_path=False)
                self.copy("*.dylib", dst="lib", keep_path=False)
            else:
                self.copy("*.a", dst="lib", keep_path=False)
                self.copy("*.lib", dst="lib", keep_path=False)            

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.env_info.PATH.append(os.path.join(self.package_folder, "bin"))
        # Add to path so shared objects can be found        
        if self.options.shared:
            self.env_info.PATH.append(os.path.join(self.package_folder, "lib"))
            self.env_info.PATH.append(os.path.join(self.package_folder, "bin"))                        
            self.env_info.LD_LIBRARY_PATH.append(os.path.join(self.package_folder, "lib"))
            self.env_info.DYLD_LIBRARY_PATH.append(os.path.join(self.package_folder, "lib"))

        else:
            if self.settings.os == "Linux":
                # linker flags
                if self.settings.compiler == "gcc":
                    self.cpp_info.exelinkflags = ["-static-libgcc", "-static-libstdc++", "-lpthread", "-lm", "-ldl"]
                else:
                    self.cpp_info.exelinkflags = ["-static-libstdc++", "-lpthread", "-lm", "-ldl"]
        # C++ compilation flags
        self.cpp_info.cxxflags = ["-DMKL_ILP64", "-m64"]
