% Test for branch-prediction and halts
% The static BP Unit will not predict
% forwards-facing branches.
%
% Expected output is:
%  00000000
%  00000001
%  00000000
%  00000002
%  00000000
%  00000002
%
%%%%%%%%%%%%%%%%%%%%%%
% 0
LDC r0 0
% 1
LDC r1 1
% 2
PRNT r0
% 3
PRNT r1
% 4
ADD r1 r1 r1
% 5
PRNT r0
% 6
PRNT r1
% 7
BZ r0 end
% 8
ADD r1 r1 r1
% 9
PRNT r0
% 10
PRNT r1
% 11
HLT
end:
% 12
PRNT r0
% 13
PRNT r1
% 14
HLT