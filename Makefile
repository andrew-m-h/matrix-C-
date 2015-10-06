invert-matrix:
	g++ -Wall -fexceptions -O2 -std=c++11 -o invert-matrix main.cpp
	strip invert-matrix

check-syntax:
	gcc -o -Wall -S ${CHK_SOURCES}
