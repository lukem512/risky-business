% Simple benchmark #1
% This prints the first n Fibonacci numbers
%
% Expected output is:
%  00000000
%  00000001
%  00000001
%  00000002
%  00000003
%  00000005
%  00000008
%  0000000d
%  00000015
%  00000022
%  00000037
%  00000059
%  00000090
%  000000e9
%  00000179
%  00000262
%
% i = 0
LDC r0 0
% max = n
LDC r1 15
% incr = 1
LDC r2 1
% cur = 1
LDC r10 1
% prev = 0
LDC r11 0
% Print the first character, 0
PRNT r0
% while (i < 10)
loop:
CMP r6 r0 r1
BGTEZ r6 end
% print(cur)
PRNT r10
% PRNT r12
% tmp = cur
MOV r12 r10
% cur = cur + prev
ADD r10 r10 r11
% prev = tmp
MOV r11 r12
% i = i + incr
ADD r0 r0 r2
B loop
end:
HLT