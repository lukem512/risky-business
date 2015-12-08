TODO List
---------

[x] FU to hold instructions until passed to DU
[x] FetchUnitManager should pass a vector of all other FUs when executing fus[i].tick to allow dependence-checking
[x] Pass DecodeUnitManager to FU
[x] DU to hold instructions until passed to EU
[x] Pass ExecutionUnitManager to DU
[x] EU to hold instructions until execution is finished
[x] Fetch step needs to change
[x] State to use new *Manager classes
[x] Clean up comments and dead code
[x] Test
[ ] Multiple instruction lengths
[ ] Out-of-Order execution
[ ] Static branch prediction
[ ] Dynamic branch prediction