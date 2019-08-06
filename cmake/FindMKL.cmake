include(HelperFunctions)
# Find the Math Kernel Library from Intel

if (BUILD_STATIC_LIBS)
  set(MKL_LIB "mkl_intel_lp64" "mkl_gnu_thread" "mkl_core")
  add_mkl_dependency(NAME mkl_static REQD_LIBS ${MKL_LIB} STATIC)
endif()

if (BUILD_SHARED_LIBS)
  set(MKL_LIB "mkl_rt")
  add_mkl_dependency(NAME mkl_shared REQD_LIBS ${MKL_LIB})  
endif()
