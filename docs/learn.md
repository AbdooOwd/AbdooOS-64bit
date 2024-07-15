# The map of knowledge

## OS Design

Know what you're coding. Don't just cod while having in mind
"what am I doing?" or "I don't know what to do now".
Make a "map" for the development of your OS.

[This](https://osdev.miraheze.org/wiki/Creating_an_Operating_System) is a good map.

## Compatibility

### Hardware Compatibility

Make the OS Compatible with real hardware. You should study
the OS design and CPU architecture so it works on multiple
computers or at least one of them rather than only in
a virtual environment.
you should study the good practices in OSDev to
realize that.

### CPU Compatibility

Once your OS is ready for one CPU architecture, 
make it compatible with another one.
This is usually made using *macros* in C.

For the project's code structure, that would mean putting all
the architercure related stuff in an `arch/` directory.
For example, you'd put your screen driver and interrupts code
in `arch/x86`. Then you could implement the same features
for a different architecture.

## Testing

You could code a little driver or function to test **ALL** the drivers.
For example, having a `test_drivers` function in a `driver_tests.c` file.
Then use these drivers with all their available methods and "ways of use".

## Memory-related

### Underrated stuff (*I* Underrated them)

-	Memory Maps.
-	Paging.
-	Virtual Memory.

Memory isn't just setting an area as used (;