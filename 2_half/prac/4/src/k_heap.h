#ifndef K_HEAP_H
#define K_HEAP_H

struct k_heap
{
    int     k;
    int *data;

    size_t     size;
    size_t capacity;
};

bool k_heap_ctor(k_heap *const hp, const int k, const size_t capacity);
void k_heap_dtor(k_heap *const hp);

bool k_heap_insert     (k_heap *const hp, const int val);
int  k_heap_get_min    (k_heap *const hp);
int  k_heap_extract_min(k_heap *const hp);

bool k_heap_sort       (k_heap *const hp, int *const arr, const int arr_size);

#endif //K_HEAP_H