#include <stdlib.h>
#include "buffer.h"

void Queue::initialise(int Capacity)
{
    //set rear before front AND size=0 to signal queue is empty
    i_front=0;
    i_rear=Capacity-1;
    size=0;
    capacity=Capacity;
    array=(int *)malloc(Capacity*sizeof(int));
}

Queue& Queue::operator=(const Queue& queue)
{
    if(this!=&queue)
    {
        i_front=queue.i_front;
        i_rear=queue.i_rear;
        size=queue.size;
        capacity=queue.capacity;
        if(array)
        {
            free(array);
            array=(int *)malloc(capacity*sizeof(int));
            int i;
            for(i=0;i<capacity;i++)
            {
                array[i]=queue.array[i];
            }
        }
    }

    return *this;
}

Queue::~Queue()
{
    free(array);
}

bool Queue::enqueue(int new_element)
{
    if(isFull())
    {
        return false;
    }
    else
    {
        i_rear=(i_rear+1)%capacity;
        size++;
        array[i_rear]=new_element;
        return true;
    }
}

int Queue::dequeue()
{
    if(isEmpty())
    {
        return QUEUE_FAILURE;
    }
    else
    {
        int res=array[i_front];
        i_front=(i_front+1)%capacity;
        size--;
        return res;
    }
}

int Queue::front()
{
    if(isEmpty())
    {
        return QUEUE_FAILURE;
    }
    else
    {
        return array[i_front];
    }
}

int Queue::rear()
{
    if(isEmpty())
    {
        return QUEUE_FAILURE;
    }
    else
    {
        return array[i_rear];
    }
}

bool Queue::isEmpty()
{
    return (size==0);
}

bool Queue::isFull()
{
    return (size==capacity);
}

void Stack::initialise(int Capacity)
{
    size=0;
    capacity=Capacity;
    array=(int *)malloc(Capacity*sizeof(int));
}

Stack& Stack::operator=(const Stack& stack)
{
    if(this!=&stack)
    {
        size=stack.size;
        capacity=stack.capacity;
        if(array)
        {
            free(array);
            array=(int *)malloc(capacity*sizeof(int));
            int i;
            for(i=0;i<capacity;i++)
            {
                array[i]=stack.array[i];
            }
        }
    }

    return *this;
}

Stack::~Stack()
{
    free(array);
}

bool Stack::push(int new_element)
{
    if(isFull())
    {
        return false;
    }
    else
    {
        size++;
        array[size-1]=new_element;
        return true;
    }
}

int Stack::pop()
{
    if(isEmpty())
    {
        return STACK_FAILURE;
    }
    else
    {
        int res=array[size-1];
        size--;
        return res;
    }
}

int Stack::top()
{
    if(isEmpty())
    {
        return STACK_FAILURE;
    }
    else
    {
        return array[size-1];
    }
}

bool Stack::isEmpty()
{
    return (size==0);
}

bool Stack::isFull()
{
    return (size==capacity);
}