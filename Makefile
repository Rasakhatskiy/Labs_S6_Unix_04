all: main

main: main.cpp
	g++ -std=c++11 -pthread -Wall -o main main.cpp