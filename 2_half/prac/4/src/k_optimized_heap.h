#ifndef K_OPTIMIZED_HEAP_H
#define K_OPTIMIZED_HEAP_H

struct child_id
{
    int key;
    int ind;
};

struct child_heap
{
    child_id *data;

    int size;
    int capacity;

    k_optimized_heap *src_hp;
};

struct k_optimized_heap
{
    int     k;
    int *data;

    int     size;
    int capacity;

    child_heap *bin_hp;
    child_id   *bin_data;
    int         bin_size;
};

bool k_optimized_heap_ctor(k_optimized_heap *const hp, const int k, const int capacity);
void k_optimized_heap_dtor(k_optimized_heap *const hp);

#endif // K_OPTIMIZED_HEAP_H