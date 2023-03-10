#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"
#include "ctype.h"
#include "time.h"

#include "stack_on_list_test.h"

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

    stack_on_list *lst = stack_on_list_new(sizeof(lst_el_type), lst_el_dtor, lst_el_dump);

    clock_t time_start = 0;

    if      (strcmp(argv[1], "1") == 0) { time_start = clock(); test1(lst); }
    else if (strcmp(argv[1], "2") == 0) { time_start = clock(); test2(lst); }
    else if (strcmp(argv[1], "3") == 0) { time_start = clock(); test3(lst); }
    else
    {
        stack_on_list_dtor(lst);
        log_free(lst);

        fprintf(stderr, "argument must e the value between \'1\' and \'3\'\n");
        return 0;
    }
    clock_t time_end = clock();

    stack_on_list_dtor(lst);
    log_free(lst);

    double exec_time = 1000.0 * (time_end - time_start) / CLOCKS_PER_SEC;
    fprintf(stderr, "execute time: %lf ms\n", exec_time);
}

//================================================================================================================================
// el_dtor, el_dump
//================================================================================================================================

void lst_el_dtor(void *const _lst_el)
{
    log_verify(_lst_el != nullptr, ;);
}

void lst_el_dump(const void *const _lst_el)
{
    if (_lst_el == nullptr) { log_error_message(nullptr); return; }

    const lst_el_type *const lst_el = (const lst_el_type *) _lst_el;
    log_tab_message("%d\n", *lst_el);
}

//================================================================================================================================
// TEST
//================================================================================================================================

#define SIZE_INIT 1000000
#define SIZE_MIN  100000
#define EL_VAL    1

void test_ctor(stack_on_list *const lst)
{
    while (lst->data.size < SIZE_INIT)
    {
        //fprintf(stderr, "size: %lu\n", lst->data.size);
        lst_el_type cur_el = EL_VAL;
        stack_on_list_push(lst, &cur_el);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void test12_iter(stack_on_list *const lst)
{
    log_verify(lst != nullptr, ;);

    const size_t beg_size = lst->data.size;
    if (beg_size < SIZE_MIN) return;

    while (2 * lst->data.size > beg_size)
    {
        stack_on_list_pop(lst);
    }

    while (4 * lst->data.size < 3 * beg_size)
    {
        lst_el_type cur_el = EL_VAL;
        stack_on_list_push(lst, &cur_el);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// test1
//--------------------------------------------------------------------------------------------------------------------------------

void test1(stack_on_list *const lst)
{
    log_verify(lst != nullptr, ;);

    test_ctor(lst);

    while (lst->data.size >= SIZE_MIN) test12_iter(lst);
}

//--------------------------------------------------------------------------------------------------------------------------------
// test2
//--------------------------------------------------------------------------------------------------------------------------------

void test2(stack_on_list *const lst)
{
    log_verify(lst != nullptr, ;);

    test_ctor(lst);

    test2_iter(lst);
    while (lst->data.size >= SIZE_MIN) test12_iter(lst);
}

void test2_iter(stack_on_list *const lst)
{
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 10000; ++j)
        {
            stack_on_list_pop(lst);
        }

        for (int j = 0; j < 10000; ++j)
        {
            lst_el_type cur_el = EL_VAL;
            stack_on_list_push(lst, &cur_el);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// test3
//--------------------------------------------------------------------------------------------------------------------------------

void test3(stack_on_list *const lst)
{
    log_verify(lst != nullptr, ;);

    test_ctor (lst);
    test3_iter(lst);
}

void test3_iter(stack_on_list *const lst)
{
    for (int i = 0; i < 1000000; ++i)
    {
        int rand_mode = rand() % 2;
        if (rand_mode == 0)
        {
            stack_on_list_pop(lst);
        }
        else
        {
            lst_el_type cur_el = EL_VAL;
            stack_on_list_push(lst, &cur_el);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// my test
//--------------------------------------------------------------------------------------------------------------------------------

void my_test(stack_on_list *const lst)
{
    log_tab_message("empty list:\n");
    stack_on_list_dump(lst);

    int cur_el = 1234;
    stack_on_list_push(lst, &cur_el);
    stack_on_list_dump(lst);

    cur_el = 345678;
    stack_on_list_push(lst, &cur_el);
    stack_on_list_dump(lst);

    stack_on_list_pop (lst);
    stack_on_list_dump(lst);
}