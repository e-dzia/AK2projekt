all:
	g++ main.cpp TimerLinux.cpp Utils.cpp -o mergesort -std=c++11 -g -pedantic -Wall -fopenmp
