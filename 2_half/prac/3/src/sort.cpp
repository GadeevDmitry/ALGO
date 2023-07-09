#include <stdio.h>

#include "test.h"

#define NDEBUG
#define NVERIFY
#define LOG_NTRACE
#define LOG_NLEAK
#include "../../../../lib/logs/log.h"
#include "../../../../lib/algorithm/algorithm.h"

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

static void int_swap(int *const a, int *const b);
static void int_swap(int *const a, int *const b)
{
    log_assert(a != nullptr);
    log_assert(b != nullptr);

    int temp = *a;
    *a = *b;
    *b = temp;
}

//--------------------------------------------------------------------------------------------------------------------------------
// quadratic
//--------------------------------------------------------------------------------------------------------------------------------

void sort_insert(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, (void) 0);

    for (size_t it  =  1; it  < size; ++ it) {
    for (size_t ind = it; ind >    0; --ind)
        {
            if (arr[ind] >= arr[ind - 1]) break;
            /* else */ int_swap(arr + ind, arr + ind - 1);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void sort_bubble(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, (void) 0);

    for (size_t it  = 0; it  < size   ; ++ it) { size_t ind_max = size - (it + 1);
    for (size_t ind = 0; ind < ind_max; ++ind)
        {
            if (arr[ind] > arr[ind + 1]) int_swap(arr + ind, arr + ind + 1);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void sort_choose(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, (void) 0);

    for (size_t it  =      0; it  < size - 1; ++ it) {
    for (size_t ind = it + 1; ind < size    ; ++ind)
        {
            if (arr[ind] < arr[it]) int_swap(arr + ind, arr + it);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// quick
//--------------------------------------------------------------------------------------------------------------------------------

static void   quick_sort      (int *const arr, const size_t size, int (*get_pivot) (int *, size_t));
static size_t quick_sort_frame(int *const arr, const size_t size, int (*get_pivot) (int *, size_t));

static int get_pivot_by_median_of_three  (int *const arr, const size_t size);
static int get_pivot_by_central          (int *const arr, const size_t size);
static int get_pivot_by_rand             (int *const arr, const size_t size);
static int get_pivot_by_median_of_medians(int *const arr, const size_t size);
static int kth_element                   (int *const arr, const size_t size, const size_t k);

//--------------------------------------------------------------------------------------------------------------------------------

static void quick_sort(int *const arr, const size_t size, int (*get_pivot) (int *, size_t))
{
    log_assert(arr       != nullptr);
    log_assert(get_pivot != nullptr);

    if (size <= 1) return;

    size_t pivot_ind = quick_sort_frame(arr, size, get_pivot);

    if (pivot_ind == 0) quick_sort(arr + 1, size - 1, get_pivot);
    else
    {
        quick_sort(arr,                    pivot_ind, get_pivot);
        quick_sort(arr + pivot_ind, size - pivot_ind, get_pivot);
    }
}

static size_t quick_sort_frame(int *const arr, const size_t size, int (*get_pivot) (int *, size_t))
{
    log_assert(arr       != nullptr);
    log_assert(get_pivot != nullptr);

    if (size <= 1) return 0;

    int    pivot = (*get_pivot) (arr, size);
    size_t left  = 0;
    size_t right = size - 1;

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

    log_assert_verbose(false, "undefine behaviour\n");
    return -1UL;
}

//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_median_of_three(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, (void) 0);
    quick_sort(arr, size, get_pivot_by_median_of_three);
}

static int get_pivot_by_median_of_three(int *const arr, const size_t size)
{
    log_assert(arr  != nullptr);
    log_assert(size != 0);

    int val_1 = arr[0];
    int val_2 = arr[size - 1];
    int val_3 = arr[size / 2];

    if ((val_1 <= val_2 && val_2 <= val_3) || (val_3 <= val_2 && val_2 <= val_1)) return val_2;
    if ((val_1 <= val_3 && val_3 <= val_2) || (val_2 <= val_3 && val_3 <= val_1)) return val_3;
    return val_1;
}

//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_central(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, (void) 0);
    quick_sort(arr, size, get_pivot_by_central);
}

static int get_pivot_by_central(int *const arr, const size_t size)
{
    log_assert(arr  != nullptr);
    log_assert(size != 0);

    return arr[size / 2];
}

//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_rand(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, (void) 0);
    quick_sort(arr, size, get_pivot_by_rand);
}

static int get_pivot_by_rand(int *const arr, const size_t size)
{
    log_assert(arr  != nullptr);
    log_assert(size != 0);

    return arr[(size_t) rand() % size];
}

//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_median_of_medians(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, (void) 0);
    quick_sort(arr, size, get_pivot_by_median_of_medians);
}

static int get_pivot_by_median_of_medians(int *const arr, const size_t size)
{
    log_assert(arr  != nullptr);
    log_assert(size != 0);

    size_t median_arr_size = (size + 4) / 5;
    int   *median_arr      = (int *) log_calloc(median_arr_size, sizeof(int));
    log_assert(median_arr != nullptr);

    for (size_t i = 0; i < size; i += 5)
    {
        if (i + 5 <= size) { sort_insert(arr + i, 5); median_arr[i / 5] = arr[i + 2]; }
        else               {                          median_arr[i / 5] = arr[i];     }
    }

    int ans = kth_element(median_arr, median_arr_size, median_arr_size / 2);

    log_free(median_arr);
    return ans;
}

static int kth_element(int *const arr, const size_t size, const size_t k)
{
    log_assert(arr != nullptr);
    log_assert(size > k);

    if (size == 1) return arr[0];

    size_t pivot_ind = quick_sort_frame(arr, size, get_pivot_by_median_of_medians);

    if      (pivot_ind == k) return arr[pivot_ind];
    else if (pivot_ind >  k) return kth_element(arr, pivot_ind, k);

    pivot_ind++;
    return kth_element(arr + pivot_ind, size - pivot_ind, k - pivot_ind);

}

//--------------------------------------------------------------------------------------------------------------------------------
// merge
//--------------------------------------------------------------------------------------------------------------------------------

static void merge(int *const arr, const size_t size);

//--------------------------------------------------------------------------------------------------------------------------------

void sort_merge(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, (void) 0);

    if (size <= 1) return;

    size_t mid = size / 2;

    sort_merge(arr      ,        mid);
    sort_merge(arr + mid, size - mid);
    merge     (arr, size);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void merge(int *const arr, const size_t size)
{
    log_assert(arr != nullptr);
    log_assert(size > 1);

    int *const sorted = (int *) log_calloc(size, sizeof(int));
    log_assert(sorted != nullptr);

    size_t l_cnt = 0       , l_max = size / 2;
    size_t r_cnt = size / 2, r_max = size;

    for (size_t ind = 0; ind < size; ++ind)
    {
        if      (l_cnt == l_max) { sorted[ind] = arr[r_cnt]; r_cnt++; }
        else if (r_cnt == r_max) { sorted[ind] = arr[l_cnt]; l_cnt++; }
        else
        {
            if (arr[l_cnt] < arr[r_cnt]) { sorted[ind] = arr[l_cnt]; l_cnt++; }
            else                         { sorted[ind] = arr[r_cnt]; r_cnt++; }
        }
    }

    memcpy(arr, sorted, size * sizeof(int));
    log_free(sorted);
}
