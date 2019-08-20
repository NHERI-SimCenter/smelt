#!/bin/bash

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
  mkdir build
  cd build
  conan remote add simcenter https://api.bintray.com/conan/nheri-simcenter/simcenter
  conan install ..
  cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=ON
  make
  ctest --verbose
  rm -rf *
  cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=ON
  make
  ctest --verbose  
else
  mkdir build
  cd build
  conan remote add simcenter https://api.bintray.com/conan/nheri-simcenter/simcenter
  conan install ..  
  cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=ON
  make
  ctest --verbose
  rm -rf *
  cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=ON
  make
  ctest --verbose  
  cd ..
  doxygen Doxyfile
fi
