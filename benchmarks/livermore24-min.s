% Livermore Loop #24
% This is the last Livermore Loop kernel
% Find the first minimum in an array
%
% Expected output:
%  0000002b
%  00000001
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
% Length of x
LDC r11 100
% Fill array
% Generated at random.org
%%
LDC r4 65
ST r4 1000
LDC r4 5
ST r4 1001
LDC r4 31
ST r4 1002
LDC r4 11
ST r4 1003
LDC r4 50
ST r4 1004
LDC r4 36
ST r4 1005
LDC r4 38
ST r4 1006
LDC r4 94
ST r4 1007
LDC r4 45
ST r4 1008
LDC r4 71
ST r4 1009
LDC r4 32
ST r4 1010
LDC r4 70
ST r4 1011
LDC r4 91
ST r4 1012
LDC r4 100
ST r4 1013
LDC r4 71
ST r4 1014
LDC r4 46
ST r4 1015
LDC r4 8
ST r4 1016
LDC r4 34
ST r4 1017
LDC r4 29
ST r4 1018
LDC r4 52
ST r4 1019
LDC r4 68
ST r4 1020
LDC r4 60
ST r4 1021
LDC r4 30
ST r4 1022
LDC r4 100
ST r4 1023
LDC r4 89
ST r4 1024
LDC r4 29
ST r4 1025
LDC r4 65
ST r4 1026
LDC r4 41
ST r4 1027
LDC r4 8
ST r4 1028
LDC r4 89
ST r4 1029
LDC r4 70
ST r4 1030
LDC r4 40
ST r4 1031
LDC r4 12
ST r4 1032
LDC r4 11
ST r4 1033
LDC r4 49
ST r4 1034
LDC r4 75
ST r4 1035
LDC r4 18
ST r4 1036
LDC r4 86
ST r4 1037
LDC r4 22
ST r4 1038
LDC r4 41
ST r4 1039
LDC r4 15
ST r4 1040
LDC r4 19
ST r4 1041
LDC r4 75
ST r4 1042
LDC r4 1
ST r4 1043
LDC r4 20
ST r4 1044
LDC r4 56
ST r4 1045
LDC r4 56
ST r4 1046
LDC r4 93
ST r4 1047
LDC r4 70
ST r4 1048
LDC r4 70
ST r4 1049
LDC r4 46
ST r4 1050
LDC r4 94
ST r4 1051
LDC r4 53
ST r4 1052
LDC r4 36
ST r4 1053
LDC r4 91
ST r4 1054
LDC r4 59
ST r4 1055
LDC r4 39
ST r4 1056
LDC r4 48
ST r4 1057
LDC r4 17
ST r4 1058
LDC r4 74
ST r4 1059
LDC r4 96
ST r4 1060
LDC r4 76
ST r4 1061
LDC r4 5
ST r4 1062
LDC r4 17
ST r4 1063
LDC r4 74
ST r4 1064
LDC r4 56
ST r4 1065
LDC r4 29
ST r4 1066
LDC r4 98
ST r4 1067
LDC r4 98
ST r4 1068
LDC r4 65
ST r4 1069
LDC r4 9
ST r4 1070
LDC r4 18
ST r4 1071
LDC r4 24
ST r4 1072
LDC r4 77
ST r4 1073
LDC r4 78
ST r4 1074
LDC r4 93
ST r4 1075
LDC r4 85
ST r4 1076
LDC r4 44
ST r4 1077
LDC r4 36
ST r4 1078
LDC r4 86
ST r4 1079
LDC r4 63
ST r4 1080
LDC r4 57
ST r4 1081
LDC r4 69
ST r4 1082
LDC r4 86
ST r4 1083
LDC r4 52
ST r4 1084
LDC r4 92
ST r4 1085
LDC r4 34
ST r4 1086
LDC r4 68
ST r4 1087
LDC r4 68
ST r4 1088
LDC r4 49
ST r4 1089
LDC r4 20
ST r4 1090
LDC r4 3
ST r4 1091
LDC r4 49
ST r4 1092
LDC r4 91
ST r4 1093
LDC r4 17
ST r4 1094
LDC r4 76
ST r4 1095
LDC r4 14
ST r4 1096
LDC r4 59
ST r4 1097
LDC r4 41
ST r4 1098
LDC r4 64
ST r4 1099
%%
% 
%
% for (l=1; l<=loop; l++)
outer:
CMP r6 r0 r1
BGTEZ r6 endouter
%
% k = 0
LDC r3 0
% m = 0
MOV r4 r3
% for (k=0; k<len(x); k++)
inner:
CMP r6 r3 r11
BGTEZ r6 endinner
%
ADD r5 r10 r3
LDR r5 r5
% x[m]
ADD r6 r10 r4
LDR r6 r6
% if (x[k] < x[m])
CMP r6 r5 r6
BLTZ r6 swap
% Call out to swap function
back:
%
% k++
ADD r3 r2 r3
B inner
endinner:
%
% i = i + incr
ADD r0 r0 r2
B outer
endouter:
% Print m after last iteration
PRNT r4
% Print value of x[m]
ADD r6 r10 r4
LDR r6 r6
PRNT r6
% And halt!
HLT
%
% Set m = k
swap:
MOV r4 r3
B back