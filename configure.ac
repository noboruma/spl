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
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  include/Makefile
  tests/Makefile
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

AC_OUTPUT
