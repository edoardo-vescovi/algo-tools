#include <stdio.h>
#include "karatsuba.h"

int main()
{
    unsigned long long factor1, factor2;

    printf("\nEnter first positive factor: ");
    scanf("%llu", &factor1);
    printf("\nEnter second positive factor: ");
    scanf("%llu", &factor2);

    unsigned long long output = karatsuba(factor1, factor2);

    if (output == factor1 * factor2)
    {
        printf("\nSUCCESS: the product is %llu.\n\n", factor1 * factor2);
    }
    else
    {
        printf("\nFAILED: the product is %llu, but the Karatsuba algorithm returns %llu.\n\n", factor1 * factor2, output);
    }

    return EXIT_SUCCESS;
}