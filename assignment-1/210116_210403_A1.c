#include <stdio.h>
#include <time.h>
#include <math.h>

typedef struct
{
    int m[2][2];
} arr2d;

arr2d powerM(int A[2][2], long long int n)
{
    arr2d I;
    I.m[0][0] = 1;
    I.m[0][1] = 0;
    I.m[1][0] = 0;
    I.m[1][1] = 1;
    if (n == 0)
        return I;
    arr2d a;
    a = powerM(A, n / 2);
    int i, j, k;
    long long int sum = 0;
    arr2d b;
    for (i = 0; i < 2; i++)
    {
        for (k = 0; k < 2; k++)
        {
            sum = 0;
            for (j = 0; j < 2; j++)
            {
                sum = sum + a.m[i][j] * a.m[j][k];
            }
            b.m[i][k] = sum % 2021;
        }
    }
    if (n % 2 == 1)
    {
        arr2d c;
        for (i = 0; i < 2; i++)
        {
            for (k = 0; k < 2; k++)
            {
                sum = 0;
                for (j = 0; j < 2; j++)
                {
                    sum = sum + b.m[i][j] * A[j][k];
                }
                c.m[i][k] = sum % 2021;
            }
        }
        return c;
    }
    else
    {
        return b;
    }
}

int CleverFib(long long int n)
{
    int A[2][2] = {{1, 1}, {1, 0}};
    arr2d B;
    B = powerM(A, n - 1);
    return B.m[0][0];
}

int IFib(long long int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    long long int i, a = 0, b = 1, temp;
    for (i = 2; i <= n; i++)
    {
        temp = a;
        a = b;
        b = temp + a;
        a = a % 2021;
        b = b % 2021;
    }
    return b % 2021;
}

int RFib(long long int n)
{
    if (n == 0)
        return 0;

    else if (n == 1)
        return 1;

    else
        return (RFib(n - 1) + RFib(n - 2)) % 2021;
}

int main()
{
    printf("\nCalculating F(n) % 2021:\n(where F(n) is nth term of Fibonacci Series)\n\n");
    printf("1 - Recursive\n2 - Iterative\n3 - Clever\n\nEnter your choice to implement method: ");
    int ch, output;
    scanf("%d", &ch);
    long long int n;
    printf("Enter n: ");
    scanf("%lld", &n);
    printf("\n");

    clock_t start_t, end_t;
    double total_t;

    switch (ch)
    {
    case 1:
        if (n <= 30)
        {
            start_t = clock();
            for (int j = 0; j < 1000; j++)
                output = RFib(n);
            end_t = clock();
            printf("F(%lld) % 2021 = %lld\n", n, output);
            total_t = (double)(end_t - start_t) / (1000 * CLOCKS_PER_SEC);
        }
        else
        {
            start_t = clock();
            printf("F(%lld) % 2021 = %lld\n", n, RFib(n));
            end_t = clock();
            total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        }
        printf("Total time taken:%.8f s\n\n", total_t);
        break;
    case 2:
        if (n <= (long long int)pow(2, 22))
        {
            start_t = clock();
            for (int j = 0; j < 1000; j++)
                output = IFib(n);
            end_t = clock();
            printf("F(%lld) % 2021 = %lld\n", n, output);
            total_t = (double)(end_t - start_t) / (1000 * CLOCKS_PER_SEC);
        }
        else
        {
            start_t = clock();
            printf("F(%lld) % 2021 = %lld\n", n, IFib(n));
            end_t = clock();
            total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        }
        printf("Total time taken:%.8f s\n\n", total_t);
        break;
    case 3:
        start_t = clock();
        for (int j = 0; j < 1000000; j++)
            output = CleverFib(n);
        end_t = clock();
        printf("F(%lld) % 2021 = %lld\n", n, output);
        total_t = (double)(end_t - start_t) / (1000000 * CLOCKS_PER_SEC);
        printf("Total time taken:%.8f s\n\n", total_t);
        break;
    default:
        printf("Invalid Input!\n\n");
        break;
    }

    /*  The following code is
        for calculating time
        for each method, taking data
        plotting graph and analyzing.


    long long int i = 2, output;
    int j, t = 38; // for IFib(n)
    // t = 63 (for CleverFib(n))
    while (t--)
    {
        n = i;
        // n = 1 to 55  for RFib(n)
        clock_t start_t, end_t;
        double total_t;

        start_t = clock();
        // for (j = 0; j < 1000000; j++)   for calculating time < 0.001 accurately
            output = RFib(n);
        // output = IFib(n);
        // output = CleverFib(n);
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        // total_t = (double)(end_t - start_t) / (1000000 * CLOCKS_PER_SEC) for calculating time < 0.001 accurately

        // printf("%f\n", total_t);

        // Next line is ommitted for RFib(n), as it iterates from 1 to 55
        i *= 2;
    }
    */

    return 0;
}