all: main.o union_find.o
	g++ main.o union_find.o -o main.out

main.o: main.cpp union_find.h
	g++ -c main.cpp

union_find.o: union_find.h union_find.cpp
	g++ -c union_find.cpp

clean:
	rm *.o
	rm *.out