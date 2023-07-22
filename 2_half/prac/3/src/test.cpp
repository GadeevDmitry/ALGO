#include "test.h"

//================================================================================================================================
// MAIN
//================================================================================================================================

int main()
{
    sort_func sort_arr[] = {
                            {sort_insert, "sort_insert"},                               // 0
                            {sort_bubble, "sort_bubble"},
                            {sort_choose, "sort_choose"},

                            {sort_by_median_of_three  , "sort_by_median_of_three  "},   // 3
                            {sort_by_central          , "sort_by_central          "},
                            {sort_by_rand             , "sort_by_rand             "},
                            {sort_by_median_of_medians, "sort_by_median_of_medians"},

                            {sort_merge               , "sort_merge               "},   // 7
                           };                                                           // 8

    #ifdef CORRECTNESS
    for (size_t it = 0; it * sizeof(sort_func) < sizeof(sort_arr); ++it)
        test_correctness(sort_arr[it]);
    #else
    FILE *output = fopen("../data/merge.csv", "w");
    test_performance(output, sort_arr + 7, sort_arr + 8);
    fclose(output);
    #endif
}

//================================================================================================================================

int *generate_array(const size_t size)
{
    int *arr = (int *) log_calloc(size, sizeof(int));
    log_assert(arr != nullptr);

    for (size_t ind = 0; ind < size; ++ind) arr[ind] = rand() % 100;
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

void test_performance(FILE *const output, const sort_func *const begin,
                                          const sort_func *const end)
{
    log_assert(output != nullptr);
    log_assert(begin  != nullptr);
    log_assert(end    != nullptr);

    fprintf(output, "size" CSV_SEP " ");
    for (const sort_func *cur = begin; cur < end; ++cur)
    {
        log_assert(cur->func != nullptr);
        log_assert(cur->name != nullptr);

        fprintf(output, "%s" CSV_SEP " ", cur->name);
    }
    putc('\n', output);

    for (size_t size = SIZE_MIN; size <= SIZE_MAX; size += SIZE_STEP)
    {
        const int *const arr_raw  = generate_array(         size);
              int *const arr_sort =    clone_array(arr_raw, size);

        fprintf(output, "%lu" CSV_SEP " ", size);
        for (const sort_func *cur = begin; cur < end; ++cur)
        {
            double time = test_performance_frame(cur->func, arr_sort, size);
            memcpy(arr_sort, arr_raw, size * sizeof(int));

            fprintf(output, "%lf" CSV_SEP " ", time);
        }
        putc('\n', output);

        log_free((int *) arr_raw );
        log_free(        arr_sort);
        log_tab_ok_message("PERFORMANCE TEST IS OK: size = %lu", "\n", size);
    }
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

double test_performance_frame(void (*sort) (int *const arr, const size_t size),
                                            int *const arr, const size_t size)
{
    log_assert(sort != nullptr);
    log_assert(arr  != nullptr);

    double time = 0.0;

    for (size_t it = 0; it < AVERAGED_NUM; ++it)
    {
        clock_t start  = clock();
        sort(arr, size);
        clock_t finish = clock();

        time += 1000.0 * (finish - start) / CLOCKS_PER_SEC;
    }

    return time / (double) AVERAGED_NUM;
}

//--------------------------------------------------------------------------------------------------------------------------------
// correctness
//--------------------------------------------------------------------------------------------------------------------------------

void test_correctness(sort_func sort)
{
    log_assert(sort.func != nullptr);
    log_assert(sort.name != nullptr);

    log_tab_service_message("CHECKING \"%s\" FOR CORRECTNESS\n{", "\n", sort.name);
    LOG_TAB++;

    bool is_sorted = true;
    for (size_t i = 0; i < 100; ++i)
    {
        int *arr_raw    = generate_array(         SIZE_CORRECTNESS);
        int *arr_sorted =    clone_array(arr_raw, SIZE_CORRECTNESS);
        
        sort.func(arr_sorted, SIZE_CORRECTNESS);
        is_sorted = is_array_correct(arr_raw, arr_sorted, SIZE_CORRECTNESS);

        log_free(arr_raw);
        log_free(arr_sorted);

        if (!is_sorted) break;
    }

    if (is_sorted) log_tab_ok_message("CHECKING SUCCESS", "\n");

    LOG_TAB--;
    log_tab_service_message("}", "\n");
}

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
