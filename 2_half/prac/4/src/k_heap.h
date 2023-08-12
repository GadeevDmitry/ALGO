#ifndef K_HEAP_H
#define K_HEAP_H

#include <stdlib.h>

struct k_heap
{
    size_t  k;
    int *data;

    size_t     size;
    size_t capacity;
};

k_heap *k_heap_new        (                  const size_t k, const size_t capacity);
bool    k_heap_ctor       (k_heap *const hp, const size_t k, const size_t capacity);
void    k_heap_free       (k_heap *const hp);
void    k_heap_dtor       (k_heap *const hp);

bool    k_heap_insert     (k_heap *const hp, const int val);
int     k_heap_get_min    (k_heap *const hp);
int     k_heap_extract_min(k_heap *const hp);

void    k_heap_sort       (int *const arr, const size_t size, const size_t k);

#endif //K_HEAP_H
