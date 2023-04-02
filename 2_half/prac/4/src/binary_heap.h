#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

struct binary_heap
{
    int *data;

    int size;
    int capacity;
};

bool bin_heap_ctor(binary_heap *const hp, const int capacity);
void bin_heap_dtor(binary_heap *const hp);

bool bin_heap_insert     (binary_heap *const hp, const int val);
int  bin_heap_get_min    (binary_heap *const hp);
int  bin_heap_extract_min(binary_heap *const hp);

bool bin_heap_sort       (binary_heap *const hp, int *const arr, const int arr_size);

#endif //BINARY_HEAP_H