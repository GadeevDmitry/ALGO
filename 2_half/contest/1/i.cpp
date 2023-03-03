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
// lsd_sort
//--------------------------------------------------------------------------------------------------------------------------------

void lsd_sort  (unsigned long long *const arr, const int size);
void count_sort(unsigned long long *const arr, const int size, const int bit);

//================================================================================================================================
// MAIN
//================================================================================================================================

int main()
{
    int size = 0;
    if (scanf("%d", &size) != 1) { /* log_error(...); */ return 0; }

    unsigned long long *arr = (unsigned long long *) calloc((size_t) size, sizeof(long long));
    if (arr == NULL) { /* log_error(...); */ return 0; }

    for (int i = 0; i < size; ++i)
    {
        if (scanf("%llu", arr+i) != 1) { /* log_error(...); */ return 0; }
    }

    lsd_sort(arr, size);
    for (int i = 0; i < size; ++i) { printf("%llu\n", arr[i]); }

    free(arr);
}

//--------------------------------------------------------------------------------------------------------------------------------
// lsd_sort
//--------------------------------------------------------------------------------------------------------------------------------

void lsd_sort(unsigned long long *const arr, const int size)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(size > 0,       ;);

    for (int bit = 0; bit < 64; ++bit)
    {
        count_sort(arr, size, bit);
    }
}

void count_sort(unsigned long long *const arr, const int size, const int bit)
{
    //log_verify(arr != nullptr, ;);
    //log_verify(size > 0,       ;);
    //log_verify(bit  > 0,       ;);

    int   set_bit_num = 0;
    int reset_bit_num = 0;

    for (int i = 0; i < size; ++i)
    {
        if (arr[i] & (1ull << bit))   set_bit_num++;
        else                        reset_bit_num++;
    }

    int   set_bit_ind = reset_bit_num;
    int reset_bit_ind = 0;

    unsigned long long *sorted = (unsigned long long *) calloc((size_t) size, sizeof(long long));
    if (sorted == NULL)
    {
        //log_error()
        return;
    }

    for (int i = 0; i < size; ++i)
    {
        if (arr[i] & (1ull << bit)) { sorted[  set_bit_ind] = arr[i];   set_bit_ind++; }
        else                        { sorted[reset_bit_ind] = arr[i]; reset_bit_ind++; }
    }

    memcpy(arr, sorted, (size_t) size * sizeof(long long));
    free(sorted);
}