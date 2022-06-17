#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <vector>
#include <algorithm>

//data must be sorted
int twoSUM(std::vector<long long int> data)
{
    int i,j;
    std::vector<long long int> targets;
    for(i=0;i<data.size();i++)
    {
        for(j=i+1;j<data.size();j++)
        {
            if(data[i]+data[j]>=-TARGET && data[i]+data[j]<=TARGET && data[i]!=data[j])
            {
                targets.push_back(data[i]+data[j]);
            }
            if(data[i]+data[j]>TARGET)
            {
                break;
            }
        }
    }

    //remove duplicates
    std::sort(targets.begin(), targets.end(), std::less<long long int>());
    targets.erase(std::unique(targets.begin(),targets.end()), targets.end());

    return targets.size();
}

//data must be sorted
int twoSUMHash(std::vector<long long int> data)
{
    //how many targets pair of distinct integers?
    //...the i-th entry has numbers in range i*TARGET and (i+1)*TARGET-1
    Hash_table hash;
    hash.initialise(abs(data.back()-data.front())/TARGET+2);
    //...add items to table
    int i;
    long long int hash_code, j, x;
    for(i=0;i<data.size();i++)
    {
        hash.insert(data[i],data[i]);
    }
    //...for each object x, consider its hash-table element i and its neighbours i-1, i+1
    std::vector<long long int> targets;
    std::vector<long long int> sub_data;
    Node *node;
    for(i=0;i<data.size();i++)
    {
        x=data[i];
        hash_code=hash.hash_function(x);
        if(hash_code>0)
        {
            node=hash.buckets[hash_code-1];
            while(node!=NULL)
            {
                sub_data.push_back(node->object);
                node=node->child;
            }
        }

        node=hash.buckets[hash_code];
        while(node!=NULL)
        {
            sub_data.push_back(node->object);
            node=node->child;
        }
        
        if(hash_code<hash.capacity-1)
        {
            node=hash.buckets[hash_code+1];
            while(node!=NULL)
            {
                sub_data.push_back(node->object);
                node=node->child;
            }
        }
        //...offset by x and check if one element is in [-TARGET,TARGET]
        for(j=0;j<sub_data.size();j++)
        {
            sub_data[j]+=x;
            if(-TARGET<=sub_data[j] && sub_data[j]<=TARGET && sub_data[j]!=2*x)
            {
                targets.push_back(sub_data[j]);
            }
        }

        sub_data.clear();
    }

    //remove duplicates
    std::sort(targets.begin(), targets.end(), std::less<long long int>());
    targets.erase(std::unique(targets.begin(),targets.end()), targets.end());

    return targets.size();
}

int main()
{
    //load data from file
    FILE *file=fopen("algo1-programming_prob-2sum.txt","r");
    std::vector<long long int> data;
    long long int x;
    while(fscanf(file,"%lli\n",&x)==1)
    {
        data.push_back(x);
    }
    fclose(file);

    //sort and remove duplicates (NOTE: unique requires sort) 
    std::sort(data.begin(), data.end(), std::less<long long int>());
    data.erase(std::unique(data.begin(),data.end()), data.end());

    //calculate
    //printf("\nThe number of target pairs is %i\n",twoSUM(data));
    printf("\nThe number of target pairs is %i\n\n",twoSUMHash(data));

    return EXIT_SUCCESS;
}