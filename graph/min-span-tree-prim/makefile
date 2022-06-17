all: main.o ../dist-dijkstra/heap.o
	g++ main.o ../dist-dijkstra/heap.o -o main.out

main.o: main.cpp ../dist-dijkstra/heap.h
	g++ -c main.cpp

heap.o: ../dist-dijkstra/heap.h ../dist-dijkstra/heap.cpp
	g++ -c ../dist-dijkstra/heap.cpp

clean:
	rm *.o
	rm *.out