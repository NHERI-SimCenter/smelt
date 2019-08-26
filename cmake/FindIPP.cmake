include(HelperFunctions)
# Find Integrated Performance Primitives from Intel

set(IPP_LIB "ippcore" "ippvm" "ipps")

if (BUILD_STATIC_LIBS)
  if (WIN32)
    set(IPP_LIB_WIN "ippcoremt" "ippvmmt" "ippsmt")
    add_ipp_dependency(NAME ipp_static REQD_LIBS ${IPP_LIB_WIN} STATIC)
  else()
    add_ipp_dependency(NAME ipp_static REQD_LIBS ${IPP_LIB} STATIC)    
  endif()
    
endif()

if (BUILD_SHARED_LIBS)
  add_ipp_dependency(NAME ipp_shared REQD_LIBS ${IPP_LIB})
endif()
