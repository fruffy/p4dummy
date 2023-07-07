# p4dummy
P4Dummy is a tool designed to learn how to write extensions for the P4 compiler ([P4C](https://github.com/p4lang/p4c)).

## Requirements
P4Dummy is written as an extension to [P4C](https://github.com/p4lang/p4c). It has no other dependencies. First P4C must be installed. Instructions can be found [here](https://github.com/p4lang/p4c#dependencies).

## Install
Once P4C has been installed, P4Dummy can be installed as an extension. The following commands provide an example:

    cd ~/p4c/
    mkdir extensions
    cd extensions
    git clone https://github.com/fruffy/p4dummy
    cd ~/p4c/build
    make
Afterwards, the P4Dummy executables can be found in the `p4c/build`folder.

## Usage
P4Dummy can only print P4 programs in various stages of compilation.

    ./p4dummy example.p4

It also has a mode where it prints an internally defined P4 program. You can enable this mode with

    ./p4dummy --use-fixed



```
