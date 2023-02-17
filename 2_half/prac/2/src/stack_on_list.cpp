#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "ctype.h"
#include "string.h"
#include "assert.h"

#include "stack_on_list.h"

//--------------------------------------------------------------------------------------------------------------------------------
// ctor-dtor
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_on_list_ctor(stack_on_list *const stk, const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                                                        void (*el_dump) (const void *const) /* = nullptr */)
{
    log_verify(stk != nullptr, false);

    return list_ctor($data, el_size, el_dtor, el_dump);
}

//--------------------------------------------------------------------------------------------------------------------------------

stack_on_list *stack_on_list_new(const size_t el_size,  void (*el_dtor) (void       *const) /* = nullptr */,
                                                        void (*el_dump) (const void *const) /* = nullptr */)
{
    stack_on_list *stk = (stack_on_list *) log_calloc(1, sizeof(stack_on_list));
    if (stk == nullptr)
    {
        log_error("log_calloc(1, sizeof(stack_on_list) = %lu) returns nullptr\n", sizeof(stack_on_list));
        return nullptr;
    }
    if (!stack_on_list_ctor(stk, el_size, el_dtor, el_dump)) { log_free(stk); return nullptr; }

    return stk;
}

//--------------------------------------------------------------------------------------------------------------------------------

void stack_on_list_dtor(void *const _stk)
{
    log_verify(_stk != nullptr, ;);

    stack_on_list *const stk = (stack_on_list *) _stk;
    list_dtor($data);
}

//--------------------------------------------------------------------------------------------------------------------------------
// push-pop
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_on_list_push(stack_on_list *const stk, const void *const data)
{
    log_verify(data != nullptr, false);

    return list_push_back($data, data);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_on_list_pop(stack_on_list *const stk, void *const data /* = nullptr */)
{
    return list_pop_back($data, data);
}

//--------------------------------------------------------------------------------------------------------------------------------
// top
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_on_list_top(const stack_on_list *const stk, void *const data)
{
    log_verify(data != nullptr, false);

    return list_back($data, data);
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void stack_on_list_dump(const void *const _stk)
{
    log_verify(_stk != nullptr, ;);

    const stack_on_list *const stk = (const stack_on_list *) _stk;
    list_dump($data);
}