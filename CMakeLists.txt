cmake_minimum_required(VERSION 3.0)

project(StochasticModule LANGUAGES CXX)

# Require C++11-compliant compiler
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OF)

# General compile settings
IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
 #SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE)

# Provide option for building testing
option(BUILD_TESTING "Enable testing for stochastic module" ON)

# Set path for external dependencies
set(CATCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/catch)
set(EIGEN_INCLUDE_DIR, ${PROJECT_SOURCE_DIR}/external/Eigen)
set(TROMPELOEIL_INCLUDE_DIR, ${PROJECT_SOURCE_DIR}/external/trompeloeil)
set(BOOST_INCLUDE_DIR, ${PROJECT_SOURCE_DIR}/external/boost)
set(KFR_INCLUDE_DIR, ${PROJECT_SOURCE_DIR}/external/kfr-1.2.0/include)

# Add external headers to include path
include_directories(${CATCH_INCLUDE_DIR})
include_directories(${EIGEN_INCLUDE_DIR})
include_directories(${TROMPELOEIL_INCLUDE_DIR})
include_directories(${BOOST_INCLUDE_DIR})
include_directories(${KFR_INCLUDE_DIR})

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
  )

add_library(lstoc SHARED ${SOURCES})

# Unit testing
if (BUILD_TESTING)
  set(TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/test/test_main.cc
    ${PROJECT_SOURCE_DIR}/test/multi_var_tests.cc
    ${PROJECT_SOURCE_DIR}/test/factory_tests.cc
    ${PROJECT_SOURCE_DIR}/test/distribution_tests.cc
  )

  add_executable(unit_tests ${TEST_SOURCES})
  target_link_libraries(unit_tests lstoc)
  add_test(NAME run_unit_tests COMMAND unit_tests)
  enable_testing()
endif()  
