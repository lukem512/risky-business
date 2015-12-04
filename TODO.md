TODO List
---------

[x] FU to hold instructions until passed to DU
[x] FetchUnitManager should pass a vector of all other FUs when executing fus[i].tick to allow dependence-checking
[ ] Pass DecodeUnitManager to FU
[ ] DU to hold instructions until passed to EU
[ ] Pass ExecutionUnitManager to DU
[ ] EU to hold instructions until execution is finished
[ ] State to use new *Manager classes
[ ] Test