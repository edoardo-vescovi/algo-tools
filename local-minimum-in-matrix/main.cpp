#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//nxm matrix - the first 3 variables are fixed by the first call
//looking into block
//  (i_ul,j_ul)    ...    (i_ul,j_br)
//  ...            ...    ...
//  (i_br,j_ul)    ...    (i_br,j_br)
void findMinimum(float *matrix, int n, int m, int i_ul, int j_ul, int i_br, int j_br, int *i_min, int *j_min)
{
    int i,j;
    bool done=false;
    
    if(i_br-i_ul<2 || j_br-j_ul<2)
    {
        //base case
        *i_min=i_ul;
        *j_min=j_ul;
        float val=matrix[i_ul*m+j_ul];
        
        for(i=i_ul;i<=i_br;i++)
        {
            for(j=j_ul;j<=j_br;j++)
            {
                if(matrix[i*m+j]<val)
                {
                    *i_min=i;
                    *j_min=j;
                    val=matrix[i*m+j];
                }
            }
        }
        done=true;
    }
    else
    {
        //recurrence
        //...pick value in middle of matrix
        int middle_i=(i_ul+i_br)/2;
        int middle_j=(j_ul+j_br)/2;
        float val=matrix[middle_i*m+middle_j];
        
        //...is that a local minimum?
        //...if yes, we're done
        //...if no, go ahead
        if(matrix[(middle_i-1)*m+middle_j]>val && matrix[(middle_i+1)*m+middle_j]>val && matrix[middle_i*m+middle_j-1]>val && matrix[middle_i*m+middle_j+1]>val)
        {
            *i_min=middle_i;
            *j_min=middle_j;
            done=true;
        }
        else
        {            
            //... run over middle row and column and find lowest value
            for(i=i_ul;i<=i_br;i++)
            {
                if(i!=middle_i && matrix[i*m+middle_j]<val)
                {
                    *i_min=i;
                    *j_min=middle_j;
                    val=matrix[i*m+middle_j];
                }
            }
            for(j=j_ul;j<=j_br;j++)
            {
                if(j!=middle_i && matrix[middle_i*m+middle_j]<val)
                {
                    *i_min=middle_i;
                    *j_min=j;
                    val=matrix[middle_i*m+j];
                }
            }
            //.. does the lowest value neighbour an even lower value?
            //...if yes, recurse inside the neighbouring sub-block
            //...if no, we're done
            if(!done && (*i_min)==middle_i && (*j_min)<middle_j)
            {
                //...so, sitting on left arm, let's look up
                if(matrix[(*i_min-1)*m+(*j_min)]<val)
                {
                    findMinimum(matrix, n, m, 0, 0, middle_i-1, middle_j-1, i_min, j_min);
                    done=true;
                }
                //..down
                if(matrix[(*i_min+1)*m+(*j_min)]<val)
                {
                    findMinimum(matrix, n, m, middle_i+1, 0, m-1, middle_j-1, i_min, j_min);
                    done=true;
                }
            }
            if(!done && (*i_min)==middle_i && (*j_min)>middle_j)
            {
                //...so, if sitting on right arm, let's look up
                if(matrix[(*i_min-1)*m+(*j_min)]<val)
                {
                    findMinimum(matrix, n, m, 0, middle_j+1, middle_i-1, m-1, i_min, j_min);
                    done=true;
                }
                //..down
                if(matrix[(*i_min+1)*m+(*j_min)]<val)
                {
                    findMinimum(matrix, n, m, middle_i+1, middle_j+1, n-1, m-1, i_min, j_min);
                    done=true;
                }
            }
            if(!done && (*j_min)==middle_j && (*i_min)<middle_i)
            {
                //...so, if sitting on top arm, let's look left
                if(matrix[(*i_min)*m+(*j_min-1)]<val)
                {
                    findMinimum(matrix, n, m, 0, 0, middle_i-1, middle_j-1, i_min, j_min);
                    done=true;
                }
                //..right
                if(matrix[(*i_min)*m+(*j_min+1)]<val)
                {
                    findMinimum(matrix, n, m, 0, middle_j+1, middle_i-1, m-1, i_min, j_min);
                    done=true;
                }
            }
            if(!done && (*j_min)==middle_j && (*i_min)>middle_i)
            {
                //...so, if sitting on bottom arm, let's look left
                if(matrix[(*i_min)*m+(*j_min-1)]<val)
                {
                    findMinimum(matrix, n, m, middle_i+1, 0, m-1, middle_j-1, i_min, j_min);
                    done=true;
                }
                //..right
                if(matrix[(*i_min)*m+(*j_min+1)]<val)
                {
                    findMinimum(matrix, n, m, middle_i+1, middle_j+1, n-1, m-1, i_min, j_min);
                    done=true;
                }
            }
        }    
    }
}

int main()
{
    //initialise
    srand(time(NULL));
    
    int size=rand()%20+1;
    float *grid=(float *)malloc(size*size*sizeof(float));
    
    int i,j;
    
    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            grid[i*size+j]=((float)rand())/((float)rand());
        }
    }
    
    //calculate
    int i_min,j_min;
    
    findMinimum(grid,size,size,0,0,size-1,size-1,&i_min,&j_min);
    
    //check
    bool check=true;
    float val=grid[i_min*size+j_min];
    bool corner=false;
    bool edgeNotCorner=false;
    
    //check on corners
    if(size>1)
    {
        if(i_min==0 && j_min==0)
        {
            check=check && val<grid[i_min*size+j_min+1];
            check=check && val<grid[(i_min+1)*size+j_min];
            corner=true;
        }
        if(i_min==0 && j_min==size-1)
        {
            check=check && val<grid[i_min*size+j_min-1];
            check=check && val<grid[(i_min+1)*size+j_min];
            corner=true;
        }
        if(i_min==size-1 && j_min==0)
        {
            check=check && val<grid[i_min*size+j_min+1];
            check=check && val<grid[(i_min-1)*size+j_min];
            corner=true;
        }
        if(i_min==size-1 && j_min==size-1)
        {
            check=check && val<grid[i_min*size+j_min-1];
            check=check && val<grid[(i_min-1)*size+j_min];
            corner=true;
        }
        
        //check on edges, corners ecluded
        if(!corner)
        {
            if(i_min==0)
            {
                check=check && val<grid[i_min*size+j_min-1];
                check=check && val<grid[i_min*size+j_min+1];
                check=check && val<grid[(i_min+1)*size+j_min];
                edgeNotCorner=true;
            }
            if(i_min==size-1)
            {
                check=check && val<grid[i_min*size+j_min-1];
                check=check && val<grid[i_min*size+j_min+1];
                check=check && val<grid[(i_min-1)*size+j_min]; 
                edgeNotCorner=true;
            }
            if(j_min==0)
            {
                check=check && val<grid[i_min*size+j_min+1];
                check=check && val<grid[(i_min+1)*size+j_min];
                check=check && val<grid[(i_min-1)*size+j_min];
                edgeNotCorner=true;
            }
            if(j_min==size-1)
            {
                check=check && val<grid[i_min*size+j_min-1];
                check=check && val<grid[(i_min+1)*size+j_min];
                check=check && val<grid[(i_min-1)*size+j_min];
                edgeNotCorner=true;
            }
        }
        
        //check in bulk
        if(!corner && !edgeNotCorner)
        {
            check=check && val<grid[i_min*size+j_min+1];
            check=check && val<grid[i_min*size+j_min-1];
            check=check && val<grid[(i_min+1)*size+j_min];
            check=check && val<grid[(i_min-1)*size+j_min];
        }
    }
    
    //print
    printf("\nSize: %i\n",size);
    printf("\nMatrix: \n\n");
    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            if((i==i_min && j==j_min)||(i==i_min-1 && j==j_min)||(i==i_min && j==j_min-1)||(i==i_min+1 && j==j_min)||(i==i_min && j==j_min+1))
            {
                printf("\033[1;31m");
            }
            printf("%f ",grid[i*size+j]);
            printf("\033[0m");
        }
        printf("\n");
    }
    
    if(check)
    {
        printf("\nSUCCESS!\n\n");
    }
    else
    {
        printf("\nFAILED!\n\n");
    }
    
    free(grid);
    
    return EXIT_SUCCESS;
}