% Game of Life
%
% Output is performed using a memory mapping.
% Memory locations from 30000 to 40000 will
% be mapped to a video output, when enabled.
% This gives 100 x 200 output.
%%%%%%%%%%%%%%%%
% Width
LDC r0 100
% Height
LDC r1 100
% Starting memory location
LDC r2 10000
% Starting I/O location
LDC r3 30000
% Alive = 1
LDC r14 1
% Dead = 0
LDC r15 0
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
% Set all values to dead
% x = 0
LDC r4 0
% max = width * height
MUL r5 r0 r1
% inc = 1
LDC r6 1
clear:
% while (x < max)
CMP r7 r4 r5
BGTEZ r7 seed
ADD r7 r2 r4
STR r7 r15
ADD r4 r4 r6
B clear
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
seed:
% Seed the grid
% bx = 30
LDC r4 30
% by = 30
LDC r5 30
% cell[bx+n][by+m] = (r0 * (by + m)) + bx + x
%% 
LDC r7 0
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 4
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 1
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 2
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 1
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 1
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 4
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 2
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 1
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 3
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 1
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 3
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 4
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 3
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 5
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 3
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 6
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 4
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 0
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 4
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 1
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 4
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 4
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 5
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 5
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 6
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 6
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 7
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 7
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 7
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 9
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 7
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 10
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 8
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 4
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 8
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 5
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 8
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 6
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 8
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 9
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 9
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 9
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 10
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 6
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 10
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 10
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 8
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%
LDC r7 11
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 6
ADD r7 r4 r7
ADD r7 r2 r6
LDC r6 1
STR r7 r6
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
% Tick!
% The scrubber example only needs 10 iterations to stabilise
% tick = 0
LDC r11 0
% maxticks = 15
LDC r12 15
tick:
CMP r4 r11 r12
BGTEZ r4 end
% x = 0
LDC r4 0
% max = width * height
MUL r5 r0 r1
% inc = 1
LDC r6 1
% Next state start location
LDC r7 20000
loop:
% while (x < max)
CMP r8 r4 r5
BGTEZ r8 break
% Offset to cell
ADD r8 r0 r4
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
% Determine number of neighbours
% TODO - this checks out-of-bounds
%
%  [x-1][y-1] = -(width+1)
%  [x][y-1] = -width
%  [x+1][y-1] = -(width-1)
%
%  [x-1][y] = -1
%  [x+1][y] = 1
%
%  [x-1][y+1] = width-1
%  [x][y+1] = width
%  [x+1][y+1] = width+1
%
LDC r9 0
% width + 1 (using the Live flag)
ADD r10 r0 r14
% r13 = offset
% [x-1][y-1] = -width - 1
SUB r13 r8 r10
% if (M[r13] == 1) n++
LDR r10 r13
ADD r9 r9 r13
% Increment location (again, using the Live flag)
% [x][y-1] = -width
ADD r13 r13 r14
LDR r10 r13
ADD r9 r9 r13
% Increment location
% [x+1][y-1] = -width + 1
ADD r13 r13 r14
LDR r10 r13
ADD r9 r9 r13
% offset - 1
% [x-1][y] = -1
SUB r13 r8 r14
LDR r10 r13
ADD r9 r9 r13
% offset + 1
% [x+1][y] = 1
ADD r13 r8 r14
LDR r10 r13
ADD r9 r9 r13
% width - 1
SUB r10 r0 r14
% r13 = offset
% [x-1][y+1] = width-1
ADD r13 r8 r10
% if (M[r13] == 1) n++
LDR r10 r13
ADD r9 r9 r13
% Increment location
% [x][y+1] = width
ADD r13 r13 r14
LDR r10 r13
ADD r9 r9 r13
% Increment location
% [x+1][y+1] = width+1
ADD r13 r13 r14
LDR r10 r13
ADD r9 r9 r13
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
% Get liveness of cell
LDR r10 r8
% Offset to new state cell
ADD r11 r7 r4
% Store old liveness
STR r11 r10
% Increment iterator
ADD r4 r4 r6
% Is it alive?
CMP r10 r10 r1
BLTEZ r10 dead
alive:
% if less than two => dead
LDC r10 2
CMP r10 r9 r10
BLTZ r10 die
% if greater than three => dead
LDC r10 3
CMP r10 r9 r10
BGTZ r10 die
B loop
die:
% Set to dead
STR r11 r15
B loop
dead:
% if equal to three => alive
LDC r10 3
CMP r10 r9 r10
BZ r10 live
B loop
live:
B loop
break:
LDC r10 1
ADD r11 r11 r10
B tick
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
end:
HLT