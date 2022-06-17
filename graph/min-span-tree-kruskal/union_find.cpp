#include <stdlib.h>
#include "union_find.h"

void Union_find::initialise(int Capacity)
{
    capacity=Capacity;
    size=0;
    num_groups=0;
    nodes=(NodeUF *)malloc(capacity*sizeof(NodeUF));
}

Union_find::Union_find(const Union_find& uf)
{
    capacity=uf.capacity;
    size=uf.size;
    num_groups=uf.num_groups;
    for(int i=0;i<size;i++)
    {
        nodes[i].object=uf.nodes[i].object;
        nodes[i].index_parent=uf.nodes[i].index_parent;
        nodes[i].rank=uf.nodes[i].rank;
    }
}

Union_find& Union_find::operator=(const Union_find& uf)
{
    if(this!=&uf)
    {
        capacity=uf.capacity;
        size=uf.size;
        num_groups=uf.num_groups;
        if(nodes)
            free(nodes);
        nodes=(NodeUF *)malloc(size*sizeof(NodeUF));
        for(int i=0;i<size;i++)
        {
            nodes[i].object=uf.nodes[i].object;
            nodes[i].index_parent=uf.nodes[i].index_parent;
            nodes[i].rank=uf.nodes[i].rank;
        }
    }

    return *this;
}

Union_find::~Union_find()
{
    capacity=0;
    size=0;
    num_groups=0;
    free(nodes);
}

bool Union_find::push(int new_object)
{
    if(isFull())
    {
        return false;
    }
    else
    {
        nodes[size].object=new_object;
        nodes[size].index_parent=size;
        nodes[size].rank=0;
        num_groups++;
        size++;
        return true;
    }
}

int Union_find::find(int index)
{
    int index_tmp=index;
    while(index_tmp!=nodes[index_tmp].index_parent)
    {
        index_tmp=nodes[index_tmp].index_parent;
    }
    return index_tmp;
}

int *Union_find::find_traversed_nodes(int index, int *num_traversed_nodes)
{
    int *traversed_nodes=(int *)malloc(sizeof(int));
    traversed_nodes[0]=index;
    *num_traversed_nodes=1;

    int index_tmp=index;
    while(index_tmp!=nodes[index_tmp].index_parent)
    {
        index_tmp=nodes[index_tmp].index_parent;

        traversed_nodes=(int *)realloc(traversed_nodes,(*num_traversed_nodes+1)*sizeof(int));
        traversed_nodes[*num_traversed_nodes]=index_tmp;
        (*num_traversed_nodes)++;
    }
    

    return traversed_nodes;
}

void Union_find::Union(int index1, int index2)
{
    int parent1=find(index1);
    int parent2=find(index2);
    if(parent1!=parent2)
    {
        num_groups--;
    }

    //union by rank: identify shallower tree
    //path compression: traverse the shallower tree from called index to ancestor
    //                  and attach these nodes (with/out loose ends) to new ancestor
    
    int *traversed_nodes;
    int num_traversed_nodes;
    int i;

    if(nodes[parent1].rank>nodes[parent2].rank)
    {
        //...rewire 
        traversed_nodes=find_traversed_nodes(index2, &num_traversed_nodes);
        for(i=0;i<num_traversed_nodes;i++)
        {
            nodes[traversed_nodes[i]].index_parent=parent1;
        }
        return;
    }

    if(nodes[parent1].rank<nodes[parent2].rank)
    {
        //...rewire 
        traversed_nodes=find_traversed_nodes(index1, &num_traversed_nodes);
        for(i=0;i<num_traversed_nodes;i++)
        {
            nodes[traversed_nodes[i]].index_parent=parent2;
        }
        return;
    }

    if(nodes[parent1].rank==nodes[parent2].rank)
    {
        //...rewire
        traversed_nodes=find_traversed_nodes(index1, &num_traversed_nodes);
        for(i=0;i<num_traversed_nodes;i++)
        {
            nodes[traversed_nodes[i]].index_parent=parent2;
        }

        //...update rank of new ancestor
        nodes[parent2].rank++;
        return;
    }
}

bool Union_find::isEmpty()
{
    return size==0;
}

bool Union_find::isFull()
{
    return size==capacity;
}