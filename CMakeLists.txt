cmake_minimum_required(VERSION 3.0)

project(StochasticModule LANGUAGES CXX)

# Require C++11-compliant compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OF)

include(CMakeParseArguments)

# General compile settings
IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
 #SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE)

# Provide option for building testing
option(BUILD_TESTING "Enable testing for stochastic module" ON)
option(BUILD_STATIC_LIBS "Build the static library" ON)
option(BUILD_SHARED_LIBS "Build the shared library" ON)

# CMake Modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Add link library and include directories for MKL and IPP
find_package(MKL)
find_package(IPP)

# Set path for external dependencies
set(JSON_INCLUDE_DIR, ${PROJECT_SOURCE_DIR}/external/nlohmann_json)
set(CATCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/catch)
set(EIGEN_INCLUDE_DIR, ${PROJECT_SOURCE_DIR}/external/Eigen)
set(TROMPELOEIL_INCLUDE_DIR, ${PROJECT_SOURCE_DIR}/external/trompeloeil)
set(BOOST_INCLUDE_DIR, ${PROJECT_SOURCE_DIR}/external/boost)

# Add external headers to include path
include_directories(${EIGEN_INCLUDE_DIR})
include_directories(${BOOST_INCLUDE_DIR})
include_directories(${JSON_INCLUDE_DIR})

if (BUILD_STATIC_LIBS)
  include_directories(${mkl_static_INCLUDE_DIRS})
  include_directories(${ipp_static_INCLUDE_DIRS})
endif()

if (BUILD_SHARED_LIBS)
  include_directories(${mkl_shared_INCLUDE_DIRS})
  include_directories(${ipp_shared_INCLUDE_DIRS})
endif()

# Include directories
include_directories(BEFORE
	${PROJECT_SOURCE_DIR}/include/
	${PROJECT_SOURCE_DIR}/external/
	)

# Set sources
set(SOURCES
  ${PROJECT_SOURCE_DIR}/src/numeric_utils.cc
  ${PROJECT_SOURCE_DIR}/src/normal_multivar.cc
  ${PROJECT_SOURCE_DIR}/src/normal_dist.cc
  ${PROJECT_SOURCE_DIR}/src/lognormal_dist.cc
  ${PROJECT_SOURCE_DIR}/src/beta_dist.cc
  ${PROJECT_SOURCE_DIR}/src/inv_gauss_dist.cc
  ${PROJECT_SOURCE_DIR}/src/students_t_dist.cc
  ${PROJECT_SOURCE_DIR}/src/json_object.cc
  ${PROJECT_SOURCE_DIR}/src/vlachos_et_al.cc
  ${PROJECT_SOURCE_DIR}/src/configure.cc
  ${PROJECT_SOURCE_DIR}/src/wittig_sinha.cc
  ${PROJECT_SOURCE_DIR}/src/filter.cc
  ${PROJECT_SOURCE_DIR}/src/wind_profile.cc  
  )

# Add library as target and add libraries to link target to
if (BUILD_STATIC_LIBS)
  add_library(smelt_static STATIC ${SOURCES})
  set_target_properties(smelt_static PROPERTIES OUTPUT_NAME smelt)
  target_link_libraries(smelt_static ${mkl_static_LIBRARIES} ${ipp_static_LIBRARIES})
endif()

if (BUILD_SHARED_LIBS)
  add_library(smelt_shared SHARED ${SOURCES})
  set_target_properties(smelt_shared PROPERTIES OUTPUT_NAME smelt)
  target_link_libraries(smelt_shared ${mkl_shared_LIBRARIES} ${ipp_shared_LIBRARIES})
endif()

# Adding MATH defines for M_PI when building on Windows
if (WIN32)  
  add_compile_definitions(_USE_MATH_DEFINES)
endif()

# Unit testing
if (BUILD_TESTING)
  include_directories(${CATCH_INCLUDE_DIR})
  include_directories(${TROMPELOEIL_INCLUDE_DIR})
  
  set(TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/test/test_main.cc
    ${PROJECT_SOURCE_DIR}/test/multi_var_tests.cc
    ${PROJECT_SOURCE_DIR}/test/factory_tests.cc
    ${PROJECT_SOURCE_DIR}/test/distribution_tests.cc
    ${PROJECT_SOURCE_DIR}/test/window_func_tests.cc
    ${PROJECT_SOURCE_DIR}/test/dispatcher_tests.cc
    ${PROJECT_SOURCE_DIR}/test/numeric_utils_tests.cc
    ${PROJECT_SOURCE_DIR}/test/filter_func_tests.cc
    ${PROJECT_SOURCE_DIR}/test/json_object_tests.cc
    ${PROJECT_SOURCE_DIR}/test/stochastic_model_tests.cc
    ${PROJECT_SOURCE_DIR}/test/wind_profile_tests.cc
  )

  if (BUILD_STATIC_LIBS)
    add_executable(unit_tests_static ${TEST_SOURCES})
    target_link_libraries(unit_tests_static smelt_static -static-libgcc -static-libstdc++)
    add_test(NAME run_static_unit_tests COMMAND unit_tests_static)
  endif()

  if (BUILD_SHARED_LIBS)
    add_executable(unit_tests_shared ${TEST_SOURCES})
    target_link_libraries(unit_tests_shared smelt_shared)
    add_test(NAME run_shared_unit_tests COMMAND unit_tests_shared)
  endif()

  enable_testing()
endif()  
