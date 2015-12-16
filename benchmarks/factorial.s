% Simple benchmark #5
% This computes n! (n factorial)
%
% Expected output is:
%  0000000a
%  00058980
%
%%%%%%%%%%%%%%%%%%%%%%
% i = 1
LDC r0 1
% n = 10
LDC r1 10
% incr = 1
LDC r2 1
% result = i
MOV r3 r0
% Print n
PRNT r1
% while (i < 10)
loop:
CMP r6 r0 r1
BGTEZ r6 end
% result = result * i
MUL r3 r3 r0
% i = i + incr
ADD r0 r0 r2
B loop
end:
% Print n!
PRNT r3
HLT