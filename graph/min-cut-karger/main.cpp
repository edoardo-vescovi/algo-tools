#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_CHAR_PER_LINE 1000

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

//random permutations of integers from 1 to n
//Fisher-Yates shuffle algorithm
//returns pointer to such array
void rand_perm(int *res, int n)
{
    int i,j,tmp;

    //sorted array
    for(i=0;i<n;i++)
    {
        res[i]=i+1;
    }

    //run through array
    for(i=0;i<n;i++)
    {
        //...pick the same or a successive element
        j=i+rand()%(n-i);
        //...swap two elements
        tmp=res[i];
        res[i]=res[j];
        res[j]=tmp;
    }
}

class Graph{
    public:

        //arrays
        int *node_groups;           //the i-th entry is the group label including the i-th node
                                    //edge contractions populate groups

        int **adjacency_list;       //the i-th entry is the array of nodes adjacents to i-th node:
                                    //(1st adjacent node, ... n_j-th adjacent node)
                                    //NOTE: created and deleted in constructor
        
        int *incidence_list_first;  //the i-th entry in these are node pair linked by the i-th edge:
        int *incidence_list_second; //(1st node, 2nd node)
                                    //created with assumption 1st node < 2nd node

        int *rand_edges;            //a random permutation of edges

        //array dimensions
        int num_node_groups;        //number of node groups
                                    //and of distinct elements in node_groups

        int num_nodes;              //number of nodes
                                    //and first dimension of node_groups, adjacency_list and degrees

        int *degrees;               //the i-th entry is n_i
                                    //and ~ second dimension of adjacency_list

        int num_edges;              //number of edges
                                    //and first dimension of incidence_list_first, incidence_list_second and rand_edges

        //functions
        Graph();
        Graph(const char *namefile);
        Graph(const Graph& old);
        Graph& operator=(const Graph& old);
        void karger(int iterations, int *num_crossing_edges);
        ~Graph();
};

Graph::Graph()
{
    node_groups=NULL;
    adjacency_list=NULL;
    incidence_list_first=NULL;
    incidence_list_second=NULL;
    rand_edges=NULL;
    degrees=NULL;
}

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
    for(i=0;i<num_nodes;i++)
    {
        adjacency_list[i]=(int *)malloc(degrees[i]*sizeof(int));
        fscanf(file,"%i",&(adjacency_list[i][0]));
        for(j=0;j<degrees[i];j++)
        {
            fscanf(file,"%i",&(adjacency_list[i][j]));
        }
    }
    fclose(file);

    //calculate node_groups
    node_groups=(int *)malloc(num_nodes*sizeof(int));
    for(i=0;i<num_nodes;i++)
    {
        node_groups[i]=i+1;
    }

    //calculate num_node_groups
    num_node_groups=num_nodes;

    //calculate num_edges
    num_edges=0;
    for(i=0;i<num_nodes;i++)
    {
        num_edges+=degrees[i];
    }
    num_edges=num_edges/2;

    //calculate incidence_list
    //and clear adjacency_list
    incidence_list_first=(int *)malloc(num_edges*sizeof(int));
    incidence_list_second=(int *)malloc(num_edges*sizeof(int));
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
                k++;
            }
        }
        free(adjacency_list[i]);
        adjacency_list[i]=NULL;
    }
    free(adjacency_list);
    adjacency_list=NULL;

    //calculate rand_edges
    rand_edges=(int *)malloc(num_edges*sizeof(int));
    rand_perm(rand_edges,num_edges);
}

Graph::Graph(const Graph& old)
{
    num_node_groups=old.num_node_groups;
    num_nodes=old.num_nodes;
    num_edges=old.num_edges;

    int i;
    
    node_groups=(int *)malloc(num_nodes*sizeof(int));
    for(i=0;i<num_nodes;i++)
    {
        node_groups[i]=old.node_groups[i];
    }

    adjacency_list=NULL;

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

    rand_edges=(int *)malloc(num_edges*sizeof(int));
    rand_perm(rand_edges,num_edges);
}

Graph& Graph::operator=(const Graph& old)
{    
    if(this!=&old)
    { 
        num_node_groups=old.num_node_groups;
        num_nodes=old.num_nodes;
        num_edges=old.num_edges;

        int i;

        if(node_groups)
        {
            free(node_groups);
        }
        node_groups=(int *)malloc(num_nodes*sizeof(int));
        for(i=0;i<num_nodes;i++)
        {
            node_groups[i]=old.node_groups[i];
        }
        
        adjacency_list=NULL;

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

        if(degrees)
        {
            free(rand_edges);
        }
        rand_edges=(int *)malloc(num_edges*sizeof(int));
        for(i=0;i<num_edges;i++)
        {
            rand_edges[i]=old.rand_edges[i];
        }
        
        rand_perm(rand_edges,num_edges);
    }

    return *this;
}

void Graph::karger(int iterations, int *num_crossing_edges)
{
    //...repeat algorithm many times and save results
    Graph graph_copy;
    int it, i, step, edge, endpoint1, endpoint2, node_group1, node_group2, min_node_group, max_node_group;

    for(it=0;it<iterations;it++)
    {
        //...make local copy
        graph_copy=*this;

        //...run over randomised list of edges
        step=0;
        
        //...as long as there are 2 node groups
        while (graph_copy.num_node_groups>2)
        {
            //...pick random edge
            edge=(graph_copy).rand_edges[step];
            //...read endpoints
            endpoint1= graph_copy.incidence_list_first[edge-1];
            endpoint2 = graph_copy.incidence_list_second[edge - 1];

            node_group1 = graph_copy.node_groups[endpoint1 - 1];
            node_group2 = graph_copy.node_groups[endpoint2 - 1];

            //...is the edge incident on endpoints belonging in the same group?
            //...if yes: discard this self-loop and continue
            //...if no: go ahead
            if(node_group1!=node_group2)
            {
                //...read the node group labels
                min_node_group=min(node_group1,node_group2);
                max_node_group=max(node_group1,node_group2);
                //...run through group labels and identify two groups
                for (i = 0; i < graph_copy.num_nodes; i++)
                {
                    if (graph_copy.node_groups[i] == max_node_group)
                    {
                        graph_copy.node_groups[i] = min_node_group;
                    }
                }
                //...decrease number of distinct groups
                (graph_copy.num_node_groups)--;
            }

            step++;
        }

        //...there are 2 nodes and num_edges edges,
        //...edges are both crossing and self-loops
        //...count the former
        num_crossing_edges[it]=0;
        for (edge = 1; edge <= (graph_copy.num_edges); edge++)
        {
            endpoint1 = graph_copy.incidence_list_first[edge - 1];
            endpoint2 = graph_copy.incidence_list_second[edge - 1];

            node_group1 = graph_copy.node_groups[endpoint1 - 1];
            node_group2 = graph_copy.node_groups[endpoint2 - 1];
            if(node_group1!=node_group2)
            {
                (num_crossing_edges[it])++;
            }
        }
    }
}

Graph::~Graph()
{
    free(node_groups);
    free(incidence_list_first);
    free(incidence_list_second);
    free(degrees);
    free(rand_edges);
}

int main()
{
    //initialise
    srand(time(NULL));
    
    //load graph
    Graph graph("kargerMinCut.txt");

    //calculate number of crossing edges of cuts
    int iterations=(int) (((float)graph.num_nodes) * ((float)graph.num_nodes) * log(((float)graph.num_nodes)));
    int *num_crossing_edges=(int *)malloc(iterations*sizeof(int));

    graph.karger(iterations, num_crossing_edges);
    
    //calculate mininum number
    int it;
    int min=num_crossing_edges[0];
    for(it=1;it<iterations;it++)
    {
        if(num_crossing_edges[it]<min)
        {
            min=num_crossing_edges[it];
        }
    }

    //print
    printf("\nThe mininum number of crossing edges is %i in %i iterations of Karger's algorithm.\n\n",min,iterations);
    //clean up
    free(num_crossing_edges);

    return EXIT_SUCCESS;
}