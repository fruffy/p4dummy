# p4dummy
P4Dummy showcases how a compiler back end can be written for the official P4 compiler ([P4C](https://github.com/p4lang/p4c)). P4Dummy shows how to integrate with P4C's visitor library, parser, and front end. It does nothing but apply a series of compiler passes and echo the final version of the program.

## Requirements
P4Dummy is written as an extension to [P4C](https://github.com/p4lang/p4c). It has no other dependencies. P4C extensions are symlinked into the `extensions` folder of P4C and are automatically picked up once CMake is run. An example workflow could be:
```
cd p4c/
mkdir extensions
cd extensions
git clone https://github.com/fruffy/p4dummy
cd p4c/build
cmake ..
make
```
This assumes that P4C is setup correctly Instructions can be found [here](https://github.com/p4lang/p4c#dependencies).

Afterwards, the `p4dummy` executable can be found in the `p4c/build`folder.

## Usage
Executing P4Dummy will echo P4 programs in various stages of compilation (e.g., before/after the front or mid end):

    ./build/p4dummy example.p4

P4Dummy also has a mode where it does not require a particular input program and instead just echoes an internally defined program. You can toggle this mode with:

    ./build/p4dummy --use-fixed

## TODO
- Also build P4Dummy using Bazel
