#ifndef K_OPTIMIZED_HEAP_H
#define K_OPTIMIZED_HEAP_H

#include <stdlib.h>

//================================================================================================================================

typedef int key_t;

//--------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    key_t  key;
    size_t pos;
}
node_t;

//--------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    node_t *kth_data;
    node_t *bin_data;

    size_t  bin_size;
    size_t  bin_capacity;
}
binary_heap;

//--------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    node_t *kth_data;
    node_t *bin_unit;

    size_t k;
    size_t size;
    size_t capacity;

    binary_heap *bin_heap_arr;
}
kth_optimized_heap;

//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// kth_optimized_heap
//--------------------------------------------------------------------------------------------------------------------------------

kth_optimized_heap *kth_optimized_heap_new        (                                      const size_t k, const size_t capacity);
bool                kth_optimized_heap_ctor       (      kth_optimized_heap *const heap, const size_t k, const size_t capacity);
void                kth_optimized_heap_free       (      kth_optimized_heap *const heap);
void                kth_optimized_heap_dtor       (      kth_optimized_heap *const heap);

bool                kth_optimized_heap_insert     (      kth_optimized_heap *const heap, const key_t key);

bool                kth_optimized_heap_get_min    (const kth_optimized_heap *const heap, key_t *const min_key);
bool                kth_optimized_heap_extract_min(      kth_optimized_heap *const heap, key_t *const min_key = nullptr);

#endif // K_OPTIMIZED_HEAP_H
