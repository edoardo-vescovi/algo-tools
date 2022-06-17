all: main.o bloom_filter.o
	g++ main.o bloom_filter.o -o main.out

main.o: main.cpp bloom_filter.h
	g++ -c main.cpp

bloom_filter.o: bloom_filter.h bloom_filter.cpp
	g++ -c bloom_filter.cpp

clean:
	rm *.o
	rm *.out