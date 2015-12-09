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
[x] Static branch prediction
[ ] -eus [1|2] -dus 6 -fus 6 -f compiled/benchmarks/[fib|bubble].bin, -eus [1|2|3] -dus 6 -fus 6 -f compiled/benchmarks/sop.bin give wonky results
[ ] -eus n -fus n -dus <n gives wonky result
[ ] Dynamic branch prediction
[ ] Multiple instruction lengths
[ ] Out-of-Order execution