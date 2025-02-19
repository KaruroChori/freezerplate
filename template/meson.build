project(
    '<?WRITE(env.child("project").attribute("name").as_string("undefined"))?>',
    ['cpp'],
    version: '0.1.0',
    meson_version: '>= 1.1',
    default_options: ['cpp_std=c++23'],
)

add_global_arguments(['-Wno-unused-function'], language: 'cpp')
add_global_arguments(['-Wno-unused-function'], language: 'c')

miniz_dep = dependency('miniz')

if get_option('compress')
    opts = ['-DTE4_COMPRESS']
else 
    opts=[]
endif

cli = executable(
    'te4',
    ['main.cpp'],
    dependencies: [miniz_dep],
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

#if get_option('tests')
#    subdir(['./test/'])
#endif

#subdir(['./metadata/'])

#pconf = import('pkgconfig')
#pconf.generate(
#    te4_lib,
#    description: 'portable project template generator',
#    url: 'https://github.com/KaruroChori/te4',
#    version: meson.project_version(),
#)