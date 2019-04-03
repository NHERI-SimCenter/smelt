# Find Integrated Performance Primitives from Intel

set(IPP_LIB "ippcore" "ippvm" "ipps")

function(ipp_libs)
  foreach(s ${ARGV})
    find_library(${s}_LIBRARY
      NAMES ${s}
      PATHS $ENV{IPPROOT}/lib
      $ENV{IPPROOT}/lib/intel64
      $ENV{IPPROOT}/../compiler/lib/intel64
      NO_DEFAULT_PATH)
    if(NOT ${s}_LIBRARY)
      message(FATAL_ERROR "NOT FOUND: " ${s})
    endif()
  
    list(APPEND IPP_FOUND_LIBRARIES ${${s}_LIBRARY})
  endforeach()
 
  set(IPP_INCLUDE_DIRS $ENV{IPPROOT}/include PARENT_SCOPE)
  set(IPP_LIBRARIES ${IPP_FOUND_LIBRARIES} PARENT_SCOPE)

endfunction()

ipp_libs(${IPP_LIB})
