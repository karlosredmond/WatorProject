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
 
