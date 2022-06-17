#include <stdio.h>
#include "karatsuba.h"

int main()
{
    unsigned long long factor1, factor2;

    bool success = true;

    for (factor1 = 0; factor1 <= 1000; factor1++)
    {
        for (factor2 = 0; factor2 <= 1000; factor2++)
        {

            unsigned long long output = karatsuba(factor1, factor2);

            if (output != factor1 * factor2)
            {
                success = false;
                break;
            }
                }
    }

    if (success)
    {
        printf("\nPASSED!\n\n");
    }
    else
    {
        printf("\nFAILED!\n\n");
    }

    return EXIT_SUCCESS;
}