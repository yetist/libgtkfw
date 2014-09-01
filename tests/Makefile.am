include $(top_srcdir)/Makefile.decl

AM_CPPFLAGS =				\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/gfw		\
	$(GTK_CFLAGS)

TEST_DEPS = \
	$(top_builddir)/gfw/libgtkfw-1.0.la

LDADDS = \
	$(top_builddir)/gfw/libgtkfw-1.0.la \
	$(GTK_LIBS)			\
	-lm


noinst_PROGRAMS =			\
	button				\
	window				\
	shape-window				\
	ipentry				\
	piechart

piechart_DEPENDENCIES = $(TEST_DEPS)
button_DEPENDENCIES = $(TEST_DEPS)
window_DEPENDENCIES = $(TEST_DEPS)
shape_window_DEPENDENCIES = $(TEST_DEPS)
ipentry_DEPENDENCIES = $(TEST_DEPS)

piechart_LDADD = $(LDADDS)
button_LDADD = $(LDADDS)
window_LDADD = $(LDADDS)
shape_window_LDADD = $(LDADDS)
ipentry_LDADD = $(LDADDS)

piechart_SOURCES = piechart-example.c
button_SOURCES = button.c
window_SOURCES = window.c
shape_window_SOURCES = shape-window.c
ipentry_SOURCES = ipaddress.c

EXTRA_DIST += 			\
	background.png		\
	enter.png		\
	leave.png		\
	login-button-active.png	\
	login-button-normal.png	\
	press.png

-include $(top_srcdir)/git.mk

