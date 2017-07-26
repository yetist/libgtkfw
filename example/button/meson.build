sources = [
'main.c',
]

main = executable('button',
    sources,
    include_directories : [top_srcdir, top_libdir],
    link_with: [lib_gtkfw],
    dependencies : gtk_deps,
)
