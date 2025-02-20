You can use `./build.sh` to handle it automatically for your system.

## Switching to uncompressed version

At the moment it is a bit of a mess. You will have to patch the current `template.cpp` to remove children from the entrypoint `writer_dir_0` before rebuilding.  
I plan to provide a better fix for that at some point.
