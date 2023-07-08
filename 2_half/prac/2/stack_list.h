#ifndef STACK_LIST_H
#define STACK_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "../../../lib/list/list.h"

//================================================================================================================================
// STACK BY LIST
//================================================================================================================================

typedef list stack;

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

                stack *stack_new(const size_t el_size);
__always_inline stack *stack_new(const size_t el_size)
{
    return list_new(el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------

                bool stack_ctor(stack *const stk, const size_t el_size);
__always_inline bool stack_ctor(stack *const stk, const size_t el_size)
{
    return list_ctor(stk, el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

                void stack_free(void *const stk);
__always_inline void stack_free(void *const stk)
{
    return list_free(stk);
}

//--------------------------------------------------------------------------------------------------------------------------------

                void stack_dtor(void *const stk);
__always_inline void stack_dtor(void *const stk)
{
    return list_dtor(stk);
}

//--------------------------------------------------------------------------------------------------------------------------------
// query
//--------------------------------------------------------------------------------------------------------------------------------

                bool stack_push(stack *const stk, const void *const data);
__always_inline bool stack_push(stack *const stk, const void *const data)
{
    return list_push_back(stk, data);
}

//--------------------------------------------------------------------------------------------------------------------------------

                bool stack_pop(stack *const stk);
__always_inline bool stack_pop(stack *const stk)
{
    return list_pop_back(stk);
}

//--------------------------------------------------------------------------------------------------------------------------------

                void *stack_front(stack *const stk);
__always_inline void *stack_front(stack *const stk)
{
    return list_back(stk);
}

#endif //STACK_LIST_H
