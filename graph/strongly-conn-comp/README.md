# Algorithms based on breath-/deep-first search
Perform multiple operations on three graphs.  

1) The first graph is undirected and small (see picture).  
Start from a given point, explore and label nodes via BFS and DFS, calculate distances from it via BFS, identify and label connected components.

2) The second graph is directed and small (see picture).  
Start from a given point, explore and label nodes via BFS and DFS, label points according to a topological order via DFS.

2) The third graph is directed and large.  
Identify and label the strongly connected components via Kosaraju's algorithm https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm.

Compile: make  
Run: ./main.out  
Input:  choice of graph and starting node  
Output: prints node table, see above
