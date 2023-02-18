#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "stack_test.h"

//================================================================================================================================
// MAIN
//================================================================================================================================

int main(const int argc, const char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "you should print number of the test to run\n");
        return 0;
    }
    if (argc >= 3)
    {
        fprintf(stderr, "you should print only one argument - number of the test to run\n");
        return 0;
    }

    stack *stk = stack_new(sizeof(stk_el_type), nullptr, nullptr);

    clock_t time_start = 0;

    if      (strcmp(argv[1], "1") == 0) { time_start = clock(); test1(stk); }
    else if (strcmp(argv[1], "2") == 0) { time_start = clock(); test2(stk); }
    else if (strcmp(argv[1], "3") == 0) { time_start = clock(); test3(stk); }
    else
    {
        stack_dtor(stk);
        log_free(stk);

        fprintf(stderr, "argument must e the value between \'1\' and \'3\'\n");
        return 0;
    }
    clock_t time_end = clock();

    stack_dtor(stk);
    log_free(stk);

    double exec_time = 1000.0 * (time_end - time_start) / CLOCKS_PER_SEC;
    fprintf(stderr, "execute time: %lf ms\n", exec_time);
}

//================================================================================================================================
// TEST
//================================================================================================================================

#define SIZE_INIT 1000000
#define SIZE_MIN  100000
#define EL_VAL    1

void test_ctor(stack *const stk)
{
    while (stk->size < SIZE_INIT)
    {
        //fprintf(stderr, "size: %lu\n", lst->data.size);
        stk_el_type cur_el = EL_VAL;
        stack_push(stk, &cur_el);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void test12_iter(stack *const stk)
{
    log_verify(stk != nullptr, ;);

    const size_t beg_size = stk->size;
    if (beg_size < SIZE_MIN) return;

    while (2 * stk->size > beg_size)
    {
        stack_pop(stk);
    }

    while (4 * stk->size < 3 * beg_size)
    {
        stk_el_type cur_el = EL_VAL;
        stack_push(stk, &cur_el);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// test1
//--------------------------------------------------------------------------------------------------------------------------------

void test1(stack *const stk)
{
    log_verify(stk != nullptr, ;);

    test_ctor(stk);

    while (stk->size >= SIZE_MIN) test12_iter(stk);
}

//--------------------------------------------------------------------------------------------------------------------------------
// test2
//--------------------------------------------------------------------------------------------------------------------------------

void test2(stack *const stk)
{
    log_verify(stk != nullptr, ;);

    test_ctor(stk);

    test2_iter(stk);
    while (stk->size >= SIZE_MIN) test12_iter(stk);
}

void test2_iter(stack *const stk)
{
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 10000; ++j)
        {
            stack_pop(stk);
        }

        for (int j = 0; j < 10000; ++j)
        {
            stk_el_type cur_el = EL_VAL;
            stack_push(stk, &cur_el);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// test3
//--------------------------------------------------------------------------------------------------------------------------------

void test3(stack *const stk)
{
    log_verify(stk != nullptr, ;);

    test_ctor (stk);
    test3_iter(stk);
}

void test3_iter(stack *const stk)
{
    for (int i = 0; i < 1000000; ++i)
    {
        int rand_mode = rand() % 2;
        if (rand_mode == 0)
        {
            stack_pop(stk);
        }
        else
        {
            stk_el_type cur_el = EL_VAL;
            stack_push(stk, &cur_el);
        }
    }
}
