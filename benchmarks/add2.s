% Simple test ADD #2
% r1 = 14 (0xe)
% r2 = -6 (0xfffffffa)
% r3 = r1 + r2 
% therefore, r3 == 8
LDC r1 14
PRNT r1
LDC r2 -6
PRNT r2
ADD r3 r1 r2
PRNT r3
HLT