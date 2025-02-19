<? auto proj_name = env.child("project").attribute("name").as_string("undefined"); ?>

project(
    '<?WRITE(proj_name)?>',
    ['cpp'],
    version: '0.1.0',
    meson_version: '>= 1.1',
    default_options: ['cpp_std=c++23'],
)

add_global_arguments(['-Wno-unused-function'], language: 'cpp')
add_global_arguments(['-Wno-unused-function'], language: 'c')

miniz_dep = dependency('miniz')


cli = executable(
    '<?WRITE(proj_name)?>',
    ['main.cpp'],
    dependencies: [miniz_dep,pugixml_dep],
    link_with: [],
    include_directories: [],
    install: true,
)

install_man('man/<?WRITE(proj_name)?>.1')

install_headers(
    [],
    subdir: '<?WRITE(proj_name)?>',
)
