#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include "math.h"

// //primality test
// bool isPrime(long long int n)
// {
//     //0,1 and 2
//     if(n==0 || n==1 || n==2)
//     {
//         return true;
//     }

//     //try finding divisors
//     long long int m;
//     for (m=2; m<n/2; m++)
//     {
//         if (n%m==0)
//         {
//             return false;
//         }
//     }
//     return true;
// }

// //next prime >=n
// long long int getPrime(long long int n)
// {
//     //make it odd
//     if(n%2==0)
//     {
//         n++;
//     }

//     //hop from odd to odd
//     while(!isPrime(n))
//     {
//         n+=2;
//     }

//     return n;
// }

void Hash_table::initialise(long long int proposed_capacity)
{
    capacity=proposed_capacity;
    buckets=(Node **)malloc(capacity*sizeof(Node *));
    int hash_code;
    for(hash_code=0;hash_code<capacity;hash_code++)
    {
        buckets[hash_code]=NULL;
    }
}

long long int Hash_table::hash_function(long long int new_key)
{
    return (long long int) (abs(new_key)/TARGET);
}

void Hash_table::insert(long long int new_object, long long int new_key)
{
    //calculate index
    long long int hash_code=hash_function(new_key);

    //go to end of DDL and insert
    if(buckets[hash_code]==NULL)
    {
        buckets[hash_code]=(Node *)malloc(sizeof(Node));
        buckets[hash_code]->key=new_key;
        buckets[hash_code]->object=new_object;
        buckets[hash_code]->parent=NULL;
        buckets[hash_code]->child=NULL;
    }
    else
    {
        Node *last_node=buckets[hash_code];
        while(last_node->child!=NULL)
        {
            last_node=last_node->child;
        }
        Node *new_node=(Node *)malloc(sizeof(Node));
        new_node->key=new_key;
        new_node->object=new_object;
        new_node->parent=last_node;
        new_node->child=NULL;
        last_node->child=new_node;
    }
}

bool Hash_table::lookup(long long int new_key)
{
    //calculate index
    long long int hash_code=hash_function(new_key);

    //go through DDL and search
    Node *node=buckets[hash_code];
    while(node!=NULL)
    {
        if(node->key==new_key)
        {
            return true;
        }
        node=node->child;
    }
    return false;
}

Hash_table::~Hash_table()
{
    long long int hash_code;
    for(hash_code=0;hash_code<capacity;hash_code++)
    {
        //go to DDL end and delete DLL backwards
        Node *node=buckets[hash_code];
        if(node==NULL)
            continue;
        
        while((node->child)!=NULL)
            node=node->child;

        Node *next_node;
        while(node!=NULL)
        {
            next_node=node->parent;
            free(node);
            if(next_node==NULL)
            {
                break;
            } else  {
                next_node->child=NULL;
                node=next_node;
            }
        }
    }
}

void Hash_table::print()
{
    int hash_code;
    for(hash_code=0;hash_code<capacity;hash_code++)
    {
        Node *node=buckets[hash_code];
        if(node==NULL)
        {
            continue;
        }
        printf("hash code %i: ",hash_code);
        while(node!=NULL)
        {
            printf("%lli ",node->object);
            node=node->child;
        }
        printf("\n");
    }
}