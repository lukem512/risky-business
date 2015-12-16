% Simple benchmark #4
% This prints the first n Square numbers
%
% Expected output is:
%  00000000
%  00000001
%  00000004
%  00000009
%  00000010
%  00000019
%  00000024
%  00000031
%  00000040
%  00000051
%  00000064
%  00000079
%  00000090
%  000000a9
%  000000c4
%
%%%%%%%%%%%%%%%%%%%%%%
% i = 0
LDC r0 0
% max = n
LDC r1 15
% incr = 1
LDC r2 1
% while (i < n)
loop:
CMP r6 r0 r1
BGTEZ r6 end
% Tn = n(n+1) / 2
MUL r3 r0 r0
PRNT r3
% i = i + incr
ADD r0 r0 r2
B loop
end:
HLT