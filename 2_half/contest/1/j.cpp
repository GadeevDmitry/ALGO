#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// merge sort
//--------------------------------------------------------------------------------------------------------------------------------

long long merge_sort(int *const arr, const int l, const int r);
long long merge     (int *const arr, const int l, const int r);

//================================================================================================================================
// MAIN
//================================================================================================================================

int main()
{
    int n = 0;
    if (scanf("%d", &n) != 1) { /* log_error(...); */ return 0; }

    int *arr = (int *) calloc((size_t) n, sizeof(int));
    if (arr == NULL) { /* log_error(...); */ return 0; }

    for (int i = 0; i < n; ++i)
    {
        if (scanf("%d", arr+i) != 1) { /* log_error(...) ;*/ return 0; }
    }

    printf("%lld\n", merge_sort(arr, 0, n));

    free(arr);
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------
// merge sort
//--------------------------------------------------------------------------------------------------------------------------------

long long merge_sort(int *const arr, const int l, const int r)
{
    //log_verify(arr != nullptr, 0ll);
    //log_verify(n > 0, 0ll);
    //log_verify(l > 0, 0ll);
    //log_verify(r > l, 0ll);

    if (l + 1 == r) { return 0ll; }

    int mid = (l + r) / 2;
    long long inversion = 0ll;

    inversion += merge_sort(arr, l, mid);
    inversion += merge_sort(arr, mid, r);
    inversion += merge     (arr, l,   r);

    return inversion;
}

long long merge(int *const arr, const int l, const int r)
{
    //log_verify(arr != nullptr, 0ll);
    //log_verify(n > 0, 0ll);
    //log_verify(l > 0, 0ll);
    //log_verify(r > l + 1, oll);

    int *sorted = (int *) calloc((size_t) (r - l), sizeof(int));
    if (sorted == NULL) { /* log_error(...); */ return 0ll; }

    long long inversion = 0ll;

    int l_cnt = l, l_max = (l + r) / 2;
    int r_cnt = (l + r) / 2, r_max = r;

    for (int i = 0; i < r - l; ++i)
    {
        if      (l_cnt == l_max) { sorted[i] = arr[r_cnt]; ++r_cnt; }
        else if (r_cnt == r_max) { sorted[i] = arr[l_cnt]; ++l_cnt; inversion += r_cnt - l_max; }
        else
        {
            if (arr[l_cnt] <= arr[r_cnt]) { sorted[i] = arr[l_cnt]; ++l_cnt; inversion += r_cnt - l_max; }
            else                          { sorted[i] = arr[r_cnt]; ++r_cnt; }
        }
    }

    memcpy(arr + l, sorted, (size_t) (r - l) * sizeof(int));
    free(sorted);

    return inversion;
}