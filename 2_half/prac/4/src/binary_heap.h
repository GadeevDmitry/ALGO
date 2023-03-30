#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

struct binary_heap
{
    int *data;

    size_t size;
    size_t capacity;
};

bool bin_heap_ctor   (binary_heap *const hp, const size_t capacity);
void bin_heap_dtor   (binary_heap *const hp);

bool bin_heap_insert     (binary_heap *const hp, const unsigned val);
int  bin_heap_get_min    (binary_heap *const hp);
bool bin_heap_extract_min(binary_heap *const hp);

#endif //BINARY_HEAP_H