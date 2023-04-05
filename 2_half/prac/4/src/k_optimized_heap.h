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

bool binary_optimize_ctor(binary_optimize *const hp, const int capacity, bin_kth_node *const bin_data,
                                                                         bin_kth_node *const kth_data);
//--------------------------------------------------------------------------------------------------------------------------------
bool binary_optimize_insert    (binary_optimize *const hp, const int key,     const int kth_pos);
bool binary_optimize_update_key(binary_optimize *const hp, const int bin_pos, const int new_key);
//--------------------------------------------------------------------------------------------------------------------------------
bin_kth_node binary_optimize_get_min    (binary_optimize *const hp);
bin_kth_node binary_optimize_extract_min(binary_optimize *const hp);
//--------------------------------------------------------------------------------------------------------------------------------
bool binary_optimize_sift_up  (binary_optimize *const hp, const int bin_pos);
bool binary_optimize_sift_down(binary_optimize *const hp, const int bin_pos);

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
void kth_optimazed_heap_dtor(kth_optimized_heap *const hp);
//--------------------------------------------------------------------------------------------------------------------------------
bool kth_optimized_heap_insert(kth_optimized_heap *const hp, const int key);
bool kth_optimized_heap_sort  (kth_optimized_heap *const hp, int *const arr, const int arr_size);
//--------------------------------------------------------------------------------------------------------------------------------
int kth_optimized_heap_get_min    (kth_optimized_heap *const hp);
int kth_optimized_heap_extract_min(kth_optimized_heap *const hp);
//--------------------------------------------------------------------------------------------------------------------------------
bool kth_optimized_heap_sift_up  (kth_optimized_heap *const hp, const int kth_pos);
bool kth_optimized_heap_sift_down(kth_optimized_heap *const hp, const int kth_pos);

#endif // K_OPTIMIZED_HEAP_H