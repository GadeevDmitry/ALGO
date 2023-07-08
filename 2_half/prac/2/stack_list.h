#ifndef STACK_LIST_H
#define STACK_LIST_H

#include <stdio.h>
#include <stdlib.h>

//#define CACHE_LIST
#ifndef CACHE_LIST
#include "../../../lib/list/list.h"
#else
#include "../../../lib/cache_friendly_list/cache_list.h"
typedef cache_list list;
#endif

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
    #ifndef CACHE_LIST
    return list_new(el_size);
    #else
    return cache_list_new(el_size);
    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------

                bool stack_ctor(stack *const stk, const size_t el_size);
__always_inline bool stack_ctor(stack *const stk, const size_t el_size)
{
    #ifndef CACHE_LIST
    return list_ctor(stk, el_size);
    #else
    return cache_list_ctor(stk, el_size);
    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

                void stack_free(void *const stk);
__always_inline void stack_free(void *const stk)
{
    #ifndef CACHE_LIST
    return list_free(stk);
    #else
    return cache_list_free(stk);
    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------

                void stack_dtor(void *const stk);
__always_inline void stack_dtor(void *const stk)
{
    #ifndef CACHE_LIST
    return list_dtor(stk);
    #else
    return cache_list_dtor(stk);
    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------
// query
//--------------------------------------------------------------------------------------------------------------------------------

                bool stack_push(stack *const stk, const void *const data);
__always_inline bool stack_push(stack *const stk, const void *const data)
{
    #ifndef CACHE_LIST
    return list_push_back(stk, data);
    #else
    return cache_list_push_back(stk, data);
    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------

                bool stack_pop(stack *const stk);
__always_inline bool stack_pop(stack *const stk)
{
    #ifndef CACHE_LIST
    return list_pop_back(stk);
    #else
    return cache_list_pop_back(stk);
    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------

                void *stack_front(stack *const stk);
__always_inline void *stack_front(stack *const stk)
{
    #ifndef CACHE_LIST
    return list_back(stk);
    #else
    return cache_list_back(stk);
    #endif
}

#endif //STACK_LIST_H
