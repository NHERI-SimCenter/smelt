# Sets the library linker flags and includes for MKL on Mac/Linux
#
#   NAME      [required]    Name to store library locations and linker flags to
#   REQD_LIBS [required]    List of libraries that should be included
#   STATIC    [optional]    Generate static library. If not specified, will generate shared
#
#  Example:
#     add_mkl_dependency(NAME mkl_static REQD_LIBS mkl_intel_lp64 mkl_gnu_thread STATIC)

function(add_mkl_dependency_unix)
    set(options STATIC)
    set(oneValueArgs NAME)
    set(multiValueArgs REQD_LIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if( NOT ARG_NAME )
      message ( FATAL_ERROR "add_mkl_dependency_unix called without a NAME" )
    endif()

    if (ARG_STATIC)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    else()
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
    endif()

    foreach(s ${ARG_REQD_LIBS})
      find_library(${s}_LIBRARY
	NAMES ${s}
	PATHS $ENV{MKLROOT}/lib
	$ENV{MKLROOT}/lib/intel64
	$ENV{MKLROOT}/../compiler/lib/intel64
	NO_DEFAULT_PATH)
      if(NOT ${s}_LIBRARY)
	message(FATAL_ERROR "NOT FOUND: " ${s})
      endif()      

      list(APPEND ${ARG_NAME}_FOUND_LIBRARIES ${${s}_LIBRARY})
    endforeach()

    set(${ARG_NAME}_INCLUDE_DIRS $ENV{MKLROOT}/include PARENT_SCOPE)

    if (ARG_STATIC)
      set(${ARG_NAME}_LIBRARIES "-Wl,--start-group" ${${ARG_NAME}_FOUND_LIBRARIES} "-Wl,--end-group" "-fopenmp" "-lgomp" "-lpthread" "-lm" "-ldl" PARENT_SCOPE)
    else()
      set(${ARG_NAME}_LIBRARIES ${${ARG_NAME}_FOUND_LIBRARIES} PARENT_SCOPE)
    endif()
endfunction(add_mkl_dependency_unix)

# Sets the library linker flags and includes for MKL on Windows
#
#   NAME      [required]    Name to store library locations and linker flags to
#   REQD_LIBS [required]    List of libraries that should be included
#   STATIC    [optional]    Generate static library. If not specified, will generate shared
#
#  Example:
#     add_mkl_dependency(NAME mkl_static REQD_LIBS mkl_intel_lp64 mkl_gnu_thread STATIC)
#
#  NOTE: Assumes conda was used to install MKL

function(add_mkl_dependency_windows)
    set(options STATIC)
    set(oneValueArgs NAME)
    set(multiValueArgs REQD_LIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if( NOT ARG_NAME )
      message ( FATAL_ERROR "add_mkl_dependency_windows called without a NAME" )
    endif()

    set(CMAKE_FIND_LIBRARY_PREFIXES "")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")    

    foreach(s ${ARG_REQD_LIBS})
      find_library(${s}_LIBRARY
	NAMES ${s}
	PATHS $ENV{MKLROOT}/lib
	$ENV{MKLROOT}/lib/intel64
	$ENV{MKLROOT}/../compiler/lib/intel64
	$ENV{MKLROOT}/Library/bin
	$ENV{MKLROOT}/Library/lib
	NO_DEFAULT_PATH)
      if(NOT ${s}_LIBRARY)
	message(FATAL_ERROR "NOT FOUND: " ${s})
      endif()     
      
      list(APPEND ${ARG_NAME}_FOUND_LIBRARIES ${${s}_LIBRARY})
    endforeach()

    if(WIN32)
      if (ARG_STATIC)
	set(${ARG_NAME}_INCLUDE_DIRS $ENV{MKLROOT}/Library/include PARENT_SCOPE)
	set(${ARG_NAME}_LIBRARIES ${${ARG_NAME}_FOUND_LIBRARIES} PARENT_SCOPE)
      else()
	
      endif()
    else()
      message(FATAL_ERROR "Called add_library_dependency_windows from non-Windows machnine")
    endif()    

endfunction(add_mkl_dependency_windows)

# Sets the library linker flags and includes for IPP on Mac/Linux
#
#   NAME      [required]    Name to store library locations and linker flags to
#   REQD_LIBS [required]    List of libraries that should be included
#   STATIC    [optional]    Generate static library. If not specified, will generate shared
#
#  Example:
#     add_ipp_dependency(NAME ipp_static REQD_LIBS ipp_intel_lp64 ipp_gnu_thread STATIC)

function(add_ipp_dependency_unix)
    set(options STATIC)
    set(oneValueArgs NAME)
    set(multiValueArgs REQD_LIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if( NOT ARG_NAME )
      message ( FATAL_ERROR "add_ipp_dependency_unix called without a NAME" )
    endif()

    if (ARG_STATIC)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    else()
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
    endif()

    foreach(s ${ARG_REQD_LIBS})
      find_library(${s}_LIBRARY
	NAMES ${s}
	PATHS $ENV{IPPROOT}/lib
	$ENV{IPPROOT}/lib/intel64
	$ENV{IPPROOT}/../compiler/lib/intel64
	NO_DEFAULT_PATH)
      if(NOT ${s}_LIBRARY)
	message(FATAL_ERROR "NOT FOUND: " ${s})
      endif()      

      list(APPEND ${ARG_NAME}_FOUND_LIBRARIES ${${s}_LIBRARY})
    endforeach()

    set(${ARG_NAME}_INCLUDE_DIRS $ENV{IPPROOT}/include PARENT_SCOPE)

    if (ARG_STATIC)
      set(${ARG_NAME}_LIBRARIES ${${ARG_NAME}_FOUND_LIBRARIES} "-lgomp" "-lpthread" "-lm" "-ldl" PARENT_SCOPE)      
    else()
      set(${ARG_NAME}_LIBRARIES ${${ARG_NAME}_FOUND_LIBRARIES} PARENT_SCOPE)
    endif()
endfunction(add_ipp_dependency_unix)

# Sets the library linker flags and includes for IPP on Windows
#
#   NAME      [required]    Name to store library locations and linker flags to
#   REQD_LIBS [required]    List of libraries that should be included
#   STATIC    [optional]    Generate static library. If not specified, will generate shared
#
#  Example:
#     add_ipp_dependency(NAME ipp_static REQD_LIBS ipp_intel_lp64 ipp_gnu_thread STATIC)
#
#  NOTE: Assumes conda was used to install IPP

function(add_ipp_dependency_windows)
    set(options STATIC)
    set(oneValueArgs NAME)
    set(multiValueArgs REQD_LIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if( NOT ARG_NAME )
      message ( FATAL_ERROR "add_ipp_dependency_windows called without a NAME" )
    endif()

    set(CMAKE_FIND_LIBRARY_PREFIXES "")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")    

    foreach(s ${ARG_REQD_LIBS})
      find_library(${s}_LIBRARY
	NAMES ${s}
	PATHS $ENV{IPPROOT}/lib
	$ENV{IPPROOT}/lib/intel64
	$ENV{IPPROOT}/../compiler/lib/intel64
	$ENV{IPPROOT}/Library/bin
	$ENV{IPPROOT}/Library/lib
	NO_DEFAULT_PATH)
      if(NOT ${s}_LIBRARY)
	message(FATAL_ERROR "NOT FOUND: " ${s})
      endif()     
      
      list(APPEND ${ARG_NAME}_FOUND_LIBRARIES ${${s}_LIBRARY})
    endforeach()

    if(WIN32)
      if (ARG_STATIC)
	set(${ARG_NAME}_INCLUDE_DIRS $ENV{IPPROOT}/Library/include PARENT_SCOPE)
	set(${ARG_NAME}_LIBRARIES ${${ARG_NAME}_FOUND_LIBRARIES} PARENT_SCOPE)
      else()
	
      endif()
    else()
      message(FATAL_ERROR "Called add_library_dependency_windows from non-Windows machnine")
    endif()    

endfunction(add_ipp_dependency_windows)
