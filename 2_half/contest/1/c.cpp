#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define int long long

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// merge_sort
//--------------------------------------------------------------------------------------------------------------------------------

int merge_sort(int *const arr, const int l, const int r);
int merge     (int *const arr, const int l, const int r);

//--------------------------------------------------------------------------------------------------------------------------------
// binary_search
//--------------------------------------------------------------------------------------------------------------------------------

int binary_search(const int n, const int *arr, const int k);
int is_good_len  (const int n, const int *arr, const int k, int len);

//================================================================================================================================
// MAIN
//================================================================================================================================

signed main()
{
    int n = 0;
    int k = 0;
    if (scanf("%lld %lld", &n, &k) != 2)
    {
        //log_error("Expected two integer input\n");
        return 0;
    }

    int *arr = (int *) calloc((size_t) n, sizeof(int));
    if (arr == NULL)
    {
        //log_error("log_calloc(n = %lld, sizeof(int) = %lu) returns nullptr\n", n, sizeof(int));
        return 0;
    }

    for (int i = 0; i < n; ++i)
    {
        if (scanf("%lld", arr + i) != 1)
        {
            //log_error("i = %lld: expected integer input\n", i);
            break;
        }
    }

    merge_sort(arr, 0, n);
    printf("%lld\n", binary_search(n, arr, k));

    free(arr);
}

//--------------------------------------------------------------------------------------------------------------------------------
// merge_sort
//--------------------------------------------------------------------------------------------------------------------------------

int merge_sort(int *const arr, const int l, const int r)
{
    //log_verify(arr != nullptr, 0);
    //log_verify(l >= 0, 0);
    //log_verify(r >  0, 0);
    //log_verify(l <  r, 0);

    if (r - l == 1) return 1;

    if (!merge_sort(arr, l, (l + r) / 2)) return 0;
    if (!merge_sort(arr, (l + r) / 2, r)) return 0;

    if (!merge(arr, l, r)) return 0;
    return 1;
}

int merge(int *const arr, const int l, const int r)
{
    //log_verify(arr != nullptr, 0);
    //log_verify(l >= 0, 0);
    //log_verify(r >  0, 0)
    //log_verify(l <  r, 0);

    int *sort_part = (int *) calloc((size_t) (r - l), sizeof(int));
    if (sort_part == NULL)
    {
        //log_error("log_calloc((size_t) (r - l) = %lu, sizeof(int) = %lu) returns nullptr", r - l, sizeof(int));
        return 0;
    }

    int l_max = (l + r) / 2, l_cnt = l;
    int r_max = r,           r_cnt = (l + r) / 2;

    for (int i = 0; i < r - l; ++i)
    {
        if      (l_cnt == l_max) { sort_part[i] = arr[r_cnt]; ++r_cnt; }
        else if (r_cnt == r_max) { sort_part[i] = arr[l_cnt]; ++l_cnt; }
        else
        {
            if (arr[l_cnt] < arr[r_cnt]) { sort_part[i] = arr[l_cnt]; ++l_cnt; }
            else                         { sort_part[i] = arr[r_cnt]; ++r_cnt; }
        }
    }

    memcpy(arr + l, sort_part, (r - l) * sizeof(int));
    free(sort_part);

    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------
// binary_search
//--------------------------------------------------------------------------------------------------------------------------------

int binary_search(const int n, const int *arr, const int k)
{
    //log_verify(arr != nullptr, -1);
    //log_verify(n >  0, -1);
    //log_verify(k >  0, -1);
    //log_verify(k <= n, -1);

    int l = -1;
    int r = arr[n - 1] - arr[0];

    while (r - l > 1)
    {
        int mid = (l + r) / 2;
        if (is_good_len(n, arr, k, mid)) r = mid;
        else                             l = mid;
    }

    return r;
}

int is_good_len(const int n, const int *arr, int k, const int len)
{
    //log_verify(arr != nullptr, 0);
    //log_verify(n >  0, 0);
    //log_verify(k >  0, 0);
    //log_verify(k <= n, 0);

    int last_light_ind = 0;
    k--;

    for (int i = 1; i < n; ++i)
    {
        if (arr[i] - arr[last_light_ind] > len)
        {
            k--;
            last_light_ind = i;
        }
    }

    return k >= 0;
}