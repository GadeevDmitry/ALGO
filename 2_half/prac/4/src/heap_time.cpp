#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LOG_NDEBUG
#include "../../../../lib/logs/log.h"

#include "binary_heap.h"
#include "k_heap.h"

#include "heap_time.h"

static const char *DAT_FILENAME = "../data/data.txt";
static const char *GPI_FILENAME = "plot.gpi";
static const char *PNG_FILENAME = "../result/plot.png";

static const int   TEST_NUM    = 5;        // число тестов для усреднения
static const int   TEST_MIN    =   1'000;
static const int   TEST_MAX    = 100'000;
static const int   TEST_STEP   =   1'000;

static const int   HP_K_PARAM  = 5;        // праметр k k-ичной кучи

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
        if  (arr_original == nullptr)                { fclose(dat_stream); return 0; }

        if (!run_frame(arr_original, n, dat_stream)) { fclose(dat_stream); return 0; }
        log_free(arr_original);
    }

    make_gpi(TEST_MIN, TEST_MAX);

    fclose(dat_stream);
}

//--------------------------------------------------------------------------------------------------------------------------------
// RUN
//--------------------------------------------------------------------------------------------------------------------------------

bool run_frame(int *const arr_original,
               const int  arr_size    , FILE *const out_stream)
{
    log_assert(arr_original != nullptr);
    log_assert(out_stream   != nullptr);

    int *arr_to_sort = (int *) log_calloc((size_t) arr_size, sizeof(int));
    if  (arr_to_sort == nullptr)
    {
        log_error("log_calloc(arr_size = %lu, sizeof(int) = %lu) returns nullptr\n", arr_size, sizeof(int));
        return false;
    }

    double times[2] = {};

    memcpy(arr_to_sort, arr_original, (size_t) arr_size * sizeof(int));
    times[0] = run_bin_heap(arr_to_sort,  arr_size);

    memcpy(arr_to_sort, arr_original, (size_t) arr_size * sizeof(int));
    times[1] = run_k_heap  (arr_to_sort,  arr_size);

    log_message(HTML_COLOR_LIME_GREEN "test success: size = %lu\n" HTML_COLOR_CANCEL, arr_size);
    log_free   (arr_to_sort);

    save_data_frame(times, 2, arr_size, out_stream);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

double run_bin_heap(int *const arr, const int arr_size)
{
    log_assert(arr != nullptr);
    log_assert(arr_size  >  0);

    binary_heap    hp = {};
    bin_heap_ctor(&hp, arr_size);

    time_t work_time = 0;

    for (int i = 0; i < TEST_NUM; ++i)
    {
        time_t start_time  = clock();
        bin_heap_sort(&hp, arr, arr_size);
        time_t finish_time = clock();

        work_time += finish_time - start_time;
    }
    bin_heap_dtor(&hp);

    return (1000.0 * (double) work_time / (double) TEST_NUM) / CLOCKS_PER_SEC;
}

double run_k_heap(int *const arr, const int arr_size)
{
    log_assert(arr != nullptr);
    log_assert(arr_size  >  0);

    k_heap hp = {};
    k_heap_ctor(&hp, HP_K_PARAM, arr_size);

    time_t work_time = 0;

    for (int i = 0; i < TEST_NUM; ++i)
    {
        time_t start_time  = clock();
        k_heap_sort(&hp, arr, arr_size);
        time_t finish_time = clock();

        work_time += finish_time - start_time;
    }
    k_heap_dtor(&hp);

    return (1000.0 * (double) work_time / (double) TEST_NUM) / CLOCKS_PER_SEC;
}

//--------------------------------------------------------------------------------------------------------------------------------
// TEST
//--------------------------------------------------------------------------------------------------------------------------------

void test_heap_sort(int *const arr, const int arr_size, bool is_binary)
{
    log_verify(arr != nullptr, (void) 0);
    log_verify(arr_size > 0,   (void) 0);

    log_message(HTML_COLOR_MEDIUM_BLUE "original:\n" HTML_COLOR_CANCEL);
    log_message(HTML_COLOR_DARK_RED);
    for (int i = 0; i < arr_size; ++i) log_message("%d ", arr[i]);
    log_message(HTML_COLOR_CANCEL "\n");

    if (is_binary)
    {
        binary_heap hp = {};

        bin_heap_ctor(&hp,      arr_size);
        bin_heap_sort(&hp, arr, arr_size);
        bin_heap_dtor(&hp);
    }
    else
    {
        k_heap hp = {};

        k_heap_ctor(&hp, HP_K_PARAM, arr_size);
        k_heap_sort(&hp, arr       , arr_size);
        k_heap_dtor(&hp);
    }

    log_message(HTML_COLOR_MEDIUM_BLUE "sorted:\n" HTML_COLOR_CANCEL);
    log_message(HTML_COLOR_LIME_GREEN);
    for (int i = 0; i < arr_size; ++i) log_message("%d ", arr[i]);
    log_message(HTML_COLOR_CANCEL "\n");

    log_free(arr);
}

int *gen_test(const int n)
{
    int *arr = (int *) log_calloc((size_t) n, sizeof(int));
    if  (arr == nullptr)
    {
        log_error("log_calloc(n = %lu, sizeof(int) = %lu) returns nullptr\n", n, sizeof(int));
        return nullptr;
    }

    for (int i = 0; i < n; ++i) { arr[i] = rand() % 100; }

    return arr;
}

//--------------------------------------------------------------------------------------------------------------------------------
// plot
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
                        "set xlabel \"array size\"\n"
                        "set ylabel \"time, ms\"\n\n"   , x_min, x_max);

    fprintf(gpi_stream, "plot \"%s\" using 1:2 with lines title \"bin\","
                            " \"%s\" using 1:3 with lines title \"k = %d\"\n", DAT_FILENAME, DAT_FILENAME, HP_K_PARAM);

    fclose(gpi_stream);
    return true;
}

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

void save_data_frame(const double *const data_frame, const int frame_size, const int elem_num, FILE *const dat_stream)
{
    log_assert(data_frame != nullptr);
    log_assert(dat_stream != nullptr);

    fprintf(dat_stream, "%10d | ", elem_num);

    for (int i = 0; i < frame_size; ++i) { fprintf(dat_stream, "%15.5lf | ", data_frame[i]); }
    putc('\n', dat_stream);
}
