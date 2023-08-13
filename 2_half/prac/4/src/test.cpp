#include "test.h"

//================================================================================================================================

int main()
{
    #ifdef CORRECTNESS

    test_bin_heap_correctness();

    for (size_t k_ind = 0; k_ind * sizeof(size_t) < sizeof(K_HEAP_PARAM); ++k_ind)
    {
        test_k_heap_correctness(k_heap_sort            , K_HEAP_PARAM[k_ind]);
        test_k_heap_correctness(kth_optimized_heap_sort, K_HEAP_PARAM[k_ind]);
    }

    return 0;
    #endif

    FILE *output = fopen("../data/binary.csv", "w");
    log_assert(output != nullptr);

    fprintf(output, "size" CSV_SEP " ");
    test_performance(output);
    fclose (output);
}

//================================================================================================================================

int *generate_array(const size_t size)
{
    int *arr = (int *) log_calloc(size, sizeof(int));
    log_assert(arr != nullptr);

    for (size_t ind = 0; ind < size; ++ind) arr[ind] = rand() % MAX_ARR_VALUE;
    return arr;
}

//--------------------------------------------------------------------------------------------------------------------------------

int *clone_array(const int *const arr, const size_t size)
{
    log_assert(arr != nullptr);

    int *clone = (int *) log_calloc(size, sizeof(int));
    log_assert(clone != nullptr);

    memcpy(clone, arr, size * sizeof(int));
    return clone;
}

//--------------------------------------------------------------------------------------------------------------------------------
// performance
//--------------------------------------------------------------------------------------------------------------------------------

void test_performance(FILE *const output)
{
    log_verify(output != nullptr, (void) 0);

    test_performance_header(output);

    for (size_t size = SIZE_MIN; size <= SIZE_MAX; size += SIZE_STEP)
    {
        int *arr = generate_array(size);
        log_assert(arr != nullptr);

        fprintf(output, "%lu" CSV_SEP " ", size);
        test_performance_heap(output, arr, size);

        log_free(arr);
        log_tab_ok_message("PERFORMANCE TEST IS OK: size = %lu", "\n", size);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void test_performance_header(FILE *const output)
{
    log_assert(output != nullptr);

    #if defined(BINARY)

    fprintf(output, "binary" CSV_SEP "\n");

    #elif defined(KTH)

    for (size_t k_ind = 0; k_ind * sizeof(size_t) < sizeof(K_HEAP_PARAM); ++k_ind)
        fprintf(output, "%lu_heap" CSV_SEP " ", K_HEAP_PARAM[k_ind]);
    putc('\n', output);

    #elif defined(KTH_OPTIMIZED)

    for (size_t k_ind = 0; k_ind * sizeof(size_t) < sizeof(K_HEAP_PARAM); ++k_ind)
        fprintf(output, "%lu_optimized_heap" CSV_SEP " ", K_HEAP_PARAM[k_ind]);
    putc('\n', output);

    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_performance_heap(FILE *const output, const int *const arr, const size_t size)
{
    log_assert(output != nullptr);
    log_assert(arr    != nullptr);

    #if defined(BINARY)
    test_performance_bin_heap(output, arr, size);
    #elif defined(KTH)
    test_performance_k_heap(output, k_heap_sort, arr, size);
    #elif defined(KTH_OPTIMIZED)
    test_performance_k_heap(output, kth_optimized_heap_sort, arr, size);
    #endif

    putc('\n', output);
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

#define run_performance(time, sort, arr_raw, arr_sort, size, ...)                                                               \
        for (size_t it = 0; it < AVERAGED_NUM; ++it)                                                                            \
        {                                                                                                                       \
            memcpy(arr_sort, arr_raw, size);                                                                                    \
                                                                                                                                \
            clock_t start  = clock();                                                                                           \
            sort(arr_sort, size, ##__VA_ARGS__);                                                                                \
            clock_t finish = clock();                                                                                           \
                                                                                                                                \
            time += 1000.0 * (double) (finish - start) / CLOCKS_PER_SEC;                                                        \
        }                                                                                                                       \
        time /= (double) AVERAGED_NUM;

//--------------------------------------------------------------------------------------------------------------------------------

void test_performance_bin_heap(FILE *const output, const int *const arr, const size_t size)
{
    log_assert(output   != nullptr);
    log_assert(arr      != nullptr);

    int *arr_sort = clone_array(arr, size);
    log_verify(arr_sort != nullptr, (void) 0);

    double time = 0.0;
    run_performance(time, bin_heap_sort, arr, arr_sort, size)

    fprintf (output, "%lf" CSV_SEP " ", time);
    log_free(arr_sort);
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_performance_k_heap(FILE *const output, void (*k_sort) (int *const arr, const size_t size, const size_t k),
                                                           const int *const arr, const size_t size)
{
    log_assert(output != nullptr);
    log_assert(k_sort != nullptr);
    log_assert(arr    != nullptr);

    int *arr_sort = clone_array(arr, size);
    log_verify(arr_sort != nullptr, (void) 0);

    for (size_t k_ind = 0; k_ind * sizeof(size_t) < sizeof(K_HEAP_PARAM); ++k_ind)
    {
        memcpy(arr_sort, arr, size);

        double time = 0.0;
        run_performance(time, k_sort, arr, arr_sort, size, K_HEAP_PARAM[k_ind])

        fprintf(output, "%lf" CSV_SEP " ", time);
    }

    log_free(arr_sort);
}

//--------------------------------------------------------------------------------------------------------------------------------

#undef run_performance

//--------------------------------------------------------------------------------------------------------------------------------
// correctness
//--------------------------------------------------------------------------------------------------------------------------------

#define run_correctness(sort, ...)                                                                                              \
{                                                                                                                               \
        bool is_sorted = true;                                                                                                  \
        for (size_t i = 0; i < 100; ++i)                                                                                        \
        {                                                                                                                       \
            int *arr_raw    = generate_array(         SIZE_CORRECTNESS);                                                        \
            int *arr_sorted =    clone_array(arr_raw, SIZE_CORRECTNESS);                                                        \
                                                                                                                                \
            sort(arr_sorted, SIZE_CORRECTNESS, ##__VA_ARGS__);                                                                  \
            is_sorted = is_array_correct(arr_raw, arr_sorted, SIZE_CORRECTNESS);                                                \
                                                                                                                                \
            log_free(arr_raw);                                                                                                  \
            log_free(arr_sorted);                                                                                               \
                                                                                                                                \
            if (!is_sorted) break;                                                                                              \
        }                                                                                                                       \
                                                                                                                                \
        if (is_sorted) log_tab_ok_message("CHECKING SUCCESS", "\n");                                                            \
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_bin_heap_correctness()
{
    log_tab_service_message("CHECKING \"bin_heap_sort\" FOR CORRECTNESS\n{", "\n");
    LOG_TAB++;

    run_correctness(bin_heap_sort)

    LOG_TAB--;
    log_tab_service_message("}", "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_k_heap_correctness(void (*k_sort) (int *const arr, const size_t size, const size_t k),
                                                                                const size_t k)
{
    log_assert(k_sort != nullptr);

    log_tab_service_message("CHECKING \"k = %lu\" FOR CORRECTNESS\n{", "\n");
    LOG_TAB++;

    run_correctness(k_sort, k)

    LOG_TAB--;
    log_tab_service_message("}", "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

#undef run_correctness

//--------------------------------------------------------------------------------------------------------------------------------

bool is_array_correct(const int *const arr_raw,
                      const int *const arr_sorted, const size_t size)
{
    log_assert(arr_raw    != nullptr);
    log_assert(arr_sorted != nullptr);

    size_t        count[MAX_ARR_VALUE] = {};
    size_t source_count[MAX_ARR_VALUE] = {};

    for (size_t ind = 0; ind < size; ++ind) count[arr_raw[ind]]++;
    memcpy(source_count, count, MAX_ARR_VALUE * sizeof(size_t));

    for (size_t ind = 0; ind < size; ++ind)
    {
        if (ind != 0 && arr_sorted[ind] < arr_sorted[ind - 1]) { dump_incorrect_array(arr_raw, arr_sorted, size);               return false; }
        if (count[arr_sorted[ind]] == 0UL)                     { dump_incorrect_count(arr_raw, arr_sorted, size, source_count); return false; }

        count[arr_sorted[ind]]--;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void dump_incorrect_array(const int *const arr_raw,
                          const int *const arr_sorted, const size_t size)
{
    log_assert(arr_raw    != nullptr);
    log_assert(arr_sorted != nullptr);

    log_tab_error_message("SORTING failed:", "\n");
    dump_array(arr_raw, arr_sorted, size);
}

//--------------------------------------------------------------------------------------------------------------------------------

void dump_array(const int *const arr_raw,
                const int *const arr_sorted, const size_t size)
{
    log_tab_service_message("source array:", "");

    for (size_t ind = 0; ind < size; ++ind) log_message(" %5d", arr_raw[ind]);

    log_tab_service_message("\n"
                            "sorted array:", "");

    for (size_t ind = 0; ind < size; ++ind)
    {
        if (ind == 0 || arr_sorted[ind] >= arr_sorted[ind - 1]) log_ok_message   (" %5d", "", arr_sorted[ind]);
        else                                                    log_error_message(" %5d", "", arr_sorted[ind]);
    }

    log_tab_service_message("", "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void dump_incorrect_count(const int *const arr_raw,
                          const int *const arr_sorted, const size_t size, size_t *const count)
{
    log_assert(arr_raw    != nullptr);
    log_assert(arr_sorted != nullptr);
    log_assert(count      != nullptr);

    log_tab_error_message("COUNTING failed:", "\n");
    dump_count(arr_raw, arr_sorted, size, count);
}

//--------------------------------------------------------------------------------------------------------------------------------

void dump_count(const int *const arr_raw,
                const int *const arr_sorted, const size_t size, size_t *const count)
{
    log_assert(arr_raw    != nullptr);
    log_assert(arr_sorted != nullptr);
    log_assert(count      != nullptr);

    log_tab_service_message("source array:", "");

    for (size_t ind = 0; ind < size; ++ind) log_message(" %5d", arr_raw[ind]);

    log_tab_service_message("\n"
                            "sorted array:", "");

    for (size_t ind = 0; ind < size; ++ind)
    {
        if (count[arr_sorted[ind]] == 0UL) log_error_message(" %5d", "", arr_sorted[ind]);
        else { count[arr_sorted[ind]]--;   log_ok_message   (" %5d", "", arr_sorted[ind]); }
    }

    log_tab_service_message("", "\n");
}
