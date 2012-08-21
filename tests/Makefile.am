include $(top_srcdir)/Makefile.decl

AM_CPPFLAGS =				\
	-I$(top_srcdir)			\
	$(GTK_CFLAGS)

DEPS = \
	$(top_builddir)/gfw/libgtkfw-1.0.la

LDADDS = \
	$(top_builddir)/gfw/libgtkfw-1.0.la \
	$(GTK_LIBS)			\
	-lm


noinst_PROGRAMS =			\
	button				\
	window				\
	piechart

piechart_DEPENDENCIES = $(TEST_DEPS)
button_DEPENDENCIES = $(TEST_DEPS)
window_DEPENDENCIES = $(TEST_DEPS)

piechart_LDADD = $(LDADDS)
button_LDADD = $(LDADDS)
window_LDADD = $(LDADDS)

piechart_SOURCES = piechart-example.c
button_SOURCES = window.c
window_SOURCES = window.c


EXTRA_DIST += 			\
	gradient1.png		\
	prop-editor.h		\
	testgtk.1 		\
	testgtk.css		\
	testgtk2.css		\
	3DRings.xpm		\
	FilesQueue.xpm		\
	Modeller.xpm		\
	check-y.xpm		\
	check-n.xpm		\
	marble.xpm		\
	test.xpm		\
	check-y.xpm		\
	check-n.xpm		\
	test.xpm		\
	merge-1.ui		\
	merge-2.ui		\
	merge-3.ui		\
	gnome-textfile.png	\
	makefile.msc		\
	widget-factory.ui

-include $(top_srcdir)/git.mk

