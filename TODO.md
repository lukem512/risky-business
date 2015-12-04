TODO List
---------

[x] FU to hold instructions until passed to DU
[x] FetchUnitManager should pass a vector of all other FUs when executing fus[i].tick to allow dependence-checking
[x] Pass DecodeUnitManager to FU
[x] DU to hold instructions until passed to EU
[x] Pass ExecutionUnitManager to DU
[x] EU to hold instructions until execution is finished
[ ] State to use new *Manager classes
[ ] Test
[ ] Multiple instruction lengths