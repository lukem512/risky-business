% A more complex benchmark
% This application sorts an array of numbers using Bubble Sort
%
%%%%%%%%%%%%%%%%%%%%%%
% Address of A (A)
LDC r0 100
% Length of A (len)
LDC r1 5
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% Define A
LDC r12 100
LDC r13 5
STR r12 r13
LDC r12 101
LDC r13 4
STR r12 r13
LDC r12 102
LDC r13 3
STR r12 r13
LDC r12 103
LDC r13 2
STR r12 r13
LDC r12 104
LDC r13 1
STR r12 r13
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% Print unsorted array
LDC r3 0
LDC r4 1
% while (i < len)
CMP r5 r3 r1
BLTZ r5 1
B 5
%
ADD r13 r0 r3
LD r9 r13
PRNT r9
ADD r3 r3 r4
%
B -8
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% swapped = false
LDC r2 0
% false
LDC r15 0
% i = 1
LDC r3 1
% incr = 1
LDC r4 1
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% while (i < len)
CMP r5 r3 r1
BLTZ r5 1
B 14
%
% r10 = i-1
LDC r14 1
SUB r10 r3 r14
% r8 = A[i-1]
ADD r12 r0 r10
LD r8 r12
% r9 = A[i]
ADD r13 r0 r3
LD r9 r13
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% if(A[i-1] < A[i])
CMP r11 r8 r9
BLTZ r11 1
B 3
%
% store A[i] in A[i-1]
STR r12 A[i]
% store A[i-1] in A[i]
STR r13 A[i-1]
% swapped = true
LDC r2 1
%
% i = i + incr
ADD r3 r3 r4
%
B -17
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% Repeat loop if swapped == false
CMP r5 r2 r15
% Jump to "LDC r2 0"
BZ r5 -23
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% Print SORTED array
LDC r3 0
LDC r4 1
% while (i < len)
CMP r5 r3 r1
BLTZ r5 1
B 5
%
ADD r13 r0 r3
LD r9 r13
PRNT r9
ADD r3 r3 r4
%
B -8
%%%%%%%%%%%%%%%%%%%%%%
%
HLT