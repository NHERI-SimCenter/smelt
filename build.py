#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess
from bincrafters import build_template_default

if __name__ == "__main__":

    # subprocess.run(["conan", "profile", "new", "default", "--detect"])
    # subprocess.run(["conan", "profile", "update", "settings.compiler.libcxx=libstdc++11", "default"])    
    
    builder = build_template_default.get_builder(build_types=["Release", "Debug"], archs=["x86_64"])
    builder.run()
