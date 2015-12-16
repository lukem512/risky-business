% Simple benchmark #3
% This prints the first n Triangle numbers
%
% Expected output is:
%  00000000
%  00000001
%  00000003
%  00000006
%  0000000a
%  0000000f
%  00000015
%  0000001c
%  00000024
%  0000002d
%  00000037
%  00000042
%  0000004e
%  0000005b
%  00000069
%
%%%%%%%%%%%%%%%%%%%%%%
% i = 0
LDC r0 0
% max = n
LDC r1 15
% incr = 1
LDC r2 1
% div = 2
LDC r3 2
% while (i < 10)
loop:
CMP r6 r0 r1
BGTEZ r6 end
% Tn = n(n+1) / 2
ADD r4 r0 r2
MUL r4 r0 r4
DIV r4 r4 r3
PRNT r4
% i = i + incr
ADD r0 r0 r2
B loop
end:
HLT