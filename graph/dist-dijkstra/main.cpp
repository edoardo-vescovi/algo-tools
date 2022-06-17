#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

#define MAX_CHAR_PER_LINE 1000
#define INF_DISTANCE 1000000

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
                                        //and first dimension of incidence_list_first and incidence_list_second

        int ref_node;                   //reference node
        int *distances;                 //distances of nodes from that

        //functions
        Graph(const char *namefile);
        Graph(const Graph& old);
        Graph& operator=(const Graph& old);
        void dijkstra(int start_node);              //find distances between a node and all nodes
        void dijkstraHeap(int start_node);          //as above, using heap
        ~Graph();
};

Graph::Graph(const char *namefile)
{
    FILE *file=fopen(namefile,"r");

    int i,j,k,l;

    //read num_nodes
    num_nodes=0;
    char *c=(char *)malloc(MAX_CHAR_PER_LINE*sizeof(char));
    while(fgets(c, MAX_CHAR_PER_LINE, file)!=NULL)
    {
        num_nodes++;
    }
    rewind(file);
    
    //read degrees
    degrees=(int *)malloc(num_nodes*sizeof(int));
    i=0;
    while(fgets(c, 1000, file)!=NULL)
    {
        degrees[i]=0;
        j=0;
        while(c[j]!='\0')
        {
            if(c[j]=='\t')
            {
                degrees[i]+=1;
            }
            j++;
        }
        degrees[i]-=1;
        
        i++;
    }
    free(c);
    rewind(file);

    //read adjacency_list
    adjacency_list=(int **)malloc(num_nodes*sizeof(int *));
    adjacency_list_weights=(int **)malloc(num_nodes*sizeof(int *));
    for(i=0;i<num_nodes;i++)
    {
        adjacency_list[i]=(int *)malloc(degrees[i]*sizeof(int));
        adjacency_list_weights[i]=(int *)malloc(degrees[i]*sizeof(int));
        fscanf(file,"%i",&(adjacency_list[i][0]));
        for(j=0;j<degrees[i];j++)
        {
            fscanf(file,"%i,%i",&(adjacency_list[i][j]),&(adjacency_list_weights[i][j]));
        }
    }
    fclose(file);

    //calculate num_edges
    num_edges=0;
    for(i=0;i<num_nodes;i++)
    {
        num_edges+=degrees[i];
    }
    num_edges=num_edges/2;

    //calculate incidence_list
    incidence_list_first=(int *)malloc(num_edges*sizeof(int));
    incidence_list_second=(int *)malloc(num_edges*sizeof(int));
    incidence_list_weights=(int *)malloc(num_edges*sizeof(int));
    k=0;
    bool ignore;
    //...run over pairs of adjacent points
    for(i=0;i<num_nodes;i++)
    {
        for(j=0;j<degrees[i];j++)
        {
            //...has the incident edge been added yet?
            //...if yes: ignore
            //...if no: add it 
            ignore=false;
            for(l=0;l<k;l++)
            {
                if(incidence_list_first[l]==min(i+1,adjacency_list[i][j]) && incidence_list_second[l]==max(i+1,adjacency_list[i][j]))
                {
                    ignore=true;
                    break;
                }
            }
            if(!ignore)
            {
                incidence_list_first[k]=min(i+1,adjacency_list[i][j]);
                incidence_list_second[k]=max(i+1,adjacency_list[i][j]);
                incidence_list_weights[k]=adjacency_list_weights[i][j];
                k++;
            }
        }
    }

    //allocate memory for distances
    distances=(int *)malloc(num_nodes*sizeof(int));
}

Graph::Graph(const Graph& old)
{
    num_nodes=old.num_nodes;
    num_edges=old.num_edges;
    ref_node=old.ref_node;

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

    distances=(int *)malloc(num_nodes*sizeof(int));
    for(i=0;i<num_nodes;i++)
    {
        distances[i]=old.distances[i];
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
        ref_node=old.ref_node;

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

        if(distances)
        {
            free(distances);
        }
        distances=(int *)malloc(num_nodes*sizeof(int));
        for(i=0;i<num_nodes;i++)
        {
            distances[i]=old.distances[i];
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
    free(distances);
}

void Graph::dijkstra(int start_node)
{
    //the i-th entry tells if we have calculated the distance to the i-th node
    int *visited_nodes=(int *)malloc(num_nodes*sizeof(int));
    //initialise this and distances
    int i;
    for(i=0;i<num_nodes;i++)
    {
        visited_nodes[i]=false;
        distances[i]=INF_DISTANCE;
    }
    visited_nodes[start_node-1]=true;
    distances[start_node-1]=0;
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

    //...as long as we have to calculate distances
    while(num_visited_nodes<num_nodes)
    {
        edge_greedy=-1;
        tmp_greedy=-1;

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

            tmp=distances[endpoint_in-1]+incidence_list_weights[edge-1];

            if(tmp_greedy<0 || tmp<tmp_greedy)
            {
                edge_greedy=edge;
                endpoint_out_greedy=endpoint_out;
                tmp_greedy=tmp;
            }
        }

        if(tmp_greedy>=0)
        {
            distances[endpoint_out_greedy-1]=tmp_greedy;
            visited_nodes[endpoint_out_greedy-1]=true;
        }
        num_visited_nodes++;
    }
    free(visited_nodes);
}

void Graph::dijkstraHeap(int start_node)
{
    //objects are non-visited nodes, keys are mininum of the greedy scores of the adjacent visited nodes.
    Heap heap;
    heap.initialise(num_nodes);
    heap.insert(start_node,0);
    int i;
    for(i=1;i<start_node;i++)
    {
        heap.insert(i,INF_DISTANCE);
    }
    for(i=start_node+1;i<=num_nodes;i++)
    {
        heap.insert(i,INF_DISTANCE);
    }

    //...as long as we have to calculate distances
    while(!heap.isEmpty())
    {
        //...extract object with mininum key
        int endpoint_in_greedy=heap.minObject();
        distances[endpoint_in_greedy-1]=heap.minKey();
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
                heap.insert(endpoint_out,min(key,distances[endpoint_in_greedy-1]+adjacency_list_weights[endpoint_in_greedy-1][edge-1]));
            }
        }
    }
}

int main()
{
    Graph graph("dijkstraData.txt");

    graph.dijkstra(1);

    printf("\nThe shortest distances from node 1 to nodes 7,37,59,82,99,115,133,165,188,197 are:");
    printf("\n%i,%i,%i,%i,%i,%i,%i,%i,%i,%i\n",graph.distances[6],graph.distances[36],graph.distances[58],graph.distances[81],graph.distances[98],graph.distances[114],graph.distances[132],graph.distances[164],graph.distances[187],graph.distances[196]);

    graph.dijkstraHeap(1);

    printf("\nThe shortest distances from node 1 to nodes 7,37,59,82,99,115,133,165,188,197 are:");
    printf("\n%i,%i,%i,%i,%i,%i,%i,%i,%i,%i\n\n",graph.distances[6],graph.distances[36],graph.distances[58],graph.distances[81],graph.distances[98],graph.distances[114],graph.distances[132],graph.distances[164],graph.distances[187],graph.distances[196]);

    return EXIT_SUCCESS;
}