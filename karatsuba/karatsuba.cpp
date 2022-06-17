#include "karatsuba.h"

// calculate number of digits in integer x
unsigned long long numberOfDigits(unsigned long long x)
{
    if (x == 0)
    {
        return 1;
    }
    else
    {
        return floor(log10(x) + 1);
    }
}

// min function
unsigned long long min(unsigned long long x, unsigned long long y)
{
    if (x < y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

// power 10^n
unsigned long long power10(unsigned long long n)
{
    return (unsigned long long)(pow(10, n) + 0.1);
}

// Karatsuba algorithm
unsigned long long karatsuba(unsigned long long x, unsigned long long y)
{
    // base case
    if (numberOfDigits(x) == 1 || numberOfDigits(y) == 1)
    {
        return x * y;
    }
    else
    {
        unsigned long long a, b, c, d;
        unsigned long long m = min(numberOfDigits(x), numberOfDigits(y)) / 2;
        // x or y is split into m+m digits, the other is generally not.

        a = x / power10(m);
        b = x % power10(m);
        c = y / power10(m);
        d = y % power10(m);

        // x = a * 10^m + b
        // y = c * 10^m + d

        unsigned long long tmp1, tmp2, tmp3;
        tmp1 = karatsuba(a, c);
        tmp2 = karatsuba(b, d);
        tmp3 = karatsuba(a + b, c + d) - tmp1 - tmp2;
        return tmp1 * power10(2 * m) + tmp2 + tmp3 * power10(m);
    }
}