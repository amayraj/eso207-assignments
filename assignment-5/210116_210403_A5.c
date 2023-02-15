#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

/*Below are the variables used for counting the number of comparsions*/
// long long int Mcomp, Qcomp;

/*Implementation of Merge Sort and Improved Merge Sort*/

/*Function to merge two sorted arrays*/
void Merge(double *A, int s, int e, double *C)
{
    int i = s;
    int j = (s + e) / 2 + 1;
    int k = 0;
    while ((i <= (s + e) / 2) && (j <= e))
    {
        // Mcomp++;
        if (A[i] < A[j])
        {
            C[k] = A[i];
            i++;
            k++;
        }
        else
        {
            C[k] = A[j];
            j++;
            k++;
        }
    }
    while (i <= (s + e) / 2)
    {
        C[k] = A[i];
        i++;
        k++;
    }
    while (j <= e)
    {
        C[k] = A[j];
        j++;
        k++;
    }
}

/*Standard Merge Sort code with the redundant overheads*/
void MergeSort(double *A, int s, int e)
{
    if (s >= e)
        return;
    else
    {
        int mid = (s + e) / 2;

        MergeSort(A, s, mid);
        MergeSort(A, mid + 1, e);

        double *C = (double *)malloc((e - s + 1) * sizeof(double)); //temporary array which stores the sorted array after merging
        
        Merge(A, s, e, C);

        /*Copying back C[0...(e-s)] to A[s...e]*/
        int k = 0;
        for (int i = s; i <= e; i++)
        {
            A[i] = C[k];
            k++;
        }
        free(C); //freeing the temporary array C
    }
}

/*The Improved Merge Sort Code which does not invoke the Merge() function if the subarray to the left of middle element and right of middle element are sorted */
void ImprovedMergeSort(double *A, int s, int e)
{
    if (s >= e)
        return;
    else
    {
        int mid = (s + e) / 2;

        ImprovedMergeSort(A, s, mid);
        ImprovedMergeSort(A, mid + 1, e);

        if (A[mid] <= A[mid + 1])
        {
            return;
        }

        double *C = (double *)malloc((e - s + 1) * sizeof(double)); //temporary array which stores the sorted array after merging 
        
        Merge(A, s, e, C);

        /*Copying back C[0...(e-s)] to A[s...e]*/
        int k = 0;
        for (int i = s; i <= e; i++)
        {
            A[i] = C[k];
            k++;
        }
        free(C); //freeing the temporary array C
    }
}

/*Implementation of Quick Sort*/

/*Function to Partition the array about the pivot*/
int Partition(double *A, int l, int r)
{
    double pivot = A[l], temp; //We have chosen the first element of the subarray as the pivot
    int i = r, j = l + 1;
    while (j <= i)
    {
        // Qcomp++;
        if (A[j] <= pivot)
            j++;
        else
        {
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
            i--;
        }
    }
    temp = A[i];
    A[i] = pivot;
    A[l] = temp;
    return i; //returning the index of the pivot
}

/*Standard Quick Sort Code*/
void QuickSort(double *A, int l, int r)
{
    if (l < r)
    {
        int i = Partition(A, l, r);
        QuickSort(A, l, i - 1);
        QuickSort(A, i + 1, r);
    }
}

int main()
{
    int n, itr, i, M_vs_Q;
    srand((unsigned)time(NULL)); //to ensure that the numbers are uniform
    int MAXRANGE = 50; //We have taken numbers from 0 to 50

    clock_t start_t, end_t;
    double total_tM, total_tQ;

    FILE *fptr = fopen("1.4_MQ.csv", "w+");
    if (fptr == NULL)
    {
        printf("Error! File doesn't exist");
        exit(1);
    }
    // fprintf(fptr, "%s, %s, %s, %s, %s\n", "n", "Mcomp", "Qcomp", "total_tM", "total_tQ");
    fprintf(fptr, "%s, %s, %s\n", "n", "total_tM", "total_tQ");

    for (n = 100; n <= 1e6; n *= 10)
    {
        double *A1 = (double *)malloc(n * sizeof(double));
        double *A2 = (double *)malloc(n * sizeof(double));
        M_vs_Q = 0;
        for (itr = 1; itr <= 500; itr++)
        {
            for (i = 0; i < n; i++)
            {
                A1[i] = A2[i] = ((double)rand() / (double)RAND_MAX) * MAXRANGE;
            }

            // Mcomp = 0;
            start_t = clock();
            // MergeSort(A1, 0, n - 1);
            ImprovedMergeSort(A1, 0, n - 1);
            end_t = clock();
            total_tM = ((double)(end_t - start_t) * 1000000) / CLOCKS_PER_SEC;

            // Qcomp = 0;
            start_t = clock();
            QuickSort(A2, 0, n - 1);
            end_t = clock();
            total_tQ = ((double)(end_t - start_t) * 1000000) / CLOCKS_PER_SEC;

            if (total_tM < total_tQ)
                M_vs_Q++;

            fprintf(fptr, "%d, %lf, %lf\n", n, total_tM, total_tQ);
        }
        fprintf(fptr, "%d\n", M_vs_Q);
        free(A1);
        free(A2);
    }
    return 0;
}