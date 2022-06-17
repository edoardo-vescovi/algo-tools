all: main.o hash_table.o
	g++ main.o hash_table.o -o main.out

main.o: main.cpp hash_table.h
	g++ -c main.cpp

hash_table.o: hash_table.h hash_table.cpp
	g++ -c hash_table.cpp

clean:
	rm *.o
	rm *.out