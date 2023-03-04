#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

int arr_init(int *const arr, const int n, const int a0, const int a1);

//--------------------------------------------------------------------------------------------------------------------------------
// kth_element
//--------------------------------------------------------------------------------------------------------------------------------

int kth_element  (int *const arr, const int n, const int k, int *const ans);
int get_pivot_ind(int *const arr, const int n);

//--------------------------------------------------------------------------------------------------------------------------------
// median arr
//--------------------------------------------------------------------------------------------------------------------------------

void get_median_arr(int *const arr, const int n, int *const median_arr);
void bubble_sort   (int *const arr, const int n);
void int_swap      (int *const a  , int *const b);

//================================================================================================================================
// MAIN
//================================================================================================================================

int main()
{
    int n = 0, k = 0, a0 = 0, a1 = 0;
    if (scanf("%d %d %d %d", &n, &k, &a0, &a1) != 4) { /* log_error(...); */ return 0; }
    --k;

    int *arr = (int *) calloc((size_t) n, sizeof(int));
    if (arr == NULL)               { /* log_error(...); */ return 0; }
    if (!arr_init(arr, n, a0, a1)) { /* log_error(...); */ return 0; }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //for (int i = 0; i < n; ++i) { log_message("%7d ",      i); }
    //log_message("\n");
    //for (int i = 0; i < n; ++i) { log_message("%7d ", arr[i]); }
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    int ans = 0;
    if (!kth_element(arr, n, k, &ans)) { /* log_error(...); */ return 0; }
    printf("%d\n", ans);

    free(arr);
}

int arr_init(int *const arr, const int n, const int a0, const int a1)
{
    //log_verify(arr != nullptr, 0);
    //log_verify(n > 1, 0);

    arr[0] = a0;
    arr[1] = a1;

    for (int i = 2; i < n; ++i)
    {
        long long arr_im1 = arr[i - 1];
        long long arr_im2 = arr[i - 2];

        long long arr_i = (123*arr_im1 + 45*arr_im2) % (10000000 + 4321);
        arr[i] = (int) arr_i;
    }

    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------
// kth_element
//--------------------------------------------------------------------------------------------------------------------------------

int kth_element(int *const arr, const int n, const int k, int *const ans)
{
    //log_verify(arr != nullptr, -1);
    //log_verify(k > 0, -1);
    //log_verify(n > k, -1);

    if (n == 1) { *ans = arr[0]; return 1; }

    int  pivot_ind = 0;
    if ((pivot_ind = get_pivot_ind(arr, n)) == -1) return 0;

    int_swap(arr + pivot_ind, arr); // pivot на нулевой индекс
    int lower_ind = 1;              // lower_ind - индекс следующего элемента, меньшего, чем pivot
    for (int i = 1; i < n; ++i)
    {
        if (arr[i] < arr[0]) { int_swap(arr+i, arr+lower_ind); ++lower_ind; }
    //  else                 { ничего не делаем, т.к. большие элементы окажутся левее меньших после цикла }
    }
    lower_ind--;                          // теперь lower_ind - индекс последнего элемента, меньшего  pivot
    int_swap(arr+lower_ind, arr);         // pivot на lower_ind, теперь все меньшие - слева, большие - справа

    if (lower_ind == k) { *ans = arr[lower_ind]; return 1; }
    if (lower_ind >  k)
    {
        if (!kth_element(arr, lower_ind, k, ans)) return 0;
        return 1;
    }
    if (!kth_element(arr + (lower_ind+1), n - (lower_ind+1), k - (lower_ind+1), ans)) return 0;
    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

int get_pivot_ind(int *const arr, const int n)
{
    //log_verify(arr       != nullptr, -1);
    //log_verify(pivot_ind != nullptr, -1);
    //log_verify(k > 0, -1);
    //log_verify(n > k, -1);

    int  median_size = (n + 4) / 5;
    int *median_arr  = (int *) calloc((size_t) median_size, sizeof(int));

    if (median_arr == NULL) { /* log_error(...); */ return -1; }
    get_median_arr(arr, n, median_arr);

    int ans = 0;
    if (!kth_element(median_arr, median_size, median_size / 2, &ans)) { free(median_arr); return 0; }

    for (int i = 0; i < n; ++i)
    {
        if (arr[i] == ans) { ans = i; break; }
    }

    free(median_arr);
    return ans;
}

//--------------------------------------------------------------------------------------------------------------------------------
// median arr
//--------------------------------------------------------------------------------------------------------------------------------

void get_median_arr(int *const arr, const int n, int *const median_arr)
{
    //log_verify(arr        != nullptr, ;);
    //log_verify(median_arr != nullptr, ;);
    //log_verify(n > 0, ;);

    for (int i = 0; i < n; i += 5)
    {
        if (i + 5 <= n) { bubble_sort(arr + i, 5); median_arr[i / 5] = arr[i + 2]; }
        else            {                          median_arr[i / 5] = arr[i];     }
    }
}

void bubble_sort(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < i - 1; ++j)
        {
            if (arr[j] > arr[j + 1]) { int_swap(arr + j, arr + (j + 1)); }
        }
    }
}

void int_swap(int *const a, int *const b)
{
    //log_verify(a != nullptr, ;);
    //log_verify(b != nullptr, ;);

    int temp = *a;

    *a = *b;
    *b = temp;
}