# Find the Math Kernel Library from Intel
#
#  MKL_FOUND - System has MKL
#  MKL_INCLUDE_DIRS - MKL include files directories
#  MKL_SINGLE_SHARED_LIBRARY- The MKL single shared library
#
#  The environment variables INTEL_MKL and INTEL are used to find the library.
#  Everything else is ignored. If MKL is found "-DMKL_ILP64" is added to
#  CMAKE_C_FLAGS and CMAKE_CXX_FLAGS.
#
#  Example usage:
#
#  find_package(MKL)
#  if(MKL_FOUND)
#    target_link_libraries(TARGET ${MKL_LIBRARIES})
#  endif()

# If already in cache, be silent
# if (MKL_INCLUDE_DIRS AND MKL_LIBRARIES AND MKL_INTERFACE_LIBRARY AND
#     MKL_SEQUENTIAL_LAYER_LIBRARY AND MKL_CORE_LIBRARY)
#   set (MKL_FIND_QUIETLY TRUE)
# endif()


set(INT_LIB "mkl_rt")


# if(NOT BUILD_SHARED_LIBS)
#   set(INT_LIB "libmkl_intel_ilp64.a")
#   set(SEQ_LIB "libmkl_sequential.a")
#   set(THR_LIB "libmkl_intel_thread.a")
#   set(COR_LIB "libmkl_core.a")
# else()
#   set(INT_LIB "mkl_intel_ilp64")
#   set(SEQ_LIB "mkl_sequential")
#   set(THR_LIB "mkl_intel_thread")
#   set(COR_LIB "mkl_core")
# endif()

find_path(MKL_INCLUDE_DIR NAMES mkl.h HINTS ${PROJECT_SOURCE_DIR}/external/intel_mkl/include)

MESSAGE(STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
# MESSAGE(STATUS "INTEL_MKL: " $ENV{INTEL_MKL})


find_library(MKL_SINGLE_SHARED_LIBRARY
  NAMES ${INT_LIB}
  PATHS ${PROJECT_SOURCE_DIR}/external/intel_mkl/lib
  NO_DEFAULT_PATH
  )


MESSAGE(STATUS "INTEL_INCLUDE_DIR: " ${MKL_INCLUDE_DIR})
MESSAGE(STATUS "INTEL_SINGLE_SHARED_LIBRARY: " ${MKL_SINGLE_SHARED_LIBRARY})
MESSAGE(STATUS "INTEL_MKL lib: " $ENV{INTEL_MKL_LIB})
# find_library(MKL_INTERFACE_LIBRARY
#              NAMES ${INT_LIB}
#              PATHS ${INTEL_MKL}/lib
#                    ${INTEL_MKL}/lib/intel64
#                    ${INTEL}/mkl/lib/intel64
#              NO_DEFAULT_PATH)

# find_library(MKL_SEQUENTIAL_LAYER_LIBRARY
#              NAMES ${SEQ_LIB}
#              PATHS ${INTEL_MKL}/lib
#                    ${INTEL_MKL}/lib/intel64
#                    ${INTEL}/mkl/lib/intel64
#              NO_DEFAULT_PATH)

# find_library(MKL_CORE_LIBRARY
#              NAMES ${COR_LIB}
#              PATHS ${INTEL_MKL}/lib
#                    ${INTEL_MKL}/lib/intel64
#                    ${INTEL}/mkl/lib/intel64
#              NO_DEFAULT_PATH)

set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
set(MKL_LIBRARIES ${MKL_SINGLE_SHARED_LIBRARY})


# set(MKL_LIBRARIES ${MKL_INTERFACE_LIBRARY} ${MKL_SEQUENTIAL_LAYER_LIBRARY} ${MKL_CORE_LIBRARY})

# if (MKL_INCLUDE_DIR AND
#     MKL_INTERFACE_LIBRARY AND
#     MKL_SEQUENTIAL_LAYER_LIBRARY AND
#     MKL_CORE_LIBRARY)

#     if (NOT DEFINED ENV{CRAY_PRGENVPGI} AND
#         NOT DEFINED ENV{CRAY_PRGENVGNU} AND
#         NOT DEFINED ENV{CRAY_PRGENVCRAY} AND
#         NOT DEFINED ENV{CRAY_PRGENVINTEL})
#       set(ABI "-m64")
#     endif()

#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMKL_ILP64 ${ABI}")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMKL_ILP64 ${ABI}")

# else()

#   set(MKL_INCLUDE_DIRS "")
#   set(MKL_LIBRARIES "")
#   set(MKL_INTERFACE_LIBRARY "")
#   set(MKL_SEQUENTIAL_LAYER_LIBRARY "")
#   set(MKL_CORE_LIBRARY "")

# endif()

# Handle the QUIETLY and REQUIRED arguments and set MKL_FOUND to TRUE if
# all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MKL DEFAULT_MSG MKL_LIBRARIES MKL_INCLUDE_DIRS)
# FIND_PACKAGE_HANDLE_STANDARD_ARGS(MKL DEFAULT_MSG MKL_LIBRARIES MKL_INCLUDE_DIRS MKL_INTERFACE_LIBRARY MKL_SEQUENTIAL_LAYER_LIBRARY MKL_CORE_LIBRARY)

MARK_AS_ADVANCED(MKL_INCLUDE_DIRS MKL_LIBRARIES)
# MARK_AS_ADVANCED(MKL_INCLUDE_DIRS MKL_LIBRARIES MKL_INTERFACE_LIBRARY MKL_SEQUENTIAL_LAYER_LIBRARY MKL_CORE_LIBRARY)
