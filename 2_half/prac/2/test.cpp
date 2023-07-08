#include "test.h"

//================================================================================================================================
// MAIN
//================================================================================================================================

int main()
{
    #ifndef STACK_BY_LIST
    printf("stack results:\n");
    #else
    printf("stack by list results:\n");
    #endif

    printf("test_1: %lg ms\n"
           "test_2: %lg ms\n"
           "test_3: %lg ms\n", run_test(test_1),
                               run_test(test_2),
                               run_test(test_3));
}

//================================================================================================================================
// TEST
//================================================================================================================================

double run_test(void (*test) (stack *const tested))
{
    stack *tested = stack_new(sizeof(type_t));
    log_assert(tested != nullptr);

    clock_t start  = clock();
    test(tested);
    clock_t finish = clock();

    stack_free(tested);
    return 1000.0 * (finish - start) / CLOCKS_PER_SEC;
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_1(stack *const tested)
{
    log_assert(tested != nullptr);

    test_prepare      (tested);
    test_12_main_cycle(tested);
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_2(stack *const tested)
{
    log_assert(tested != nullptr);

    test_prepare      (tested);
    test_2_extra_cycle(tested);
    test_12_main_cycle(tested);
    test_2_extra_cycle(tested);
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_3(stack *const tested)
{
    log_assert(tested != nullptr);

    test_prepare(tested);
    test_3_cycle(tested);
}

//================================================================================================================================

void test_prepare(stack *const tested)
{
    log_assert(tested != nullptr);

    for (size_t i = 0; i < STACK_START_SIZE; ++i) stack_push(tested, &STACK_ELEM);
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_12_main_cycle(stack *const tested)
{
    log_assert(tested != nullptr);

    while (tested->size >= STACK_START_SIZE / 10)
    {
        size_t del_num = tested->size / 2;
        size_t add_num = tested->size / 4;

        for (; del_num != 0; del_num--) stack_pop (tested);
        for (; add_num != 0; add_num--) stack_push(tested, &STACK_ELEM);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_2_extra_cycle(stack *const tested)
{
    log_assert(tested != nullptr);
    log_assert(tested->size >= STACK_START_SIZE / 100);

    for (size_t i = 0; i < 100; ++i)
    {
        for (size_t j = 0; j < STACK_START_SIZE / 100; ++j) stack_pop (tested);
        for (size_t j = 0; j < STACK_START_SIZE / 100; ++j) stack_push(tested, &STACK_ELEM);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void test_3_cycle(stack *const tested)
{
    log_assert(tested != nullptr);
    log_assert(tested->size >= STACK_START_SIZE);

    for (size_t i = 0; i < STACK_START_SIZE; ++i)
    {
        int coin = rand();
        if (coin % 2 == 1) stack_push(tested, &STACK_ELEM);
        else               stack_pop (tested);
    }
}
