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
// solve
//--------------------------------------------------------------------------------------------------------------------------------

int solve(int **const shelf_a,
          int **const shelf_b,

          const int width,

          const int a_i, const int b_i);

int int_max(const int a, const int b);

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

int **double_arr_ctor(const int height, const int length);
void  double_arr_dtor(int **const  arr, const int   size);

//================================================================================================================================
// MAIN
//================================================================================================================================

const int MAX_ANS = 2e9;

int main()
{
    int n = 0, m = 0, l = 0;
    if (scanf("%d %d %d", &n, &m, &l) != 3) { /* log_error(...); */ return 0; }

    int **shelf_a = double_arr_ctor(n, l);
    int **shelf_b = double_arr_ctor(m, l);

    if (shelf_a == NULL) { /* log_error(...); */ double_arr_dtor(shelf_b, m); return 0; }
    if (shelf_b == NULL) { /* log_error(...); */ double_arr_dtor(shelf_a, n); return 0; }

    int q = 0;
    if (scanf("%d", &q) != 1)
    {
        /* log_error(...); */
        double_arr_dtor(shelf_a, n);
        double_arr_dtor(shelf_b, m);

        return 0;
    }

    while (q--)
    {
        int a_i = 0, b_i = 0;
        if (scanf("%d %d", &a_i, &b_i) != 2)
        {
            /* log_error(...); */
            double_arr_dtor(shelf_a, n);
            double_arr_dtor(shelf_b, m);

            return 0;
        }

        --a_i, --b_i;

        int ans = solve(shelf_a, shelf_b, l, a_i, b_i) + 1;
        printf("%d\n", ans);
    }

    double_arr_dtor(shelf_a, n);
    double_arr_dtor(shelf_b, m);
}

//--------------------------------------------------------------------------------------------------------------------------------
// solve
//--------------------------------------------------------------------------------------------------------------------------------

int solve(int **const shelf_a,
          int **const shelf_b,

          const int width,

          const int a_i, const int b_i)
{
    //log_verify(a != nullptr, 0);
    //log_verify(b != nullptr, 0);

    //log_verify(height_a > 0, 0);
    //log_verify(height_b > 0, 0);
    //log_verify(width > 0, 0);

    int l = 0, r = width - 1;

    while (r - l > 1)
    {
        int mid = (l + r) / 2;

        if (shelf_a[a_i][mid] == shelf_b[b_i][mid]) return mid;
        if (shelf_a[a_i][mid] >  shelf_b[b_i][mid]) r = mid;
        else                                        l = mid;
    }

    if (int_max(shelf_a[a_i][l], shelf_b[b_i][l]) > int_max(shelf_a[a_i][r], shelf_b[b_i][r])) return r;
    return l;
}

int int_max(const int a, const int b)
{
    if (a > b) return a;
    return b;
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

int **double_arr_ctor(const int height, const int length)
{
    int **arr = (int **) calloc((size_t) height, sizeof(int *));
    if (arr == NULL) { /* log_error(...); */ return NULL; }

    for (int i = 0; i < height; ++i)
    {
        arr[i] = (int *) calloc((size_t) length, sizeof(int));

        if (arr[i] == NULL)
        {
            /*log_error(...); */
            double_arr_dtor(arr, i);
            return NULL;
        }

        for (int j = 0; j < length; ++j)
        {
            if (scanf("%d", arr[i] + j) != 1)
            {
                /* log_error(...); */
                double_arr_dtor(arr, i + 1);
                return NULL;
            }
        }
    }

    return arr;
}

//--------------------------------------------------------------------------------------------------------------------------------

void double_arr_dtor(int **const arr, const int size)
{
    if (arr == NULL) return;

    for (int i = 0; i < size; ++i) free(arr[i]);
    free(arr);
}