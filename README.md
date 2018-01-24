# Wator Simulation 4th Yr Project

### Project Overview
Joint Project for 4th Year Distributed and Concurrent Devices, Software Development IT Carlow. 

This project is a Wa-tor simulation https://en.wikipedia.org/wiki/Wa-Tor. 

The initial goal is to produce a sequential implementation of the well known Wa-tor world
simulation.

The 2nd problem will be to implement OpenMP to speed up the processing by utilizing threads.

Benchmarking will also take place to indicate the speed increase/decrease when manipulating thread count with OpenMP.

Finally, the code will be profiled using gprof profiler. 
This will give an indication into where the processor is spending the 
most amount of time, thus leading to code refactoring.

### Design

For the design & implementation of this project, the world is monitored using two 3D arrays. The two arrays are called ocean and oceanCopy. ocean is used by sfml to displat the current locations of the Wa-tor denizens. The oceanCopy array is used to store the moves for each iteration before displaying. When each iteration is complete, the oceanCopy is transferred to ocean.
The three layers can be described as follows:
          ocean[][][0] is used to store the ascii representation of the denizen (0 for shark, . for fish).
          ocean[][][1] is used to store the breed times.
          ocean[][][2] is used to store the starve time for the shark.
The same description can be applied to ocean copy.

### Tools & Environment

Code written in C++.

Emacs environment was used for production.

g++ compiler used with sfml libraries and flags.

Documents produced utilizing Doxygen.

### How to run & dependencies

This project requires that the sfml libraries are installed on the machine and that the sfml environment is setup.

Assuming that your environment is setup correctly, the project can be run by using the following command in a bash terminal:
         
         make SFML
         
         ./watorMainSFML

Documentation for the project can be produced using Doxegen as follows(assumes existing installation of Doxygen):
          
          make doc $L = "Whatever you want to call the project" $B = "Brief for your project"
          
The make doc command requires that a template Doxyfile exists on the root directory of your machine, the Doxyfile parameters for project name and project brief will be overwritten by $L and $B.

### Benchmarking for sequential implementation

The purpose of the benchmarking is to compare the speed of the each iteration against different grid sizes. The grid sizes used are 20 x 20, 40 x 40, 80 x 80, 160 x 160, 320 x 320. Data is collected for 10,000 iterations for each grid size. This produced the following data:

	20x20	          40x40	          80x80	          160x160	          320x320
Max	0.013842	          0.011893	          0.013627	          0.013534	          0.033594
Std Dev +	0.007058205	0.006236237	0.007727532	0.010408402	0.030848818
Average	0.000274411	0.000579474	0.001828064	0.007282804	0.028103637
Std Dev -	0.00018856	0.000400241	0.00107	          0.003709932	0.013204447
Min	0.000153	          0.000326	          0.000756	          0.00223	          0.007033

A visual representation:



