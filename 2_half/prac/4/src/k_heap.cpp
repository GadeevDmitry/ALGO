#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "math.h"

#include "../../../../lib/logs/log.h"

//--------------------------------------------------------------------------------------------------------------------------------
// function declaration
//--------------------------------------------------------------------------------------------------------------------------------

#include "k_heap.h"

static bool k_heap_sift_up  (k_heap *const hp, const int ind);
static bool k_heap_sift_down(k_heap *const hp, const int ind);

//--------------------------------------------------------------------------------------------------------------------------------
// dsl
//--------------------------------------------------------------------------------------------------------------------------------

#define $k          hp->k
#define $data       hp->data
#define $size       hp->size
#define $capacity   hp->capacity

//--------------------------------------------------------------------------------------------------------------------------------
// body
//--------------------------------------------------------------------------------------------------------------------------------

bool k_heap_ctor(k_heap *const hp, const int k, const size_t capacity)
{
    log_verify(hp != nullptr, false);
    log_verify(k        > 0 , false);
    log_verify(capacity > 0 , false);

    $data = (int *) log_calloc(capacity, sizeof(int));
    if ($data == nullptr)
    {
        log_error("Can't allocate memory for k_heap.data:\n"
                  "log_calloc(capacity = %lu, sizeof(int) = %lu) returns nullptr\n", capacity, sizeof(int));
        return false;
    }

    $k        = k;
    $capacity = capacity;
    $size     = 0;

    return true;
}

void k_heap_dtor(k_heap *const hp)
{
    if (hp == nullptr) return;

    log_free($data);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool k_heap_insert(k_heap *const hp, const int val)
{
    log_verify(hp != nullptr    , false);
    log_verify($size < $capacity, false);

    $data[$size] = val;
    $size++;

    return k_heap_sift_up(hp, $size - 1);
}

//--------------------------------------------------------------------------------------------------------------------------------

int k_heap_get_min(k_heap *const hp)
{
    log_verify(hp    != nullptr, 0);
    log_verify($size !=       0, 0);

    return $data[0];
}

int k_heap_extract_min(k_heap *const hp)
{
    log_verify(hp    != nullptr, false);
    log_verify($size !=       0, false);

    int ans = $data[0];
    int_swap ($data, $data + $size - 1);
    $size--;

    if ($size == 0) return true;

    k_heap_sift_down(hp, 0);
    return ans;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool k_heap_sort(k_heap *const hp, int *const arr, const int arr_size)
{
    log_verify(hp != nullptr, false);
    log_verify(arr_size >  0, false);

    for (int i = 0; i < arr_size; ++i)          k_heap_insert     (hp, arr[i]);
    for (int i = 0; i < arr_size; ++i) arr[i] = k_heap_extract_min(hp);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool k_heap_sift_up(k_heap *const hp, const int ind)
{
    log_verify(hp  !=    nullptr, false);
    log_verify(ind >=          0, false);
    log_verify(ind < (int) $size, false);

    if (ind == 0) return true;

    const int par_ind = (ind - 1) / $k;
    if ($data[par_ind] > $data[ind])
    {
        int_swap($data + ind, $data + par_ind);
        return k_heap_sift_up    (hp, par_ind);
    }
    return true;
}

static bool k_heap_sift_down(k_heap *const hp, const int ind)
{
    log_verify(hp  !=    nullptr, false);
    log_verify(ind >=          0, false);
    log_verify(ind < (int) $size, false);

    int min_val = 2e9;
    int min_ind =  -1;

    const int min_child_ind = $k *  ind + 1;
    const int max_child_ind = $k * (ind + 1);

    for (int i = min_child_ind; i <= max_child_ind && i < $size; ++i)
    {
        if ($data[i] < min_val) { min_val = $data[i]; min_ind = i; }
    }

    if ($data[ind] > min_val)
    {
        int_swap($data + ind, $data + min_ind);
        return k_heap_sift_down  (hp, min_ind);
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

static void int_swap(int *const a, int *const b)
{
    log_verify(a != nullptr, ;);
    log_verify(b != nullptr, ;);

    unsigned temp = *a;
    *a =   *b;
    *b = temp;
}