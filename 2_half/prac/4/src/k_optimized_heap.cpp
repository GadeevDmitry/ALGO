#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "math.h"

#define NLOG
#define LOG_NVERIFY
#include "../../../../lib/logs/log.h"

#include "k_optimized_heap.h"
//================================================================================================================================
// STATIC GLOBAL
//================================================================================================================================

#define COMMA ,

static const int INF = (int) 2e9;

//--------------------------------------------------------------------------------------------------------------------------------
// binary_optimize
//--------------------------------------------------------------------------------------------------------------------------------

static bool binary_optimize_ctor(binary_optimize *const hp, const int capacity, bin_kth_node *const bin_data,
                                                                                bin_kth_node *const kth_data);
//--------------------------------------------------------------------------------------------------------------------------------
static bool binary_optimize_insert    (binary_optimize *const hp, const int key,     const int kth_pos);
static bool binary_optimize_update_key(binary_optimize *const hp, const int bin_pos, const int new_key);
//--------------------------------------------------------------------------------------------------------------------------------
static bin_kth_node binary_optimize_get_min    (binary_optimize *const hp);
static bin_kth_node binary_optimize_extract_min(binary_optimize *const hp);
//--------------------------------------------------------------------------------------------------------------------------------
static bool binary_optimize_sift_up  (binary_optimize *const hp, const int bin_pos);
static bool binary_optimize_sift_down(binary_optimize *const hp, const int bin_pos);
//--------------------------------------------------------------------------------------------------------------------------------
static void binary_optimize_dump         (binary_optimize *const hp);
static void binary_optimize_bin_data_dump(binary_optimize *const hp);

//--------------------------------------------------------------------------------------------------------------------------------
// kth_optimized_heap
//--------------------------------------------------------------------------------------------------------------------------------

static bool kth_optimized_heap_sift_up  (kth_optimized_heap *const hp, const int kth_pos);
static bool kth_optimized_heap_sift_down(kth_optimized_heap *const hp, const int kth_pos);
//--------------------------------------------------------------------------------------------------------------------------------
static void kth_optimized_heap_data_dump(kth_optimized_heap *const hp);
static void kth_optimized_heap_unit_dump(kth_optimized_heap *const hp);

//================================================================================================================================
// DSL
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// common
//--------------------------------------------------------------------------------------------------------------------------------

#define $size       hp->size
#define $capacity   hp->capacity

//--------------------------------------------------------------------------------------------------------------------------------
// kth_optimized_heap
//--------------------------------------------------------------------------------------------------------------------------------

#define $k          hp->k
#define $data       hp->data
#define $bin_hp     hp->bin_heap_unit

//--------------------------------------------------------------------------------------------------------------------------------
// binary_optimize
//--------------------------------------------------------------------------------------------------------------------------------

#define $bin_data   hp->bin_data
#define $kth_data   hp->kth_data

//================================================================================================================================
// KTH_OPTIMAZED_HEAP
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor-dtor
//--------------------------------------------------------------------------------------------------------------------------------

bool kth_optimized_heap_ctor(kth_optimized_heap *const hp, const int k, const int capacity)
{
    log_verify(hp != nullptr, false);
    log_verify(k        >  1, false);
    log_verify(capacity >  0, false);

    int           bin_hp_unit_size = (capacity + k - 1) / k;    // количество бинарных куч
    bin_kth_node *bin_hp_data      = nullptr;                   // массив с данными бинарных куч

   $data        = (bin_kth_node    *) log_calloc((size_t) capacity        , sizeof(bin_kth_node   ));  // данные k-ичной кучи
   $bin_hp      = (binary_optimize *) log_calloc((size_t) bin_hp_unit_size, sizeof(binary_optimize));  // массив бинарных куч
    bin_hp_data = (bin_kth_node    *) log_calloc((size_t) capacity        , sizeof(bin_kth_node   ));

    log_verify($data        != nullptr, false);
    log_verify($bin_hp      != nullptr, false);
    log_verify( bin_hp_data != nullptr, false);

    $k        =        k;
    $size     =        0;
    $capacity = capacity;

    int data_offset = 0;
    for (int i = 0; i < bin_hp_unit_size; ++i)
    {
        binary_optimize_ctor($bin_hp + i, k, bin_hp_data + data_offset, $data);
        data_offset += k;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void kth_optimized_heap_dtor(kth_optimized_heap *const hp)
{
    if (hp == nullptr) return;

    log_free($data);
    log_free($bin_hp->bin_data);
    log_free($bin_hp);
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert, sort
//--------------------------------------------------------------------------------------------------------------------------------

bool kth_optimized_heap_insert(kth_optimized_heap *const hp, const int key)
{
    log_verify(hp !=     nullptr, false);
    log_verify($size < $capacity, false);

    $data[$size].key = key; $size++;
    if   ($size == 1) return true;

    int bin_hp_unit_ind = ($size - 2) / $k;
    binary_optimize_insert($bin_hp + bin_hp_unit_ind, key, $size - 1);

    return kth_optimized_heap_sift_up(hp, $size - 1);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool kth_optimized_heap_sort(void *const _hp, int *const arr, const int arr_size)
{
    log_verify(_hp != nullptr, false);
    log_verify(arr_size >   0, false);

    kth_optimized_heap *const hp = (kth_optimized_heap *) _hp;

    for (int i = 0; i < arr_size; ++i)          kth_optimized_heap_insert     (hp, arr[i]);
    for (int i = 0; i < arr_size; ++i) arr[i] = kth_optimized_heap_extract_min(hp);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// min
//--------------------------------------------------------------------------------------------------------------------------------

int kth_optimized_heap_get_min(kth_optimized_heap *const hp)
{
    log_verify(hp != nullptr, 0);
    log_verify($size !=    0, 0);

    return $data[0].key;
}

//--------------------------------------------------------------------------------------------------------------------------------

int kth_optimized_heap_extract_min(kth_optimized_heap *const hp)
{
    log_verify(hp != nullptr, 0);
    log_verify($size !=    0, 0);

    int ans = $data[0].key;
    $size--;

    if ($size == 0) return ans;

    const int bin_unit_last_ind = ($size - 1) / $k;

    const int last_kth_pos = $size;
    const int last_bin_pos = $data[last_kth_pos].pos;

    binary_optimize_update_key ($bin_hp + bin_unit_last_ind, last_bin_pos, -INF);
    binary_optimize_extract_min($bin_hp + bin_unit_last_ind);

    kth_optimized_heap_sift_down(hp, 0);
    return ans;
}

//--------------------------------------------------------------------------------------------------------------------------------
// sift
//--------------------------------------------------------------------------------------------------------------------------------

static bool kth_optimized_heap_sift_up(kth_optimized_heap *const hp, const int kth_pos)
{
    log_verify(hp != nullptr, false);
    log_verify(kth_pos >= 0 , false);

    if (kth_pos == 0) return true;

    const int kth_par_pos = (kth_pos - 1) / $k;
    const int bin_par_pos = $data[kth_par_pos].pos;
    const int     par_key = $data[kth_par_pos].key;

    const int bin_pos = $data[kth_pos].pos;
    const int     key = $data[kth_pos].key;

    if (par_key <= key) return true;

    const int par_bin_unit_ind = (kth_par_pos - 1) / $k;
    const int     bin_unit_ind = (kth_pos     - 1) / $k;

    if (kth_par_pos == 0)
    {
        $data[0].key = key;

        binary_optimize_update_key($bin_hp + bin_unit_ind, bin_pos, par_key);

        return true;
    }

    binary_optimize_update_key($bin_hp +     bin_unit_ind,     bin_pos, par_key);
    binary_optimize_update_key($bin_hp + par_bin_unit_ind, bin_par_pos,     key);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool kth_optimized_heap_sift_down(kth_optimized_heap *const hp, const int kth_pos)
{
    log_verify(hp   != nullptr, false);
    log_verify(kth_pos < $size, false);

    if (kth_pos * $k + 1 >= $size) return true;

    int bin_unit_ind        = (kth_pos - 1) / $k;
    int bin_unit_ind_child  =  kth_pos;

    bin_kth_node min_child_node     = binary_optimize_get_min($bin_hp + bin_unit_ind_child);
    int          min_child_key      = min_child_node.key;

    int     key = $data[kth_pos].key;
    int bin_pos = $data[kth_pos].pos;

    if (min_child_key < key)
    {
                            binary_optimize_update_key($bin_hp + bin_unit_ind_child,       0,           key);
        if (kth_pos != 0)   binary_optimize_update_key($bin_hp + bin_unit_ind      , bin_pos, min_child_key);
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void kth_optimized_heap_dump(kth_optimized_heap *const hp)
{
    log_tab_message(HTML_COLOR_MEDIUM_BLUE
                    "kth_optimized_heap (addr: %p)\n" HTML_COLOR_CANCEL
                    "{\n",                     hp);
    if (hp == nullptr) {    log_tab_message("}\n"); return; }
    LOG_TAB++;

    log_tab_message("k           = %d\n"
                    "size        = %d\n"
                    "capacity    = %d\n\n", $k, $size, $capacity);

    log_tab_message("data        = %p\n", $data);
    if             ($data != nullptr) kth_optimized_heap_data_dump(hp);

    log_tab_message("bin_hp_unit = %p\n", $bin_hp);
    if             ($bin_hp != nullptr) kth_optimized_heap_unit_dump(hp);

    LOG_TAB--;
    log_tab_message("}\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void kth_optimized_heap_data_dump(kth_optimized_heap *const hp)
{
    log_assert(hp    != nullptr);
    log_assert($data != nullptr);

    log_tab_message("{\n");
    LOG_TAB++;

    log_tab_message(HTML_COLOR_MEDIUM_BLUE "index  : ");
    for (int i = 0; i < $size; ++i) log_message("%10d ", i);
    log_message("\n");

    log_tab_message(HTML_COLOR_CANCEL
                    HTML_COLOR_DARK_ORANGE "key    : ");
    for (int i = 0; i < $size; ++i) log_message("%10d ", $data[i].key);
    log_tab_message("\n");

    log_tab_message(HTML_COLOR_CANCEL
                    HTML_COLOR_DARK_RED    "bin_pos: ");
    for (int i = 0; i < $size; ++i) log_message("%10d ", $data[i].pos);
    log_tab_message(HTML_COLOR_CANCEL "\n");

    LOG_TAB--;
    log_tab_message("}\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void kth_optimized_heap_unit_dump(kth_optimized_heap *const hp)
{
    log_assert(hp      != nullptr);
    log_assert($bin_hp != nullptr);

    log_tab_message("{\n");
    LOG_TAB++;

    int bin_hp_unit_size = ($capacity + $k - 1) / $k;
    log_tab_message("bin_heap_unit size = %d\n", bin_hp_unit_size);

    for (int i = 0; i < bin_hp_unit_size; ++i)
    {
        log_tab_message(HTML_COLOR_MEDIUM_BLUE "#%d" HTML_COLOR_CANCEL "\n", i);
        binary_optimize_dump($bin_hp + i);
    }

    LOG_TAB--;
    log_tab_message("}\n");
}

//================================================================================================================================
// BINARY_OPTIMAZE
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

static bool binary_optimize_ctor(binary_optimize *const hp, const int capacity, bin_kth_node *const bin_data,
                                                                                bin_kth_node *const kth_data)
{
    log_verify(hp       != nullptr, false);
    log_verify(bin_data != nullptr, false);
    log_verify(kth_data != nullptr, false);

    $bin_data = bin_data;
    $kth_data = kth_data;

    $size     =        0;
    $capacity = capacity;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert, erase, update
//--------------------------------------------------------------------------------------------------------------------------------

static bool binary_optimize_insert(binary_optimize *const hp, const int key, const int kth_pos)
{
    log_verify(hp       != nullptr, false);
    log_verify(kth_pos >         0, false);
    log_verify($size   < $capacity, false);

    $bin_data[$size]       = {key, kth_pos};
    $kth_data[kth_pos].pos = $size;

    $size++;

    return binary_optimize_sift_up(hp, $size - 1);
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool binary_optimize_update_key(binary_optimize *const hp, const int bin_pos, const int new_key)
{
    log_verify(hp !=   nullptr, false);
    log_verify(bin_pos < $size, false);

    int old_key = $bin_data[bin_pos].key;
    int kth_pos = $bin_data[bin_pos].pos;

    $bin_data[bin_pos].key = new_key;
    $kth_data[kth_pos].key = new_key;

    if (new_key > old_key)  return binary_optimize_sift_down(hp, bin_pos);
    /* else */              return binary_optimize_sift_up  (hp, bin_pos);
}

//--------------------------------------------------------------------------------------------------------------------------------
// min
//--------------------------------------------------------------------------------------------------------------------------------

static bin_kth_node binary_optimize_get_min(binary_optimize *const hp)
{
    log_verify(hp != nullptr, {0 COMMA 0});

    if ($size == 0) return {INF, INF};

    return $bin_data[0];
}

//--------------------------------------------------------------------------------------------------------------------------------

static bin_kth_node binary_optimize_extract_min(binary_optimize *const hp)
{
    log_verify(hp != nullptr, {0 COMMA 0});
    log_verify($size > 0    , {0 COMMA 0});

    if ($size == 1) { $size = 0; return $bin_data[0]; }

    bin_kth_node ans = $bin_data[0];          $size--;
    bin_kth_node_swap ($bin_data, $bin_data + $size);

    $kth_data[$bin_data[0].pos].pos = 0;

    binary_optimize_sift_down(hp, 0);
    return ans;
}

//--------------------------------------------------------------------------------------------------------------------------------
// sift
//--------------------------------------------------------------------------------------------------------------------------------

static bool binary_optimize_sift_up(binary_optimize *const hp, const int bin_pos)
{
    log_verify(hp != nullptr, false);
    log_verify(bin_pos >=  0, false);

    if (bin_pos == 0) return true;

    const int bin_par_pos = (bin_pos - 1) / 2;
    const int kth_par_pos = $bin_data[bin_par_pos].pos;
    const int     par_key = $bin_data[bin_par_pos].key;

    const int kth_pos = $bin_data[bin_pos].pos;
    const int     key = $bin_data[bin_pos].key;

    if (par_key > key)
    {
        bin_kth_node_swap($bin_data + bin_par_pos, $bin_data + bin_pos);

        $kth_data[kth_par_pos].pos = bin_pos;
        $kth_data[kth_pos]    .pos = bin_par_pos;

        return binary_optimize_sift_up(hp, bin_par_pos);
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool binary_optimize_sift_down(binary_optimize *const hp, const int bin_pos)
{
    log_verify(hp !=   nullptr, false);
    log_verify(bin_pos < $size, false);

    const int l_bin_pos = 2 *   bin_pos + 1;
    const int r_bin_pos = 1 + l_bin_pos;

    const int   kth_pos = $bin_data[  bin_pos].pos;

    const int l_kth_pos = (l_bin_pos < $size) ? $bin_data[l_bin_pos].pos : -1;
    const int r_kth_pos = (r_bin_pos < $size) ? $bin_data[r_bin_pos].pos : -1;

    const int   l_key   = (l_bin_pos < $size) ? $bin_data[l_bin_pos].key : INF;
    const int   r_key   = (r_bin_pos < $size) ? $bin_data[r_bin_pos].key : INF;
    const int bin_key   =                       $bin_data[  bin_pos].key      ;

    if (l_key <= bin_key && l_key <= r_key) {   bin_kth_node_swap($bin_data + l_bin_pos, $bin_data + bin_pos);

                                                $kth_data[l_kth_pos].pos =   bin_pos;
                                                $kth_data[  kth_pos].pos = l_bin_pos;

                                                return binary_optimize_sift_down(hp, l_bin_pos);
                                            }
    if (r_key <= bin_key)                   {   bin_kth_node_swap($bin_data + r_bin_pos, $bin_data + bin_pos);

                                                $kth_data[r_kth_pos].pos =   bin_pos;
                                                $kth_data[  kth_pos].pos = r_bin_pos;

                                                return binary_optimize_sift_down(hp, r_bin_pos);
                                            }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

static void binary_optimize_dump(binary_optimize *const hp)
{
    log_tab_message(HTML_COLOR_MEDIUM_BLUE
                    "binary_optimize (addr: %p)\n" HTML_COLOR_CANCEL
                    "{\n",                  hp);
    if (hp == nullptr) { log_tab_message("}\n"); return; }
    LOG_TAB++;

    log_tab_message("bin_data = %p\n", $bin_data);
    if             ($bin_data != nullptr) binary_optimize_bin_data_dump(hp);

    log_tab_message("kth_data = %p\n\n"
                    "size     = %d\n"
                    "capacity = %d\n", $kth_data, $size, $capacity);
    LOG_TAB--;
    log_tab_message("}\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void binary_optimize_bin_data_dump(binary_optimize *const hp)
{
    log_assert(hp        != nullptr);
    log_assert($bin_data != nullptr);

    log_tab_message("{\n");
    LOG_TAB++;

    log_tab_message(HTML_COLOR_MEDIUM_BLUE "index  : ");
    for (int i = 0; i < $size; ++i) log_message("%10d ", i);
    log_message("\n");

    log_tab_message(HTML_COLOR_CANCEL
                    HTML_COLOR_DARK_ORANGE "key    : ");
    for (int i = 0; i < $size; ++i) log_message("%10d ", $bin_data[i].key);
    log_tab_message("\n");

    log_tab_message(HTML_COLOR_CANCEL
                    HTML_COLOR_DARK_RED    "kth_pos: ");
    for (int i = 0; i < $size; ++i) log_message("%10d ", $bin_data[i].pos);
    log_tab_message(HTML_COLOR_CANCEL "\n");

    LOG_TAB--;
    log_tab_message("}\n");
}

//================================================================================================================================
// BIN_KTH_NODE
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

void bin_kth_node_swap(bin_kth_node *const a, bin_kth_node *const b)
{
    log_verify(a != nullptr, (void) 0);
    log_verify(b != nullptr, (void) 0);

    bin_kth_node temp = *a;

    *a =   *b;
    *b = temp;
}
