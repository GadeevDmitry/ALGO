#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <stdlib.h>

struct binary_heap
{
    int *data;

    size_t size;
    size_t capacity;
};

binary_heap *bin_heap_new        (                       const size_t capacity);
bool         bin_heap_ctor       (binary_heap *const hp, const size_t capacity);
void         bin_heap_free       (binary_heap *const hp);
void         bin_heap_dtor       (binary_heap *const hp);

bool         bin_heap_insert     (binary_heap *const hp, const int val);
int          bin_heap_get_min    (binary_heap *const hp);
int          bin_heap_extract_min(binary_heap *const hp);

void         bin_heap_sort       (int *const arr, const size_t size);

#endif //BINARY_HEAP_H
