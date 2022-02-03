# sld
Inefficient alternative to the C++ STL. Implemented without any C STL/C++ STL/etc. headers - though with occasional system headers such as "unistd.h".

For now, everything is header-only. Everything is tested on linux, but should work almost anywhere.

'sld' also contains material (data-structures/algorithms/etc.) not found in the C/C++ STL.
# setup/use
For most unix-like systems: the makefile includes a target 'export': `$ make export`, it simply copies the 'sld' directory to `/usr/local/include/sld/`. This makes any sld file globally available by adding, for instance, `#include <sld/sld.h>` to your C++ file.

For all other systems, or if the above solution doesn't suit you: simply copy the 'sld' directory to anywhere globally or locally accessible.
