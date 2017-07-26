sources = [
'gfw-button.c',
'gfw-button.h',
'gfw-color.c',
'gfw-color.h',
'gfw-ip-entry.c',
'gfw-ip-entry.h',
#'gfw-pie-chart.c',
#'gfw-pie-chart.h',
'gfw-window.c',
'gfw-window.h',
'gfw-zoom.c',
'gfw-zoom.h',
]

lib_gtkfw = shared_library('gtkfw',
    sources,
    include_directories : top_srcdir,
    dependencies : [gtk_deps, libm],
)
