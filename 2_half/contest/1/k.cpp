#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#include "../../../lib/logs/log.h"

//#define QUICK_SORT
//#define QUICK_SORT_FRAME
//#define LOGS

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// quick sort
//--------------------------------------------------------------------------------------------------------------------------------

void quick_sort(int *const arr, const int        n);
void int_swap  (int *const a,         int *const b);

//================================================================================================================================
// MAIN
//================================================================================================================================

int *ARR_BEGIN = NULL;

int main()
{
    int n = 0;
    if (scanf("%d", &n) != 1) { /* log_error(...); */ return 0; }

    int *arr = (int *) calloc((size_t) n, sizeof(int));
    if  (arr == NULL) { /* log_error(...); */ return 0; }

    ARR_BEGIN = arr;

    for (int i = 0; i < n; ++i)
    {
        if (scanf("%d", arr + i) != 1) { /*log_error(...); */ free(arr); return 0; }
    }

    quick_sort(arr, n);

    for (int i = 0; i < n; ++i) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
}

//--------------------------------------------------------------------------------------------------------------------------------
// quick sort
//--------------------------------------------------------------------------------------------------------------------------------

void quick_sort(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);

    if (n <= 1) return;

    int left  =     0;
    int right = n - 1;
    int pivot = arr[(left + right) / 2];

    while (left <= right)
    {
        while (arr[left]  < pivot) ++left;
        while (arr[right] > pivot) --right;

        if (left <= right)
        {
            int_swap(arr + left, arr + right);

            ++left;
            --right;
        }
    }

    quick_sort(arr       , right + 1);
    quick_sort(arr + left,  n - left);
}

void int_swap(int *const a, int *const b)
{
    //log_verify(a != nullptr, ;);
    //log_verify(b != nullptr, ;);

    int temp = *a;
    *a = *b;
    *b = temp;
}