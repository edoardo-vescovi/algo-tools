#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>
#include <algorithm>
#include "union_find.h"

int min(int x, int y)
{
    if(x<y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

int max(int x, int y)
{
    if(x>y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

class Graph{
    public:

        //arrays
        int **adjacency_list;           //the i-th entry is the array of nodes adjacents to i-th node:
                                        //(1st adjacent node, ... n_j-th adjacent node)
        int **adjacency_list_weights;   //as above, for weights
        
        int *incidence_list_first;      //the i-th entry in these are node pair linked by the i-th edge:
        int *incidence_list_second;     //(1st node, 2nd node)
                                        //created with assumption 1st node < 2nd node
        int *incidence_list_weights;    //the i-th entry is the weight of the i-th edge

        //array dimensions
        int num_nodes;                  //number of nodes
                                        //and first dimension of node_groups, adjacency_list, degrees and distances

        int *degrees;                   //the i-th entry is n_i
                                        //and ~ second dimension of adjacency_list

        int num_edges;                  //number of edges
                                        //and first dimension of incidence_list_first, incidence_list_second and MST

        bool *MST;                      //the i-th entry tells if the i-th edge belongs to the MST

        //functions
        void initialise(const char *namefile);
        void KruskalMSTUnionFind();     //find edges of mininum spanning tree, using union-find
        void clear();
};

void Graph::initialise(const char *namefile)
{
    FILE *file=fopen(namefile,"r");

    int i,j,k,l;

    //read num_nodes and num_edges
    fscanf(file,"%i %i\n",&num_nodes,&num_edges);

    //read incidence_list
    //calculate degrees and adjacency_list
    incidence_list_first=(int *)malloc(num_edges*sizeof(int));
    incidence_list_second=(int *)malloc(num_edges*sizeof(int));
    incidence_list_weights=(int *)malloc(num_edges*sizeof(int));

    adjacency_list=(int **)malloc(num_nodes*sizeof(int *));
    adjacency_list_weights=(int **)malloc(num_nodes*sizeof(int *));
    for(i=0;i<num_nodes;i++)
    {
        adjacency_list[i]=NULL;
        adjacency_list_weights[i]=NULL;
    }

    degrees=(int *)calloc(num_nodes,sizeof(int));

    int node1, node2, deg1, deg2, weight;
    for(i=0;i<num_edges;i++)
    {
        fscanf(file,"%i %i %i\n",&node1,&node2,&weight);
        incidence_list_first[i]=min(node1,node2);
        incidence_list_second[i]=max(node1,node2);
        incidence_list_weights[i]=weight;

        deg1=++degrees[node1-1];
        deg2=++degrees[node2-1];

        adjacency_list[node1-1]=(int *)realloc(adjacency_list[node1-1],deg1*sizeof(int));
        adjacency_list[node2-1]=(int *)realloc(adjacency_list[node2-1],deg2*sizeof(int));
        adjacency_list[node1-1][deg1-1]=node2;
        adjacency_list[node2-1][deg2-1]=node1;

        adjacency_list_weights[node1-1]=(int *)realloc(adjacency_list_weights[node1-1],deg1*sizeof(int));
        adjacency_list_weights[node2-1]=(int *)realloc(adjacency_list_weights[node2-1],deg2*sizeof(int));
        adjacency_list_weights[node1-1][deg1-1]=weight;
        adjacency_list_weights[node2-1][deg2-1]=weight;
    }

    fclose(file);

    //allocate memory for MST
    MST=(bool *)malloc(num_edges*sizeof(bool));
}

void Graph::clear()
{
    int i;
    for(i=0;i<num_nodes;i++)
    {
        free(adjacency_list[i]);
        free(adjacency_list_weights[i]);
    }
    free(adjacency_list);
    free(adjacency_list_weights);

    free(incidence_list_first);
    free(incidence_list_second);
    free(incidence_list_weights);
    
    free(degrees);
    free(MST);
}

//pair of edge and weight
class Pair
{
    public:
        int x;
        int y;
};

bool less_than(Pair pair1, Pair pair2)
{
    return (pair1.y)<(pair2.y);
}

//we capitalise differences from Dijkstra's and Prim's algorithms
void Graph::KruskalMSTUnionFind()
{
    //SORT edges in ascending weight
    std::vector<Pair> sorted_edges;
    Pair sorted_edge;
    int edge;
    for(edge=1;edge<=num_edges;edge++)
    {
        sorted_edge.x=edge;
        sorted_edge.y=incidence_list_weights[edge-1];
        sorted_edges.push_back(sorted_edge);
    }
    std::sort(sorted_edges.begin(),sorted_edges.end(),&less_than);

    //NO STARTING NODE

    //initialise UNION-FIND and MST
    Union_find node_groups;
    node_groups.initialise(num_nodes);
    int i;
    for(i=0;i<num_nodes;i++)
    {
        node_groups.push(i+1);
    }
    for(i=0;i<num_edges;i++)
    {
        MST[i]=false;
    }

    //during an iteration
    //...crossing edge that minimises greedy score so far
    int edge_greedy;
    int sorted_edge_greedy=1;
    //...its endpoints
    int endpoint_out_greedy;
    //...quantity that minimises greedy score so far
    int tmp_greedy;

    //...analogues, that may beat above
    //int edge;
    int endpoint_in,endpoint_out;
    int tmp;

    //...as long as we have to calculate MST
    while(sorted_edge_greedy<=num_edges)
    {
        edge_greedy=sorted_edges[sorted_edge_greedy-1].x;
        //...add edge if a cycle is not created
        if(node_groups.find(incidence_list_first[edge_greedy-1]-1)!=node_groups.find(incidence_list_second[edge_greedy-1]-1))
        {
            node_groups.Union(incidence_list_first[edge_greedy-1]-1,incidence_list_second[edge_greedy-1]-1);
            MST[edge_greedy-1]=true;
        }
        sorted_edge_greedy++;
    }
}

int main()
{
    Graph graph;
    graph.initialise("../min-span-tree-Prim/edges.txt");

    graph.KruskalMSTUnionFind();
    int edge,res=0;
    for(edge=0;edge<=graph.num_edges;edge++)
    {
        if(graph.MST[edge])
        {
            res+=graph.incidence_list_weights[edge];
        }
    }

    printf("\nThe cost of a minimum spanning tree is: %i\n\n",res);

    graph.clear();

    return EXIT_SUCCESS;
}
