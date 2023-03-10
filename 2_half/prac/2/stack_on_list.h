#ifndef STACK_ON_LIST_H
#define STACK_ON_LIST_H

#include "../../../lib/logs/log.h"
#include "../../../lib/algorithm/algorithm.h"
#include "../../../lib/list/list.h"

//================================================================================================================================
// DSL
//================================================================================================================================

#define $data &stk->data

//================================================================================================================================
// STRUCT
//================================================================================================================================

struct stack_on_list
{
    list data;
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor-dtor
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_on_list_ctor(stack_on_list *const stk, const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                                                        void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

stack_on_list *stack_on_list_new(const size_t el_size,  void (*el_dtor) (void       *const) = nullptr,
                                                        void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

void stack_on_list_dtor(void *const _stk);

//--------------------------------------------------------------------------------------------------------------------------------
// push-pop
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_on_list_push(stack_on_list *const stk, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_on_list_pop(stack_on_list *const stk, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// top
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_on_list_top(const stack_on_list *const stk, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void stack_on_list_dump(const void *const _stk);

#endif //STACK_ON_LIST_H