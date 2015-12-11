% Simple test MUL #2
% r1 = 14 (0xe)
% r2 = -6 (0xfffffffa)
% r3 = r1 * r2
% therefore, r3 == -84 (0xffffffac)
LDC r1 14
PRNT r1
LDC r2 -6
PRNT r2
MUL r3 r1 r2
PRNT r3
HLT