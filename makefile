all:
	g++ -c main.cpp cpu.cpp debug.cpp
	g++ -o main.exe main.o cpu.o debug.cpp
	./main.exe

