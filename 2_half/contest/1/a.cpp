#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

#define STACK_CAPACITY 10000

struct stack
{
    int    data[STACK_CAPACITY];
    size_t size;
};

#define $data stk->data
#define $size stk->size

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// CALLS
//--------------------------------------------------------------------------------------------------------------------------------

int call_dispatch(struct stack *const stk, const char *const str);

void call_push    (struct stack *const stk);
void call_pop     (struct stack *const stk);
void call_back    (struct stack *const stk);
void call_size    (struct stack *const stk);
void call_clear   (struct stack *const stk);
void call_exit    ();

//--------------------------------------------------------------------------------------------------------------------------------
// STACK
//--------------------------------------------------------------------------------------------------------------------------------

int  stack_ctor (      struct stack *const stk);
int  stack_push (      struct stack *const stk, const int pushed);
int  stack_pop  (      struct stack *const stk, int *const top);
int  stack_back (const struct stack *const stk, int *const top);
int  stack_size (const struct stack *const stk, size_t *const size);
void stack_clear(      struct stack *const stk);

//================================================================================================================================
// MAIN
//================================================================================================================================

int main()
{
    struct stack stk;
    stack_ctor((struct stack *) &stk);

    char str[100] = "\0";
    do
    {
        if (scanf("%s", str) != 1)
        {
            //log_error("Expected string input\n");
            //return 0;
        }
    } while (!call_dispatch(&stk, str));
}

//================================================================================================================================
// CALLS
//================================================================================================================================


int call_dispatch(struct stack *const stk, const char *const str)
{
    //log_verify(stk != nullptr, false);

    int is_exit = 0;

    if (strcmp(str, "push" ) == 0) { call_push (stk);              }
    if (strcmp(str, "pop"  ) == 0) { call_pop  (stk);              }
    if (strcmp(str, "back" ) == 0) { call_back (stk);              }
    if (strcmp(str, "size" ) == 0) { call_size (stk);              }
    if (strcmp(str, "clear") == 0) { call_clear(stk);              }
    if (strcmp(str, "exit" ) == 0) { call_exit ()   ; is_exit = 1; }
    //else                         { log_error("Undefined call\n");}

    return is_exit;
}

void call_push(struct stack *const stk)
{
    int pushed = 0;
    if (scanf("%d", &pushed) != 1)
    {
        //log_error("Expected number input after call \"push\"\n");
        return;
    }

    if (stack_push(stk, pushed)) printf("ok\n");
}

void call_pop(struct stack *const stk)
{
    int top = 0;
    if (stack_pop(stk, &top)) printf("%d\n", top);
    else                      printf("error\n");
}

void call_back(struct stack *const stk)
{
    int top = 0;
    if (stack_back(stk, &top)) printf("%d\n", top);
    else                       printf("error\n");
}

void call_size(struct stack *const stk)
{
    size_t size = 0;
    stack_size(stk, &size);

    printf("%lu\n", size);
}

void call_clear(struct stack *const stk)
{
    stack_clear(stk);
    printf("ok\n");
}

void call_exit()
{
    printf("bye\n");
}

//================================================================================================================================
// STACK
//================================================================================================================================

int stack_ctor(struct stack *const stk)
{
    //log_verify(stk != nullptr, false);

    $size = 0;

    return 1;
}

int stack_push(struct stack *const stk, const int pushed)
{
    //log_verify(stk   != nullptr,        false);
    //log_verify($size <  STACK_CAPACITY, false);

    $data[$size] = pushed;
    $size       += 1;

    return 1;
}

int stack_pop(struct stack *const stk, int *const top)
{
    //log_verify(stk   != nullptr, false);
    //log_verify(top   != nullptr, false);

    if ($size == 0) return 0;
    *top = $data[$size - 1];
    $size--;

    return 1;
}

int stack_back(const struct stack *const stk, int *const top)
{
    //log_verify(stk   != nullptr, false);
    //log_verify(top   != nullptr, false);

    if ($size == 0) return 0;
    *top = $data[$size - 1];

    return 1;
}

int stack_size(const struct stack *const stk, size_t *const size)
{
    //log_verify(stk  != nullptr, false);
    //log_verify(size != nullptr, false);

    *size = $size;

    return 1;
}

void stack_clear(struct stack *const stk)
{
    //log_verify(stk != nullptr, ;);

    $size = 0;
}
