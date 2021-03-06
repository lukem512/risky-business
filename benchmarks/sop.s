% Simple benchmark #2
% This performs a Sum of Products
%
% Expected output is:
%  0000001e
%  00000002
%  00000004
%  00000006
%  00000008
%  0000000a
%
%%%%%%%%%%%%%%%%%%%%%%
% Address of A
LDC r0 100
% Length of A
LDC r1 5
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% Define A
LDC r3 1
ST r3 100
LDC r3 2
ST r3 101
LDC r3 3
ST r3 102
LDC r3 4
ST r3 103
LDC r3 5
ST r3 104
% ADDR 11 ^^
%%%%%%%%%%%%%%%%%%%%%%
% Address of B (B)
LDC r10 200
% Length of B
% = length of A
LDC r11 5
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% Define B
LDC r12 200
LDC r13 2
STR r12 r13
LDC r12 201
LDC r13 2
STR r12 r13
LDC r12 202
LDC r13 2
STR r12 r13
LDC r12 203
LDC r13 2
STR r12 r13
LDC r12 204
LDC r13 2
STR r12 r13
% ADDR ^ 28
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% sum = 0
LDC r9 0
% incr = 1
LDC r2 1
% i = 0
LDC r3 0
% while (i < len(A))
loop:
% ADDR v 32
CMP r4 r3 r1
BLTZ r4 1
% ADDR v 34
B print
% Address of A[i]
ADD r5 r0 r3
% Address of B[i]
ADD r6 r10 r3
% Value of A[i]
LDR r7 r5
% Value of B[i]
LDR r8 r6
% A[i] = A[i] * B[i]
MUL r7 r7 r8
STR r5 r7
% sum = sum + A[i]
ADD r9 r9 r7
% i = i + incr
ADD r3 r3 r2
B loop
% ADDR ^ 43
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
print:
% Print sum (ADDR v 44)
PRNT r9
% Print product array
LDC r3 0
LDC r4 1
printloop:
% while (i < len) (ADDR v 47)
CMP r5 r3 r1
BLTZ r5 1
B halt
% (ADDR ^ 49)
ADD r13 r0 r3
LDR r9 r13
PRNT r9
ADD r3 r3 r4
% (ADDR v 54)
B printloop
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
halt:
HLT
% ADDR ^ 55