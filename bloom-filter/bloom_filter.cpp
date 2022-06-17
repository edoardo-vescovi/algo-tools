#include <stdlib.h>
#include <math.h>
#include "bloom_filter.h"

int charToInt(unsigned char c)
{
    return (int)c;
}

unsigned char intToChar(int i)
{
    return (unsigned char)i;
}

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

void Bloom_filter::initialise(int num_bytes, int proposed_num_hash_functions)
{
    bytes.clear();
    bytes.assign(num_bytes,intToChar(0));
    num_hash_functions=min(proposed_num_hash_functions,8*num_bytes);
}

std::vector<int> Bloom_filter::hash_functions(std::string new_object)
{
    //random permutations of distinct integers
    //Fisher-Yates shuffle algorithm
    std::vector<int> hash_codes;
    hash_codes.clear();
    int i=0,j,tmp;

    //sorted array
    unsigned int seed=0;
    while(new_object[i]!='\0')
    {
        seed+=charToInt(new_object[i]);
        i++;
    }
    srand(seed);

    for(i=0;i<8*bytes.size();i++)
    {
        hash_codes.push_back(i);
    }

    //run through array
    for(i=0;i<hash_codes.size();i++)
    {
        //...pick the same or a successive element
        j=i+rand()%(hash_codes.size()-i);
        //...swap two elements
        tmp=hash_codes[i];
        hash_codes[i]=hash_codes[j];
        hash_codes[j]=tmp;
    }
    //printf("(%i %i %i)",hash_codes[0],hash_codes[1],hash_codes[2]);

    hash_codes.erase(hash_codes.end()-hash_codes.size()+num_hash_functions,hash_codes.end());
    return hash_codes;
}

void Bloom_filter::insert(std::string new_object)
{
    //generate hash codes
    std::vector<int> hash_codes;
    hash_codes=hash_functions(new_object);
    
    int bit_index, bit;
    for(bit_index=0;bit_index<hash_codes.size();bit_index++)
    {
        //turn on the bits
        bit=hash_codes[bit_index];
        bytes[bit/8]=bytes[bit/8] | intToChar(0b1<<(7-bit%8));
    }
}

bool Bloom_filter::lookup(std::string new_object)
{
    bool res=true;

    //generate hash codes
    std::vector<int> hash_codes;
    hash_codes=hash_functions(new_object);

    int bit_index, bit;
    for(bit_index=0;bit_index<hash_codes.size();bit_index++)
    {
        //check all bits are on
        bit=hash_codes[bit_index];
        if(charToInt(bytes[bit/8]>>(7-bit%8))%2==0)
        {
            res=false;
            break;
        }
    }
    return res;
}

Bloom_filter::~Bloom_filter()
{
    bytes.clear();
    num_hash_functions=0;
}