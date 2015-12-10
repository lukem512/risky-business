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
[x] -eus [2|3] -dus 6 -fus 6 -f compiled/benchmarks/[fib|bubble].bin gives wonky results
[x] -eus [1|2|3] -dus 6 -fus 6 -f compiled/benchmarks/sop.bin gives wonky results
[x] -eus 3 -fus 6 -dus 7 -f compiled/benchmarks/sop.bin stalls
[x] -eus n -fus n -dus <n gives wonky result
[x] Parameterize branch prediction flag
[ ] Execution breaks when run without branch prediction
[ ] Dynamic branch prediction
[ ] Multiple instruction lengths
[ ] Out-of-Order execution