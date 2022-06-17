all: main.o buffer.o graph.o
	g++ main.o buffer.o graph.o -o main.out

main.o: main.cpp
	g++ -c main.cpp

buffer.o: buffer.cpp buffer.h
	g++ -c buffer.cpp

graph.o: graph.cpp graph.h
	g++ -c graph.cpp

clean:
	rm *.o
	rm *.out