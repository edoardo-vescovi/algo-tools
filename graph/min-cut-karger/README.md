# Minimum cut
Find the number of edges crossing the minimum cut of a connected graph, repeating the Karger's algorithm https://en.wikipedia.org/wiki/Karger%27s_algorithm multiples times. A single execution tries (#nodes)^2 * log(#nodes) times to find a minimum cut and returns the lowest value. This procedure finds the real minimum cut with probability greater than 1/#nodes.  

Time complexity: #nodes^2 * #edges  
Compile: g++ main.cpp -o main.out  
Run: ./main.out  
Output: minimum cut
