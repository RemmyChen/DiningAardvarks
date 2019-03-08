# aardvarks
Philosophers' table with a twist : aardvarks eating ants from anthills

One of the most famous problems in concurrency is that of the "Dining Philosophers", who must share knives and forks in a concurrent manner.
This is the "Dining Aardvarks", where there are 11 aardvarks and 3 anthills, there are 100 ants in each anthill, and only 3 aardvarks can share one anthill at a time. It takes one second of real time for an aardvark to slurp up an ant, and another second for the aardvark to swallow it, during which time it is not using the anthill and another aardvark can start slurping. However, should an aardvark attempt to slurp an ant from an anthill where 3 aardvarks are already slurping, or make any other kind of mistake, including attempting to slurp from an already empty anthill or an anthill that doesn't exist, it will be 4 seconds before the aardvark is available to slurp again. The simulator calls that 'sulking'. The goal is for the aardvarks to finish eating all the ants from the anthills as soon as possible - without deadlocking or race conditions!



