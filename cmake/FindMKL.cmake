include(HelperFunctions)
# Find the Math Kernel Library from Intel

if (BUILD_STATIC_LIBS)
  set(MKL_LIB "mkl_intel_lp64" "mkl_gnu_thread" "mkl_core")

  if(WIN32)
    add_mkl_dependency_windows(NAME mkl_static REQD_LIBS ${MKL_LIB} STATIC)
  else()
    add_mkl_dependency_unix(NAME mkl_static REQD_LIBS ${MKL_LIB} STATIC)    
  endif()
endif()

if (BUILD_SHARED_LIBS)
  set(MKL_LIB "mkl_rt")

  if(WIN32)
    add_mkl_dependency_windows(NAME mkl_shared REQD_LIBS ${MKL_LIB})
  else()
    add_mkl_dependency_unix(NAME mkl_shared REQD_LIBS ${MKL_LIB})    
  endif()  
endif()
