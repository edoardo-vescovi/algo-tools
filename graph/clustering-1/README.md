# Single-link k-clustering algorithm

The file lists all node pairs, connected by an edge with a given weight. The program groups them in k=4 clusters of maximum spacing https://www.cs.princeton.edu/~wayne/kleinberg-tardos/pearson/04MinimumSpanningTrees.pdf.

Algorithm paradigm: greedy  
Time complexity: (#nodes + #edges)* log*(#nodes)  (thanks to lazy union, union by rank and path compression)  
Compile: make  
Run: ./main.out  
Output: maximum spacing
