project(
    'te4',
    ['cpp'],
    version: '0.1.0',
    meson_version: '>= 1.1',
    default_options: ['cpp_std=c++23'],
)

add_global_arguments(['-Wno-unused-function'], language: 'cpp')
add_global_arguments(['-Wno-unused-function'], language: 'c')

miniz_dep = dependency('miniz')
pugixml_dep = dependency('pugixml')

if get_option('compress')
    opts = ['-DTE4_COMPRESS','-DTE4_INTERNAL']
else 
    opts=['-DTE4_INTERNAL']
endif

cli = executable(
    'te4',
    ['main.cpp', 'template.cpp'],
    dependencies: [miniz_dep, pugixml_dep],
    link_with: [],
    include_directories: [],
    install: true,
    cpp_args: opts
)

install_man('man/te4.1')

install_headers(
    [],
    subdir: 'te4',
)
