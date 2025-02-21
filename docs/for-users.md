## Preparing the source folder

## Writing templates

## Custom entrypoints

### Custom header

Custom code written in file `head.frag.cpp`. It will be placed before the main function definition.

### Body override

Custom code written in file `body.frag.cpp`. It will be placed inside the main function definition and replaces the default root entry point for the embedded filesystem.

## `env` files

You are free to structure your env files as you prefer. It might also be a good idea to provide a grammar for them in one of the few formats which are common for XML.

## Macros & helpers

### Time

- `TIME::YEAR()`
- `TIME::DATE()` in `YYYY-MM-DD` format.
- `TIME::CLOCK()` in `HH:MM:SS` format.
- `TIME::STAMP()`

These values are computed once when loading the program, so subsequent calls are always ensured to result in the same values.

### Writers

- `WRITE(x)` with no escaping.
- `WRITE_C(x)` for C-like escaping in the context of strings
- `WRITE_MESON(x)` for meson-like escaping in the context of strings
- `WRITE_XML(x)` with escaping in the context of attribute values. For DATA blocks, please use `CDATA` and `WRITE(X)`

### XML paths

- `"path"_node(base,defval)` with defval optional.
- `"path"_attr(base,defval)` with defval optional.

Syntax is `/` to separate children, and `~attribute` to access the attribute `attribute`. Special attributes `~!txt` and `~!name` to access the content or the element name.  
If a path starts with `/` it is absolute. If it starts with `$` it is based on `base`.
