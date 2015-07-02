AC_INIT([spl], [1.0], [legris.thomas@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
#AC_PREREQ([1.0])
AC_CONFIG_SRCDIR([include/NDSignal.hh])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

PKG_CHECK_MODULES([CHECK], [check >= 0.9.8])
#check/Makefile

# enable library creation
AC_PROG_RANLIB

# Checks for programs
AC_PROG_CXX
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  include/Makefile
  tests/Makefile
  tools/Makefile
])

## Add --enable-debug
AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

## Add --enable-gpu
AC_ARG_ENABLE(gpu,
AS_HELP_STRING([--enable-gpu],
               [enable GPU, default: no]),
[case "${enableval}" in
             yes) gpu=true ;;
             no)  gpu=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-gpu]) ;;
esac],
[gpu=false])

AM_CONDITIONAL(GPU, test x"$gpu" = x"true")

# Check Library
AS_IF([test x"$gpu" = x"true"],
[AC_LANG_PUSH([C++])
AC_ARG_WITH(libopencl-inc,
  [--with-libopencl-inc=LIBOPENCL_INC_PATH (path containing libopencl includes)],
  [AC_CHECK_HEADER($withval/CL/cl.hpp, # Check if lib is valid in given paths
                   [libopencl_inc_dir=$withval],
                   [AC_MSG_ERROR([libopencl enabled but headers not found])])],
  [AC_CHECK_HEADER(CL/cl.hpp,[],[AC_MSG_ERROR([libopencl enabled but headers not found])])]) # Check if lib is valid in global paths

AC_ARG_WITH(libopencl-lib,
  [--with-libopencl-lib=LIBOPENCL_LIB_PATH (path containing libopencl object)],
  [AC_CHECK_FILE($withval/libOpenCL.so, # Check if lib is valid in given paths
                   [libopencl_lib_dir=$withval],
                   [AC_MSG_ERROR([libopencl enabled but library not found])])],
  [AC_CHECK_LIB(OpenCL, clGetPlatformIDs, [], [AC_MSG_ERROR([OpenCL enabled but library not found.])])])
AC_LANG_POP]
,[]
)
# create library variables
OPENCL_HH="$libopencl_inc_dir"
OPENCL_LIB="$libopencl_lib_dir"

# export library variables
AC_SUBST([OPENCL_HH])
AC_SUBST([OPENCL_LIB])

AC_CONFIG_LINKS([tests/kernels/set210.cl:tests/kernels/set210.cl])
AC_CONFIG_LINKS([tests/kernels/add2vectors.cl:tests/kernels/add2vectors.cl])

AC_OUTPUT
