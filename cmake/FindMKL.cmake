# Find the Math Kernel Library from Intel

set(MKL_LIB "mkl_rt")

# For Linux and Mac
function(mkl_libs_unix)
  foreach(s ${ARGV})
    find_library(${s}_LIBRARY
      NAMES ${s}
      PATHS $ENV{MKLROOT}/lib
      $ENV{MKLROOT}/lib/intel64
      $ENV{MKLROOT}/../compiler/lib/intel64
      NO_DEFAULT_PATH)
    if(NOT ${s}_LIBRARY)
      message(FATAL_ERROR "NOT FOUND: " ${s})
    endif()      
    
    list(APPEND MKL_FOUND_LIBRARIES ${${s}_LIBRARY})
  endforeach()

  set(MKL_INCLUDE_DIRS $ENV{MKLROOT}/include PARENT_SCOPE)
  set(MKL_LIBRARIES ${MKL_FOUND_LIBRARIES} PARENT_SCOPE)    
 
endfunction()

# For Windows. NOTE: Assumes conda was used to install MKL
function(mkl_libs_windows)
  set(CMAKE_FIND_LIBRARY_PREFIXES "")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")  
  foreach(s ${ARGV})
    find_library(${s}_LIBRARY
      NAMES ${s}
      PATHS $ENV{MKLROOT}/lib
      $ENV{MKLROOT}/lib/intel64
      $ENV{MKLROOT}/../compiler/lib/intel64
      $ENV{MKLROOT}/Library/bin
      NO_DEFAULT_PATH)
    if(NOT ${s}_LIBRARY)
      message(FATAL_ERROR "NOT FOUND: " ${s})
    endif()     
    
    list(APPEND MKL_FOUND_LIBRARIES ${${s}_LIBRARY})
  endforeach()
  
  if(WIN32)
    set(MKL_INCLUDE_DIRS $ENV{MKLROOT}/Library/include PARENT_SCOPE)
    set(MKL_LIBRARIES ${MKL_FOUND_LIBRARIES} PARENT_SCOPE)      
  endif()

endfunction()

if(WIN32)
 mkl_libs_windows(${MKL_LIB})  
endif()

if(NOT WIN32)
  mkl_libs_unix(${MKL_LIB})
endif()
