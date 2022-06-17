#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h> //need to install GMP

//minimum of two numbers
size_t min(size_t x, size_t y)
{    
    if (x<y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

//number of digits
size_t numberOfDigits(mpz_t x)
{    
    char *buff = (char *) malloc (mpz_sizeinbase(x, 10) + 1);
    gmp_sprintf(buff, "%Zd", x);
    
    return strlen(buff);
}

//multiply by 10^n
void multiply10(mpz_t res, mpz_t x, size_t n)
{    
    char *buff = (char *) malloc (mpz_sizeinbase(x, 10) + n + 1);
    gmp_sprintf(buff, "%Zd", x);
    size_t length = strlen(buff);
    
    int i;
    for(i=0;i<=n-1;i++)
    {
        buff[length+i]='0';
    }
    buff[length+n]='\0';
        
    mpz_set_str(res, buff, 10);
}

//multiply by 10^(2*n)
void multiply102(mpz_t res, mpz_t x, size_t n)
{    
    multiply10(res, x, 2*n);
}

//quotient of division by 10^n
void quotient10(mpz_t res, mpz_t x, size_t n)
{   
    char *buff = (char *) malloc (mpz_sizeinbase(x, 10) + 1);
    gmp_sprintf(buff, "%Zd", x);
    size_t length = strlen(buff);
    
    buff[length-n]='\0';
        
    mpz_set_str(res, buff, 10);
}

//reminder of division by 10^n
void reminder10(mpz_t res, mpz_t x, size_t n)
{   
    char *buff = (char *) malloc (mpz_sizeinbase(x, 10) + 1);
    gmp_sprintf(buff, "%Zd", x);
    size_t length = strlen(buff);
        
    int i;
    for(i=0;i<=n-1;i++)
    {
        buff[i]=buff[i+length-n];
    }
    buff[n]='\0';
        
    mpz_set_str(res, buff, 10);
}

//Karatsuba algorithm
void karatsuba(mpz_t res, mpz_t x, mpz_t y)
{
    // base case
    if (numberOfDigits(x) == 1 || numberOfDigits(y) == 1)
    {
        mpz_mul(res, x, y);
    }
    else
    {
        mpz_t a, b, c, d;
        mpz_inits(a,b,c,d,NULL);

        int m=min(numberOfDigits(x), numberOfDigits(y))/2;
        
        // x or y is split into m+m digits, the other is generally not.
        // x = a * 10^m + b
        // y = c * 10^m + d

        //a=x/(10^m)
        quotient10(a, x, m);
        //b=x%(10^m)
        reminder10(b, x, m);
        //c=y/(10^m)
        quotient10(c, y, m);
        //d=y%(10^m)
        reminder10(d, y, m);
        
        mpz_t ac, bd, aPlusbTimescPlusdMinusacMinusbd,tmp0, tmp00;
        mpz_inits(ac,bd,aPlusbTimescPlusdMinusacMinusbd,tmp0, tmp00, NULL);

        //a*c
        karatsuba(ac, a, c);

        //b*d
        karatsuba(bd, b, d);

        //a+b
        mpz_add(tmp0, a, b);
        //c+d
        mpz_add(tmp00, c, d);
        //(a+b)*(c+d)
        karatsuba(aPlusbTimescPlusdMinusacMinusbd, tmp0, tmp00);
        //(a+b)*(c+d)-a*c
        mpz_sub(aPlusbTimescPlusdMinusacMinusbd, aPlusbTimescPlusdMinusacMinusbd, ac);
        //(a+b)*(c+d)-a*c-b*d
        mpz_sub(aPlusbTimescPlusdMinusacMinusbd, aPlusbTimescPlusdMinusacMinusbd, bd);

        mpz_t addend_1, addend_3;
        mpz_inits(addend_1, addend_3, NULL);
        
        //a*c*10^(2m)
        multiply102(addend_1, ac, m);

        //((a+b)*(c+d)-a*c-b*d)*10^m
        multiply10(addend_3, aPlusbTimescPlusdMinusacMinusbd, m);

        //sum addends
        mpz_add(res, addend_1, bd);
        mpz_add(res, addend_3, res);
    }
}

int main()
{
    //input
    mpz_t factor1, factor2, output, rightAnswer;
    mpz_inits(factor1,factor2,output,rightAnswer,NULL);
    
    mpz_set_str(factor1, "2718281828459045235360287471352662497757247093699959574966967", 10);
    mpz_set_str(factor2, "6277240766303535475945713821785251664274274663919320030599218", 10);
    mpz_set_str(rightAnswer, "17063309507905232681397562722896967388623159770327731688435171979480249150768710251785803464799537846507242728041566031806", 10);

    //multiply
    karatsuba(output, factor1, factor2);

    //print
    printf("\nFirst factor = ");
    gmp_printf("%Zd\n", factor1);
    printf("\nSecond factor = ");
    gmp_printf("%Zd\n", factor2);
    printf("\nProduct = ");
    gmp_printf("%Zd\n\n", output);
    
    if(mpz_cmp(output,rightAnswer)==0)
    {
        printf("SUCCESS!\n\n");
    }
    else
    {
        printf("FAILED!\n\n");
    }
    
    return EXIT_SUCCESS;
}