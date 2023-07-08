#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NDEBUG
#define NVERIFY
#define LOG_NTRACE
#define LOG_NLEAK
#include "../../../lib/logs/log.h"

#define STACK_BY_LIST
#ifndef STACK_BY_LIST
#include "../../../lib/stack/stack.h"
#else
#include "stack_list.h"
#endif

//================================================================================================================================
// GLOBAL
//================================================================================================================================

typedef int type_t;

const size_t STACK_START_SIZE = 1'000'000;
const type_t STACK_ELEM       = 10;

//================================================================================================================================
// TEST
//================================================================================================================================

double run_test(void (*test) (stack *const tested));
void   test_1  (stack *const tested);
void   test_2  (stack *const tested);
void   test_3  (stack *const tested);

void test_prepare      (stack *const tested);
void test_12_main_cycle(stack *const tested);
void test_2_extra_cycle(stack *const tested);
void test_3_cycle      (stack *const tested);

#endif //TEST_H
