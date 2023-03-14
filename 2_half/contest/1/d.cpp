#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// queue
//--------------------------------------------------------------------------------------------------------------------------------

#define DATA_CAPACITY 200000

struct queue
{
    int data[DATA_CAPACITY];
    int size;

    int head;   ///< индекс первого элемента
    int tail;   ///< индекс последнего элемента
};

//--------------------------------------------------------------------------------------------------------------------------------

#define $h_data hat->data
#define $h_size hat->size
#define $head   hat->head
#define $tail   hat->tail

//--------------------------------------------------------------------------------------------------------------------------------

//#define log_verify_queue(q, ret_val)                    
//        log_verify(q != nullptr, ret_val);              
//        log_verify(q->head < DATA_CAPACITY, ret_val);   
//        log_verify(q->tail < DATA_CAPACITY, ret_val);

//--------------------------------------------------------------------------------------------------------------------------------
// array_min
//--------------------------------------------------------------------------------------------------------------------------------

struct queue_min
{
    int value;
    int index;
};
const struct queue_min POISON_QUEUE_MIN = {-1, -1};

struct array_min
{
    struct queue_min data[DATA_CAPACITY];

    int size;
    int min_pos;
};

//--------------------------------------------------------------------------------------------------------------------------------

#define $arr_data   arr_min->data
#define $arr_size   arr_min->size
#define $min_pos    arr_min->min_pos

//--------------------------------------------------------------------------------------------------------------------------------

//#define log_verify_arr_min(arr_min, ret_val)        
//        log_verify(arr_min != nullptr, ret_val);    
//        log_verify(arr_min->size >= 0, ret_val);

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// calls
//--------------------------------------------------------------------------------------------------------------------------------

void call_dispatch(const char *const str, struct queue *const hat, struct array_min *const arr_min);

void call_enqueue(struct queue *const hat, struct array_min *const arr_min);
void call_dequeue(struct queue *const hat, struct array_min *const arr_min);
void call_front  (struct queue *const hat);
void call_size   (struct queue *const hat);
void call_clear  (struct queue *const hat, struct array_min *const arr_min);
void call_min    (struct array_min *const arr_min);

//--------------------------------------------------------------------------------------------------------------------------------
// queue
//--------------------------------------------------------------------------------------------------------------------------------

void queue_ctor (struct queue *const q);
int  queue_push (struct queue *const hat, const int val);
int  queue_pop  (struct queue *const hat, struct queue_min *const front);
int  queue_front(struct queue *const hat, struct queue_min *const front);
void queue_clear(struct queue *const hat);
//void queue_dump (const queue *const hat);

//--------------------------------------------------------------------------------------------------------------------------------
// array_min
//--------------------------------------------------------------------------------------------------------------------------------

void             array_min_ctor   (struct array_min *const arr_min);
void             array_min_upd    (struct array_min *const arr_min, const struct queue_min new_min);
void             array_min_push   (struct array_min *const arr_min, const struct queue_min pushed);
int              array_min_get_min(struct array_min *const arr_min, int *const result);
void             array_min_clear  (struct array_min *const arr_min);
//void      array_min_dump   (const array_min *const arr_min);

//================================================================================================================================
// MAIN
//================================================================================================================================

#define CMD_SIZE 100

int main()
{
    int m = 0;
    if (scanf("%d", &m) != 1)
    {
        //log_error("Expected integer input\n");
        return 0;
    }

    struct queue hat;
    queue_ctor(&hat);

    struct array_min arr_min;
    array_min_ctor(&arr_min);

    char cmd[CMD_SIZE] = "\0";
    while (m--)
    {
        if (scanf("%99s", cmd) != 1)
        {
            //log_error("Expected string input\n");
            break;
        }
        /*/>>>>>>>>>>>>>>>>>>>>>>>>
        log_tab_message("-------------------\n"
                        "call %s\n"
                        "===================\n"
                        "BEFORE:\n", cmd);
        queue_dump(&hat);
        array_min_dump(&arr_min);
        //<<<<<<<<<<<<<<<<<<<<<<<<*/
        call_dispatch(cmd, &hat, &arr_min);
        /*/>>>>>>>>>>>>>>>>>>>>>>>>
        log_tab_message("AFTER:\n");
        queue_dump(&hat);
        array_min_dump(&arr_min);
        //<<<<<<<<<<<<<<<<<<<<<<<<*/
    }
}   

//--------------------------------------------------------------------------------------------------------------------------------
// calls
//--------------------------------------------------------------------------------------------------------------------------------

void call_dispatch(const char *const str, struct queue *const hat, struct array_min *const arr_min)
{
    //log_verify(str     != nullptr, ;);
    //log_verify(hat     != nullptr, ;);
    //log_verify(arr_min != nullptr, ;);

    if      (strcmp(str, "enqueue") == 0) call_enqueue(hat, arr_min);
    else if (strcmp(str, "dequeue") == 0) call_dequeue(hat, arr_min);
    else if (strcmp(str,   "front") == 0) call_front  (hat);
    else if (strcmp(str,    "size") == 0) call_size   (hat);
    else if (strcmp(str,   "clear") == 0) call_clear  (hat, arr_min);
    else if (strcmp(str,     "min") == 0) call_min    (arr_min);
    //else                                      log_error("Undefined call\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void call_enqueue(struct queue *const hat, struct array_min *const arr_min)
{
    //log_verify_queue  (hat,     ;);
    //log_verify_arr_min(arr_min, ;);

    int iq = 0;
    if (scanf("%d", &iq) != 1)
    {
        //log_error("Expected integer input\n");
        return;
    }

    int pos = queue_push(hat, iq);
    array_min_upd(arr_min, (struct queue_min) {iq, pos});

    printf("ok\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void call_dequeue(struct queue *const hat, struct array_min *const arr_min)
{
    //log_verify_queue  (hat,     ;);
    //log_verify_arr_min(arr_min, ;);

    struct queue_min front /* = {} */;
    if (!queue_pop(hat, &front)) { printf("error\n"); return; }

    if ($arr_data[$min_pos].index == front.index) $min_pos++;
    printf("%d\n", front.value);
}

//--------------------------------------------------------------------------------------------------------------------------------

void call_front(struct queue *const hat)
{
    //log_verify_queue  (hat,     ;);
    //log_verify_arr_min(arr_min, ;);

    struct queue_min front /* = {} */;
    if (!queue_front(hat, &front)) { printf("error\n"); return; }

    printf("%d\n", front.value);    
}

//--------------------------------------------------------------------------------------------------------------------------------

void call_size(struct queue *const hat)
{
    //log_verify_queue  (hat,     ;);
    //log_verify_arr_min(arr_min, ;);

    printf("%d\n", $h_size);
}

//--------------------------------------------------------------------------------------------------------------------------------

void call_clear(struct queue *const hat, struct array_min *const arr_min)
{
    //log_verify_queue  (hat,     ;);
    //log_verify_arr_min(arr_min, ;);

    queue_clear(hat);
    array_min_clear(arr_min);

    printf("ok\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void call_min(struct array_min *const arr_min)
{
    //log_verify_queue(hat,       ;);
    //log_verify_arr_min(arr_min, ;);

    int res = 0;
    if (!array_min_get_min(arr_min, &res)) { printf("error\n"); return; }

    printf("%d\n", res);
}

//--------------------------------------------------------------------------------------------------------------------------------
// queue
//--------------------------------------------------------------------------------------------------------------------------------

void queue_ctor(struct queue *const q)
{
    //log_verify(q != nullptr, ;);

    q->size = 0;
    q->head = q->tail = -1;
}

//--------------------------------------------------------------------------------------------------------------------------------

int queue_push(struct queue *const hat, const int val)
{
    //log_verify_queue(hat, -1);

    $tail = ($tail + 1) % DATA_CAPACITY;
    if ($h_size == 0) $head = $tail;

    $h_data[$tail] = val;
    $h_size++;

    return $tail;
}

int queue_pop(struct queue *const hat, struct queue_min *const front)
{
    //log_verify_queue(hat,        0);
    //log_verify(front != nullptr, 0);

    if ($h_size == 0) return 0;

    *front = (struct queue_min) { $h_data[$head], $head };
    $head  = ($head + 1) % DATA_CAPACITY;
    $h_size--;

    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

int queue_front(struct queue *const hat, struct queue_min *const front)
{
    //log_verify_queue(hat, 0);
    //log_verify(front != nullptr, 0);

    if ($h_size == 0) return 0;

    *front = (struct queue_min) { $h_data[$head], $head };
    return 1;
}

void queue_clear(struct queue *const hat)
{
    //log_verify_queue(hat, ;);

    queue_ctor(hat);
}

/*
void queue_dump(const queue *const hat)
{
    log_tab_message("queue (addr: %p)\n"
                    "{\n", hat);
    if (hat == nullptr) { log_tab_message("}\n"); return; }

    LOG_TAB++;
    log_tab_message("head = %d\n"
                    "tail = %d\n"
                    "size = %d\n", $head, $tail, $h_size);
    log_tab_message("data: ");

    for (int i = 0; i < $h_size; ++i) { log_message("%d ", $h_data[i]); }

    LOG_TAB--;
    log_tab_message("\n}\n");
}
*/
//--------------------------------------------------------------------------------------------------------------------------------
// array_min
//--------------------------------------------------------------------------------------------------------------------------------

void array_min_ctor(struct array_min *const arr_min)
{
    //log_verify(arr_min != nullptr, ;);

    arr_min->size    =  0;
    arr_min->min_pos =  0;
}

//--------------------------------------------------------------------------------------------------------------------------------

void array_min_upd(struct array_min *const arr_min, const struct queue_min new_min)
{
    //log_verify_arr_min(arr_min, ;);

    if ($arr_size == $min_pos)
    {
        array_min_push(arr_min, new_min);
        return;
    }

    if ($arr_data[$arr_size - 1].value < new_min.value)
    {
        array_min_push(arr_min, new_min);
        return;
    }

    for (int i = $arr_size; i > $min_pos; i--)
    {
        if ($arr_data[i - 1].value >= new_min.value)
        {
            $arr_data[i - 1] = new_min;
            $arr_size = i;
        }
        else { break; }
    }
}

void array_min_push(struct array_min *const arr_min, const struct queue_min pushed)
{
    //log_verify_arr_min(arr_min, ;);
    //log_verify($arr_size < DATA_CAPACITY, ;);

    $arr_data[$arr_size] = pushed;
    $arr_size++;
}

//--------------------------------------------------------------------------------------------------------------------------------

int array_min_get_min(struct array_min *const arr_min, int *const result)
{
    //log_verify_arr_min(arr_min, 0);
    //log_verify(result != nullptr, 0);

    if ($arr_size == $min_pos) return 0;

    *result = $arr_data[$min_pos].value;
    return 1;
}

void array_min_clear(struct array_min *const arr_min)
{
    //log_verify_arr_min(arr_min, ;);

    array_min_ctor(arr_min);
}

/*
void array_min_dump(const array_min *const arr_min)
{
    log_tab_message("array_min (addr: %p)\n"
                    "{\n", arr_min);
    if (arr_min == nullptr) { log_tab_message("}\n"); return; }

    LOG_TAB++;
    log_tab_message("size    = %d\n"
                    "min_pos = %d\n", $arr_size, $min_pos);
    log_tab_message("data: ");

    for (int i = 0; i < $arr_size; ++i) { log_message("%d ", $arr_data[i]); }

    LOG_TAB--;
    log_tab_message("\n}\n");
}
*/