## Process this file with automake to produce Makefile.in
AM_CPPFLAGS = -I$(srcdir)/../include $(OTHER_CPPFLAGS) -std=c++1y -I@OPENCL_HH@
AM_LDFLAGS = -L@OPENCL_LIB@ -lOpenCl

bin_PROGRAMS = raw2csv

AM_DEFAULT_SOURCE_EXT = .cc

