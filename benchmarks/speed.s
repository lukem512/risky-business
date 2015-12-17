% Simple benchmark #6
% This does nothing but [hopefully] go very fast.
%
%%%%%%%%%%%%%%%%%%%%%%
% i = 0
LDC r0 0
% max
LDC r1 1000
loop:
CMP r3 r0 r1
BGTEZ r3 end
%
% 14 independent ADD instructions
ADD r2 r2 r2
ADD r3 r3 r3
ADD r4 r4 r4
ADD r5 r5 r5
ADD r6 r6 r6
ADD r7 r7 r7
ADD r8 r8 r8
ADD r9 r9 r9
ADD r10 r10 r10
ADD r11 r11 r11
ADD r12 r12 r12
ADD r13 r13 r13
ADD r14 r14 r14
ADD r15 r15 r15
%
LDC r2 1
ADD r0 r0 r2
B loop
end:
HLT