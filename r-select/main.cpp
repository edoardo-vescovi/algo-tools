#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//------------------------------------ quicksort starts ------------------------------------
//first two variables are fixed by first call
void quickSort(float *array, int i_start, int i_end)
{
    //base case: do nothing

    //non-base case
    if(i_start != i_end)
    {
        //divide
        //...choose pivot and push it to first position
        float tmp;
        int i_pivot=i_start;
        
        //...partition right sub-array array[i_pivot]... array[i_end]
        //...at beginning of each step we have
        //...i_pivot | ...(those<pivot)... | i ...(those>pivot)... | j ...(unvisited yet)... i_end
        int i=i_start+1;
        int j;
        for(j=i_start+1; j<=i_end; j++)
        {
            if(array[j]<array[i_start])
            {
                tmp=array[j];
                array[j]=array[i];
                array[i]=tmp;
                i++;
            }
        }
        //...push pivot to right position
        tmp=array[i-1];
        array[i-1]=array[i_start];
        array[i_start]=tmp;

        //conquer
        if(i_start<=i-2)
        {
            quickSort(array, i_start, i-2);
        }
        if(i<=i_end)
        {
            quickSort(array, i, i_end);
        }

        //combine: do nothing
    }
}

//------------------------------------ quicksort ends ------------------------------------

//first two entries are fixed by first call
void RSelect(float *array, int i_start, int i_end, int order_statistic)
{
    //base case: do nothing

    //non-base case
    if(i_start != i_end)
    {
        //divide
        //...choose pivot and push it to first position
        
        int i_pivot=i_start+(rand()%(i_end-i_start+1));

        float tmp=array[i_start];
        array[i_start]=array[i_pivot];
        array[i_pivot]=tmp;

        //...partition right sub-array array[i_pivot]... array[i_end]
        //...at beginning of each step we have
        //...i_pivot | ...(those<pivot)... | i ...(those>pivot)... | j ...(unvisited yet)... i_end
        int i=i_start+1;
        int j;
        for(j=i_start+1; j<=i_end; j++)
        {
            if(array[j]<array[i_start])
            {
                tmp=array[j];
                array[j]=array[i];
                array[i]=tmp;
                i++;
            }
        }
        //...push pivot to right position
        tmp=array[i-1];
        array[i-1]=array[i_start];
        array[i_start]=tmp;

        //conquer
        //...pivot is at position i-1
        //...i-th order statistic will be at position order_statistic-1
        if(order_statistic-1==i-1)
        {
            return;
        }

        if(order_statistic-1<i-1)
        {
            RSelect(array, i_start, i-2, order_statistic);
            return;
        }
        
        if(order_statistic-1>i-1)
        {
            RSelect(array, i, i_end, order_statistic);
            return;
        }

        //combine: do nothing
    }
}

int main()
{
    //initialise
    srand(time(NULL));
    int length=rand()%10+1;

    //original, to keep
    float *array=(float *)malloc(length*sizeof(float));
    //edited by RSelect
    float *array2=(float *)malloc(length*sizeof(float));
    //edited by QuickSort
    float *array3=(float *)malloc(length*sizeof(float));

    int i;
    for(i=0;i<length;i++)
    {
        array[i]=((float) rand())/((float) RAND_MAX);
        array2[i]=array[i];
        array3[i]=array[i];
    }

    int order_statistic=1+rand()%length;

    //calculate
    RSelect(array2, 0, length-1, order_statistic);

    //sort and check
    quickSort(array3, 0, length-1);

    //printf
    printf("\n%i-th order statistic\n", order_statistic);

    printf("\nArray:\n");
    for(i=0;i<length;i++)
    {
        if(array[i]==array3[order_statistic-1])
        {
            printf("\033[1;31m");
        }
        printf("%f ",array[i]);
        printf("\033[0m");
    }
    
    printf("\n\nSorted array:\n");
    for(i=0;i<length;i++)
    {
        if(i==order_statistic-1)
        {
            printf("\033[1;31m");
        }
        printf("%f ",array3[i]);
        printf("\033[0m");
    }

    if(array2[order_statistic-1]-array3[order_statistic-1]<0.001)
    {
        printf("\n\nSUCCESS!\n\n");
    }
    else
    {
        printf("\n\nFAILED!\n\n");
    }

    free(array);
    free(array2);
    free(array3);

    return EXIT_SUCCESS;
}