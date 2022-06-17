#include <stdio.h>
#include <stdlib.h>

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
        int **adjacency_list;           //the i-th entry is the array of head-nodes coming from the i-th tail-node:
                                        //(1st head-node, ... n_j-th head-node)
        int **adjacency_list_weights;   //as above, for weights

        int *incidence_list_tail;       //the i-th entry is the tail-node of the i-th edge
        int *incidence_list_head;       //the i-th entry is the head-node of the i-th edge

        int *incidence_list_weights;    //the i-th entry is the weight of the i-th edge

        //array dimensions
        int num_nodes;                  //number of nodes
                                        //and first dimension of node_groups, adjacency_list and degrees

        int *degrees;                   //the i-th entry is n_i
                                        //and ~ second dimension of adjacency_list

        int num_edges;                  //number of edges
                                        //and first dimension of incidence_list_tail and incidence_list_head

        //functions
        Graph(const char *namefile);
        Graph(const Graph& old);
        Graph& operator=(const Graph& old);
        int floydwarshall();            //find minimum of distances between all pairs of nodes
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
    incidence_list_tail=(int *)malloc(num_edges*sizeof(int));
    incidence_list_head=(int *)malloc(num_edges*sizeof(int));
    incidence_list_weights=(int *)malloc(num_edges*sizeof(int));

    adjacency_list=(int **)malloc(num_nodes*sizeof(int *));
    adjacency_list_weights=(int **)malloc(num_nodes*sizeof(int *));
    for(i=0;i<num_nodes;i++)
    {
        adjacency_list[i]=NULL;
        adjacency_list_weights[i]=NULL;
    }

    degrees=(int *)calloc(num_nodes,sizeof(int));

    int tail, head, deg, weight;
    for(int i=0;i<num_edges;i++)
    {
        fscanf(file,"%i %i %i\n",&tail,&head,&weight);
        incidence_list_tail[i]=tail;
        incidence_list_head[i]=head;
        incidence_list_weights[i]=weight;

        deg=++degrees[tail-1];

        adjacency_list[tail-1]=(int *)realloc(adjacency_list[tail-1],deg*sizeof(int));
        adjacency_list[tail-1][deg-1]=head;

        adjacency_list_weights[tail-1]=(int *)realloc(adjacency_list_weights[tail-1],deg*sizeof(int));
        adjacency_list_weights[tail-1][deg-1]=weight;
    }
    fclose(file);
}

Graph::Graph(const Graph& old)
{
    num_nodes=old.num_nodes;
    num_edges=old.num_edges;

    int i,j;

    incidence_list_tail=(int *)malloc(num_edges*sizeof(int));
    incidence_list_head=(int *)malloc(num_edges*sizeof(int));
    for(i=0;i<num_edges;i++)
    {
        incidence_list_tail[i]=old.incidence_list_tail[i];
        incidence_list_head[i]=old.incidence_list_head[i];
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

        if(incidence_list_tail)
        {
            free(incidence_list_tail);
        }
        incidence_list_tail=(int *)malloc(num_edges*sizeof(int));
        for(i=0;i<num_edges;i++)
        {
            incidence_list_tail[i]=old.incidence_list_tail[i];
        }

        if(incidence_list_head)
        {
            free(incidence_list_head);
        }
        incidence_list_head=(int *)malloc(num_edges*sizeof(int));
        for(i=0;i<num_edges;i++)
        {
            incidence_list_head[i]=old.incidence_list_head[i];
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

    free(incidence_list_tail);
    free(incidence_list_head);
    free(incidence_list_weights);
    
    free(degrees); 
}

int Graph::floydwarshall()
{
    //solution
    //entry (i,j,k) = Floyd-Warshall array
    //i,j,k=1,2,...num_nodes
    int ***solution = (int ***)malloc(num_nodes*sizeof(int **));
    for (int i=0; i<num_nodes; ++i)
    {
        solution[i] = (int **)malloc(num_nodes*sizeof(int *));
        for (int j=0; j<num_nodes; ++j)
            solution[i][j] = (int *)malloc(num_nodes*sizeof(int));
    }
    
    // base cases
    for (int nodei=1; nodei<=num_nodes; ++nodei)
    {
        for (int nodej=1; nodej<=num_nodes; ++nodej)
            solution[nodei-1][nodej-1][0] = (nodei==nodej ? 0 : INF_DISTANCE);
        for(int k=0; k<degrees[nodei-1]; ++k)
            solution[nodei-1][adjacency_list[nodei-1][k]-1][0] = adjacency_list_weights[nodei-1][k];
    }

    // recursion
    for (int nodek=2; nodek<=num_nodes; ++nodek)
        for (int nodei=1; nodei<=num_nodes; ++nodei)
            for (int nodej=1; nodej<=num_nodes; ++nodej)
                solution[nodei-1][nodej-1][nodek-1] = min(solution[nodei-1][nodej-1][nodek-2], solution[nodei-1][nodek-1][nodek-2]+solution[nodek-1][nodej-1][nodek-2]);


    // minimum distance
    int res = solution[0][1][num_nodes-1];
    for(int nodei = 1; nodei <= num_nodes; ++nodei)
    {
        for(int nodej = 1; nodej<=num_nodes; ++nodej)
            if (nodei!=nodej)
                res = min(res, solution[nodei-1][nodej-1][num_nodes-1]);
    }

    // check negative cycles
    for (int nodei=1; nodei<=num_nodes; ++nodei)
        if (solution[nodei-1][nodei-1][num_nodes-1] < 0)
        {
            res = -INF_DISTANCE;
            break;
        }

    for (int i=0; i<num_nodes; ++i)
    {
        for (int j=0; j<num_nodes; ++j)
            free(solution[i][j]);
        free(solution[i]);
    }
    free(solution);

    return res;
}

int main()
{
    const char* filenames[] = {"g1.txt"};
    for(int i=0; i<1; ++i)
    {
        Graph graph(filenames[i]);

        int x = graph.floydwarshall();
        if(x == -INF_DISTANCE)
            printf("\nThe graph %s contains a negative cycle\n", filenames[i]);
        else
        {
            printf("\nThe shortest distance among all-pairs shortest paths in the graph %s is:", filenames[i]);
            printf("\n%i\n", x);
        }
    }

    return EXIT_SUCCESS;
}