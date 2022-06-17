# Huffman coding

Huffman algorithm https://en.wikipedia.org/wiki/Huffman_coding: translate symbols into codewords, with the objective to minimise the average encoding length.  
The file contains 1000 entries, which are the weights (i.e. expected words' frequencies in a text) of the symbols labeled by 1, 2, ... 1000. The program organises them into a binary tree. Each leaf is a symbol, with codeword (010...) given by the sequence of moves (left, right, left...) from the root down to the leaf. Each non-leaf node contains a meta-symbol, i.e. the sequence of symbols that live below that node.  
The average encoding length is the average depth of all symbols in the binary tree, weighted by the frequencies.  

Compile: g++ main.cpp -o main.out  
Run: ./main.out  
Output: maximum/minimum length of characters (0,1) necessary to write a codeword, i.e. the tree's maximum/minimum depth.
