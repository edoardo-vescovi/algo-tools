#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../dist-dijkstra/heap.h"

#define INF_WEIGHT INT_MAX

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
                                        //and first dimension of incidence_list_first, incidence_list_second and MST

        bool *MST;                      //the i-th entry tells if the i-th edge belongs to the MST

        //functions
        Graph(const char *namefile);
        Graph(const Graph& old);
        Graph& operator=(const Graph& old);
        void PrimMST();                 //find edges of mininum spanning tree
        void PrimMSTHeap();             //as above, using heap
        ~Graph();
};

Graph::Graph(const char *namefile)
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

    adjacency_list_weights=(int **)malloc(num_nodes*sizeof(int *));

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

Graph::Graph(const Graph& old)
{
    num_nodes=old.num_nodes;
    num_edges=old.num_edges;

    int i,j;

    incidence_list_first=(int *)malloc(num_edges*sizeof(int));
    incidence_list_second=(int *)malloc(num_edges*sizeof(int));
    MST=(bool *)malloc(num_edges*sizeof(bool));
    for(i=0;i<num_edges;i++)
    {
        incidence_list_first[i]=old.incidence_list_first[i];
        incidence_list_second[i]=old.incidence_list_second[i];
        MST[i]=old.MST[i];
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

        if(MST)
        {
            free(MST);
        }
        MST=(bool *)malloc(num_edges*sizeof(bool));
        for(i=0;i<num_edges;i++)
        {
            MST[i]=old.MST[i];
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
    free(MST);
}

//we capitalise differences from Dijkstra's algorithm
void Graph::PrimMST()
{
    //a RANDOM starting point will lead to one MST
    int start_node=rand()%num_nodes+1;

    //the i-th entry tells if we considered the i-th node
    int *visited_nodes=(int *)malloc(num_nodes*sizeof(int));
    //initialise this and MST
    int i;
    for(i=0;i<num_nodes;i++)
    {
        visited_nodes[i]=false;
    }
    visited_nodes[start_node-1]=true;
    for(i=0;i<num_edges;i++)
    {
        MST[i]=false;
    }
    visited_nodes[start_node-1]=true;
    int num_visited_nodes=1;

    //during an iteration
    //...crossing edge that minimises greedy score so far
    int edge_greedy;
    //...its endpoints
    int endpoint_out_greedy;
    //...quantity that minimises greedy score so far
    int tmp_greedy;

    //...analogues, that may beat above
    int edge;
    int endpoint_in,endpoint_out;
    int tmp;

    //...as long as we have to calculate MST
    while(num_visited_nodes<num_nodes)
    {
        edge_greedy=-INF_WEIGHT;
        tmp_greedy=-INF_WEIGHT;

        //...find crossing edges and calculate greedy score
        for(edge=1;edge<=num_edges;edge++)
        {
            int node1=incidence_list_first[edge-1];
            int node2=incidence_list_second[edge-1];

            if((visited_nodes[node1-1] && visited_nodes[node2-1])||(!visited_nodes[node1-1] && !visited_nodes[node2-1]))
            {
                continue;
            }

            if(visited_nodes[node1-1] && !visited_nodes[node2-1])
            {
                endpoint_in=node1;
                endpoint_out=node2;
            }
            
            if(visited_nodes[node2-1] && !visited_nodes[node1-1])
            {
                endpoint_in=node2;
                endpoint_out=node1;
            }

            tmp=incidence_list_weights[edge-1];

            if(tmp_greedy==-INF_WEIGHT || tmp<tmp_greedy)
            {
                edge_greedy=edge;
                endpoint_out_greedy=endpoint_out;
                tmp_greedy=tmp;
            }
        }

        MST[edge_greedy-1]=true;
        visited_nodes[endpoint_out_greedy-1]=true;

        num_visited_nodes++;
    }
    free(visited_nodes);
}

void Graph::PrimMSTHeap()
{
    //a RANDOM starting point will lead to one MST
    int start_node=rand()%num_nodes+1;

    //initialise MST
    int i;
    for(i=0;i<num_edges;i++)
    {
        MST[i]=false;
    }

    //objects are non-visited nodes, keys are mininum of the greedy scores of the adjacent visited nodes.
    Heap heap;
    heap.initialise(num_nodes);
    heap.insert(start_node,0);
    for(i=1;i<start_node;i++)
    {
        heap.insert(i,INF_WEIGHT);
    }
    for(i=start_node+1;i<=num_nodes;i++)
    {
        heap.insert(i,INF_WEIGHT);
    }

    //...as long as we have to calculate distances
    while(!heap.isEmpty())
    {
        //...extract object with mininum key
        int endpoint_in_greedy=heap.minObject();
        int weight=heap.minKey();
        int edge_greedy;
        if(endpoint_in_greedy!=start_node)
        {
            for(edge_greedy=1;edge_greedy<=num_edges;edge_greedy++)
            {
                if((incidence_list_first[edge_greedy-1]==endpoint_in_greedy || incidence_list_second[edge_greedy-1]==endpoint_in_greedy) && incidence_list_weights[edge_greedy-1]==weight)
                {
                    MST[edge_greedy-1]=true;
                    break;
                }
            }
        }
        heap.extractMin();

        //...find adjacent nodes and recalculate their greedy scores
        int edge;
        for(edge=1;edge<=degrees[endpoint_in_greedy-1];edge++)
        {
            int endpoint_out=adjacency_list[endpoint_in_greedy-1][edge-1];
            if(heap.doesItContain(endpoint_out))
            {
                int key=heap.key(endpoint_out);
                heap.deleteObject(endpoint_out);
                heap.insert(endpoint_out,min(key,adjacency_list_weights[endpoint_in_greedy-1][edge-1]));
            }
        }
    }
}

int main()
{
    Graph graph("edges.txt");

    srand(time(NULL));
    graph.PrimMST();
    int edge,res=0;
    for(edge=0;edge<=graph.num_edges;edge++)
    {
        if(graph.MST[edge])
        {
            res+=graph.incidence_list_weights[edge];
        }
    }

    printf("\nThe cost of a minimum spanning tree is: %i\n",res);

    graph.PrimMSTHeap();
    res=0;
    for(edge=0;edge<=graph.num_edges;edge++)
    {
        if(graph.MST[edge])
        {
            res+=graph.incidence_list_weights[edge];
        }
    }

    printf("\nThe cost of a minimum spanning tree is: %i\n\n",res);

    return EXIT_SUCCESS;
}
