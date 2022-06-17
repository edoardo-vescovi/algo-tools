#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// http://users.csc.calpoly.edu/~dekhtyar/349-Spring2010/lectures/lec03.349.pdf

int count_comparisons=0;

void findLargestTwoBruteForce(float *array, int size, int *i_max, int *i_next_max)
{
    (*i_max)=0;
    
    int i;
    for(i=1;i<size;i++)
    {
        if(array[i]>array[*i_max])
        {
            (*i_max)=i;
        }
    }
    
    (*i_next_max) = ((*i_max+1)%size);
    
    for(i=0;i<size;i++)
    {
        if(i!=(*i_max) && array[i]>array[*i_next_max])
        {
            (*i_next_max)=i;
        }
    }
}

//first two arguments are fixed by first call
void findLargestTwoSmart(float *array, int size, int i_start, int i_end, int *i_largest, int *i_candidatesSecondLargest, int *sizeCandidates)
{   
    //base case
    if(i_start+1==i_end)
    {
        count_comparisons++;
        if(array[i_start]>array[i_end])
        {
            (*i_largest)=i_start;
            i_candidatesSecondLargest[0]=i_end;
            (*sizeCandidates)=1;
        }
        else
        {
            (*i_largest)=i_end;
            i_candidatesSecondLargest[0]=i_start;
            (*sizeCandidates)=1;
        }
    }
    else
    {
        //divide
        int i_largest1;
        int i_largest2;
        
        int *i_candidatesSecondLargest1=(int *)malloc(size*sizeof(int));
        int *i_candidatesSecondLargest2=(int *)malloc(size*sizeof(int));
        
        int sizeCandidates1=0;
        int sizeCandidates2=0;
    
        //conquer
        findLargestTwoSmart(array, size, i_start, (i_start+i_end)/2, &i_largest1, i_candidatesSecondLargest1, &sizeCandidates1);
        findLargestTwoSmart(array, size, (i_start+i_end)/2+1, i_end, &i_largest2, i_candidatesSecondLargest2, &sizeCandidates2);
                
        //combine
        count_comparisons++;
        if(array[i_largest1] < array[i_largest2])
        {
            
            free(i_candidatesSecondLargest1);
            sizeCandidates1=0;
            
            *i_largest = i_largest2;
            *sizeCandidates=sizeCandidates2+1;
            
            int i;
            for(i=0;i<sizeCandidates2;i++)
            {
                i_candidatesSecondLargest[i]=i_candidatesSecondLargest2[i];
            }
            i_candidatesSecondLargest[sizeCandidates2]=i_largest1;
        }
        else
        {
            free(i_candidatesSecondLargest2);
            sizeCandidates2=0;
            
            *i_largest = i_largest1;
            *sizeCandidates=sizeCandidates1+1;
             
            int i;
            for(i=0;i<sizeCandidates1;i++)
            {
                i_candidatesSecondLargest[i]=i_candidatesSecondLargest1[i];
            }
            i_candidatesSecondLargest[sizeCandidates1]=i_largest2;
        }
    }
}

int main()
{   
    //initialise
    srand(time(NULL));
    int size = (int)pow(2,rand()%8+1);
    float *array = (float *)malloc(size*sizeof(float));
    
    int i;
    for(i=0;i<size;i++)
    {
        array[i]=((float) rand())/((float) RAND_MAX);
    }
    
    //calculate largest and list of those beaten by it
    int i_largest;
    int *i_candidatesSecondLargest=(int *)malloc(size*sizeof(int));
    int sizeCandidates=0;
    findLargestTwoSmart(array, size, 0, size-1, &i_largest, i_candidatesSecondLargest, &sizeCandidates);
    //find largest among those beaten
    int i_secondToLargest=i_candidatesSecondLargest[0];

    for(i=1;i<sizeCandidates;i++)
    {
        count_comparisons++;
        if(array[i_candidatesSecondLargest[i]]>array[i_secondToLargest])
        {
            i_secondToLargest=i_candidatesSecondLargest[i];
        }
    }
    
    //check
    int i_largest_c, i_secondLargest_c;
    findLargestTwoBruteForce(array, size, &i_largest_c, &i_secondLargest_c);
    
    //print
    printf("\n");
    for(i=0;i<size;i++)
    {
        if(i==i_largest_c)
        {
            printf("\033[1;31m");
        }
        if(i==i_secondLargest_c)
        {
            printf("\033[0;35m");
        }
        printf("%f ",array[i]);
        printf("\033[0m");
    }

    if (i_largest == i_largest_c && i_secondToLargest == i_secondLargest_c && count_comparisons == size + (int)log2(size) - 2)
    {
        printf("\n\nSUCCESS!\n\n");
    }
    else
    {
        printf("\n\nFAILED!\n\n");
    }
    
    return EXIT_SUCCESS;
}