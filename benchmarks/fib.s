% Simple test for branching
% This prints the first 10 Fibonacci numbers
% i = 0
LDC r0 0
% max = 10
LDC r1 10
% cur = 1
LDC r10 1
% prev = 1
LDC r11 1
% Print the first character, 0
PRNT r0
% while (i < 10)
CMP r2 r0 r10
BLTZ r2 1
B 4
PRNT r10
ADD r10 r10 r11
% MOV r11 r10
ADD r11 r10 0
B -7
HLT
