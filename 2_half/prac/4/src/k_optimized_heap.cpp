#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "math.h"

#define LOG_NVERIFY
#include "../../../../lib/logs/log.h"

//================================================================================================================================
// STATIC FUNCTION DECLARATION
//================================================================================================================================

#include "k_optimized_heap.h"

static bool child_heap_ctor(child_heap *const hp, const int capacity, child_id *const data, k_optimized_heap *const src_hp);

//================================================================================================================================
// DSL
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// common
//--------------------------------------------------------------------------------------------------------------------------------

#define $data       hp->data
#define $size       hp->size
#define $capacity   hp->capacity

//--------------------------------------------------------------------------------------------------------------------------------
// k_optimized_heap
//--------------------------------------------------------------------------------------------------------------------------------

#define $k          hp->k
#define $bin_hp     hp->bin_hp
#define $bin_data   hp->bin_data
#define $bin_size   hp->bin_size

//--------------------------------------------------------------------------------------------------------------------------------
// child_heap
//--------------------------------------------------------------------------------------------------------------------------------

#define $src_hp     hp->src_hp

//--------------------------------------------------------------------------------------------------------------------------------
// child_heap
//--------------------------------------------------------------------------------------------------------------------------------

#define $key        child->key
#define $ind        child->ind

//================================================================================================================================
// K_OPTIMIZED_HEAP
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor-dtor
//--------------------------------------------------------------------------------------------------------------------------------

bool k_optimized_heap_ctor(k_optimized_heap *const hp, const int k, const int capacity)
{
    log_verify(hp != nullptr, false);
    log_verify(k         > 1, false);
    log_verify(capacity  > 0, false);

    $data = (int *) log_calloc((size_t) capacity, sizeof(int));
    log_verify($data != nullptr, false);

    $k        =         k;
    $size     =         0;
    $capacity =  capacity;
    $bin_size = (capacity + k - 1) / k;

    $bin_hp   = (child_heap *) log_calloc($bin_size, sizeof(child_heap));
    $bin_data = (child_id   *) log_calloc(capacity , sizeof(child_id  ));

    log_verify($bin_hp   != nullptr, false);
    log_verify($bin_data != nullptr, false);

    int data_offset = 0;
    for (int i = 0; i < $bin_size; ++i) { child_heap_ctor($bin_hp + i, k, $bin_data + data_offset, hp); data_offset += $k; }

    return true;
}

void k_optimized_heap_dtor(k_optimized_heap *const hp)
{
    if (hp == nullptr) return;

    log_free($data);
    log_free($bin_data);
    log_free($bin_hp);
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert
//--------------------------------------------------------------------------------------------------------------------------------



//================================================================================================================================
// CHILD_HEAP
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor-dtor
//--------------------------------------------------------------------------------------------------------------------------------

static bool child_heap_ctor(child_heap *const hp, const int capacity, child_id *const data, k_optimized_heap *const src_hp)
{
    log_verify(hp     != nullptr, false);
    log_verify(data   != nullptr, false);
    log_verify(src_hp != nullptr, false);
    log_verify(capacity >      0, false);

    $data     = data;
    $size     = 0;
    $capacity = capacity;
    $src_hp   = src_hp;
}

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

static void int_swap(int *const a, int *const b)
{
    log_verify(a != nullptr, ;);
    log_verify(b != nullptr, ;);

    int temp = *a;
    *a =   *b;
    *b = temp;
}