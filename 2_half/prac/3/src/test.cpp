#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "../../../../lib/logs/log.h"
#include "../../../../lib/algorithm/algorithm.h"

#include "sort.h"
#include "test.h"

//================================================================================================================================
// TEST
//================================================================================================================================

const char *DAT_FILENAME = "../data/data.txt";
const char *GPI_FILENAME = "plot.gpi";
const char *PNG_FILENAME = "../result/plot.png";

const int   TEST_NUM  = 5;          // число тестов для усреднения
const int   TEST_MIN  = 1'000;
const int   TEST_MAX  = 10'000'000;
const int   TEST_STEP = 100'000;

//--------------------------------------------------------------------------------------------------------------------------------
// MAIN
//--------------------------------------------------------------------------------------------------------------------------------

int main()
{
    FILE *const dat_stream = open_data_file();
    if         (dat_stream == nullptr) return 0;

    for (int n = TEST_MIN; n <= TEST_MAX; n += TEST_STEP)
    {
        int *arr_original = gen_test(n);
        if  (arr_original == nullptr)                     { fclose(dat_stream); return 0; }

        if (!run_test_frame(arr_original, n, dat_stream)) { fclose(dat_stream); return 0; }
        log_free(arr_original);
    }

    make_gpi(TEST_MIN, TEST_MAX);

    fclose(dat_stream);
}

//--------------------------------------------------------------------------------------------------------------------------------
// RUN
//--------------------------------------------------------------------------------------------------------------------------------

bool run_test_frame(int *const arr_original, const int n, FILE *const out_stream)
{
    log_assert(arr_original != nullptr);
    log_assert(out_stream   != nullptr);

    int *arr_to_sort = (int *) log_calloc((size_t) n, sizeof(int));
    if  (arr_to_sort == nullptr)
    {
        log_error("log_calloc(n = %lu, sizeof(int) = %lu) returns nullptr\n", n, sizeof(int));
        return false;
    }

    double times[4] = {};

    memcpy(arr_to_sort, arr_original, (size_t) n * sizeof(int));
    times[0] = run_test(arr_to_sort , n, sort_by_median_of_three);

    memcpy(arr_to_sort, arr_original, (size_t) n * sizeof(int));
    times[1] = run_test(arr_to_sort , n, sort_by_central);

    memcpy(arr_to_sort, arr_original, (size_t) n * sizeof(int));
    times[2] = run_test(arr_to_sort,  n, sort_by_rand);

    memcpy(arr_to_sort, arr_original, (size_t) n * sizeof(int));
    times[3] = run_test(arr_to_sort , n, merge_sort);

    log_message(HTML_COLOR_LIME_GREEN "test success: n = %lu\n" HTML_COLOR_CANCEL, n);
    log_free   (arr_to_sort);

    save_data_frame(times, 4, n, out_stream);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

double run_test(int *const arr, const int n, void (*sort) (int *, int))
{
    log_assert(arr  != nullptr);
    log_assert(sort != nullptr);

    time_t work_time = 0;

    for (int i = 0; i < TEST_NUM; ++i)
    {
        time_t start_time  = clock();
        (*sort) (arr, n);
        time_t finish_time = clock();

        work_time += finish_time - start_time;
    }

    return (1000.0 * (double) work_time / (double) TEST_NUM) / CLOCKS_PER_SEC;
}

//--------------------------------------------------------------------------------------------------------------------------------
// TEST_GEN
//--------------------------------------------------------------------------------------------------------------------------------

int *gen_test(const int n)
{
    int *arr = (int *) log_calloc((size_t) n, sizeof(int));
    if  (arr == nullptr)
    {
        log_error("log_calloc(n = %lu, sizeof(int) = %lu) returns nullptr\n", n, sizeof(int));
        return nullptr;
    }

    for (int i = 0; i < n; ++i) { arr[i] = rand(); }

    return arr;
}

//--------------------------------------------------------------------------------------------------------------------------------
// DATA
//--------------------------------------------------------------------------------------------------------------------------------

FILE *open_data_file()
{
    FILE *const dat_stream = fopen(DAT_FILENAME, "w");
    if         (dat_stream == nullptr)
    {
        log_error("can't open \"%s\"\n", DAT_FILENAME);
    }

    return dat_stream;
}

//--------------------------------------------------------------------------------------------------------------------------------

void save_data_frame(const double *const data_frame, const int frame_size, const int elem_num, FILE *const dat_stream)
{
    log_assert(data_frame != nullptr);
    log_assert(dat_stream != nullptr);

    fprintf(dat_stream, "%10d | ", elem_num);

    for (int i = 0; i < frame_size; ++i) { fprintf(dat_stream, "%15.5lf | ", data_frame[i]); }
    putc('\n', dat_stream);
}

//--------------------------------------------------------------------------------------------------------------------------------
// PLOT
//--------------------------------------------------------------------------------------------------------------------------------

bool make_gpi(const int x_min, const int x_max)
{
    FILE *const gpi_stream = fopen(GPI_FILENAME, "w");
    if         (gpi_stream == nullptr)
    {
        log_error("can't open \"%s\"\n", GPI_FILENAME);
        return false;
    }

    fprintf(gpi_stream, "set terminal png\n"
                        "set output \"%s\"\n\n"
                        "set datafile separator \"|\"\n"
                        "set grid\n\n", PNG_FILENAME);

    fprintf(gpi_stream, "set xrange[%d:%d]\n"
                        "set xlabel \"Array size\"\n"
                        "set ylabel \"time, ms\"\n\n"   , x_min, x_max);

    fprintf(gpi_stream, "plot \"%s\" using 1:2 with lines title \"merge\"\n", DAT_FILENAME);

    fclose(gpi_stream);
    return true;
}