from conans import ConanFile, CMake, tools
import os

class smeltConan(ConanFile):
    name = "smelt"
    version = "1.2.0"
    description = "Stochastic, Modular, and Extensible Library for Time histories"
    license = "BSD 2-Clause"
    author = "Michael Gardner mhgardner@berkeley.edu"
    url = "https://github.com/NHERI-SimCenter/smelt"
    settings = {"os": None, "build_type": None, "compiler": None, "arch": ["x86_64"]}
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    build_policy = "missing"
    requires = "mkl-include/2019.4@simcenter/stable", \
               "ipp-include/2019.4@simcenter/stable", \
               "eigen/3.3.7@conan/stable", \
               "clara/1.1.5@bincrafters/stable", \
               "jsonformoderncpp/3.7.0@vthiery/stable", \
               "Catch2/2.9.2@catchorg/stable", \
               "boost/1.71.0@conan/stable"

    # Custom attributes for Bincrafters recipe conventions
    _source_subfolder = "source_subfolder"
    _build_subfolder = "build_subfolder"

    # scm = {
    #     "type": "git",  # Use "type": "svn", if local repo is managed using SVN
    #     "subfolder": _source_subfolder,
    #     "url": "auto",
    #     "revision": "auto"
    # }
    
    def source(self):
       git = tools.Git(folder=self._source_subfolder)
       git.clone("https://github.com/shellshocked2003/smelt.git", "stable/1.2.0")        

    def configure(self):
        self.options["boost"].header_only = True            

    def build_requirements(self):
        if self.options.shared:
            self.build_requires("mkl-shared/2019.4@simcenter/stable")
            self.build_requires("ipp-shared/2019.4@simcenter/stable")
            # self.build_requires("intel-openmp/2019.4@simcenter/stable")
        else:
            self.build_requires("mkl-static/2019.4@simcenter/stable")
            self.build_requires("ipp-static/2019.4@simcenter/stable")
            
    def configure_cmake(self):
        cmake = CMake(self, msbuild_verbosity='detailed')
        
        # put definitions here so that they are re-used in cmake between
        # build() and package()
        if self.options.shared == "True":
            cmake.definitions["BUILD_SHARED_LIBS"] = "ON"
            cmake.definitions["BUILD_STATIC_LIBS"] = "OFF"
        else:
            cmake.definitions["BUILD_SHARED_LIBS"] = "OFF"
            cmake.definitions["BUILD_STATIC_LIBS"] = "ON"

        cmake.configure(source_folder=self._source_subfolder)
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
        self.copy("*.h", dst="include", src=self._source_subfolder + "/include")
        self.copy("*.tcc", dst="include", src=self._source_subfolder + "/include")
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
        self.cpp_info.includedirs = ['include']
        self.env_info.PATH.append(os.path.join(self.package_folder, "bin"))
        # Add to path so shared objects can be found        
        if self.options.shared:
            self.env_info.PATH.append(os.path.join(self.package_folder, "lib"))
            self.env_info.PATH.append(os.path.join(self.package_folder, "bin"))                        
            self.env_info.LD_LIBRARY_PATH.append(os.path.join(self.package_folder, "lib"))
            self.env_info.DYLD_LIBRARY_PATH.append(os.path.join(self.package_folder, "lib"))

        # C++ compilation flags
        self.cpp_info.cxxflags = ["-m64"]
