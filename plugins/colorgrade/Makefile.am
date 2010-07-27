plugin_LTLIBRARIES = colorgrade.la
colorgrade_la_LDFLAGS = -avoid-version -module -shared 
colorgrade_la_LIBADD = $(top_builddir)/plugins/colors/libcolors.la
colorgrade_la_SOURCES = colorgrade.C colorgradeconfig.C colorgradewindow.C
AM_CXXFLAGS = $(LARGEFILE_CFLAGS)

INCLUDES = -I$(top_srcdir)/guicast -I$(top_srcdir)/cinelerra -I$(top_srcdir)/quicktime -I$(top_srcdir)/plugins/colors
LIBTOOL = $(SHELL) $(top_builddir)/libtool $(LTCXX_FLAGS)

noinst_HEADERS = \
	aggregated.h \
	colorgrade.h colorgradeconfig.h colorgradewindow.h \
	colorgrade.inc colorgradeconfig.inc colorgradewindow.inc \
	max_picon_png.h mid_picon_png.h min_picon_png.h picon_png.h 
EXTRA_DIST = picon.png max_picon.png mid_picon.png min_picon.png
