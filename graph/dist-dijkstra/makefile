all: main.o heap.o
	g++ main.o heap.o -o main.out

main.o: main.cpp
	g++ -c main.cpp

heap.o: heap.cpp heap.h
	g++ -c heap.cpp

clean:
	rm *.o
	rm *.out