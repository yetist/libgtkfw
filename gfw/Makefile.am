
INCLUDES = 					\
	-I$(top_srcdir)				\
	-I$(srcdir)				\
	$(WARN_CFLAGS)				\
	$(GTK_CFLAGS)

source_h = \
	gfw-button.h	\
	gfw-window.h	\
	gfw-ip-entry.h	\
	gfw-pie-chart.h

source_c = \
	gfw-button.c	\
	gfw-window.c	\
	gfw-ip-entry.c	\
	gfw-pie-chart.c

gtkfwincludedir=$(includedir)/libgtkfw-1.0
gtkfwinclude_HEADERS = gfw.h

gfwincludedir=$(includedir)/libgtkfw-1.0/gfw
gfwinclude_HEADERS = $(source_h)

lib_LTLIBRARIES = libgtkfw-1.0.la

libgtkfw_1_0_la_SOURCES = $(source_c)
libgtkfw_1_0_la_LDFLAGS = $(GTK_CFLAGS) \
	-version-info $(LT_VERSION_INFO)

libgtkfw_1_0_la_LIBADD = $(GTK_LIBS)
