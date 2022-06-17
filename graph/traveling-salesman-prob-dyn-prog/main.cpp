#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

#define ui unsigned int
#define INF_DISTANCE 1000000.

template <typename T>
T min(T x, T y)
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

float distance(float x1, float y1, float x2, float y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

class Graph{
    public:

        //arrays
        float **matrix_weights;             //the (i,j) entry is the weight of the edge connecting the nodes i,j

        //array dimensions
        int num_nodes;                      //number of nodes

        //functions
        Graph(const char *namefile);
        Graph(const Graph& old);
        Graph& operator=(const Graph& old);
        float TSP();                        //minimum cost of a traveling salesman tour
        ~Graph();
};

Graph::Graph(const char *namefile)
{
    FILE *file=fopen(namefile,"r");

    //read num_nodes
    fscanf(file,"%i\n",&num_nodes);

    float *colx = (float *)malloc(num_nodes*sizeof(float));
    float *coly = (float *)malloc(num_nodes*sizeof(float));

    for(int i=0;i<num_nodes;++i)
        fscanf(file,"%f %f\n",colx+i,coly+i);

    fclose(file);

    //calculate matrix_weights
    matrix_weights=(float **)malloc(num_nodes*sizeof(float *));

    for(int node1=1;node1<=num_nodes;++node1)
    {
        matrix_weights[node1-1]=(float *)malloc(num_nodes*sizeof(float));
        for(int node2=1;node2<=num_nodes;++node2)
            matrix_weights[node1-1][node2-1]=distance(colx[node1-1], coly[node1-1], colx[node2-1], coly[node2-1]);
    }

    free(colx);
    free(coly);
}

Graph::Graph(const Graph& old)
{
    num_nodes=old.num_nodes;

    matrix_weights=(float **)malloc(num_nodes*sizeof(float *));
    for(int i=0;i<num_nodes;i++)
    {
        matrix_weights[i]=(float *)malloc(num_nodes*sizeof(float));
        for(int j=0;j<num_nodes;j++)
            matrix_weights[i][j]=old.matrix_weights[i][j];
    }
}

Graph& Graph::operator=(const Graph& old)
{    
    if(this!=&old)
    { 
        num_nodes=old.num_nodes;

        matrix_weights=(float **)malloc(num_nodes*sizeof(int *));
        for(int i=0;i<num_nodes;i++)
        {
            matrix_weights[i]=(float *)malloc(num_nodes*sizeof(float));
            for(int j=0;j<num_nodes;j++)
                matrix_weights[i][j]=old.matrix_weights[i][j];
        }
    }

    return *this;
}

Graph::~Graph()
{
    int i;
    for(i=0;i<num_nodes;i++)
        free(matrix_weights[i]);
    free(matrix_weights);
}

float Graph::TSP()
{
    //solution
    //entry (indexS,j) = TSP array
    //indexS = 0,1,...2^(n-1)-1 and j=0,1,...n
    //indexS is a bitmask: bit no. 1 tells if S includes node 2, ... bit no. n-1 tells if S includes node n
    //j=0 tells destination is node 1, ... j=n-1 tells destination is node n
    ui size = pow(2,num_nodes-1);
    float** solution = (float **)malloc(size*sizeof(float *));
    
    for (ui indexS=0; indexS<size; ++indexS)
        solution[indexS] = (float *)malloc(num_nodes*sizeof(float));
    
    //initalisation, including impossible cases
    for (ui indexS=0; indexS<size; ++indexS)
        for(int node=1; node<=num_nodes; ++node)
            solution[indexS][node-1] = INF_DISTANCE;

    // base cases
    solution[0][0] = 0.;

    // recursion
    for(size_t m=2;m<=num_nodes;++m)
        for (ui indexS=0; indexS<size; ++indexS)
        {
            std::vector<int> nodesInS;
            nodesInS.push_back(1);
            for(int node=2; node<=num_nodes; ++node)
                if(indexS & (1<<(node-2)))
                    nodesInS.push_back(node);

            if(nodesInS.size()!=m)
                continue;

            for(const int& nodej : nodesInS)
            {
                if (nodej==1)
                    continue;
                
                std::vector<float> temp;
                for(const int& nodek : nodesInS)
                    if(nodek != nodej)
                        temp.push_back(solution[indexS & ~(1<<(nodej-2))][nodek-1] + matrix_weights[nodek-1][nodej-1]);
                solution[indexS][nodej-1] = *std::min_element(temp.begin(), temp.end());
            }
        }

    // minimum cost
    std::vector<float> temp;
    for(int nodej=2; nodej<=num_nodes;++nodej)
        temp.push_back(solution[size-1][nodej-1] + matrix_weights[nodej-1][0]);
    float res = *std::min_element(temp.begin(), temp.end());

    for (ui indexS=0; indexS<size; ++indexS)
        free(solution[indexS]);
    free(solution);

    return res;
}

int main()
{
    Graph graph("tsp.txt");

    printf("\nThe minimum cost of a traveling salesman tour is:");
    printf("\n%i\n", (int)graph.TSP());

    return EXIT_SUCCESS;
}