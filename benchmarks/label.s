% Testing labels
LDC r0 1
LDC r1 5
loop:
PRNT r0
ADD r0 r0 r0
CMP r2 r0 r1
BLTZ r2 testlabel1
HLT
