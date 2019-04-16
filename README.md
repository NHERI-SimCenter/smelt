# smelt
**S**tochastic, **M**odular, and **E**xtensible **L**ibrary for **T**ime histories

[![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://raw.githubusercontent.com/NHERI-SimCenter/smelt/master/LICENSE)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://nheri-simcenter.github.io/smelt/)
[![Build Status](https://travis-ci.org/NHERI-SimCenter/smelt.svg?branch=master)](https://travis-ci.org/NHERI-SimCenter/smelt)
[![Build Status](https://dev.azure.com/shellshocked2003/shellshocked2003/_apis/build/status/NHERI-SimCenter.smelt?branchName=master)](https://dev.azure.com/shellshocked2003/shellshocked2003/_build/latest?definitionId=2&branchName=master)

## What is `smelt`?

`smelt` is a C++ library for stochastically generating time histories for different types of
natural hazards &mdash for example, acceleration time histories for earthquake scenarios or velocity
time histories for wind loading. The library has been designed with modularity and extensiblity
in mind such that new models can be easily added, making them available other software
that uses `smelt`.

The API documentation can be viewed [here](https://nheri-simcenter.github.io/smelt/).

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
