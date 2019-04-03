# Find the Math Kernel Library from Intel

set(MKL_LIB "mkl_rt")

function(mkl_libs)
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

mkl_libs(${MKL_LIB})
