#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sort.h"

#define NDEBUG
#define NVERIFY
#define LOG_NTRACE
#define LOG_NLEAK
#include "../../../../lib/logs/log.h"
#include "../../../../lib/algorithm/algorithm.h"

//================================================================================================================================
// GLOBAL
//================================================================================================================================

struct sort_func
{
    void (*func) (int *const arr, const size_t size);
    const char *name;
};

#define CSV_SEP ";"

//--------------------------------------------------------------------------------------------------------------------------------
// performance
//--------------------------------------------------------------------------------------------------------------------------------

const size_t SIZE_MIN  =      1'000;
const size_t SIZE_MAX  = 10'000'000;
const size_t SIZE_STEP =    100'000;

const size_t AVERAGED_NUM = 5;

//--------------------------------------------------------------------------------------------------------------------------------
// correctness
//--------------------------------------------------------------------------------------------------------------------------------

const int    MAX_ARR_VALUE    = 100;
const size_t SIZE_CORRECTNESS = 100;

//================================================================================================================================
// FUNCTION
//================================================================================================================================

int *generate_array(                      const size_t size);
int *   clone_array(const int *const arr, const size_t size);

void   test_performance      (FILE *const output, const sort_func *const begin, const sort_func *const end);
double test_performance_frame(void (*sort)(int *const arr, const size_t size),
                                           int *const arr, const size_t size);

void test_correctness         (sort_func sort);
bool is_array_correct         (const int *const arr_raw, const int *const arr_sorted, const size_t size);
void dump_incorrect_array     (const int *const arr_raw, const int *const arr_sorted, const size_t size);
void dump_array               (const int *const arr_raw, const int *const arr_sorted, const size_t size);
void dump_incorrect_count     (const int *const arr_raw, const int *const arr_sorted, const size_t size, size_t *const count);
void dump_count               (const int *const arr_raw, const int *const arr_sorted, const size_t size, size_t *const count);

#endif //TEST_H
