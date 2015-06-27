## Process this file with automake to produce Makefile.in
AM_CPPFLAGS = -I$(srcdir)/../include $(OTHER_CPPFLAGS) -std=c++1y -DCHECK -I@OPENCL_HH@
AM_LDFLAGS = -L@OPENCL_LIB@ -lOpenCl

check_PROGRAMS = check_signal_access check_signal_utils check_mean check_histogram check_mean check_sampling check_signal_io check_differentials

if GPU
check_PROGRAMS += check_cl
endif

AM_DEFAULT_SOURCE_EXT = .cc

#CPPFLAGS+=-I$(top_builddir)/src/

TESTS = $(check_PROGRAMS)
