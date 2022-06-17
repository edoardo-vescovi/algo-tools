all: main.o ../min-span-tree-kruskal/union_find.o
	g++ main.o ../min-span-tree-kruskal/union_find.o -o main.out

main.o: main.cpp ../min-span-tree-kruskal/union_find.h
	g++ -c main.cpp

union_find.o: ../min-span-tree-kruskal/union_find.h ../min-span-tree-kruskal/union_find.cpp
	g++ -c ../min-span-tree-kruskal/union_find.cpp

clean:
	rm *.o
	rm *.out