## Process this file with automake to produce Makefile.in
AM_CPPFLAGS = -I$(srcdir)/../include/spl $(OTHER_CPPFLAGS) -std=c++11 -DCHECK

check_PROGRAMS = check_signal_access check_signal_utils check_mean check_sampling check_histogram 
AM_DEFAULT_SOURCE_EXT = .cc

#CPPFLAGS+=-I$(top_builddir)/src/

TESTS = $(check_PROGRAMS)
