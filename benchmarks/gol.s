% Game of Life
%
% Output is performed using a memory mapping.
% Memory locations from 30000 to 40000 will
% be mapped to a video output, when enabled.
% This gives 100 x 100 output.
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
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 1
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 2
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 1
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 1
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 4
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 2
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 1
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 3
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 1
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 3
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 4
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 3
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 5
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 3
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 6
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 4
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 0
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 4
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 1
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 4
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 4
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 5
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 5
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 6
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 6
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 7
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 3
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 7
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 7
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 9
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 7
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 10
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 8
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 4
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 8
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 5
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 8
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 6
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 8
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 9
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 9
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 9
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 10
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 6
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 10
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 7
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 10
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 8
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%
LDC r7 11
ADD r6 r5 r7
MUL r6 r0 r6
LDC r7 6
ADD r7 r4 r7
ADD r6 r2 r6
ADD r7 r6 r7
LDC r6 1
STR r7 r6
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
% Tick!
% The scrubber example only needs 10 iterations to stabilise
% tick = 0
LDC r11 0
% maxticks = 1
LDC r12 1
tick:
CMP r4 r11 r12
BGTEZ r4 end
PRNT r11
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
PRNT r4
% Offset to cell
ADD r8 r4 r2
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
% Determine number of neighbours
%
%  1. [x-1][y-1] = -(width+1)
%  2. [x][y-1] = -width
%  3. [x+1][y-1] = -(width-1)
%
%  4. [x-1][y] = -1
%  5. [x+1][y] = 1
%
%  6. [x-1][y+1] = width-1
%  7. [x][y+1] = width
%  8. [x+1][y+1] = width+1
%
% neighbours = 0
LDC r9 0
one:
% width + 1 (using the Live flag)
ADD r10 r0 r14
% r13 = offset
% [x-1][y-1] = -width - 1
SUB r13 r8 r10
% Check if within cell area
% cellmin = r2
CMP r10 r13 r2
BLTZ r10 two
% cellmax = r2 + (r0 * r1) = r2 + r5
ADD r10 r2 r5
CMP r10 r13 r10
BGTEZ r10 two
% if (M[r13] == 1) n++
LDR r10 r13
ADD r9 r9 r10
two:
% Increment location (again, using the Live flag)
% [x][y-1] = -width
ADD r13 r13 r14
% Check if within cell area
% cellmin = r2
CMP r10 r13 r2
BLTZ r10 three
% cellmax = r2 + (r0 * r1) = r2 + r5
ADD r10 r2 r5
CMP r10 r13 r10
BGTEZ r10 three
LDR r10 r13
ADD r9 r9 r10
three:
% Increment location
% [x+1][y-1] = -width + 1
ADD r13 r13 r14
% Check if within cell area
% cellmin = r2
CMP r10 r13 r2
BLTZ r10 four
% cellmax = r2 + (r0 * r1) = r2 + r5
ADD r10 r2 r5
CMP r10 r13 r10
BGTEZ r10 four
LDR r10 r13
ADD r9 r9 r10
four:
% offset - 1
% [x-1][y] = -1
SUB r13 r8 r14
% Check if within cell area
% cellmin = r2
CMP r10 r13 r2
BLTZ r10 five
% cellmax = r2 + (r0 * r1) = r2 + r5
ADD r10 r2 r5
CMP r10 r13 r10
BGTEZ r10 five
LDR r10 r13
ADD r9 r9 r10
five:
% offset + 1
% [x+1][y] = 1
ADD r13 r8 r14
% Check if within cell area
% cellmin = r2
CMP r10 r13 r2
BLTZ r10 six
% cellmax = r2 + (r0 * r1) = r2 + r5
ADD r10 r2 r5
CMP r10 r13 r10
BGTEZ r10 six
LDR r10 r13
ADD r9 r9 r10
six:
% width - 1
SUB r10 r0 r14
% r13 = offset
% [x-1][y+1] = width-1
ADD r13 r8 r10
% if (M[r13] == 1) n++
% Check if within cell area
% cellmin = r2
CMP r10 r13 r2
BLTZ r10 seven
% cellmax = r2 + (r0 * r1) = r2 + r5
ADD r10 r2 r5
CMP r10 r13 r10
BGTEZ r10 seven
LDR r10 r13
ADD r9 r9 r10
seven:
% Increment location
% [x][y+1] = width
ADD r13 r13 r14
% Check if within cell area
% cellmin = r2
CMP r10 r13 r2
BLTZ r10 eight
% cellmax = r2 + (r0 * r1) = r2 + r5
ADD r10 r2 r5
CMP r10 r13 r10
BGTEZ r10 eight
LDR r10 r13
ADD r9 r9 r10
eight:
% Increment location
% [x+1][y+1] = width+1
ADD r13 r13 r14
% Check if within cell area
% cellmin = r2
CMP r10 r13 r2
BLTZ r10 liveness
% cellmax = r2 + (r0 * r1) = r2 + r5
ADD r10 r2 r5
CMP r10 r13 r10
BGTEZ r10 liveness
LDR r10 r13
ADD r9 r9 r10
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
liveness:
% Get liveness of cell
LDR r10 r8
% Offset to new state cell
ADD r13 r7 r4
% Store old liveness
STR r13 r10
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
STR r13 r15
B loop
dead:
% if equal to three => alive
LDC r10 3
CMP r10 r9 r10
BZ r10 live
B loop
live:
STR r13 r14
B loop
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
break:
% Replace current values with new values
% x = 0
LDC r4 0
% max = width * height
MUL r10 r0 r1
replace:
% while (x < max)
CMP r13 r4 r10
BGTEZ r13 backtoloop
% Memory address of old cell
ADD r13 r2 r4
% Memory address of new cell
ADD r9 r7 r4
% Replace old with new
LDR r9 r9
STR r13 r9
% Inc x
ADD r4 r4 r14
B replace
%
backtoloop:
ADD r11 r11 r14
B tick
%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%
end:
HLT