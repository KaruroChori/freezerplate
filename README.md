> [!IMPORTANT]  
> It works, but there is near-zero documentation on how to make it possible.

A generator of generators for portable project templates.  
Templates are good to avoid writing the same boilerplate code over and over each time.  
But what to do when you need some form of parametrization for those boilerplates? How to avoid manually updating all instances in which your project name should be?  
Fret not! TE4 is the unlikely solution to a problem no one knew they had.

Basically, it allows you to define a template directory with all files inside, and it creates a single c++ file that once compiled is able to fully replicate that template structure allowing for an environment file to parametrize the output.

## General workflow?

- Add all your template files to a folder (you can nest as much as you want, or you stack is depleted)
- Introduce code blocks `<?` `?>` embedding any arbitrary code within your files. You have also access to an XML environment file to use as you wish.
- Compile your templates in a standalone (and optionally compressed) generator.
- Profit!

## Examples

> [!TIP]  
> `TE4` uses `TE4` to generate most of the boilerplate for the newly generated generators.  
> Like inception but for boring people wasting too much time in front of a screen.

`te4 ./template/ ./dist/` will generate in `./dist` all files including the `main.cpp` which is renamed `template.cpp` in the root directory.
