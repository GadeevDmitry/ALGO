#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// MAIN
//================================================================================================================================

struct segment
{
    int l;
    int r;
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// merge sort
//--------------------------------------------------------------------------------------------------------------------------------

void merge_sort(struct segment *const arr, const int l_sort, const int r_sort);
void merge     (struct segment *const arr, const int l_sort, const int r_sort);

//--------------------------------------------------------------------------------------------------------------------------------
// scan_line
//--------------------------------------------------------------------------------------------------------------------------------

void scan_line(const int n, struct segment *const arr, int *const comb_size);
int    get_max(const int a, const int b);

//================================================================================================================================
// MAIN
//================================================================================================================================

int main()
{
    int n = 0;
    if (scanf("%d", &n) != 1)
    {
        //log_error("Expected integer input\n");
        return 0;
    }

    struct segment *arr = (struct segment *) calloc((size_t) n, sizeof(struct segment));
    if (arr == NULL)
    {
        //log_error("log_calloc((size_t) n = %lu, sizeof(segment) = %lu) returns nullptr\n", n, sizeof(segment));
        return 0;
    }

    for (int i = 0; i < n; ++i)
    {
        int l = 0;
        int r = 0;
        if (scanf("%d %d", &l, &r) != 2)
        {
            //log_error("Expected two integers input\n");
            return 0;
        }

        arr[i] = (struct segment) {l, r};
    }

    merge_sort(arr, 0, n);

    int comb_size = 0;
    scan_line(n, arr, &comb_size);

    printf("%d\n", comb_size);
    for (int i = 0; i < comb_size; ++i) printf("%d %d\n", arr[i].l, arr[i].r);

    free(arr);
}

//--------------------------------------------------------------------------------------------------------------------------------
// merge sort
//--------------------------------------------------------------------------------------------------------------------------------

void merge_sort(struct segment *const arr, const int l_sort, const int r_sort)
{
    //log_verify(arr != nullptr,  ;);
    //log_verify(l_sort >= 0,     ;);
    //log_verify(l_sort < r_sort, ;);

    if (l_sort + 1 == r_sort) return;

    int mid = (l_sort + r_sort) / 2;

    merge_sort(arr, l_sort, mid   );
    merge_sort(arr, mid   , r_sort);
    merge     (arr, l_sort, r_sort);
}

void merge(struct segment *const arr, const int l_sort, const int r_sort)
{
    //log_verify(arr != nullptr,      ;);
    //log_verify(l_sort >= 0,         ;);
    //log_verify(r_sort - l_sort > 1, ;);

    struct segment *sorted = (struct segment *) calloc((size_t) r_sort - l_sort, sizeof(struct segment));
    if (sorted == NULL)
    {
        //log_error("log_calloc((size_t) r_sort - l_sort = %lu, sizeof(segment) = %lu) returns nullptr\n", r_sort - l_sort, sizeof(segment));
        return;
    }

    int l_cnt = l_sort, l_max = (l_sort + r_sort) / 2;
    int r_cnt = (l_sort + r_sort) / 2, r_max = r_sort;

    for (int i = 0; i < r_sort - l_sort; ++i)
    {
        if      (l_cnt == l_max) { sorted[i] = arr[r_cnt]; ++r_cnt; }
        else if (r_cnt == r_max) { sorted[i] = arr[l_cnt]; ++l_cnt; }
        else
        {
            if (arr[l_cnt].l < arr[r_cnt].l) { sorted[i] = arr[l_cnt]; ++l_cnt; }
            else                             { sorted[i] = arr[r_cnt]; ++r_cnt; }
        }
    }

    memcpy(arr + l_sort, sorted, (r_sort - l_sort) * sizeof(struct segment));
    free(sorted);
}

//--------------------------------------------------------------------------------------------------------------------------------
// scan_line
//--------------------------------------------------------------------------------------------------------------------------------

void scan_line(const int n, struct segment *const arr, int *const comb_size)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(comb_size != nullptr, ;);
    //log_verify(n > 0, ;);

    int comb_ind = 0;
    int next_ind = 1;

    for (; next_ind < n; ++next_ind)
    {
        if (arr[next_ind].l <= arr[comb_ind].r)
        {
            arr[comb_ind].r = get_max(arr[comb_ind].r, arr[next_ind].r);
        }
        else
        {
            ++comb_ind;
            arr[comb_ind] = arr[next_ind];
        }
    }

    *comb_size = comb_ind + 1;
}

int get_max(const int a, const int b)
{
    if (a > b) return a;
    return b;
}