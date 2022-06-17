#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <queue>

long long int max(long long int x, long long int y)
{
    if(x<y)
    {
        return y;
    }
    else
    {
        return x;
    }
}

int does_vector_include(std::vector<int> vec, int num)
{
    int i;
    for(i=0;i<vec.size();i++)
    {
        if(vec[i]==num)
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    //input data
    int num_nodes;
    std::vector<long long int> weights;

    //read file
    //nodes are labeled by i=1,2,...num_nodes
    //vectors start from 0
    FILE *file=fopen("mwis.txt","r");
    fscanf(file,"%i\n",&num_nodes);

    int i;
    long long int j;
    weights.push_back(0);       //0-th node doesn't exist
    for(i=0;i<num_nodes;i++)
    {
        fscanf(file,"%lli\n",&j);
        weights.push_back(j);
    }
    fclose(file);

    //0-th entry is total weight of MWIS of the empty graph
    //i-th entry is total weight of MWIS of the partial graph from node 1 to i
    std::vector<long long int> MWISweights;
    MWISweights.push_back(0);
    MWISweights.push_back(weights[1]);
    for(i=2;i<=num_nodes;i++)
    {
        MWISweights.push_back(max(MWISweights[i-1],MWISweights[i-2]+weights[i]));
    }

    //nodes in MWIS
    std::vector<int> MWISnodes;
    i=num_nodes;
    while(i>=1)
    {
        if(MWISweights[i]==MWISweights[i-1])
        {
            i-=1;
        }
        else
        {
            MWISnodes.push_back(i);
            i-=2;
        }
    }
    
    //print
    printf("\nDoes the maximum-weight indipendent set include the nodes 1,2,3,4,17,117,517 and 997? ");
    printf("%i",does_vector_include(MWISnodes,1));
    printf("%i",does_vector_include(MWISnodes,2));
    printf("%i",does_vector_include(MWISnodes,3));
    printf("%i",does_vector_include(MWISnodes,4));
    printf("%i",does_vector_include(MWISnodes,17));
    printf("%i",does_vector_include(MWISnodes,117));
    printf("%i",does_vector_include(MWISnodes,517));
    printf("%i\n\n",does_vector_include(MWISnodes,997));
    
    return EXIT_SUCCESS;
}