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
LDC r0 0
LDC r1 1
PRNT r0
PRNT r1
ADD r1 r1 r1
PRNT r0
PRNT r1
BZ r0 end
ADD r1 r1 r1
PRNT r0
PRNT r1
HLT
end:
PRNT r0
PRNT r1
HLT