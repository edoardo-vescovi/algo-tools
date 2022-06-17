# Local minimum in matrix of integers

A local minimum is an entry (i,j) smaller than its top (i-1,j), bottom (i+1,j), left (i,j-1) and right neighbors (i,j+1), provided these exist.

Algorithm paradigm: divide-and-conquer  
Time complexity: n  
Compile: g++ main.cpp -o main.out  
Run: ./main.out  
Output: highlight local lowest value in random matrix and its neighbours (red)
