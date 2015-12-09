% Simple test branching
%
% Expected output is:
%  00000000
%  00000001
%  00000002
%  00000003
%  00000004
%  00000005
%  00000006
%  00000007
%  00000008
%  00000009
%%%%%%%%%%%%%%%%%%%%%%
LDC r0 0
LDC r1 10
LDC r2 1
B loop
NOP
NOP
loop:
PRNT r0
ADD r0 r0 r2
% while (i < 10)
CMP r6 r0 r1
BLTZ r6 loop
HLT	