all: main.o ../../2sum/hash_table.o ../min-span-tree-kruskal/union_find.o
	g++ main.o ../../2sum/hash_table.o ../min-span-tree-kruskal/union_find.o -o main.out

main.o: main.cpp ../../2sum/hash_table.h ../min-span-tree-kruskal/union_find.h
	g++ -c main.cpp

hash_table.o: ../../2sum/hash_table.h ../../2sum/hash_table.cpp
	g++ -c ../../2sum/hash_table.cpp

union_find.o: ../min-span-tree-kruskal/union_find.h ../min-span-tree-kruskal/union_find.cpp
	g++ -c ../min-span-tree-kruskal/union_find.cpp

clean:
	rm *.o
	rm *.out