#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define NDEBUG
#define NVERIFY
#define LOG_NLEAK
#define LOG_NTRACE
#include "../../../../lib/logs/log.h"
#include "../../../../lib/algorithm/algorithm.h"

#include "k_optimized_heap.h"

//================================================================================================================================

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

typedef struct kth_optimized_heap
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

static void kth_optimized_heap_sift_up                   (      kth_optimized_heap *const heap, const size_t kth_pos);
static void kth_optimized_heap_sift_down                 (      kth_optimized_heap *const heap, const size_t kth_pos);
static void kth_optimized_heap_node_swap                 (      kth_optimized_heap *const heap, const size_t kth_1_pos, const size_t kth_2_pos);

static void kth_optimized_heap_dump_kth_data             (const kth_optimized_heap *const heap);
static void kth_optimized_heap_dump_kth_data_bin_heap_ind(const kth_optimized_heap *const heap);
static void kth_optimized_heap_dump_bin_heap_arr         (const kth_optimized_heap *const heap);

//--------------------------------------------------------------------------------------------------------------------------------
// binary_heap
//--------------------------------------------------------------------------------------------------------------------------------

static void   binary_heap_ctor        (      binary_heap *const heap, node_t *const kth_data, node_t *const bin_data, const  size_t capacity);

static void   binary_heap_insert      (      binary_heap *const heap, const key_t key, const size_t kth_pos);
static void   binary_heap_erase       (      binary_heap *const heap,                  const size_t bin_pos);

static void   binary_heap_decrease_key(      binary_heap *const heap, const size_t bin_pos, const key_t new_key);

static void   binary_heap_extract_min (      binary_heap *const heap);
static size_t binary_heap_get_min_ind (const binary_heap *const heap);

static void   binary_heap_dump        (const binary_heap *const heap);

static void   binary_heap_sift_up     (binary_heap *const heap, const size_t bin_pos);
static void   binary_heap_sift_down   (binary_heap *const heap, const size_t bin_pos);
static void   binary_heap_node_swap   (binary_heap *const heap, const size_t bin_1_pos, const size_t bin_2_pos);

//--------------------------------------------------------------------------------------------------------------------------------
// node_t
//--------------------------------------------------------------------------------------------------------------------------------

static void node_arr_dump      (const node_t *const node_arr, const size_t arr_size);
static void node_arr_dump_field(const node_t *const node_arr, const size_t arr_size, const bool is_key);

//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// kth_optimized_heap
//--------------------------------------------------------------------------------------------------------------------------------

#define GET_PAR_IND(         heap, kth_pos) ((kth_pos - 1) / heap->k)
#define GET_L_CHILD_IND(     heap, kth_pos) (heap->k * kth_pos + 1)
#define GET_R_CHILD_IND(     heap, kth_pos) (heap->k * (kth_pos + 1))

#define GET_KEY(             heap, kth_pos) heap->kth_data[kth_pos].key
#define GET_BIN_IND(         heap, kth_pos) heap->kth_data[kth_pos].pos
#define GET_PAR_KEY(         heap, kth_pos) GET_KEY(heap, GET_PAR_IND(heap, kth_pos))

#define SET_KEY(             heap, kth_pos, key_val) GET_KEY    (heap, kth_pos) = key_val;
#define SET_BIN_IND(         heap, kth_pos, ind_val) GET_BIN_IND(heap, kth_pos) = ind_val;
#define SET_PAR_KEY(         heap, kth_pos, key_val) GET_PAR_KEY(heap, kth_pos) = key_val;

#define GET_BIN_HEAP_IND(    heap, kth_pos) ((kth_pos - 1) / heap->k)
#define GET_BIN_HEAP(        heap, kth_pos) (heap->bin_heap_arr + GET_BIN_HEAP_IND(heap, kth_pos))

#define GET_PAR_BIN_HEAP_IND(heap, kth_pos) GET_BIN_HEAP_IND(heap, GET_PAR_IND(kth_pos))
#define GET_PAR_BIN_HEAP(    heap, kth_pos) GET_BIN_HEAP    (heap, GET_PAR_IND(kth_pos))

#define GET_CHILD_BIN_HEAP_IND(    kth_pos) kth_pos
#define GET_CHILD_BIN_HEAP(  heap, kth_pos) heap->bin_heap_arr + GET_CHILD_BIN_HEAP_IND(kth_pos)

//--------------------------------------------------------------------------------------------------------------------------------

kth_optimized_heap *kth_optimized_heap_new(const size_t k, const size_t capacity)
{
    kth_optimized_heap *heap = (kth_optimized_heap *) log_calloc(1, sizeof(kth_optimized_heap));
    log_verify(heap != nullptr, nullptr);

    if (!kth_optimized_heap_ctor(heap, k, capacity))
    {
        log_free(heap);
        return nullptr;
    }

    return heap;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool kth_optimized_heap_ctor(kth_optimized_heap *const heap, const size_t k, const size_t capacity)
{
    log_verify(heap != nullptr, false);

    heap->k        = k;
    heap->size     = 0;
    heap->capacity = capacity;

    size_t bin_heap_arr_capacity = (capacity + k - 1) / k;

    heap->kth_data     = (node_t *)      log_calloc(capacity,              sizeof(node_t));
    heap->bin_unit     = (node_t *)      log_calloc(capacity,              sizeof(node_t));
    heap->bin_heap_arr = (binary_heap *) log_calloc(bin_heap_arr_capacity, sizeof(binary_heap));

    node_t *bin_heap_data = heap->bin_unit;

    for (size_t arr_ind = 0; arr_ind < bin_heap_arr_capacity; ++arr_ind)
    {
        binary_heap_ctor(heap->bin_heap_arr + arr_ind, heap->kth_data, bin_heap_data, k);
        bin_heap_data += k;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void kth_optimized_heap_free(kth_optimized_heap *const heap)
{
    kth_optimized_heap_dtor(heap);
    log_free(heap);
}

//--------------------------------------------------------------------------------------------------------------------------------

void kth_optimized_heap_dtor(kth_optimized_heap *const heap)
{
    if (heap == nullptr) return;

    log_free(heap->kth_data);
    log_free(heap->bin_unit);
    log_free(heap->bin_heap_arr);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool kth_optimized_heap_insert(kth_optimized_heap *const heap, const key_t key)
{
    log_verify(heap != nullptr,             false);
    log_verify(heap->size < heap->capacity, false);

    SET_KEY(heap, heap->size, key)
    heap->size++;

    if (heap->size == 1) return true;

    binary_heap_insert(GET_BIN_HEAP(heap, heap->size - 1), key, heap->size - 1);
    kth_optimized_heap_sift_up(heap, heap->size - 1);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool kth_optimized_heap_get_min(const kth_optimized_heap *const heap, key_t *const min_key)
{
    log_verify(heap    != nullptr, false);
    log_verify(min_key != nullptr, false);
    log_verify(heap->size > 0,     false);

    *min_key = GET_KEY(heap, 0);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool kth_optimized_heap_extract_min(kth_optimized_heap *const heap, key_t *const min_key /* = nullptr */)
{
    log_verify(heap != nullptr, false);
    log_verify(heap->size != 0, false);

    #define try_store_min_key(min_key_val)                    \
        if (min_key != nullptr) *min_key = min_key_val;

    key_t key = GET_KEY(heap, 0);

    if (heap->size == 1)
    {
        heap->size = 0;
        try_store_min_key(key)
        return true;
    }

    kth_optimized_heap_node_swap(heap, 0, heap->size - 1);

    binary_heap_erase(GET_BIN_HEAP(heap, heap->size - 1), GET_BIN_IND(heap, heap->size - 1));
    heap->size--;

    kth_optimized_heap_sift_down(heap, 0);

    try_store_min_key(key)
    return true;

    #undef try_store_min_key
}

//--------------------------------------------------------------------------------------------------------------------------------

void kth_optimized_heap_sort(int *const arr, const size_t size, const size_t k)
{
    log_verify(arr != nullptr, (void) 0);

    kth_optimized_heap *heap = kth_optimized_heap_new(k, size);

    for (size_t i = 0; i < size; ++i) kth_optimized_heap_insert(heap, (key_t) arr[i]);
    for (size_t i = 0; i < size; ++i) kth_optimized_heap_extract_min(heap, arr + i);

    kth_optimized_heap_free(heap);
}

//--------------------------------------------------------------------------------------------------------------------------------

void kth_optimized_heap_dump(const kth_optimized_heap *const heap)
{
    log_verify(heap != nullptr, (void) 0);

    log_tab_service_message("kth_optimized_heap (addr: %p)\n"
                            "{", "\n", heap);
    LOG_TAB++;

    usual_field_dump("k       ", "%lu", heap->k);
    usual_field_dump("size    ", "%lu", heap->size);
    usual_field_dump("capacity", "%lu", heap->capacity);
    log_message("\n");

    log_tab_service_message("kth_data\n"
                            "{", "\n");
    LOG_TAB++;
    kth_optimized_heap_dump_kth_data(heap);
    LOG_TAB--;

    log_tab_service_message("}", "\n\n");

    log_tab_service_message("bin_heap_arr\n"
                            "{", "\n");
    LOG_TAB++;
    kth_optimized_heap_dump_bin_heap_arr(heap);
    LOG_TAB--;

    log_tab_service_message("}", "\n");

    LOG_TAB--;
    log_tab_service_message("}", "\n\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void kth_optimized_heap_dump_kth_data(const kth_optimized_heap *const heap)
{
    log_assert(heap != nullptr);

    kth_optimized_heap_dump_kth_data_bin_heap_ind(heap);
    node_arr_dump(heap->kth_data, heap->size);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void kth_optimized_heap_dump_kth_data_bin_heap_ind(const kth_optimized_heap *const heap)
{
    log_assert(heap != nullptr);

    log_tab_service_message("bin_heap_ind: ", "");

    if (heap->size == 0) { log_message("\n"); return; }

    log_warning_message("   #", " ");

    for (size_t ind = 1; ind < heap->size; ++ind)
    {
        log_service_message("%4lu", " ", GET_BIN_HEAP_IND(heap, ind));
    }

    log_message("\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void kth_optimized_heap_dump_bin_heap_arr(const kth_optimized_heap *const heap)
{
    log_assert(heap != nullptr);

    if (heap->size <= 1) return;

    size_t bin_heap_arr_size = GET_BIN_HEAP_IND(heap, heap->size - 1) + 1;

    for (size_t ind = 0; ind < bin_heap_arr_size; ++ind)
    {
        log_tab_service_message("binary_heap #%lu\n"
                                "{", "\n",    ind);
        LOG_TAB++;
        binary_heap_dump(heap->bin_heap_arr + ind);
        LOG_TAB--;

        log_tab_service_message("}", "\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void kth_optimized_heap_sift_up(kth_optimized_heap *const heap, const size_t kth_pos)
{
    log_assert(heap != nullptr);
    log_assert(heap->size > kth_pos);

    if (kth_pos == 0) return;

    size_t par_kth_pos = GET_PAR_IND(heap, kth_pos);

    if (GET_KEY(heap, kth_pos) < GET_KEY(heap, par_kth_pos))
    {
        kth_optimized_heap_node_swap(heap, kth_pos, par_kth_pos);
        kth_optimized_heap_sift_up(heap, par_kth_pos);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void kth_optimized_heap_sift_down(kth_optimized_heap *const heap, const size_t kth_pos)
{
    log_assert(heap != nullptr);
    log_assert(heap->size > kth_pos);

    if (GET_L_CHILD_IND(heap, kth_pos) >= heap->size) return;

    size_t min_child_kth_pos = binary_heap_get_min_ind(GET_CHILD_BIN_HEAP(heap, kth_pos));

    if (GET_KEY(heap, kth_pos) > GET_KEY(heap, min_child_kth_pos))
    {
        kth_optimized_heap_node_swap(heap, kth_pos, min_child_kth_pos);
        kth_optimized_heap_sift_down(heap, min_child_kth_pos);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void kth_optimized_heap_node_swap(kth_optimized_heap *const heap, const size_t kth_1_pos, const size_t kth_2_pos)
{
    log_assert(heap != nullptr);
    log_assert(heap->size > kth_1_pos);
    log_assert(heap->size > kth_2_pos);
    log_assert(kth_1_pos != kth_2_pos);

    size_t bin_heap_1 = GET_BIN_HEAP_IND(heap, kth_1_pos);
    size_t bin_heap_2 = GET_BIN_HEAP_IND(heap, kth_2_pos);

    if (kth_1_pos != 0) binary_heap_erase(heap->bin_heap_arr + bin_heap_1, GET_BIN_IND(heap, kth_1_pos));
    if (kth_2_pos != 0) binary_heap_erase(heap->bin_heap_arr + bin_heap_2, GET_BIN_IND(heap, kth_2_pos));

    key_t key_1 = GET_KEY(heap, kth_1_pos);
    key_t key_2 = GET_KEY(heap, kth_2_pos);

    SET_KEY(heap, kth_1_pos, key_2);
    SET_KEY(heap, kth_2_pos, key_1);

    if (kth_1_pos != 0) binary_heap_insert(heap->bin_heap_arr + bin_heap_1, key_2, kth_1_pos);
    if (kth_2_pos != 0) binary_heap_insert(heap->bin_heap_arr + bin_heap_2, key_1, kth_2_pos);
}

//--------------------------------------------------------------------------------------------------------------------------------

#undef GET_PAR_IND
#undef GET_L_CHILD_IND
#undef GET_R_CHILD_IND

#undef GET_KEY
#undef GET_BIN_IND
#undef GET_PAR_KEY

#undef SET_KEY
#undef SET_BIN_IND
#undef SET_PAR_KEY

#undef GET_BIN_HEAP_IND
#undef GET_BIN_HEAP

#undef GET_PAR_BIN_HEAP_IND
#undef GET_PAR_BIN_HEAP

#undef GET_CHILD_BIN_HEAP_IND
#undef GET_CHILD_BIN_HEAP

//--------------------------------------------------------------------------------------------------------------------------------
// binary_heap
//--------------------------------------------------------------------------------------------------------------------------------

#define GET_PAR_IND(      bin_pos) ((bin_pos - 1) / 2)
#define GET_L_CHILD_IND(  bin_pos) (2*bin_pos + 1)
#define GET_R_CHILD_IND(  bin_pos) (2*bin_pos + 2)

#define GET_NODE(   heap, bin_pos) heap->bin_data[bin_pos]
#define GET_KEY(    heap, bin_pos) GET_NODE(heap, bin_pos).key
#define GET_KTH_IND(heap, bin_pos) GET_NODE(heap, bin_pos).pos
#define GET_PAR_KEY(heap, bin_pos) GET_KEY(heap, GET_PAR_IND(bin_pos))

#define SET_NODE(   heap, bin_pos, node_val) GET_NODE   (heap, bin_pos) = node_val;
#define SET_KEY(    heap, bin_pos,  key_val) GET_KEY    (heap, bin_pos) =  key_val;
#define SET_KTH_IND(heap, bin_pos,  ind_val) GET_KTH_IND(heap, bin_pos) =  ind_val;
#define SET_PAR_KEY(heap, bin_pos,  key_val) GET_PAR_KEY(heap, bin_pos) =  key_val;

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_ctor(binary_heap *const heap, node_t *const kth_data,
                                                      node_t *const bin_data,
                                                      const  size_t capacity)
{
    log_assert(heap     != nullptr);
    log_assert(kth_data != nullptr);
    log_assert(bin_data != nullptr);

    heap->kth_data = kth_data;
    heap->bin_data = bin_data;

    heap->bin_size     = 0;
    heap->bin_capacity = capacity;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_insert(binary_heap *const heap, const key_t key, const size_t kth_pos)
{
    log_assert(heap != nullptr);
    log_assert(heap->bin_size < heap->bin_capacity);

    SET_KEY    (heap, heap->bin_size, key)
    SET_KTH_IND(heap, heap->bin_size, kth_pos)

    heap->kth_data[kth_pos].pos = heap->bin_size;
    heap->bin_size++;

    binary_heap_sift_up(heap, heap->bin_size - 1);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_erase(binary_heap *const heap, const size_t bin_pos)
{
    log_assert(heap != nullptr);
    log_assert(heap->bin_size > bin_pos);

    extern const key_t INF;

    binary_heap_decrease_key(heap, bin_pos, -INF);
    binary_heap_extract_min (heap);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_decrease_key(binary_heap *const heap, const size_t bin_pos, const key_t new_key)
{
    log_assert(heap != nullptr);
    log_assert(heap->bin_size > bin_pos);
    log_assert(GET_KEY(heap, bin_pos) > new_key);

    SET_KEY(heap, bin_pos, new_key)
    binary_heap_sift_up(heap, bin_pos);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_extract_min(binary_heap *const heap)
{
    log_assert(heap != nullptr);
    log_assert(heap->bin_size != 0);

    if (heap->bin_size == 1) { heap->bin_size = 0; return; }

    binary_heap_node_swap(heap, 0, heap->bin_size - 1);
    heap->bin_size--;

    binary_heap_sift_down(heap, 0);
}

//--------------------------------------------------------------------------------------------------------------------------------

static size_t binary_heap_get_min_ind(const binary_heap *const heap)
{
    log_assert(heap != nullptr);
    log_assert(heap->bin_size != 0);

    return GET_KTH_IND(heap, 0);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_dump(const binary_heap *const heap)
{
    log_assert(heap != nullptr);

    usual_field_dump("size    ", "%lu", heap->bin_size);
    usual_field_dump("capacity", "%lu", heap->bin_capacity);

    node_arr_dump(heap->bin_data, heap->bin_size);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_sift_up(binary_heap *const heap, const size_t bin_pos)
{
    log_assert(heap != nullptr);
    log_assert(bin_pos < heap->bin_size);

    if (bin_pos == 0) return;

    size_t par_bin_pos = GET_PAR_IND(bin_pos);

    if (GET_KEY(heap, bin_pos) < GET_KEY(heap, par_bin_pos))
    {
        binary_heap_node_swap(heap, bin_pos, par_bin_pos);
        binary_heap_sift_up(heap, par_bin_pos);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_sift_down(binary_heap *const heap, const size_t bin_pos)
{
    log_assert(heap  != nullptr);
    log_assert(heap->bin_size > bin_pos);

    extern const key_t INF;

    size_t l_bin_pos = GET_L_CHILD_IND(bin_pos);
    size_t r_bin_pos = GET_R_CHILD_IND(bin_pos);

    key_t key   =                              GET_KEY(heap,   bin_pos);
    key_t key_l = l_bin_pos < heap->bin_size ? GET_KEY(heap, l_bin_pos) : INF;
    key_t key_r = r_bin_pos < heap->bin_size ? GET_KEY(heap, r_bin_pos) : INF;

    if (key_l < key && key_l <= key_r)
    {
        binary_heap_node_swap(heap, bin_pos, l_bin_pos);
        binary_heap_sift_down(heap, l_bin_pos);
    }
    else if (key_r < key && key_r <= key_l)
    {
        binary_heap_node_swap(heap, bin_pos, r_bin_pos);
        binary_heap_sift_down(heap, r_bin_pos);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_heap_node_swap(binary_heap *const heap, const size_t bin_1_pos, const size_t bin_2_pos)
{
    log_assert(heap != nullptr);
    log_assert(heap->bin_size > bin_1_pos);
    log_assert(heap->bin_size > bin_2_pos);
    log_assert(bin_1_pos != bin_2_pos);

    heap->kth_data[GET_KTH_IND(heap, bin_1_pos)].pos = bin_2_pos;
    heap->kth_data[GET_KTH_IND(heap, bin_2_pos)].pos = bin_1_pos;

    node_t temp = GET_NODE(heap, bin_1_pos);

    SET_NODE(heap, bin_1_pos, GET_NODE(heap, bin_2_pos))
    SET_NODE(heap, bin_2_pos, temp);
}

//--------------------------------------------------------------------------------------------------------------------------------

#undef GET_PAR_IND
#undef GET_L_CHILD_IND
#undef GET_R_CHILD_IND

#undef GET_NODE
#undef GET_KEY
#undef GET_KTH_IND
#undef GET_PAR_KEY

#undef SET_NODE
#undef SET_KEY
#undef SET_KTH_IND
#undef SET_PAR_KEY

//--------------------------------------------------------------------------------------------------------------------------------
// node_t
//--------------------------------------------------------------------------------------------------------------------------------

static void node_arr_dump(const node_t *const node_arr, const size_t arr_size)
{
    node_arr_dump_field(node_arr, arr_size, true);
    node_arr_dump_field(node_arr, arr_size, false);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void node_arr_dump_field(const node_t *const node_arr, const size_t arr_size, const bool is_key)
{
    log_assert(node_arr != nullptr);

    if (is_key) log_tab_service_message("key         : ", "");
    else        log_tab_service_message("pos         : ", "");

    for (size_t ind = 0; ind < arr_size; ++ind)
    {
        if (is_key) log_message("%4d " , node_arr[ind].key);
        else        log_message("%4lu ", node_arr[ind].pos);
    }

    log_message("\n");
}
