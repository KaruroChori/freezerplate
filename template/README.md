# <?WRITE_XML("/project~name"_attr(env).value_or("undefined"))?>

Custom parametric template built with [freezerplate](https://github.com/KaruroChori/freezerplate) by <?WRITE_XML("/project~author"_attr(env).value_or("anonymous"))?>.

<?
auto repo = "/project~repo"_attr(env);
if(repo.has_value()){
?>

You can find us in [our repo](<?WRITE_XML(repo.value())?>).

<?
}
?>

## Description

<?
auto description = "/project/description~!txt"_attr(env);
WRITE_XML(description.value_or("No description provided"))
?>

## How to compile

You will need:

- A recent version of [meson](https://mesonbuild.com/). Muon/Samurai will also work, but it requires changes in the bash script for building.
- If you want to compile for **wasm32** you will need the latest version of the [zig toolchain](https://ziglang.org/). Other compilers are viable, but you will have to provide your own configuration files for them.
- A supported C++ toolchain, like modern clang or gcc versions.

Then run:

```bash
./build.sh
```

## How to use

```bash
<?WRITE_XML("/project~name"_attr(env).value_or("undefined"))?> dest-dir [config.xml]
```

## Licence

<?WRITE_XML("/project/licence~mnemonic"_attr(env).value_or("Unkown"))?>, check [./LICENCE.md] for details.
