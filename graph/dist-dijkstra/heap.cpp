#include <stdlib.h>
#include "heap.h"

//swap array[i] and array[j]
void swap(int *array, int i, int j)
{
    int tmp=array[i];
    array[i]=array[j];
    array[j]=tmp;
}

int Heap::parent(int i_index)
{
    if(i_index==0)
    {
        return HEAP_FAILURE;
    }
    else
    {
        return (i_index-1)/2;
    }
}

int Heap::left_child(int i_index)
{
    if(2*i_index+1>size-1)
    {
        return HEAP_FAILURE;
    }
    else
    {
        return 2*i_index+1;
    }  
}

int Heap::right_child(int i_index)
{
    if(2*i_index+2>size-1)
    {
        return HEAP_FAILURE;
    }
    else
    {
        return 2*i_index+2;
    }  
}

void Heap::initialise(int Capacity)
{
    size=0;
    capacity=Capacity;
    objects=(int *)malloc(Capacity*sizeof(int));
    keys=(int *)malloc(Capacity*sizeof(int));
}

Heap& Heap::operator=(const Heap& heap)
{
    if(this!=&heap)
    {
        size=heap.size;
        capacity=heap.capacity;
        objects=(int *)malloc(capacity*sizeof(int));
        keys=(int *)malloc(capacity*sizeof(int));
    }
    return *this;
}

Heap::~Heap()
{
    free(objects);
    free(keys);
}

bool Heap::insert(int new_object, int new_key)
{
    if(isFull())
    {
        return false;
    }
    
    size++;
    objects[size-1]=new_object;
    keys[size-1]=new_key;
    
    //bubble up to preserve heap property
    if(size!=1)
    {
        int i_bubble_up=size-1;
        while(true)
        {
            if(parent(i_bubble_up)==HEAP_FAILURE)
            {
                break;
            }

            if(keys[parent(i_bubble_up)]>keys[i_bubble_up])
            {
                swap(keys,parent(i_bubble_up),i_bubble_up);
                swap(objects,parent(i_bubble_up),i_bubble_up);
                i_bubble_up=parent(i_bubble_up);
            }
            else
            {
                break;
            }
        }
    }

    return true;
}

int Heap::extractMin()
{
    if(isEmpty())
    {
        return HEAP_FAILURE;
    }

    if(size==1)
    {
        size--;
        return objects[0];
    }

    //bubble down to preserve heap property
    int res=objects[0];

    keys[0]=keys[size-1];
    objects[0]=objects[size-1];
    size--;

    int i_bubble_down=0;

    while(true)
    {
        if(left_child(i_bubble_down)==HEAP_FAILURE && right_child(i_bubble_down)==HEAP_FAILURE)
        {
            break;
        }

        if(right_child(i_bubble_down)==HEAP_FAILURE)
        {
            if(keys[left_child(i_bubble_down)]<keys[i_bubble_down])
            {
                swap(keys,left_child(i_bubble_down),i_bubble_down);
                swap(objects,left_child(i_bubble_down),i_bubble_down);
            }
            break;
        }

        if(keys[left_child(i_bubble_down)]<keys[i_bubble_down] || keys[right_child(i_bubble_down)]<keys[i_bubble_down])
        {
            if(keys[left_child(i_bubble_down)]<keys[right_child(i_bubble_down)])
            {
                swap(keys,left_child(i_bubble_down),i_bubble_down);
                swap(objects,left_child(i_bubble_down),i_bubble_down);
                i_bubble_down=left_child(i_bubble_down);
            }
            else
            {
                swap(keys,right_child(i_bubble_down),i_bubble_down);
                swap(objects,right_child(i_bubble_down),i_bubble_down);
                i_bubble_down=right_child(i_bubble_down);
            }
        }
        else
        {
            break;
        }
    }

    return res;
}

bool Heap::deleteObject(int new_object)
{
    //find object
    int i;
    for(i=0;i<size;i++)
    {
        if(objects[i]==new_object)
        {
            break;
        }
    }

    //not found
    if(i==size)
    {
        return false;
    }

    //change its key
    keys[i]=HEAP_FAILURE;

    //bubble up to preserve heap property and eliminate it
    int i_bubble_up=i;
    while(true)
    {
        if(parent(i_bubble_up)==HEAP_FAILURE)
        {
            break;
        }

        if(keys[parent(i_bubble_up)]>keys[i_bubble_up])
        {
            swap(keys,parent(i_bubble_up),i_bubble_up);
            swap(objects,parent(i_bubble_up),i_bubble_up);
            i_bubble_up=parent(i_bubble_up);
        }
        else
        {
            break;
        }
    }
    extractMin();
    return true;
}

int Heap::minObject()
{
    if(isEmpty())
    {
        return HEAP_FAILURE;
    }
    else
    {
        return objects[0];
    }
}

int Heap::minKey()
{
    if(isEmpty())
    {
        return HEAP_FAILURE;
    }
    else
    {
        return keys[0];
    }
}

bool Heap::doesItContain(int new_object)
{
    int i;
    for(i=0;i<size;i++)
    {
        if(objects[i]==new_object)
        {
            return true;
        }
    }
    return false;
}

int Heap::key(int new_object)
{
    int i;
    for(i=0;i<size;i++)
    {
        if(objects[i]==new_object)
        {
            return keys[i];
        }
    }
    return HEAP_FAILURE;
}

bool Heap::isEmpty()
{
    return (size==0);
}

bool Heap::isFull()
{
    return (size==capacity);
}