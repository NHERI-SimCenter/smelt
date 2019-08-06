include(HelperFunctions)
# Find Integrated Performance Primitives from Intel

set(IPP_LIB "ippcore" "ippvm" "ipps")


if (BUILD_STATIC_LIBS)
  if(WIN32)
    add_ipp_dependency_windows(NAME ipp_static REQD_LIBS ${IPP_LIB} STATIC)
  else()
    add_ipp_dependency_unix(NAME ipp_static REQD_LIBS ${IPP_LIB} STATIC)    
  endif()
endif()

if (BUILD_SHARED_LIBS)
  if(WIN32)
    add_ipp_dependency_windows(NAME ipp_shared REQD_LIBS ${IPP_LIB})
  else()
    add_ipp_dependency_unix(NAME ipp_shared REQD_LIBS ${IPP_LIB})
  endif()  
endif()
