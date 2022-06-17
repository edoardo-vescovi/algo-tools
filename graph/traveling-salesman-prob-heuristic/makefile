all: main.o
	g++ main.o -o main.out

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm *.o
	rm *.out