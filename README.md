# risky-business
A small simulator for a RISC machine. To use, write a simple program and run it through the assembler using ````Assembler.assemble(string source)````, load the output into the simulated memory, using offset ````0```` as the entry point, then call ````State.tick()```` until computation has completed. The file ````main.cpp```` contains code to load and assemble a file from source. Some example, benchmark programs can be found in the ````benchmarks```` folder.

The processor can be run using ```./proc -f compiled/input.bin [-d] [-no-pipeline] [-no-branch-prediction] [-no-dynamic-branch-prediction] [-eus n] [-dus n] [-fus n]```. A full list of options can be obtained using ```./proc -h```. The assembler can also be run as a standalone applicaton, using ```./ass -f benchmark/input.s [-d] [-o compiled/output.bin]```.

The instruction set is minimal and RISC-like. Each instruction takes a maximum of three arguments, either registers or an immediate value.

````
ADD r1 r2 r3
SUB r1 r2 r3
MUL r1 r2 r3
DIV r1 r2 r3

CMP r1 r2 r3

LD r1 im1
ST r1 im1

LDR r1 r2
STR r1 r2

LDC r1 im1

B im1
BZ r1 im1
BNZ r1 im1
BLTZ r1 im1
BGTZ r1 im1
BLTEZ r1 im1
BGTEZ r1 im1

MOV r1 r2

NOP
HLT
````

The instruction set also includes a ```PRNT``` psuedo-instruction that will output the contents of a register operand.

To build, type ```make```. The source uses C++11 but should compile with -std=c++0x on older versions of GCC. The processor simulation and the assembler can be built separately using ```make proc``` and ```make ass```, respectively.
