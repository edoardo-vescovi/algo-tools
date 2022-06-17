#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//point on the plane
class Point
{
    public:
        int label;  //unique label; must be initalised; references the point after copying/sorting in different arrays
        float x;    //x coordinate
        float y;    //y coordinate
};

//------------------------------- OLD MERGE SORT, EDITED TO SORT POINTS BY X,Y-COORDINATE AND RETURN ALSO ORDER -------------------------------

//split n-element array into two, with lengths
void split_array(Point *input_array, int length_input_array, Point *output_array1, Point *output_array2)
{
    int i;

    for (i = 0; i < length_input_array / 2; i++)
    {
        output_array1[i] = input_array[i];
        output_array2[i] = input_array[i + length_input_array / 2];
    }

    if (length_input_array % 2 == 1)
    {
        output_array2[length_input_array / 2] = input_array[length_input_array - 1];
    }
}

//merge two sorted lists into one,
//while sorting their elements
void merge_array(Point *input_array1, Point *input_array2, int length_input_array1, int length_input_array2, Point *output_array, char c)
{

    int i = 0, j = 0, k;

    for (k = 0; k < length_input_array1 + length_input_array2; k++)
    {
        //when not reached end of either array
        if (i < length_input_array1 && j < length_input_array2)
        {
            
            if(c=='x')
            {
                if (input_array1[i].x < input_array2[j].x)
                {
                    output_array[k] = input_array1[i];
                    i++;
                }
                else
                {
                    output_array[k] = input_array2[j];
                    j++;
                }   
            }
            
            if(c=='y')
            {
                if (input_array1[i].y < input_array2[j].y)
                {
                    output_array[k] = input_array1[i];
                    i++;
                }
                else
                {
                    output_array[k] = input_array2[j];
                    j++;
                }   
            }
            
        } else {
            //when reached end of first array
            if (i == length_input_array1)
            {
                output_array[k] = input_array2[j];
                j++;
            }
            else
            {
                //when reached end of second array
                if (j == length_input_array2)
                {
                    output_array[k] = input_array1[i];
                    i++;
                }
            }
        }
    }
}

//the algorithm proper
void merge_sort(Point *input_array, int length_input_array, Point *output_array, char c)
{
    switch (length_input_array)
    {
    case 1:
        // base case
        output_array[0] = input_array[0];
        break;

    default:
        // non-base case...
        Point left1[length_input_array / 2];
        Point right1[length_input_array / 2 + length_input_array % 2];

        Point left2[length_input_array / 2];
        Point right2[length_input_array / 2 + length_input_array % 2];

        //... we split into 2 arrays
        split_array(input_array, length_input_array, left1, right1);

        //... we sort separately
        merge_sort(left1, length_input_array / 2, left2,c);
        merge_sort(right1, length_input_array / 2 + length_input_array % 2, right2,c);

        //... we join while sorting
        merge_array(left2, right2, length_input_array / 2, length_input_array / 2 + length_input_array % 2, output_array,c);
        break;
    }
}

//------------------------------- NEW SUBROUTINES -------------------------------

float minf(float x, float y)
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

int mini(int x, int y)
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


int maxi(int x, int y)
{
    if(x>y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

//find a point with a given label in an array
//assuming it can be found
int findLabel(Point *array, int n, int lab)
{
    int i;
    for(i=0;i<n;i++)
    {
        if(((array[i]).label)==lab)
        {
            break;
        }
    }
    return i;
}

//distance between points
float d(Point p1, Point p2)
{
   return sqrt(powf((p1.x-p2.x),2) + powf((p1.y-p2.y),2));
}

void closestPairBruteForce(Point *points, int size, int *index1, int *index2)
{
    *index1=0;
    *index2=1;
    float min_distance=d(points[0],points[1]);
    
    int i,j;
    for(i=0;i<size;i++)
    {
        for(j=i+1;j<size;j++)
        {
            if(d(points[i],points[j])<min_distance)
            {
                *index1=i;
                *index2=j;
                min_distance=d(points[i],points[j]);
            }
        }
    }
}

//input points must be ordered by x-coordinates
void closestPairSmart(Point *points, int size, int *index1, int *index2, float *distance)
{
    switch(size)
    {   
        //base case
        case 2:
            *index1=0;
            *index2=1;
            (*distance)=d(points[0],points[1]);
            break;
        
        //base case
        case 3:
            *index1=0;
            *index2=1;
            (*distance)=d(points[0],points[1]);
            
            if(d(points[0],points[2])<(*distance))
            {
                *index1=0;
                *index2=2;
                (*distance)=d(points[0],points[2]);
            }
                          
            if(d(points[1],points[2])<(*distance))
            {
                *index1=1;
                *index2=2;
                (*distance)=d(points[1],points[2]);
            }
            break;
        
        //non-base case
        default:
            //divide
            int i1L, i2L;
            float minDistanceL;
            
            int i1R, i2R;
            float minDistanceR;
            
            //conquer: recursive calls for left pairs and right pairs
            closestPairSmart(points, size/2, &i1L, &i2L, &minDistanceL);
            closestPairSmart(points+size/2, size/2+size%2, &i1R, &i2R, &minDistanceR);
            i1R+=size/2;
            i2R+=size/2;
            
            //combine: left-right pairs
            int i1LR, i2LR;
            float minDistanceLR;
            
            if(minDistanceL<minDistanceR)   //pick pair in left half
            {
                minDistanceLR=minDistanceL;
                i1LR=i1L;
                i2LR=i2L;
            }
            else                            //pick pair in right half
            {
                minDistanceLR=minDistanceR;
                i1LR=i1R;
                i2LR=i2R;
            }
            
            //we have the pair labeled by i1LR and i2LR
            //does a split pair exist?
            
            //combine: find a possible split pair
            //populate the strip...
            Point *pointsInStrip=NULL;
            int sizeStrip=0;
            int i;
            for(i=size/2-1;i>=0;i--)
            {
                if((points[i].x)>=(points[size/2-1].x)-minDistanceLR)
                {
                    sizeStrip++;
                    pointsInStrip=(Point *)realloc(pointsInStrip,sizeStrip*sizeof(Point));
                    pointsInStrip[sizeStrip-1]=points[i];
                }
                else
                {
                    break;
                }
            }
            for(i=size/2;i<size;i++)
            {
                if((points[i].x)<=(points[size/2-1].x)+minDistanceLR)
                {
                    sizeStrip++;
                    pointsInStrip=(Point *)realloc(pointsInStrip,sizeStrip*sizeof(Point));
                    pointsInStrip[sizeStrip-1]=points[i];
                }
                else
                {
                    break;
                }
            }
            
            //...sort strip points by y-coordinate
            Point *pointsInStripSortedbyY=(Point *)malloc(sizeStrip*sizeof(Point));
            merge_sort(pointsInStrip, sizeStrip, pointsInStripSortedbyY,'y');
            
            //...try to find split pair
            float proposalMinDistance=minDistanceLR;
            int j;
            for(i=0;i<sizeStrip;i++)
            {
                for(j=i+1;j<minf(i+8,sizeStrip);j++)
                {
                    if(d(pointsInStripSortedbyY[i],pointsInStripSortedbyY[j])<proposalMinDistance)
                    {
                        proposalMinDistance=d(pointsInStripSortedbyY[i],pointsInStripSortedbyY[j]);
                        i1LR=findLabel(points,size,(pointsInStripSortedbyY[i]).label);
                        i2LR=findLabel(points,size,(pointsInStripSortedbyY[j]).label);
                    }
                }
            }
            
            *index1=i1LR;
            *index2=i2LR;
            (*distance)=proposalMinDistance;

            free(pointsInStrip);
            free(pointsInStripSortedbyY);

            break;
    }
}

//------------------------------- MAIN -------------------------------

int main(){
    
    //scatter at least 2 points on plane
    srand(time(NULL));
    
    int size=rand()%10000+2;
    Point *points = (Point *)malloc(size*sizeof(Point));

    int i;
    for(i=0;i<size;i++)
    {
        points[i].label=i;
        points[i].x=((float) rand())/((float) RAND_MAX);
        points[i].y=((float) rand())/((float) RAND_MAX);
    }
    
    //sort by x-coordinate
    Point *sortedPointsByX =(Point *)malloc(size * sizeof(Point));
    merge_sort(points, size, sortedPointsByX, 'x');

    //calculate
    int index1, index2;
    float min_distance;
    closestPairSmart(sortedPointsByX, size, &index1, &index2, &min_distance);
    
    //check
    int iindex1, iindex2;
    closestPairBruteForce(sortedPointsByX, size, &iindex1, &iindex2);
    
    //print
    printf("\nNumber of points: %i",size);
    printf("\nClosests points:  (%f,%f) and (%f,%f)",sortedPointsByX[index1].x,sortedPointsByX[index1].y,sortedPointsByX[index2].x,sortedPointsByX[index2].y);
    printf("\nDistance: %f\n", min_distance);
    
    if(mini(index1,index2)==mini(iindex1,iindex2) && maxi(index1,index2)==maxi(iindex1,iindex2))
    {
        printf("\nSUCCESS!\n\n");
    }
    else
    {
        printf("\nFAILED!\n\n");
    }

    free(points);
    free(sortedPointsByX);

    return EXIT_SUCCESS;
}