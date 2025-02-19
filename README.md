> [!IMPORTANT]  
> It works, but its documentation is severely lacking!  
> I am currently working on proper docs and better examples.

# 🥶🥏

## What's that?
Everyone loves templates: the let us reuse the same boilerplate code over and over, for each project we start and never finish.  
Just think how frustrating it would be to bake cookies and having to manually shape each one of them.  
However, in most scenarios, a naive copy & paste of files is not the best approach.  
We might need to place our project name in several spots, or pre-generate multiple subfolders with the same pattern based on information which is application specific, only know when initializing a new project.  
Not having a way to properly express this parametrization can be frustrating and time-consuming.  
Fret not! `freezerplate` is the unlikely solution to a problem no one knew they had!

`freezerplate` allows you to automatically generate the C++ code of a program which can fully build your boilerplate just passing the destination folder and the environment file with the specific parametrization.  
To generate the initial C++ code, a folder containing (recursively) all the boilerplate files is needed, but those will be embedded in the source itself, so the actual distribution of the template initializer is just as simple as a single executable.

### Can I eat it?

I would advise against.  

### Features
- Optional compression for files. The generated template builders can act as glorified `tar.gz` archives, with the additional spice of arbitrary code execution.
- XML environment file passed to each template entry to customize the final output at runtime.

### Non Features
- Binary files like sqlite databases or images can be tracked and added, but they cannot be reasonably changed by freezerplate. I would suggest generating them based on some post-script.

## General workflow?

- Add all your template files to a folder (you can nest as much as you want, or as much as your stack can take, whichever gives up first)
- Introduce code blocks `<?` `?>` embedding any arbitrary C++ code within your files. You are given access to an XML environment file to use as you wish.
- Compile your templates in a standalone (and optionally compressed) generator.
- (optional) Customize the main of the generated file if needed.
- Profit!

### Usage

> [!TIP]  
> Did you know `freezerplate` uses `freezerplate` itself to generate most of the boilerplate for the newly generated templates builders?  
> Why am I even asking, it is not like you knew about this project up til 5 minutes ago.  

For example, `freezerplate ./template/ ./dist/` will generate in `./dist` all files including the `main.cpp` which is renamed `template.cpp` in the root directory.

