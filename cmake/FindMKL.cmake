# Find the Math Kernel Library from Intel
#
#  MKL_FOUND - System has MKL
#  MKL_INCLUDE_DIRS - MKL include files directories
#  MKL_SINGLE_SHARED_LIBRARY- The MKL single shared library
#
#  Example usage:
#
#  find_package(MKL)
#  if(MKL_FOUND)
#    target_link_libraries(TARGET ${MKL_LIBRARIES})
#  endif()


set(INT_LIB "mkl_rt")

find_path(MKL_INCLUDE_DIR NAMES mkl.h HINTS ${PROJECT_SOURCE_DIR}/external/intel_mkl/include)

find_library(MKL_SINGLE_SHARED_LIBRARY
  NAMES ${INT_LIB}
  PATHS ${PROJECT_SOURCE_DIR}/external/intel_mkl/lib
  NO_DEFAULT_PATH
  )

set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
set(MKL_LIBRARIES ${MKL_SINGLE_SHARED_LIBRARY})
