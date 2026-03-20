# Testbencher
Testbencher is an [verilator](https://github.com/verilator/verilator) based "framework" for testing (System)Verilog designs.

## Capabilities
Currently supports effective simulation of designs requiring multiple independent clock signals with different characteristics as well as simulating VGA display.

## Building
>Requires verilator in `PATH` as well as min. CMake 3.28

Just use provided CMake script.

- When using Display Simulation, set CMake variable `USE_DISPLAY_SIM` to `ON`. `OFF` otherwise.

- When using when you want to generate VCD Trace File, set CMake variable `TRACE_ENABLE` to `ON`. `OFF` otherwise.

>Misconfiguration will likely result in errors in runtime!

## Usage
See [main.cpp](src/main.cpp) file, it contains example test setup.

Currently there's no proper documentation, you have to take a look at [Testbench.hpp](include/Testbench.hpp) and [Display.hpp](include/Display.hpp) files to get the API (it's not complicated tho).

## Credits:

[V. Hunter Adams:](anhunteradams.com/DE1/VGA_Driver/Driver.html) VGA Driver used in example test setup.

## TODO:
This design is incomplete; along with *_more_* optimizations, custom unit tests will be added, proper documentation will be written and more!
