% A more complex benchmark
% This application sorts an array of numbers using Bubble Sort
% The Bubble Sort algorithm is the naive one, with no optimisations
%
%%%%%%%%%%%%%%%%%%%%%%
% Address of A (A)
LDC r0 1000
% Length of A (len)
LDC r1 100
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
% Define A
LDC r4 36
ST r4 1000
LDC r4 65
ST r4 1001
LDC r4 43
ST r4 1002
LDC r4 47
ST r4 1003
LDC r4 79
ST r4 1004
LDC r4 65
ST r4 1005
LDC r4 1
ST r4 1006
LDC r4 57
ST r4 1007
LDC r4 40
ST r4 1008
LDC r4 86
ST r4 1009
LDC r4 57
ST r4 1010
LDC r4 65
ST r4 1011
LDC r4 47
ST r4 1012
LDC r4 64
ST r4 1013
LDC r4 63
ST r4 1014
LDC r4 64
ST r4 1015
LDC r4 13
ST r4 1016
LDC r4 30
ST r4 1017
LDC r4 22
ST r4 1018
LDC r4 76
ST r4 1019
LDC r4 32
ST r4 1020
LDC r4 48
ST r4 1021
LDC r4 92
ST r4 1022
LDC r4 81
ST r4 1023
LDC r4 23
ST r4 1024
LDC r4 43
ST r4 1025
LDC r4 20
ST r4 1026
LDC r4 95
ST r4 1027
LDC r4 12
ST r4 1028
LDC r4 32
ST r4 1029
LDC r4 41
ST r4 1030
LDC r4 34
ST r4 1031
LDC r4 51
ST r4 1032
LDC r4 60
ST r4 1033
LDC r4 44
ST r4 1034
LDC r4 59
ST r4 1035
LDC r4 12
ST r4 1036
LDC r4 16
ST r4 1037
LDC r4 13
ST r4 1038
LDC r4 51
ST r4 1039
LDC r4 82
ST r4 1040
LDC r4 21
ST r4 1041
LDC r4 85
ST r4 1042
LDC r4 40
ST r4 1043
LDC r4 72
ST r4 1044
LDC r4 93
ST r4 1045
LDC r4 84
ST r4 1046
LDC r4 20
ST r4 1047
LDC r4 65
ST r4 1048
LDC r4 10
ST r4 1049
LDC r4 2
ST r4 1050
LDC r4 99
ST r4 1051
LDC r4 26
ST r4 1052
LDC r4 34
ST r4 1053
LDC r4 13
ST r4 1054
LDC r4 51
ST r4 1055
LDC r4 99
ST r4 1056
LDC r4 74
ST r4 1057
LDC r4 52
ST r4 1058
LDC r4 57
ST r4 1059
LDC r4 44
ST r4 1060
LDC r4 70
ST r4 1061
LDC r4 83
ST r4 1062
LDC r4 78
ST r4 1063
LDC r4 24
ST r4 1064
LDC r4 8
ST r4 1065
LDC r4 45
ST r4 1066
LDC r4 83
ST r4 1067
LDC r4 47
ST r4 1068
LDC r4 57
ST r4 1069
LDC r4 12
ST r4 1070
LDC r4 43
ST r4 1071
LDC r4 89
ST r4 1072
LDC r4 84
ST r4 1073
LDC r4 51
ST r4 1074
LDC r4 29
ST r4 1075
LDC r4 77
ST r4 1076
LDC r4 19
ST r4 1077
LDC r4 29
ST r4 1078
LDC r4 34
ST r4 1079
LDC r4 22
ST r4 1080
LDC r4 28
ST r4 1081
LDC r4 34
ST r4 1082
LDC r4 38
ST r4 1083
LDC r4 14
ST r4 1084
LDC r4 52
ST r4 1085
LDC r4 81
ST r4 1086
LDC r4 22
ST r4 1087
LDC r4 78
ST r4 1088
LDC r4 71
ST r4 1089
LDC r4 62
ST r4 1090
LDC r4 2
ST r4 1091
LDC r4 71
ST r4 1092
LDC r4 82
ST r4 1093
LDC r4 91
ST r4 1094
LDC r4 42
ST r4 1095
LDC r4 54
ST r4 1096
LDC r4 80
ST r4 1097
LDC r4 97
ST r4 1098
LDC r4 31
ST r4 1099
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
printu:
% Print UNSORTED array
LDC r3 0
LDC r4 1
% while (i < len)
printuloop:
CMP r5 r3 r1
BGTEZ r5 repeat
ADD r13 r0 r3
LDR r9 r13
PRNT r9
ADD r3 r3 r4
B printuloop
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
repeat:
% swapped = false
LDC r3 0
% i = 1
LDC r4 1
% incr = 1
LDC r5 1
% while (i < len) {
while:
CMP r6 r4 r1
BGTEZ r6 endwhile
% Address of A[i-1]
SUB r7 r4 r5
ADD r8 r0 r7
% Address of A[i]
ADD r9 r0 r4
% A[i-1]
LDR r10 r8
% A[i]
LDR r11 r9
% if (A[i-1] > A[i]) {
CMP r12 r10 r11
BLTEZ r12 endif
% SWAP THEM
STR r8 r11
STR r9 r10
% swapped = true
LDC r3 1
endif:
% i = i + incr
ADD r4 r4 r5
B while
endwhile:
% r13 = false
LDC r13 0
% if (swapped == false)
CMP r14 r3 r13
BNZ r14 repeat
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
prints:
% Print SORTED array
LDC r3 0
LDC r4 1
% while (i < len)
printsloop:
CMP r5 r3 r1
BGTEZ r5 end
ADD r13 r0 r3
LDR r9 r13
PRNT r9
ADD r3 r3 r4
B printsloop
%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%
end:
HLT