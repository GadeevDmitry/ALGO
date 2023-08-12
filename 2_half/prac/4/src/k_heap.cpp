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

//--------------------------------------------------------------------------------------------------------------------------------
// function declaration
//--------------------------------------------------------------------------------------------------------------------------------

#include "k_heap.h"

static bool k_heap_sift_up  (k_heap *const hp, const size_t ind);
static bool k_heap_sift_down(k_heap *const hp, const size_t ind);

static void int_swap(int *const a, int *const b);

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

k_heap *k_heap_new(const size_t k, const size_t capacity)
{
    k_heap *hp = (k_heap *) log_calloc(1, sizeof(k_heap));
    log_verify(hp != nullptr, nullptr);

    if (!k_heap_ctor(hp, k, capacity)) { log_free(hp); return nullptr; }
    return hp;
}

bool k_heap_ctor(k_heap *const hp, const size_t k, const size_t capacity)
{
    log_verify(hp != nullptr, false);

    $data = (int *) log_calloc((size_t) capacity, sizeof(int));
    log_verify($data != nullptr, false);

    $k        = k;
    $capacity = capacity;
    $size     = 0;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void k_heap_free(k_heap *const hp)
{
    k_heap_dtor(hp);
    log_free   (hp);
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
    log_verify(hp    != nullptr, 0);
    log_verify($size !=       0, 0);

    int ans = $data[0];
    int_swap ($data, $data + $size - 1);
    $size--;

    if ($size == 0) return ans;

    k_heap_sift_down(hp, 0);
    return ans;
}

//--------------------------------------------------------------------------------------------------------------------------------

void k_heap_sort(int *const arr, const size_t size, const size_t k)
{
    log_verify(arr != nullptr, (void) 0);

    k_heap *hp = k_heap_new(k, size);

    for (size_t i = 0; i < size; ++i)          k_heap_insert(hp, arr[i]);
    for (size_t i = 0; i < size; ++i) arr[i] = k_heap_extract_min(hp);

    k_heap_free(hp);
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool k_heap_sift_up(k_heap *const hp, const size_t ind)
{
    log_verify(hp  != nullptr, false);
    log_verify(ind <    $size, false);

    if (ind == 0) return true;

    size_t par_ind = (ind - 1) / $k;
    if ($data[par_ind] > $data[ind])
    {
        int_swap($data + ind, $data + par_ind);
        return k_heap_sift_up    (hp, par_ind);
    }
    return true;
}

static bool k_heap_sift_down(k_heap *const hp, const size_t ind)
{
    log_verify(hp  != nullptr, false);
    log_verify(ind <    $size, false);

    int    min_val =  2e9;
    size_t min_ind = -1UL;

    size_t min_child_ind = $k *  ind + 1;
    size_t max_child_ind = $k * (ind + 1);

    for (size_t i = min_child_ind; i <= max_child_ind && i < $size; ++i)
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

    int temp = *a;
    *a =   *b;
    *b = temp;
}
