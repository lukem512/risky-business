% Simple test MUL #1
% r1 = 14 (0xe)
% r2 = 6
% r3 = r1 * r2
% therefore, r3 == 84 (0x54)
LDC r1 14
PRNT r1
LDC r2 6
PRNT r2
MUL r3 r1 r2
PRNT r3
HLT