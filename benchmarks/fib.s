% Simple test for branching
% This prints the first n Fibonacci numbers
% i = 0
LDC r0 0
% max = n
LDC r1 15
% incr = 1
LDC r2 1
% zero = 0
LDC r3 0
% cur = 1
LDC r10 1
% prev = 0
LDC r11 0
% Print the first character, 0
PRNT r0
% while (i < 10)
loop:
CMP r6 r0 r1
BLTZ r6 1
B end
% print(cur)
PRNT r10
% PRNT r12
% tmp = cur
% This is instead of MOV as my IS does not (yet) have one
ADD r12 r10 r3
% cur = cur + prev
ADD r10 r10 r11
% prev = tmp
ADD r11 r12 r3
% i = i + incr
ADD r0 r0 r2
B loop
end:
HLT