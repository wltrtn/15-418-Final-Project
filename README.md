# 15-418-Final-Project Proposal

Walter Tan (waltert)

Parallel Sudoku Solver

### Summary
I will be implementing both serial and parallel versions of an algorithm that solves Sudoku puzzles, and hope to analyze the different speedups that parallelism can give. I plan to use OpenMP to parallelize the algorithm.

### Background 
Sudoku is a combinatoral puzzle game where the goal is to fill a 9x9 grid with numbers such that each row, column, and 3x3 subgrid contains each number 1 through 9 only once. At the start of the game the player starts with a partially filled grid, where the selection and amount of squares filled typically affects the difficulty of that particular puzzle. Sudoku puzzles first appeared in the late 1970s under a different name, but did not gain popularity until a Japanese puzzle company officially published it under the name "Sudoku". Sudoku puzzles are typically 9x9 grids with 3x3 subgrids, but also come in many variants. Smaller and larger grid sizes are possible, as well as different shapes. Variants also include extra constraints, which can limit the ways numbers can be filled.

Sudoku solvers are programs that find solutions to Sudoku puzzles, and typically do so through brute force search. Given a start grid, the solver will attempt every possible combination of numbers for the grid. In particular, many solvers use backtracking algorithms, where branches of possible solutions are explored completely before moving on to the next branch. The number of possible combinations is huge, so parallelism gives the perfect opportunity to speed up this search. Not all search patterns are dependent on each other, so different patterns can be searched for in parallel.

### Challenge
First off, for a standard 9x9 Sudoku grid, the number of solved grids amounts to about 6.7 x 10^21. Even when giving a partially filled grid to start with, many possible solutions are left, so a parallelized solver would need to efficiently test all of these. Challenges with parallelization will involve determining exactly what parts can be parallelized, especially for a backtracking solution that involves depth-first search. Depth-first search seems hard to parallelize because it relies on stacks, which threads cannot work together on without high contention. Also, is it not obvious how to distribute the work of searching for solutions among many workers. Synchronization and communication is also a challenge, as I will have to figure out how many different workers can combine and communicate their results.

### Resources
For my code, I plan to use OpenMP code from previous assignments as a starting point. Various resources and tutorials for OpenMP are available online. For implementing my solver algorithm, I plan to search online for resources that can provide a good starting point. For data sets of unsolved Sudoku puzzles, I will probably search various websites. For computing resources, I plan to use Gates cluster machines. If more computing power is needed, I plan to use the Pittsburgh Supercomputing Cluster machines. 

### Goals and Deliverables
75% Target: I plan to have both a serial and parallel version of a Sudoku solver algorithm implemented.

100% Target: I plan to implement more than one solver algorithm. 

125% Target: I plan to extend my implementations to work with Sudoku grids of sizes larger than 9x9.

### Schedule
March 27 - April 2: Implement serial version of a Sudoku Solver (DONE)

April 3 - April 9: Start working on a parallel version of the solver (DONE)

April 10 - April 16: Finish implementing the parallel version. (DONE)

April 17 - April 23: Implement a different solver algorithm. (DONE)

April 24 - April 30: Start working on and complete the project report (DONE)

May 1 - May 4: Prepare for final presentation (DONE)

### MILESTONE (April 11)
So far, I have finished implemented a serial version of a Sudoku Solver. The algorithm is a brute force DFS algorithm, which attempts to find solutions by filling values for all empty squares, testing the values for legalness, and then backtracking if the value is not legal. The algorithm is correct for boards of various sizes. 9x9 boards seem to take at most 20s to solve. Larger 16x16 and 25x25 boards can take up to several minutes to solve, but this indicates good potential for parallel speedup.

With respect to my goals and deliverables, I am a little behind schedule. While my algorithm works for boards of varying sizes, I have not implemented parallelism yet. I believe I will still be able to achieve this goal in time. I am not sure if I can still achieve the goal of implementing more than one solver algorithm in time, as right now I am just focusing on my parallelizing my serial algorithm. At the poster session, I plan to show various graphs detailing different speedups from running my parallel algorithm on varying number of cores. The only results I have right now are computation times for my serial algorithm, as well as the corresponding solved boards. Most 9x9 boards are solved within seconds, with a worst case taking up to 15 seconds. Larger boards take anywhere from 10s of seconds to several minutes.

Issues that concern me the most are finding out how to parallelize my current serial code. My algorithm implements DFS by using recursion, which I believe might be hard to parallelize. However, if I start my work early I believe I can overcome these hurdles.

### FINAl REPORT

Presentation Video: https://drive.google.com/file/d/1wx8JZa-gGyuG5a8D7S2Ohr2uDS-Bjhj-/view?usp=sharing

Final Report pdf and docx are in the repo.
