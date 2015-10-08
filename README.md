# risky-business
A small simulator for a RISC machine. To use, write a simple program and run it through the assembler using ````Assembler.assemble(string source)````, load the output into the simulated memory, using offset ````0```` as the entry point, then call ````State.tick()```` until computation has completed.

The instruction set is minimal and RISC-like. Each instruction takes a maximum of three arguments, either registers or an immediate value.

````
ADD r1 r2 r3
SUB r1 r2 r3
MUL r1 r2 r3

CMP r1 r2 r3

LD r1 im1
LDC r1 im1

STR im1 r1

B im1
BZ r1 im1

NOP
````

The instruction set also includes a ```PRNT``` psuedo-instruction that will output the contents of a register operand.
