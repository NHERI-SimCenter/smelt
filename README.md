# smelt
**S**tochastic, **M**odular, and **E**xtensible **L**ibrary for **T**ime histories

## Package Status

| Documentation | License | DOI | Bintray | Windows | Linux & macOS |
|:--------:|:---------:|:---------:|:-------------:|:--------------:|:-----------------:|
|[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://nheri-simcenter.github.io/smelt/)|[![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://raw.githubusercontent.com/NHERI-SimCenter/smelt/master/LICENSE)|[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.2697656.svg)](https://doi.org/10.5281/zenodo.2697656)|[ ![Download](https://api.bintray.com/packages/nheri-simcenter/simcenter/smelt%3Asimcenter/images/download.svg) ](https://bintray.com/nheri-simcenter/simcenter/smelt%3Asimcenter/_latestVersion)|[![Build status](https://ci.appveyor.com/api/projects/status/rcal46kf9i84b83k?svg=true)](https://ci.appveyor.com/project/shellshocked2003/smelt)|[![Build Status](https://travis-ci.org/shellshocked2003/smelt.svg?branch=master)](https://travis-ci.org/shellshocked2003/smelt)|

## What is `smelt`?

`smelt` is a C++ library for stochastically generating time histories for different types of
natural hazards&mdash;for example, acceleration time histories for earthquake scenarios or velocity
time histories for wind loading. The library has been designed with modularity and extensiblity
in mind such that new models can be easily added, making them available in other software
that uses `smelt`.

More detailed information, including building instructions, are available in the [repository wiki](https://github.com/NHERI-SimCenter/smelt/wiki).

## Requirements

`smelt` requires the following packages:
- [Intel Math Kernel Library (MKL)](https://software.intel.com/en-us/mkl)
- [Intel Integrated Performance Primitives (IPP)](https://software.intel.com/en-us/intel-ipp)

Mac and Windows users can install them through [`conda`](https://docs.conda.io/en/latest/) while Ubuntu users can install them using `apt`.
Otherwise, these packages can alse be downloaded directly from Intel and installed manually. Currently, `smelt` has been tested on the following system configurations:

- Ubuntu 18.04 with GCC 7.3.0
- Ubuntu 16.04
  - GCC 7.4.0
  - Clang 7.0.0
- MacOS 10.14 with Xcode 10.2
- MacOS 10.13 with GCC 7.4.0
- Windows 10 with Visual Studio 19
- Windows Server 2016 with Visual Studio 17

## License

`smelt` is distributed under the [BSD 2-Clause license](https://raw.githubusercontent.com/NHERI-SimCenter/smelt/master/LICENSE).

## Acknowledgement

This material is based upon work supported by the National Science Foundation under Grant No. 1612843.

## Contact

Michael Gardner, NHERI SimCenter, UC Berkeley, mhgardner@berkeley.edu

Please visit the [NHERI Simcenter](https://simcenter.designsafe-ci.org/) webpage for more information on the SimCenter, including other SimCenter applications, FAQ, and how to collaborate.
