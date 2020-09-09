Author: Bruno Rodriguez and Nilay

This repository holds the source and header files (c++) which are used to build a game similar to pacman. It is preferred to install cmake (3.5.1-1ubuntu3) to build these files. The dependencies are enlisted in CMakeLists.txt. Lastest cmake can be installed by running `sudo apt install cmake`. 

Steps to build these files: 

1. Clone the gitlab repository using ssh or https.

2. cd into the cloned folder and make a new directory named "build".
```
mkdir build
```

3. use cmake to generate the `Makefile`. Since `CMakeLists.txt` already holds the dependecies, running the below command will directly generate the `Makefile` for you
```
cmake ..
```

4. After having generated the `Makefile`, run the below command in the same `/build` directory, to generate the executables for you.
```
make all
```

5. execute the generated binary to play the game on the terminal
```
./pacman_orig
```

6. we liked the assignment and tried to go ahead a bit with the game. It has different controls, but it works. 
```
./pacman
```
