% Livermore Loop #1
% This is the first Livermore Loop kernel
% It is a basic hydrodynamic simulation fragment.
%
%%%%%%%%%%%%%%%%%%%%%%
%
% l = 0
LDC r0 0
% loop = 10
LDC r1 10
% incr = 1
LDC r2 1
%
% Pointer to x[]
LDC r10 1000
% Pointer to y[]
LDC r11 2000
% Pointer to z[]
LDC r12 3000
% q
LDC r13 10
% r
LDC r14 14
% t
LDC r15 1200
%
% for (l=1; l<=loop; l++)
outer:
CMP r6 r0 r1
BGTEZ r6 endouter
%
% k = 0
LDC r3 0
% n = 10
LDC r4 10
% for (k=0; k<n; k++)
inner:
CMP r6 r3 r4
BGTEZ r6 endinner
%
% offset to z[k]
ADD r8 r12 r3
% offset to z[k+10]
LDC r9 10
ADD r8 r9 r8
% r * z[k+10]
LDR r9 r8
MUL r9 r14 r9
% offset to z[k+10+1]
ADD r8 r2 r8
% t * z[k+11]
LDR r8 r8
MUL r8 r15 r8
% r*z[k+10] + t*z[k+11]
ADD r9 r8 r9
% offset to y[k]
ADD r8 r11 r3
% y[k]*(r*z[k+10] + t*z[k+11])
LDR r8 r8
MUL r8 r9 r8
% q + y[k]*(r*z[k+10] + t*z[k+11])
ADD r8 r13 r8
% offset to x[k]
ADD r9 r10 r3
% x[k] = q + y[k]*(r*z[k+10] + t*z[k+11])
STR r9 r8
% k++
ADD r3 r2 r3
B inner
endinner:
%
% i = i + incr
ADD r0 r0 r2
B outer
endouter:
HLT