#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include "../../2sum/hash_table.h"
#include "../min-span-tree-kruskal/union_find.h"

class Graph{
    public:

        //array dimensions
        int num_nodes;                  //number of node_groups and nodes
                                        //and first dimension of bits

        int num_bits;                   //second dimension of bits
        char **bits2;                   //node labels: strings of 0's and 1's
        long long int *bits10;          //node labels: corresponding numbers in base 10

        Union_find node_groups;         //the i-th entry tells which cluster the i-th node belongs to

        //functions
        Graph(const char *namefile);
        Graph(const Graph& old);
        Graph& operator=(const Graph& old);
        bool have_nodes_same_bits(int node1,int node2);
        int clusteringHammingUnionFind();           //split graph into maximum-spacing k clusters, using union-find
                                                    //ensures maximum spacing >=3
                                                    //returns number of clusters k
        ~Graph();
};

long long int charToInt(char *c, int length)
{
    long long int res=0;
    int i;
    for(i=0;i<length;i++)
    {
        res+=(((long long int)c[i])<<(length-i-1));
    }
    return res;
}

Graph::Graph(const char *namefile)
{
    FILE *file=fopen(namefile,"r");

    int i,j,c;

    //read num_nodes and num_bits
    fscanf(file,"%i %i\n",&num_nodes,&num_bits);

    //read bits2
    //calculate bits10
    bits2=(char **)malloc(num_nodes*sizeof(char *));
    bits10=(long long int *)malloc(num_nodes*sizeof(long long int ));
    for(i=0;i<num_nodes;i++)
    {
        bits2[i]=(char *)malloc(num_bits*sizeof(char));
        for(j=0;j<num_bits;j++)
        {
            fscanf(file,"%i ",&c);
            bits2[i][j]=(char)c;
        }
        bits10[i]=charToInt(bits2[i], num_bits);
    }

    fclose(file);

    //allocate memory for node_groups
    node_groups.initialise(num_nodes);
}

Graph::Graph(const Graph& old)
{
    num_nodes=old.num_nodes;
    num_bits=old.num_bits;
    node_groups=old.node_groups;

    int i,j;

    bits10=(long long int*)malloc(num_nodes*sizeof(long long int));
    bits2=(char **)malloc(num_nodes*sizeof(char *));
    for(i=0;i<num_nodes;i++)
    {
        bits10[i]=old.bits10[i];
        bits2[i]=(char *)malloc(num_bits*sizeof(char));
        for(j=0;j<num_bits;j++)
        {
            bits2[i][j]=old.bits2[i][j];
        }
    }
}

Graph& Graph::operator=(const Graph& old)
{    
    if(this!=&old)
    { 
        num_nodes=old.num_nodes;
        num_bits=old.num_bits;
        node_groups=old.node_groups;

        int i,j;

        if(bits10)
        {
            free(bits10);
        }
        bits10=(long long int*)malloc(num_nodes*sizeof(long long int));
        for(i=0;i<num_nodes;i++)
        {
            bits10[i]=old.bits10[i];
        }

        if(bits2)
        {
            free(bits2);
        }
        bits2=(char **)malloc(num_nodes*sizeof(char *));
        for(i=0;i<num_bits;i++)
        {
            bits2[i]=(char *)malloc(num_bits*sizeof(char));
            for(j=0;j<num_bits;j++)
            {
                bits2[i][j]=old.bits2[i][j];
            }
        }
    }

    return *this;
}

int Graph::clusteringHammingUnionFind()
{
    int node1, node2, i, j, j1, j2;

    //initialise hash table with nodes (key = node label; object = node number; previous implementation forces use of TARGET)
    //initialise union-find
    Hash_table hash;
    hash.initialise((((long long int)1)<<num_bits)/TARGET+2);

    for(node1=1;node1<=num_nodes;node1++)
    {
        hash.insert(node1,bits10[node1-1]);
        node_groups.push(node1);
    }

    //list of possibly-existing labels
    char **neighbouring_bits2=(char **)malloc((1+num_bits+(num_bits*(num_bits-1))/2)*sizeof(char *));
    long long int *neighbouring_bits10=(long long int *)malloc((1+num_bits+(num_bits*(num_bits-1))/2)*sizeof(long long int));

    //for each node...
    for(node1=1;node1<=num_nodes;node1++)
    {
        //...copy bits
        for(i=0;i<1+num_bits+(num_bits*(num_bits-1))/2;i++)
        {
            neighbouring_bits2[i]=(char *)malloc(num_bits*sizeof(char));
            for(j=0;j<num_bits;j++)
            {
                neighbouring_bits2[i][j]=bits2[node1-1][j];
            }
        }
        //...possibly-existing labels with Hamming distance 0: no flips
        neighbouring_bits10[0]=charToInt(neighbouring_bits2[0],num_bits);
        //...possibly-existing labels with Hamming distance 1: flip 1 bit
        for(j=0;j<num_bits;j++)
        {
            neighbouring_bits2[j+1][j]=(char)(1-neighbouring_bits2[j+1][j]);
            neighbouring_bits10[j+1]=charToInt(neighbouring_bits2[j+1],num_bits);
        }
        //...possibly-existing labels with Hamming distance 2: flip 2 bits
        i=1+num_bits;
        for(j1=0;j1<num_bits;j1++)
        {
            for(j2=j1+1;j2<num_bits;j2++)
            {
                neighbouring_bits2[i][j1]=(char)(1-neighbouring_bits2[i][j1]);
                neighbouring_bits2[i][j2]=(char)(1-neighbouring_bits2[i][j2]);
                neighbouring_bits10[i]=charToInt(neighbouring_bits2[i],num_bits);
                i++;
            }
        }

        //...run over neighbouring labels
        for(i=0;i<1+num_bits+(num_bits*(num_bits-1))/2;i++)
        {
            //...if a neighbouring label exist in hash table
            if(hash.lookup(neighbouring_bits10[i]))
            {
                //...find corresponding nodes and join them with node1
                for(node2=1;node2<=num_nodes;node2++)
                {
                    if(node1!=node2 && bits10[node2-1]==neighbouring_bits10[i])
                    {
                        node_groups.Union(node1-1,node2-1);
                    }
                }
            }
        }
    }

    //clear
    for(i=0;i<1+num_bits+(num_bits*(num_bits-1))/2;i++)
    {
        free(neighbouring_bits2[i]);
    }
    free(neighbouring_bits2);
    free(neighbouring_bits10);

    //return number k of clusters
    return node_groups.num_groups;
}

Graph::~Graph()
{
    int i;
    for(i=0;i<num_nodes;i++)
    {
        free(bits2[i]);
    }
    free(bits2);
    free(bits10);
}

int main()
{
    Graph graph("clustering_big.txt");

    printf("\nAfter joining all nodes with Hamming distance 0,1 and 2, the number of clusters is: %i\n\n",graph.clusteringHammingUnionFind());

    return EXIT_SUCCESS;
}