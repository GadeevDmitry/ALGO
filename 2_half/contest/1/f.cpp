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
// solve_task
//--------------------------------------------------------------------------------------------------------------------------------

void solve_task(const int n, const int *const arr);

//--------------------------------------------------------------------------------------------------------------------------------
// solve_query
//--------------------------------------------------------------------------------------------------------------------------------

int solve_query(const int *const pref_min, const int *const suff_min, int l, int r);

//--------------------------------------------------------------------------------------------------------------------------------
// set_min
//--------------------------------------------------------------------------------------------------------------------------------

void set_min(const int n, const int *const arr, int *const pref_min, int *const suff_min);
int  get_min(const int a, const int b);

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

    int *arr = (int *) calloc((size_t) n, sizeof(int));
    if (arr == NULL)
    {
        //log_error("log_calloc((size_t) n = %lu, sizeof(int) = %lu) returns nullptr\n", n, sizeof(int));
        return 0;
    }

    for (int i = 0; i < n; ++i)
    {
        if (scanf("%d", arr+i) != 1)
        {
            //log_error("i = %d: expected integer input\n", i);
            return 0;
        }
    }

    solve_task(n, arr);
    free(arr);
}

//--------------------------------------------------------------------------------------------------------------------------------
// solve_task
//--------------------------------------------------------------------------------------------------------------------------------

void solve_task(const int n, const int *const arr)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(n > 0, ;);

    int *pref_min = (int *) calloc((size_t) n, sizeof(int));
    if (pref_min == NULL)
    {
        //log_error("log_calloc((size_t) n = %lu, sizeof(int) = %lu) returns nullptr\n", n, sizeof(int));
        return;
    }

    int *suff_min = (int *) calloc((size_t) n, sizeof(int));
    if (suff_min == NULL)
    {
        //log_error("log_calloc((size_t) n = %lu, sizeof(int) = %lu) returns nullptr\n", n, sizeof(int));
        free(pref_min);
        return;
    }

    set_min(n, arr, pref_min, suff_min);

    int q = 0;
    if (scanf("%d", &q) != 1)
    {
        //log_error("Expected integer input\n");
        free(pref_min);
        free(suff_min);
        return;
    }

    while (q--)
    {
        int l = 0;
        int r = 0;
        if (scanf("%d %d", &l, &r) != 2)
        {
            //log_error("Expected two integers input\n");
            free(pref_min);
            free(suff_min);
            return;
        }

        printf("%d\n", solve_query(pref_min, suff_min, l, r));
    }

    free(pref_min);
    free(suff_min);
}

//--------------------------------------------------------------------------------------------------------------------------------
// solve_query
//--------------------------------------------------------------------------------------------------------------------------------

int solve_query(const int *const pref_min, const int *const suff_min, int l, int r)
{
    //log_verify(pref_min != nullptr, 0);
    //log_verify(suff_min != nullptr, 0);
    //log_verify(n > 0,  0);
    //log_verify(l > 0,  0);
    //log_verify(r > 0,  0);
    //log_verify(l <= n, 0);
    //log_verify(r <= n, 0);
    //log_verify(l <= r, 0);

    l--;
    r--;

    return get_min(pref_min[l], suff_min[r]);
}

//--------------------------------------------------------------------------------------------------------------------------------
// set_min
//--------------------------------------------------------------------------------------------------------------------------------

void set_min(const int n, const int *const arr, int *const pref_min, int *const suff_min)
{
    //log_verify(arr      != nullptr, ;);
    //log_verify(pref_min != nullptr, ;);
    //log_verify(suff_min != nullptr, ;);
    //log_verify(n > 0, ;);

    pref_min[0] = arr[0];
    for (int i = 1; i < n; ++i)
    {
        pref_min[i] = get_min(pref_min[i - 1], arr[i]);
    }

    suff_min[n - 1] = arr[n - 1];
    for (int i = n - 2; i >= 0; --i)
    {
        suff_min[i] = get_min(suff_min[i + 1], arr[i]);
    }
}

int get_min(const int a, const int b)
{
    if (a < b) return a;
    return b;
}