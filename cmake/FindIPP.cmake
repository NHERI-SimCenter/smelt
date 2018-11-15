# Find Integrated Performance Primitives from Intel
#
#  IPP_FOUND - System has IPP
#  IPP_INCLUDE_DIRS - IPP include files directories
#  IPP_CORE_LIB - IPP core library
#  IPP_VM_LIB - IPP vector math library
#
#  Example usage:
#
#  find_package(IPP)
#  if(IPP_FOUND)
#    target_link_libraries(TARGET ${IPP_LIBRARIES})
#  endif()


set(CORE_LIB "ippcore")
set(VM_LIB "ippvm")
set(S_LIB "ipps")

find_path(IPP_INCLUDE_DIR NAMES ipps.h HINTS ${PROJECT_SOURCE_DIR}/external/intel_ipp/include)

find_library(IPP_CORE_LIB
  NAMES ${CORE_LIB}
  PATHS ${PROJECT_SOURCE_DIR}/external/intel_ipp/lib
  NO_DEFAULT_PATH
  )

find_library(IPP_VM_LIB
  NAMES ${VM_LIB}
  PATHS ${PROJECT_SOURCE_DIR}/external/intel_ipp/lib
  NO_DEFAULT_PATH
  )

find_library(IPP_S_LIB
  NAMES ${S_LIB}
  PATHS ${PROJECT_SOURCE_DIR}/external/intel_ipp/lib
  NO_DEFAULT_PATH
  )

set(IPP_INCLUDE_DIRS ${IPP_INCLUDE_DIR})
set(IPP_LIBRARIES ${IPP_CORE_LIB} ${IPP_VM_LIB} ${IPP_S_LIB})
