#ifndef K_OPTIMIZED_HEAP_H
#define K_OPTIMIZED_HEAP_H

#include <stdlib.h>

//================================================================================================================================

typedef int                       key_t;
typedef struct kth_optimized_heap kth_optimized_heap;

//================================================================================================================================

const key_t INF = 2'000'000'000;

//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// kth_optimized_heap
//--------------------------------------------------------------------------------------------------------------------------------

kth_optimized_heap *kth_optimized_heap_new        (                                      const size_t k, const size_t capacity);
bool                kth_optimized_heap_ctor       (      kth_optimized_heap *const heap, const size_t k, const size_t capacity);
void                kth_optimized_heap_free       (      kth_optimized_heap *const heap);
void                kth_optimized_heap_dtor       (      kth_optimized_heap *const heap);

bool                kth_optimized_heap_insert     (      kth_optimized_heap *const heap, const key_t key);

void                kth_optimized_heap_dump       (const kth_optimized_heap *const heap);

bool                kth_optimized_heap_get_min    (const kth_optimized_heap *const heap, key_t *const min_key);
bool                kth_optimized_heap_extract_min(      kth_optimized_heap *const heap, key_t *const min_key = nullptr);

void                kth_optimized_heap_sort       (int *const arr, const size_t size, const size_t k);

#endif // K_OPTIMIZED_HEAP_H
