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

void quick_sort      (int *const arr, const int n);
void quick_sort_frame(int *const arr, const int n, int *const _pivot_ind, const int is_dump);

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

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

#ifdef LOGS
void arr_dump(const int *const arr, const int        n);
#endif
void int_swap(      int *const   a,       int *const b);

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

#ifdef  QUICK_SORT
#define quick_sort_before_frame                                                                         \
                                                                                                        \
        log_tab_message(HTML_COLOR_MEDIUM_BLUE "\n"                                                     \
                        "quick_sort(l = %d, r = %d)\n"                                                  \
                        "{"                                                                             \
                        HTML_COLOR_CANCEL "\n",                                                         \
                        arr      - ARR_BEGIN,                                                           \
                       (arr + n) - ARR_BEGIN);                                                          \
        LOG_TAB++;                                                                                      \
        arr_dump(arr, n);
#else
#define quick_sort_before_frame ;
#endif

#ifdef  QUICK_SORT
#define quick_sort_after_all                                                                            \
                                                                                                        \
        log_tab_message(HTML_COLOR_MEDIUM_BLUE  "\n"                                                    \
                                                "AFTER ALL" HTML_COLOR_CANCEL "\n");                    \
        arr_dump(arr, n);                                                                               \
                                                                                                        \
        LOG_TAB--;                                                                                      \
        log_tab_message(HTML_COLOR_MEDIUM_BLUE "}" HTML_COLOR_CANCEL "\n");
#else
#define quick_sort_after_all ;
#endif

void quick_sort(int *const arr, const int n)
{
    //log_verify(arr != nullptr, ;);

    if (n <= 1) return;

    quick_sort_before_frame

    int pivot_ind = 0;  // pivot_ind after sort
    quick_sort_frame(arr, n, &pivot_ind, 1);

    quick_sort(arr, pivot_ind);
    quick_sort(arr + (pivot_ind + 1), n - (pivot_ind + 1));

    quick_sort_after_all
}

#ifdef  QUICK_SORT_FRAME
#define quick_sort_frame_before_sort                                                                    \
                                                                                                        \
        log_tab_message(HTML_COLOR_MEDIUM_BLUE "\n"                                                     \
                        "quick_sort_frame(l = %d, r = %d)\n"                                            \
                        "{"                                                                             \
                        HTML_COLOR_CANCEL "\n",                                                         \
                        arr      - ARR_BEGIN,                                                           \
                       (arr + n) - ARR_BEGIN);                                                          \
        LOG_TAB++;                                                                                      \
        arr_dump(arr, n);                                                                               \
                                                                                                        \
        log_tab_message("pivot_ind:%d\n", pivot_ind + (arr - ARR_BEGIN));
#else
#define quick_sort_frame_before_sort ;
#endif

#ifdef  QUICK_SORT_FRAME
#define quick_sort_frame_after_sort                                                                     \
                                                                                                        \
        log_tab_message(HTML_COLOR_MEDIUM_BLUE  "\n"                                                    \
                                                "AFTER SORT" HTML_COLOR_CANCEL "\n");                   \
        arr_dump(arr, n);                                                                               \
        log_tab_message("pivot_ind:%d\n", pivot_ind + (arr - ARR_BEGIN));                               \
                                                                                                        \
        LOG_TAB--;                                                                                      \
        log_tab_message(HTML_COLOR_MEDIUM_BLUE "}" HTML_COLOR_CANCEL "\n");
#else
#define quick_sort_frame_after_sort ;
#endif

void quick_sort_frame(int *const arr, const int n, int *const _pivot_ind, const int is_dump)
{
    //log_verify(arr       != nullptr, ;);
    //log_verify(pivot_ind != nullptr, ;);
    //log_verify(n > 0, ;);

    int  pivot_ind = 0;
    if ((pivot_ind = get_pivot_ind(arr, n)) == -1) { /* log_error(...); */ return; }

    if (is_dump) { quick_sort_frame_before_sort }

    int_swap(arr + pivot_ind, arr); // pivot на нулевой индекс
    int lower_ind = 1;              // lower_ind - индекс следующего элемента, меньшего, чем pivot
    for (int i = 1; i < n; ++i)
    {
        if (arr[i] < arr[0]) { int_swap(arr+i, arr+lower_ind); ++lower_ind; }
    //  else                 { ничего не делаем, т.к. большие элементы окажутся левее меньших после цикла }
    }
    pivot_ind = lower_ind - 1;            // теперь pivot_ind - индекс последнего элемента, меньшего  pivot
    int_swap(arr+pivot_ind, arr);         // pivot на lower_ind, теперь все меньшие - слева, большие - справа

    *_pivot_ind = pivot_ind;

    if (is_dump) { quick_sort_frame_after_sort }
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

    int pivot_ind = 0;  //pivot_ind after sort
    quick_sort_frame(arr, n, &pivot_ind, 0);

    if (pivot_ind == k) { *ans = arr[pivot_ind]; return 1; }
    if (pivot_ind >  k)
    {
        if (!kth_element(arr, pivot_ind, k, ans)) return 0;
        return 1;
    }

    ++pivot_ind;

    if (!kth_element(arr + pivot_ind, n - pivot_ind, k - pivot_ind, ans)) return 0;
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
        for (int j = 0; j < n - (i + 1); ++j)
        {
            if (arr[j] > arr[j + 1]) { int_swap(arr + j, arr + (j + 1)); }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

#ifdef LOGS
void arr_dump(const int *const arr, const int n)
{
    int l_ind = (int) (arr - ARR_BEGIN);
    int r_ind = l_ind + n;

    log_tab_message(HTML_COLOR_DARK_ORANGE
                    "INDEX: ");
    for (int ind = l_ind; ind < r_ind; ++ind) { log_message("%4d ", ind); }

    log_message("\n"
                "       ");
    for (int ind = l_ind; ind < r_ind; ++ind) { log_message("%4d ", arr[ind - l_ind]); }

    log_message(HTML_COLOR_CANCEL "\n");
}
#endif

void int_swap(int *const a, int *const b)
{
    //log_verify(a != nullptr, ;);
    //log_verify(b != nullptr, ;);

    int temp = *a;

    *a = *b;
    *b = temp;
}