#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "../min-span-tree-kruskal/union_find.h"

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
                                        //and first dimension of node_groups, adjacency_list and degrees

        int *degrees;                   //the i-th entry is n_i
                                        //and ~ second dimension of adjacency_list

        int num_edges;                  //number of edges
                                        //and first dimension of incidence_list_first, and incidence_list_second

        Union_find node_groups;         //the i-th entry tells which cluster the i-th node belongs to

        //functions
        Graph(const char *namefile);
        Graph(const Graph& old);
        Graph& operator=(const Graph& old);
        int clusteringUnionFind(int k);             //split graph into maximum-spacing k clusters, using union-find
                                                    //returns maximum spacing
        ~Graph();
};

Graph::Graph(const char *namefile)
{
    FILE *file=fopen(namefile,"r");

    int i,j,k,l;

    //read num_nodes and num_edges
    fscanf(file,"%i\n",&num_nodes);
    num_edges=(num_nodes*(num_nodes+1))/2;

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

    //allocate memory for node_groups
    node_groups.initialise(num_edges);
}

Graph::Graph(const Graph& old)
{
    num_nodes=old.num_nodes;
    num_edges=old.num_edges;
    node_groups=old.node_groups;

    int i,j;

    incidence_list_first=(int *)malloc(num_edges*sizeof(int));
    incidence_list_second=(int *)malloc(num_edges*sizeof(int));
    for(i=0;i<num_edges;i++)
    {
        incidence_list_first[i]=old.incidence_list_first[i];
        incidence_list_second[i]=old.incidence_list_second[i];
    }

    degrees=(int *)malloc(num_nodes*sizeof(int));
    for(i=0;i<num_nodes;i++)
    {
        degrees[i]=old.degrees[i];
    }

    incidence_list_weights=(int *)malloc(num_edges*sizeof(int));
    for(i=0;i<num_edges;i++)
    {
        incidence_list_weights[i]=old.incidence_list_weights[i];
    }

    adjacency_list=(int **)malloc(num_nodes*sizeof(int *));
    adjacency_list_weights=(int **)malloc(num_nodes*sizeof(int *));
    for(i=0;i<num_nodes;i++)
    {
        adjacency_list[i]=(int *)malloc(degrees[i]*sizeof(int));
        adjacency_list_weights[i]=(int *)malloc(degrees[i]*sizeof(int));
        for(j=0;j<degrees[i];j++)
        {
            adjacency_list[i][j]=old.adjacency_list[i][j];
            adjacency_list_weights[i][j]=old.adjacency_list_weights[i][j];
        }
    }
}

Graph& Graph::operator=(const Graph& old)
{    
    if(this!=&old)
    { 
        num_nodes=old.num_nodes;
        num_edges=old.num_edges;
        node_groups=old.node_groups;

        int i,j;

        if(incidence_list_first)
        {
            free(incidence_list_first);
        }
        incidence_list_first=(int *)malloc(num_edges*sizeof(int));
        for(i=0;i<num_edges;i++)
        {
            incidence_list_first[i]=old.incidence_list_first[i];
        }

        if(incidence_list_second)
        {
            free(incidence_list_second);
        }
        incidence_list_second=(int *)malloc(num_edges*sizeof(int));
        for(i=0;i<num_edges;i++)
        {
            incidence_list_second[i]=old.incidence_list_second[i];
        }

        if(degrees)
        {
            free(degrees);
        }
        degrees=(int *)malloc(num_nodes*sizeof(int));
        for(i=0;i<num_nodes;i++)
        {
            degrees[i]=old.degrees[i];
        }

        if(incidence_list_weights)
        {
            free(incidence_list_weights);
        }
        incidence_list_weights=(int *)malloc(num_edges*sizeof(int));
        for(i=0;i<num_edges;i++)
        {
            incidence_list_weights[i]=old.incidence_list_weights[i];
        }

        if(adjacency_list)
        {
            free(adjacency_list);
        }
        adjacency_list=(int **)malloc(num_nodes*sizeof(int *));
        for(i=0;i<num_nodes;i++)
        {
            adjacency_list[i]=(int *)malloc(degrees[i]*sizeof(int));
            for(j=0;j<degrees[i];j++)
            {
                adjacency_list[i][j]=old.adjacency_list[i][j];
            }
        }

        if(adjacency_list_weights)
        {
            free(adjacency_list_weights);
        }
        adjacency_list_weights=(int **)malloc(num_nodes*sizeof(int *));
        for(i=0;i<num_nodes;i++)
        {
            adjacency_list_weights[i]=(int *)malloc(degrees[i]*sizeof(int));
            for(j=0;j<degrees[i];j++)
            {
                adjacency_list_weights[i][j]=old.adjacency_list_weights[i][j];
            }
        }
    }

    return *this;
}

Graph::~Graph()
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
int Graph::clusteringUnionFind(int k)
{
    if(k<0 || k>num_nodes)
    {
        return -1;
    }

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

    //initialise UNION-FIND
    node_groups.initialise(num_nodes);
    int i;
    for(i=0;i<num_nodes;i++)
    {
        node_groups.push(i+1);
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

    //...as long as we have to CLUSTER
    while(node_groups.num_groups>=k)
    {
        edge_greedy=sorted_edges[sorted_edge_greedy-1].x;
        //...CONSIDER SEPARATED NODES
        if(node_groups.find(incidence_list_first[edge_greedy-1]-1)!=node_groups.find(incidence_list_second[edge_greedy-1]-1))
        {
            if(node_groups.num_groups>k)
            {
                //...IF STILL WILL HAVE k CLUSTERS, ADD JOINING EDGE
                node_groups.Union(incidence_list_first[edge_greedy-1]-1,incidence_list_second[edge_greedy-1]-1);
            }
            else
            {
                //...OTHERWISE DONE
                break;
            }
        }
        sorted_edge_greedy++;
    }
    return incidence_list_weights[edge_greedy-1];
}

int main()
{
    Graph graph("clustering1.txt");

    int k=4;
    int max_spacing=graph.clusteringUnionFind(k);
    if(max_spacing!=-1)
    {
        printf("\nThe maximum spacing of %i-clustering is: %i\n\n",k,max_spacing);
    }

    return EXIT_SUCCESS;
}
