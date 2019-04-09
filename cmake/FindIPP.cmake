# Find Integrated Performance Primitives from Intel

set(IPP_LIB "ippcore" "ippvm" "ipps")

# For Linux and Mac
function(ipp_libs_unix)
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

# For Windows. NOTE: Assumes conda was used to install IPP
function(ipp_libs_windows)
  foreach(s ${ARGV})
    find_library(${s}_LIBRARY
      NAMES ${s}
      PATHS $ENV{IPPROOT}/lib
      $ENV{IPPROOT}/lib/intel64
      $ENV{IPPROOT}/../compiler/lib/intel64
      $ENV{IPPROOT}/Library/bin      
      NO_DEFAULT_PATH)
    if(NOT ${s}_LIBRARY)
      message(FATAL_ERROR "NOT FOUND: " ${s})
    endif()
  
    list(APPEND IPP_FOUND_LIBRARIES ${${s}_LIBRARY})
  endforeach()
 
  set(IPP_INCLUDE_DIRS $ENV{IPPROOT}/include PARENT_SCOPE)
  set(IPP_LIBRARIES ${IPP_FOUND_LIBRARIES} PARENT_SCOPE)

endfunction()


if(WIN32)
 ipp_libs_windows(${IPP_LIB})  
endif()

if(NOT WIN32)
  ipp_libs_unix(${IPP_LIB})
endif()
