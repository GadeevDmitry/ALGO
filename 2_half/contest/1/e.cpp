#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

#define DATA_CAPACITY 100000

struct stack_char
{
    char data[DATA_CAPACITY];
    int  size;
};

//--------------------------------------------------------------------------------------------------------------------------------

#define $data stk->data
#define $size stk->size

//#define log_stack_char_verify(stk, ret_val)
//        log_verify(stk != nullptr, ret_val);
//        log_verify($size >= 0, ret_val);

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// correct_bracket_sequence
//--------------------------------------------------------------------------------------------------------------------------------

int is_correct_bracket_sequence(const char *str);
int check_closed_bracket       (struct stack_char *const stk, const char opened);

//--------------------------------------------------------------------------------------------------------------------------------
// stack_char
//--------------------------------------------------------------------------------------------------------------------------------

void stack_char_ctor (struct stack_char *const stk);
void stack_char_push (struct stack_char *const stk, const char pushed);
int  stack_char_pop  (struct stack_char *const stk);
int  stack_char_front(struct stack_char *const stk, char *const front);

//================================================================================================================================
// MAIN
//================================================================================================================================

#define STR_SIZE 100001

int main()
{
    char str[STR_SIZE] = "\0";
    if (scanf("%100000s", str) != 1)
    {
        //log_error("Expected bracket sequence\n");
        return 0;
    }

    if (is_correct_bracket_sequence(str)) printf("YES\n");
    else                                  printf("NO\n");
}

//--------------------------------------------------------------------------------------------------------------------------------
// correct_bracket_sequence
//--------------------------------------------------------------------------------------------------------------------------------

int is_correct_bracket_sequence(const char *str)
{
    struct stack_char stk /* = {} */;
    stack_char_ctor(&stk);

    for (int i = 0; *str != '\0' && i < STR_SIZE; ++i, ++str)
    {
        switch (*str)
        {
            case '(':
            case '{':
            case '[':   stack_char_push(&stk, *str);
                        break;

            case ')':   if (!check_closed_bracket(&stk, '(')) return 0;
                        break;
            case '}':   if (!check_closed_bracket(&stk, '{')) return 0;
                        break;
            case ']':   if (!check_closed_bracket(&stk, '[')) return 0;
                        break;

            default:    //log_error("Undefined character int bracket sequence\n");
                        return 0;
        }
    }

    return stk.size == 0;    
}

int check_closed_bracket(struct stack_char *const stk, const char opened)
{
    //log_stack_char_verify(stk, 0);

    char front = '\0';
    if (stack_char_front(stk, &front) && front == opened)
    {
        stack_char_pop(stk);
        return 1;
    }

    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack_char
//--------------------------------------------------------------------------------------------------------------------------------

void stack_char_ctor(struct stack_char *const stk)
{
    //log_stack_char_verify(stk, ;);

    $size = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

void stack_char_push(struct stack_char *const stk, const char pushed)
{
    //log_stack_char_verify(stk, ;);
    //log_verify($size < DATA_CAPACITY, ;);

    $data[$size] = pushed;
    $size++;
}

//--------------------------------------------------------------------------------------------------------------------------------

int stack_char_pop(struct stack_char *const stk)
{
    //log_stack_char_verify(stk, 0);

    if ($size == 0) return 0;

    $size--;
    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

int stack_char_front(struct stack_char *const stk, char *const front)
{
    //log_stack_char_verify(stk, 0);

    if ($size == 0) return 0;

    *front = $data[$size - 1];
    return 1;
}
