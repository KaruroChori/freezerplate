<? auto proj_name = env.child("project").attribute("name").as_string("undefined"); ?>
<? auto proj_version = env.child("project").attribute("version").as_string("0.1.0"); ?>

project(
    '<?WRITE_MESON(proj_name)?>',
    ['cpp'],
    version: '<?WRITE_MESON(proj_version)?>',
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
    '<?WRITE_MESON(proj_name)?>',
    ['main.cpp'],
    dependencies: [miniz_dep,pugixml_dep],
    link_with: [],
    include_directories: [],
    install: true,
)

#install_man('man/<?WRITE_MESON(proj_name)?>.1')

install_headers(
    [],
    subdir: '<?WRITE_MESON(proj_name)?>',
)
