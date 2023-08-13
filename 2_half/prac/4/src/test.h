#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "binary_heap.h"
#include "k_heap.h"
#include "k_optimized_heap.h"

#define NDEBUG
#define NVERIFY
#define LOG_NTRACE
#define LOG_NLEAK
#include "../../../../lib/logs/log.h"
#include "../../../../lib/algorithm/algorithm.h"

//================================================================================================================================

#define CSV_SEP ";"

//#define CORRECTNESS

//#define BINARY
//#define KTH
#define KTH_OPTIMIZED

//================================================================================================================================

const int    MAX_ARR_VALUE  = 100;
const size_t K_HEAP_PARAM[] = {5, 10, 25, 50, 100, 200, 300, 400, 500, 1000};

const size_t SIZE_MIN     =    100'000;
const size_t SIZE_MAX     = 10'000'000;
const size_t SIZE_STEP    =    100'000;
const size_t AVERAGED_NUM = 5;

const size_t SIZE_CORRECTNESS = 100;

//================================================================================================================================

int *generate_array(                      const size_t size);
int *   clone_array(const int *const arr, const size_t size);

//--------------------------------------------------------------------------------------------------------------------------------

void test_performance         (FILE *const output);
void test_performance_header  (FILE *const output);
void test_performance_heap    (FILE *const output, const int *const arr, const size_t size);

void test_performance_bin_heap(FILE *const output,           const int *const arr, const size_t size);
void test_performance_k_heap  (FILE *const output, void (*k_sort) (int *const arr, const size_t size, const size_t k),
                                                             const int *const arr, const size_t size);

//--------------------------------------------------------------------------------------------------------------------------------

void test_bin_heap_correctness();
void test_k_heap_correctness  (void (*k_sort) (int *const arr, const size_t size, const size_t k),
                                                                                  const size_t k);

bool is_array_correct         (const int *const arr_raw, const int *const arr_sorted, const size_t size);
void dump_incorrect_array     (const int *const arr_raw, const int *const arr_sorted, const size_t size);
void dump_array               (const int *const arr_raw, const int *const arr_sorted, const size_t size);
void dump_incorrect_count     (const int *const arr_raw, const int *const arr_sorted, const size_t size, size_t *const count);
void dump_count               (const int *const arr_raw, const int *const arr_sorted, const size_t size, size_t *const count);

#endif //TEST_H
