#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "../../../../lib/logs/log.h"
#include "../../../../lib/algorithm/algorithm.h"

#include "sort.h"

//================================================================================================================================
// BUBBLE SORT
//================================================================================================================================

void bubble_sort(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n - (i + 1); ++j)
        {
            if (arr[j] > arr[j + 1]) int_swap(arr + j, arr + (j + 1));
        }
    }
}

//================================================================================================================================
// INSERT SORT
//================================================================================================================================

void insert_sort(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);

    for (int i = 1; i < n; ++i)
    {
        for (int j = i; j > 0; --j)
        {
            if (arr[j] > arr[j - 1]) break;

            int_swap(arr + j, arr + (j - 1));
        }
    }
}

//================================================================================================================================
// CHOOSE SORT
//================================================================================================================================

void choose_sort(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(n > 0, ;);

    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (arr[j] < arr[i]) int_swap(arr + j, arr + i);
        }
    }
}

//================================================================================================================================
// MERGE SORT
//================================================================================================================================

void merge_sort(int *const arr, const int n)
{
    //log_assert(arr != nullptr);

    if (n <= 1) return;

    int mid = n / 2;
    merge_sort(arr      ,     mid);
    merge_sort(arr + mid, n - mid);

    merge(arr, n);
}

void merge(int *const arr, const int n)
{
    //log_assert(arr != nullptr);
    //log_assert(n > 1);

    int *const sorted = (int *) log_calloc((size_t) n, sizeof(int));
    /*
    if        (sorted == nullptr)
    {
        log_error("log_calloc((size_t) n = %lu, sizeof(int) = %lu) returns nullptr\n", n, sizeof(int));
        return;
    }
    */

    int l_cnt =     0, l_max = n / 2;
    int r_cnt = n / 2, r_max =     n;

    for (int i = 0; i < n; ++i)
    {
        if      (l_cnt == l_max) { sorted[i] = arr[r_cnt]; ++r_cnt; }
        else if (r_cnt == r_max) { sorted[i] = arr[l_cnt]; ++l_cnt; }
        else
        {
            if (arr[l_cnt] < arr[r_cnt]) { sorted[i] = arr[l_cnt]; ++l_cnt; }
            else                         { sorted[i] = arr[r_cnt]; ++r_cnt; }
        }
    }

    memcpy(arr, sorted, (size_t) n * sizeof(int));
    log_free(sorted);
}

//================================================================================================================================
// QUICK SORT
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// main
//--------------------------------------------------------------------------------------------------------------------------------

void quick_sort(int *const arr, const int n, int (*get_pivot) (int *, int))
{
    //log_assert(arr       != nullptr);
    //log_assert(get_pivot != nullptr);

    if (n <= 1) return;

    int pivot_ind = quick_sort_frame(arr, n, get_pivot);

    if (pivot_ind == 0) quick_sort(arr + 1, n - 1, get_pivot);
    else
    {
        quick_sort(arr,                 pivot_ind, get_pivot);
        quick_sort(arr + pivot_ind, n - pivot_ind, get_pivot);
    }
}

int quick_sort_frame(int *const arr, const int n, int (*get_pivot) (int *, int))
{
    //log_assert(arr       != nullptr);
    //log_assert(get_pivot != nullptr);

    if (n <= 1) return 0;

    int pivot = (*get_pivot) (arr, n);
    int left  =     0;
    int right = n - 1;

    while (true)
    {
        while (left < right && arr[left]  < pivot) ++ left;
        while (left < right && arr[right] > pivot) --right;

        if (left >  right) return left;
        if (left == right) return (arr[left] >= pivot) ? left : left + 1;

        int_swap(arr + left, arr + right);

        ++ left;
        --right;
    }

    return -1;
}

//--------------------------------------------------------------------------------------------------------------------------------
// median_of_three
//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_median_of_three(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(n >= 0, ;);

    quick_sort(arr, n, get_pivot_by_median_of_three);
}

int get_pivot_by_median_of_three(int *const arr, const int n)
{
    //log_assert(arr != nullptr);
    //log_assert(n > 0);

    int val_1 = arr[0];
    int val_2 = arr[n - 1];
    int val_3 = arr[n / 2];

    if ((val_1 <= val_2 && val_2 <= val_3) || (val_3 <= val_2 && val_2 <= val_1)) return val_2;
    if ((val_1 <= val_3 && val_3 <= val_2) || (val_2 <= val_3 && val_3 <= val_1)) return val_3;
    return val_1;
}

//--------------------------------------------------------------------------------------------------------------------------------
// central
//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_central(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(n >= 0, ;);

    quick_sort(arr, n, get_pivot_by_central);
}

int get_pivot_by_central(int *const arr, const int n)
{
    //log_assert(arr != nullptr);
    //log_assert(n > 0);

    return arr[n / 2];
}

//--------------------------------------------------------------------------------------------------------------------------------
// rand
//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_rand(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(n >= 0, ;);

    quick_sort(arr, n, get_pivot_by_rand);
}

int get_pivot_by_rand(int *const arr, const int n)
{
    //log_assert(arr != nullptr);
    //log_assert(n > 0);

    return arr[rand() % n];
}

//--------------------------------------------------------------------------------------------------------------------------------
// median of medians
//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_median_of_medians(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(n >= 0, ;);

    quick_sort(arr, n, get_pivot_by_median_of_medians);
}

int get_pivot_by_median_of_medians(int *const arr, const int n)
{
    //log_assert(arr != nullptr);
    //log_assert(n > 0);

    int  median_arr_size = (n + 4) / 5;
    int *median_arr      = (int *) log_calloc((size_t) median_arr_size, sizeof(int));
    /*
    if    (median_arr == nullptr)
    {
        log_error("log_calloc(median_arr_size = %lu, sizeof(int) = %lu) returns nullptr\n", median_arr_size, sizeof(int));
        return 0;
    }
    */

    for (int i = 0; i < n; i += 5)
    {
        if (i + 5 <= n) { insert_sort(arr + i, 5); median_arr[i / 5] = arr[i + 2]; }
        else            {                          median_arr[i / 5] = arr[i];     }
    }

    int ans = kth_element(median_arr, median_arr_size, median_arr_size / 2);

    log_free(median_arr);
    return ans;
}

int kth_element(int *const arr, const int n, const int k)
{
    //log_assert(arr != nullptr);
    //log_assert(k > 0);
    //log_assert(n > k);

    if (n == 1) return arr[0];

    int pivot_ind = quick_sort_frame(arr, n, get_pivot_by_median_of_medians);

    if      (pivot_ind == k) return arr[pivot_ind];
    else if (pivot_ind >  k) return kth_element(arr, pivot_ind, k);

    pivot_ind++;
    return kth_element(arr + pivot_ind, n - pivot_ind, k - pivot_ind);

}

//================================================================================================================================
// SWAP
//================================================================================================================================

void int_swap(int *const a, int *const b)
{
    //log_verify(a != nullptr, ;);
    //log_verify(b != nullptr, ;);

    int temp = *a;
    *a = *b;
    *b = temp;
}