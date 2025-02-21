> [!IMPORTANT]  
> It works, but its documentation is severely lacking!  
> I am currently working on proper docs and better examples.

# 🥶🥏

## What's that?

Everyone loves templates: they let us reuse the same boilerplate code over and over, for each and every project we start, never to be finished.  
However, in most scenarios, the naive copy & paste of files is not enough.  
We might need to place our project name in several spots, or pre-generate multiple subfolders with the same structure, based on information which must derive from some sort of parametrization.  
Not having a natural way to express that can be frustrating and time-consuming.

`freezerplate` allows you build parametric templates. Based on the specifications of your template, it will automatically generate the C++ code of a program which can fully build your boilerplate when run.  
Any resource needed is going to be embedded directly in the source, and then in the final executable.  
This allows to have extremely portable parametric template generators.

### Can I eat it?

I would advise against.

### Features

- Support for nested folders.
- Support for symbolic links.
- Support for raw files (binary files without templating inside them).
- Optional compression for files. The generated template builders can act as glorified `tar.gz` archives, with the additional spice of arbitrary code execution.
- The parametrization of the template generator built via `freezerplate` is defined via an environment XML file passed at runtime.

### Non Features

- Binary files like SQLite databases or images can be tracked and added, but they cannot be reasonably changed by `freezerplate`.  
  I would suggest generating them based on some post-script which can be part of the same template.
- Windows is not a native target. Fully portable builds can be obtained via `wasm`. I am using some POSIX specific and GNU C++ specific features, and I don't really want to change that.

## General workflow?

- Make sure all your relevant files are in a single folder (you can nest as much as you want, or as much as your stack can take, whichever gives up first)
- Introduce code blocks in the source files via `<?` `?>`, embedding any arbitrary C++ code within your files. You are given access to an XML environment file to use as you wish and some [helpers](./docs/templating.md).
- Compile your templates in as standalone (and optionally compressed) generator.
- (optional) Customize the `body` and `head` files if extra features are wanted.
- Profit!

### Usage

> [!TIP]  
> Did you know `freezerplate` uses `freezerplate` itself to generate most of the boilerplate for the newly generated templates builders?  
> Why am I even asking, it is not like you knew about this project up till 5 minutes ago.

For example:

```bash
freezerplate ./template-folder/ ./dist/ project.xml
```

will generate in `./dist` all files needed to compile the template builder based on files from `./template-folder/` and the extra specifications from `project.xml`.  
You can then enter in `./dist` run `./build.sh` and that is your ready-to-use builder for `./template-folder/`.
