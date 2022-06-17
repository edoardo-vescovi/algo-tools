#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_INTEGER 7

//max integer
int max(int a, int b, int c, int d)
{
    int res=a;
    
    if(b>res)
    {
        res=b;
    }
    
    if(c>res)
    {
        res=c;
    }
    
    if(d>res)
    {
        res=d;
    }
    
    return res;
}

//lowest power of 2, higher than n
int nextPowerOfTwo(int n)
{
    return max(2,pow(2, (int) ceil(log2(n))),0,0);
}

//random integer in range [1,MAX_INTEGER]
int randomInt()
{
    return (rand() % MAX_INTEGER)+1;
}

class Matrix{
    public:
        int size1;
        int size2;
        int *elements;
    
        Matrix(int n, int m);
        Matrix(int n, int m, int value);
        ~Matrix() {size1=0;size2=0;free(elements);};

        Matrix(const Matrix &mat);
        Matrix& operator=(const Matrix &matrix);

        int returnElement(int i, int j) const;
        void printElements(int n, int m);
    
        void enterElement(const int i, const int j, int value);
};

//initialise with random integers
Matrix::Matrix(int n, int m)
{
    size1=n;
    size2=m;
    elements = (int *)malloc(size1*size2*sizeof(int));
    int i;
    for(i=0;i<size1*size2;i++)
    {
        elements[i]=randomInt();
    }
}

//initialise with given value
Matrix::Matrix(int n, int m, int value)
{
    size1=n;
    size2=m;
    elements = (int *)malloc(size1*size2*sizeof(int));
    int i;
    for(i=0;i<size1*size2;i++)
    {
        elements[i]=value;
    }
}

Matrix::Matrix(const Matrix &matrix)
{
    size1=matrix.size1;
    size2=matrix.size2;
    elements = (int *)malloc(size1*size2*sizeof(int));
    int i,j;
    for(i=1;i<=size1;i++)
    {
        for(j=1;j<=size2;j++)
        {
            enterElement(i,j,matrix.returnElement(i,j));
        }
    }
}

Matrix& Matrix::operator=(const Matrix &matrix)
{
    if(this!=&matrix)
    {
        size1=matrix.size1;
        size2=matrix.size2;
        elements = (int *)malloc(size1*size2*sizeof(int));
        int i,j;
        for(i=1;i<=size1;i++)
        {
            for(j=1;j<=size2;j++)
            {
                enterElement(i,j,matrix.returnElement(i,j));
            }
        }
    }
    return *this;
}

//read element
int Matrix::returnElement(int i, int j) const
{
    return elements[(i-1)*size2+j-1];
}

//print in terminal
void Matrix::printElements(int n, int m)
{
    int i,j;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=m;j++)
        {
            printf("%i ",returnElement(i,j));
        }
        printf("\n");
    }
    printf("\n");
}

//modify element
void Matrix::enterElement(int i, int j, int value)
{
    elements[(i-1)*size2+j-1]=value;
}

//matrix addition
Matrix add(Matrix A, Matrix B)
{
    Matrix C(A.size1,A.size2);
    
    int i,j;
    for(i=1;i<=A.size1;i++)
    {
        for(j=1;j<=A.size2;j++)
        {
            C.enterElement(i,j,A.returnElement(i,j)+B.returnElement(i,j));
        }
    }
    
    return C;
}

//matrix subtraction
Matrix sub(Matrix A, Matrix B)
{
    Matrix C(A.size1,A.size2);
    
    int i,j;
    for(i=1;i<=A.size1;i++)
    {
        for(j=1;j<=A.size2;j++)
        {
            C.enterElement(i,j,A.returnElement(i,j)-B.returnElement(i,j));
        }
    }
    
    return C;
}

//matrix product
Matrix multiplyBruteForce(Matrix A, Matrix B)
{
    Matrix C(A.size1,B.size2);
    
    int i,j,k,tmp;
    for(i=1;i<=A.size1;i++)
    {
        for(j=1;j<=B.size2;j++)
        {
            tmp=0;
            for(k=1;k<=A.size2;k++)
            {
                tmp+=(A.returnElement(i,k))*(B.returnElement(k,j));
            }
            C.enterElement(i,j,tmp);
        }
    }

    return C;
}

//matrix product
Matrix multiplyStrassen(Matrix A, Matrix B)
{   
    if(A.size1==A.size2 && A.size2==B.size1 && B.size1==B.size2 && B.size2==1)
    {
        Matrix C(A.size1,B.size2);
        C.enterElement(1,1,(A.returnElement(1,1))*(B.returnElement(1,1)));
        return C;
    }
    else
    {
        //A,B ok if 2^n x 2^n,
        //zero padding otherwise
        int tmp=max(nextPowerOfTwo(A.size1),nextPowerOfTwo(A.size2),nextPowerOfTwo(B.size1),nextPowerOfTwo(B.size2));
        
        Matrix paddedA(tmp,tmp,0);
        Matrix paddedB(tmp,tmp,0);
        
        int i,j;
        for(i=1;i<=A.size1;i++)
        {
            for(j=1;j<=A.size2;j++)
            {
                paddedA.enterElement(i,j,(A.returnElement(i,j)));
            }
        }
        for(i=1;i<=B.size1;i++)
        {
            for(j=1;j<=B.size2;j++)
            {
                paddedB.enterElement(i,j,(B.returnElement(i,j)));
            }
        }
        
        //split A,B into 4 blocks each
        Matrix a(paddedA.size1 / 2, paddedA.size1 / 2, 0);
        Matrix b(paddedA.size1 / 2, paddedA.size1 / 2, 0);
        Matrix c(paddedA.size1 / 2, paddedA.size1 / 2, 0);
        Matrix d(paddedA.size1 / 2, paddedA.size1 / 2, 0);
        Matrix e(paddedA.size1 / 2, paddedA.size1 / 2, 0);
        Matrix f(paddedA.size1 / 2, paddedA.size1 / 2, 0);
        Matrix g(paddedA.size1 / 2, paddedA.size1 / 2, 0);
        Matrix h(paddedA.size1 / 2, paddedA.size1 / 2, 0);
        
        for(i=1;i<=paddedA.size1/2;i++)
        {
            for(j=1;j<=paddedA.size1/2;j++)
            {
                a.enterElement(i,j,(paddedA.returnElement(i,j)));
                b.enterElement(i,j,(paddedA.returnElement(i,paddedA.size1/2+j)));
                c.enterElement(i,j,(paddedA.returnElement(paddedA.size1/2+i,j)));
                d.enterElement(i,j,(paddedA.returnElement(paddedA.size1/2+i,paddedA.size1/2+j)));
                
                e.enterElement(i,j,(paddedB.returnElement(i,j)));
                f.enterElement(i,j,(paddedB.returnElement(i,paddedA.size1/2+j)));
                g.enterElement(i,j,(paddedB.returnElement(paddedA.size1/2+i,j)));
                h.enterElement(i,j,(paddedB.returnElement(paddedA.size1/2+i,paddedA.size1/2+j)));
            }
        }
        
        //calculate the 7 terms
        Matrix p1(paddedA.size1/2,paddedA.size1/2);
        Matrix p2(paddedA.size1/2,paddedA.size1/2);
        Matrix p3(paddedA.size1/2,paddedA.size1/2);
        Matrix p4(paddedA.size1/2,paddedA.size1/2);
        Matrix p5(paddedA.size1/2,paddedA.size1/2);
        Matrix p6(paddedA.size1/2,paddedA.size1/2);
        Matrix p7(paddedA.size1/2,paddedA.size1/2);
        
        p1=multiplyStrassen(a,sub(f,h));
        p2=multiplyStrassen(add(a,b),h);
        p3=multiplyStrassen(add(c,d),e);
        p4=multiplyStrassen(d,sub(g,e));
        p5=multiplyStrassen(add(a,d),add(e,h));
        p6=multiplyStrassen(sub(b,d),add(g,h));
        p7=multiplyStrassen(sub(a,c),add(e,f));
        
        //combine them
        Matrix tmp11=add(sub(add(p5,p4),p2),p6);
        Matrix tmp12=add(p1,p2);
        Matrix tmp21=add(p3,p4);
        Matrix tmp22=sub(sub(add(p1,p5),p3),p7);
        
        
        //build result
        Matrix C(paddedA.size1,paddedA.size1);
        for(i=1;i<=paddedA.size1/2;i++)
        {
            for(j=1;j<=paddedA.size1/2;j++)
            {
                C.enterElement(i,j,tmp11.returnElement(i,j));
                C.enterElement(i,j+paddedA.size1/2,tmp12.returnElement(i,j));
                C.enterElement(i+paddedA.size1/2,j,tmp21.returnElement(i,j));
                C.enterElement(i+paddedA.size1/2,j+paddedA.size1/2,tmp22.returnElement(i,j));
            }
        }
        
        return C;
    }
}


int main()
{
    //initialise random-number generator
    srand(time(NULL));

    //create matrices
    int size1=randomInt();
    int size2=randomInt();
    int size3=randomInt();

    Matrix A(size1,size2);
    Matrix B(size2,size3);

    printf("\nMatrix A =\n");
    A.printElements(A.size1,A.size2);
    printf("Matrix B =\n");
    B.printElements(B.size1,B.size2);
    
    printf("Brute-force multiplication A.B =\n");
    Matrix C=multiplyBruteForce(A,B);
    C.printElements(A.size1,B.size2);

    printf("Subtract Strassen multiplication A.B =\n");
    C=sub(C,multiplyStrassen(A,B));
    C.printElements(A.size1,B.size2);

    return EXIT_SUCCESS;
}