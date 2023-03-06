#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define int long long

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

struct heap_elem
{
    int key;
    int query_num;
};

struct heap
{
    struct heap_elem *data;

    int size;
    int capacity;
};

//--------------------------------------------------------------------------------------------------------------------------------
// DSL
//--------------------------------------------------------------------------------------------------------------------------------

#define $data     hp->data
#define $size     hp->size
#define $capacity hp->capacity

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//================================================================================================================================
// SOLVE
//================================================================================================================================

void query_dispatch    (struct heap *const hp, const char *const query, const int query_num, int *const elem_ind);
void query_insert      (struct heap *const hp,                          const int query_num, int *const elem_ind);
void query_get_min     (struct heap *const hp);
void query_extract_min (struct heap *const hp, int *const elem_ind);
void query_decrease_key(struct heap *const hp, int *const elem_ind);

//================================================================================================================================
// HEAP
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

int  heap_ctor(struct heap *const hp, const int capacity);
void heap_dtor(struct heap *const hp);

//--------------------------------------------------------------------------------------------------------------------------------
// insert
//--------------------------------------------------------------------------------------------------------------------------------

int heap_insert(struct heap *const hp, const struct heap_elem *const new_elem, int *const elem_ind);

//--------------------------------------------------------------------------------------------------------------------------------
// get/extract min
//--------------------------------------------------------------------------------------------------------------------------------

int heap_get_min    (const struct heap *const hp);
int heap_extract_min(      struct heap *const hp, int *const elem_ind);

//--------------------------------------------------------------------------------------------------------------------------------
// increase/decrease key
//--------------------------------------------------------------------------------------------------------------------------------

int heap_increase_key(struct heap *const hp, const int ind, const int delta_key, int *const elem_ind);
int heap_decrease_key(struct heap *const hp, const int ind, const int delta_key, int *const elem_ind);

//--------------------------------------------------------------------------------------------------------------------------------
// sift up/down
//--------------------------------------------------------------------------------------------------------------------------------

int heap_sift_up  (struct heap *const hp, const int ind, int *const elem_ind);
int heap_sift_down(struct heap *const hp, const int ind, int *const elem_ind);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

//void heap_dump(const struct heap *const hp);

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

void heap_elem_swap(struct heap_elem *const a, struct heap_elem *const b);

//================================================================================================================================
// MAIN
//================================================================================================================================

#define MAX_QUERY_SIZE 100

const int MAX_KEY =  2e9;

signed main()
{
    int q = 0;
    if (scanf("%lld", &q) != 1) { /* log_error(...); */ return 0; }

    int *elem_ind = (int *) calloc((size_t) (q + 1), sizeof(int));
    if  (elem_ind == NULL) { /* log_error(); */ return 0; }

    struct heap store;
    if (!heap_ctor(&store, q)) { /* log_error(...); */ free(elem_ind); return 0; }

    char query[MAX_QUERY_SIZE] = "\0";

    for (int query_num = 1; query_num <= q; ++query_num)
    {
        if (scanf("%99s", query) != 1)
        {
            /* log_error(...); */
            free(elem_ind);
            heap_dtor(&store);

            return 0;
        }

        query_dispatch(&store, query, query_num, elem_ind);
    }

    free(elem_ind);
    heap_dtor(&store);
}

//================================================================================================================================
// SOLVE
//================================================================================================================================

void query_dispatch(struct heap *const hp, const char *const query, const int query_num, int *const elem_ind)
{
    //log_verify(query    != nullptr, ;);
    //log_verify(elem_ind != nullptr, ;);

    //log_tab_message("query_dispatch(hp = %p, query = \"%s\", query_num = %d, elem_ind = %p)\n"
    //                "======================================================================\n", hp, query, query_num, elem_ind);
    //heap_dump(hp);

    if (strcmp(query, "insert")      == 0) query_insert      (hp, query_num, elem_ind);
    if (strcmp(query, "getMin")      == 0) query_get_min     (hp);
    if (strcmp(query, "extractMin")  == 0) query_extract_min (hp,            elem_ind);
    if (strcmp(query, "decreaseKey") == 0) query_decrease_key(hp,            elem_ind);
//  else log_error(...);
}

//--------------------------------------------------------------------------------------------------------------------------------

void query_insert(struct heap *const hp, const int query_num, int *const elem_ind)
{
    //log_verify(hp       != nullptr, ;);
    //log_verify(elem_ind != nullptr, ;);
    //log_verify(query_num > 0, ;);

    struct heap_elem cur = (struct heap_elem) {0, query_num};
    if (scanf("%lld", &(cur.key)) != 1) { /* log_error(...); */ return; }

    heap_insert(hp, &cur, elem_ind);
}

//--------------------------------------------------------------------------------------------------------------------------------

void query_get_min(struct heap *const hp)
{
    //log_verify(hp != nullptr, ;);

    printf("%lld\n", heap_get_min(hp));
}

//--------------------------------------------------------------------------------------------------------------------------------

void query_extract_min(struct heap *const hp, int *const elem_ind)
{
    //log_verify(hp       != nullptr, ;);
    //log_verify(elem_ind != nullptr, ;);

    heap_extract_min(hp, elem_ind);
}

//--------------------------------------------------------------------------------------------------------------------------------

void query_decrease_key(struct heap *const hp, int *const elem_ind)
{
    //log_verify(hp       != nullptr, ;);
    //log_verify(elem_ind != nullptr, ;);

    int ind = 0, delta_key = 0;
    if (scanf("%lld %lld", &ind, &delta_key) != 2) { /* log_error(...); */ return; }

    heap_decrease_key(hp, elem_ind[ind], delta_key, elem_ind);
}

//================================================================================================================================
// HEAP
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

int heap_ctor(struct heap *const hp, const int capacity)
{
    //log_verify(hp != nullptr, 0);
    //log_verify(capacity > 0 , 0);

    $data = (struct heap_elem *) calloc((size_t) (capacity + 1), sizeof(struct heap_elem));
    if ($data == NULL) { /*log_calloc(...); */ return 0; }

    $capacity = capacity;
    $size     =        0;

    return 1;
}

void heap_dtor(struct heap *const hp)
{
    if (hp != NULL) free($data);
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert
//--------------------------------------------------------------------------------------------------------------------------------

int heap_insert(struct heap *const hp, const struct heap_elem *const new_elem, int *const elem_ind)
{
    //log_verify(hp       != nullptr, 0);
    //log_verify(new_elem != nullptr, 0);
    //log_verify(elem_ind != nullptr, 0);

    //log_verify($size < $capacity, 0);

    $size++;
    $data[$size] = *new_elem;

    elem_ind[new_elem->query_num] = $size;
    return heap_sift_up(hp, $size, elem_ind);
}

//--------------------------------------------------------------------------------------------------------------------------------
// get/extract min
//--------------------------------------------------------------------------------------------------------------------------------

int heap_get_min(const struct heap *const hp)
{
    //log_verify(hp != nullptr, 0);
    //log_verify($size > 0, 0);

    return $data[1].key;
}

int heap_extract_min(struct heap *const hp, int *const elem_ind)
{
    //log_verify(hp       != nullptr, 0);
    //log_verify(elem_ind != nullptr, 0);

    //log_verify($size > 0, 0);

    struct heap_elem cur = $data[1];
    struct heap_elem swp = $data[$size];

    elem_ind[swp.query_num] =  1;
    elem_ind[cur.query_num] = -1;

    heap_elem_swap($data + 1, $data + $size);
    $size--;

    if (!$size) return 1;
    return heap_sift_down(hp, 1, elem_ind);
}

//--------------------------------------------------------------------------------------------------------------------------------
// increase/decrease key
//--------------------------------------------------------------------------------------------------------------------------------

int heap_increase_key(struct heap *const hp, const int ind, const int delta_key, int *const elem_ind)
{
    //log_verify(hp       != nullptr, 0);
    //log_verify(elem_ind != nullptr, 0);

    //log_verify(ind >      0, 0);
    //log_verify(ind <= $size, 0);

    $data[ind].key += delta_key;
    return heap_sift_down(hp, ind, elem_ind);
}

int heap_decrease_key(struct heap *const hp, const int ind, const int delta_key, int *const elem_ind)
{
    //log_verify(hp       != nullptr, 0);
    //log_verify(elem_ind != nullptr, 0);

    //log_verify(ind >      0, 0);
    //log_verify(ind <= $size, 0);

    $data[ind].key -= delta_key;
    return heap_sift_up(hp, ind, elem_ind);
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

/*
void heap_dump(const heap *const hp)
{
    log_tab_message("heap (addrr: %p)\n"
                    "{\n", hp);

    if (hp == nullptr) { log_tab_message("}\n"); return; }

    LOG_TAB++;
    log_tab_message("capacity: %d\n"
                    "    size: %d\n"
                    "    data:", $capacity, $size);

    for (int i = 1; i <= $size; ++i) { log_message(" %d", $data[i]); }
    log_message("\n");

    LOG_TAB--;
    log_message("}\n");
}
*/

//--------------------------------------------------------------------------------------------------------------------------------
// sift up/down
//--------------------------------------------------------------------------------------------------------------------------------

int heap_sift_up(struct heap *const hp, const int ind, int *const elem_ind)
{
    //log_verify(hp       != nullptr, 0);
    //log_verify(elem_ind != nullptr, 0);

    //log_verify(ind >      0, 0);
    //log_verify(ind <= $size, 0);

    if (ind == 1) return 1;

    struct heap_elem cur = $data[ind];
    struct heap_elem par = $data[ind / 2];

    if (cur.key < par.key)
    {
        elem_ind[cur.query_num] = ind / 2;
        elem_ind[par.query_num] = ind;

        heap_elem_swap($data + ind, $data + ind/2);
        return heap_sift_up(hp, ind / 2, elem_ind);
    }
    return 1;
}

int heap_sift_down(struct heap *const hp, const int ind, int *const elem_ind)
{
    //log_verify(hp       != nullptr, 0);
    //log_verify(elem_ind != nullptr, 0);

    //log_verify(ind >      0, 0);
    //log_verify(ind <= $size, 0);

    int left  = (2 * ind     > $size) ? MAX_KEY : $data[2 * ind    ].key;
    int right = (2 * ind + 1 > $size) ? MAX_KEY : $data[2 * ind + 1].key;

    struct heap_elem cur = $data[ind];
    struct heap_elem swp =     {0, 0};
    int          swp_ind = -1;

    if      (left  < cur.key && left  <= right) { swp = $data[2 * ind    ]; swp_ind = 2 * ind;     }
    else if (right < cur.key && right <=  left) { swp = $data[2 * ind + 1]; swp_ind = 2 * ind + 1; }
    else return 1;

    elem_ind[cur.query_num] = swp_ind;
    elem_ind[swp.query_num] =     ind;

    heap_elem_swap($data + ind, $data + swp_ind);
    return heap_sift_down(hp, swp_ind, elem_ind);
}

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

void heap_elem_swap(struct heap_elem *const a, struct heap_elem *const b)
{
    //log_verify(a != nullptr, ;);
    //log_verify(b != nullptr, ;);

    struct heap_elem temp = *a;
    *a = *b;
    *b = temp;
}