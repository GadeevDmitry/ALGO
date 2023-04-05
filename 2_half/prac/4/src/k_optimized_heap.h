#ifndef K_OPTIMIZED_HEAP_H
#define K_OPTIMIZED_HEAP_H

//================================================================================================================================
// BIN_KTH_NODE
//================================================================================================================================

struct bin_kth_node
{
    int key;
    int pos;
};

void bin_kth_node_swap(bin_kth_node *const a, bin_kth_node *const b);

//================================================================================================================================
// BINARY_OPTIMAZE
//================================================================================================================================

struct binary_optimize
{
    bin_kth_node *bin_data;
    bin_kth_node *kth_data;

    int  size;
    int  capacity;
};

//================================================================================================================================
// KTH_OPTIMAZED_HEAP
//================================================================================================================================

struct kth_optimized_heap
{
    bin_kth_node *data;

    int    k;
    int size;
    int capacity;

    binary_optimize *bin_heap_unit;
};

bool kth_optimized_heap_ctor(kth_optimized_heap *const hp, const int k, const int capacity);
void kth_optimized_heap_dtor(kth_optimized_heap *const hp);
//--------------------------------------------------------------------------------------------------------------------------------
bool kth_optimized_heap_insert(kth_optimized_heap *const  hp, const int key);
bool kth_optimized_heap_sort  (void               *const _hp, int *const arr, const int arr_size);
//--------------------------------------------------------------------------------------------------------------------------------
int kth_optimized_heap_get_min    (kth_optimized_heap *const hp);
int kth_optimized_heap_extract_min(kth_optimized_heap *const hp);
//--------------------------------------------------------------------------------------------------------------------------------
void kth_optimized_heap_dump(kth_optimized_heap *const hp);

#endif // K_OPTIMIZED_HEAP_H