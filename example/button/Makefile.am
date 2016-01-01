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

noinst_PROGRAMS = button

button_DEPENDENCIES = $(TEST_DEPS)
button_LDADD = $(LDADDS)
button_SOURCES = main.c

EXTRA_DIST += 		\
	background.png	\
	active.png	\
	normal.png	\
	prelight.png

-include $(top_srcdir)/git.mk
