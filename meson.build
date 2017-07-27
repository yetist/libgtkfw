project('gtkfw', 'c',
  version : '0.1.0',
  license : 'LGPLv2.1+',
  default_options : [
	  'warning_level=1',
	  'buildtype=debugoptimized',
	  ],
  meson_version : '>=0.40.1'
)

cc = meson.get_compiler('c')

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('GETTEXT_PACKAGE', meson.project_name())
conf.set_quoted('PACKAGE', meson.project_name())
conf.set_quoted('PACKAGE_NAME', meson.project_name())

gtk_deps = dependency('gtk+-3.0')
libm = cc.find_library('m', required: false)

top_srcdir = include_directories('.')
top_libdir = include_directories('./gfw')

subdir('gfw')
subdir('example')

pkg = import('pkgconfig')
pkg.generate(
	name: 'gtkfw-1.0',
	version: meson.project_version(),
	description: 'Gtk+ Feathur Widget Library',
	filebase: 'gtkfw-1.0',
	requires: 'gtk+-3.0',
	libraries: '-lgtkfw-1.0',
	subdirs: 'libgtkfw-1.0',
)
