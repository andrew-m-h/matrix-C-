# Invert Matrix

### Introduction
Invert matrix is a command line tool, allowing one to easily invert an n*n matrix of numbers (int/float/double/long...).
The tool is built, using my personal, though not private, Matrix library originally written for Visual C++, but converted to unix C++ to conform with the C++11 standard.

### Usage
The tool reads matricies from a file, and can output an inverted matrix to either a file, or the terminal.

    $ invert-matrix -d Dimension -i Input [-o Output] [-p Precision] [-h Help]
        Dimension: The dimension of the matrix, greater than 1
        Input: Input file name of file containing input matrix
        Precision: Desired precision of output matrix

### Input File
The input file represents a stream of numbers, which will be read, left to right, top to bottom into the matrix of given dimension (remembering that only square matricies are invertable). This means that the input file can be a list of space seperated numbers, tab seperated with newlines or any mixture.

e.g. space seperated
1 8 -9 7 5 0 1 0 4 4 0 0 1 2 5 0 0 0	1 -5 0 0 0 0 1

### Limitations
The tool uses the method of cofactor expansion to invert a matrix, this gives exponential time and limits the size of the input matrix to realistically less than 10x10. Internally, the numbers are represenetd as double precision, this leads to the all too common limitations when working with high precisions.

### Installation
There are two ways to compile the project, both use the g++ compiler. There is included a Makefile with the project, a simple call to 

    $ make
 should be enough to compile the project.
 However, for those who wish to edit the source code, a codebocks file is included as well, and can be used to compile, debug and edit the project.
To test the projecct, a test 5x5 matrix is provied in the file "matrix.txt". Run:
 
    $ ./invert-matrix -d 5 -i matrix.txt
    $ ./invert-matrix -d 5 -i matrix.txt -o matrix-output.txt
Remember, if your using code::blocks, copy matrix.txt into the directory of the binary.
The appropriate output from these tests should be printeed to the terminal and written to the file matrix-output.txt:

1 -8 9 7 17
-0 1 -0 -4 -24
0 -0 1 -2 -15
-0 0 -0 1 5
0 -0 0 -0 1	

