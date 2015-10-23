% A more complex benchmark
% This application sorts an array of numbers using Bubble Sort
% The Bubble Sort algorithm is the naive one, with no optimisations
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
%
% procedure bubbleSort( A : list of sortable items )
%   n = length(A)
%   repeat 
%     swapped = false
%     for i = 1 to n-1 inclusive do
%       /* if this pair is out of order */
%       if A[i-1] > A[i] then
%         /* swap them and remember something changed */
%         swap( A[i-1], A[i] )
%         swapped = true
%       end if
%     end for
%   until not swapped
% end procedure
%
%
% swapped = false
LDC r3 0
% i = 1
LDC r4 1
% incr = 1
LDC r5 1
% while (i < len) {
CMP r6 r4 r1
BLTZ r6 1
B 15
% if (A[i-1] > A[i]) {
% Address of A[i-1]
SUB r7 r4 r5
ADD r8 r0 r7
% Address of A[i]
ADD r9 r0 r4
% A[i-1]
LD r10 r8
% A[i]
LD r11 r9
% if (A[i] < A[i-1]) {
CMP r12 r11 r10
BLTZ r12 1
B 4
% SWAP THEM
STR r8 r11
STR r9 r10
% swapped = true
LDC r3 1
% i = i + incr
ADD r4 r4 r5
PRNT r4
B -9
% }
% }
B -18
% }
% r13 = false
LDC r13 0
% if (swapped == false)
CMP r14 r3 r13
BZ r14 1
% else jump to "swapped = false"
B -28
%
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
%%%%%%%%%%%%%%%%%%%%%%
HLT