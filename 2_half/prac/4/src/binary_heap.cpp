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

#include "binary_heap.h"

static bool bin_heap_sift_up  (binary_heap *const hp, const size_t ind);
static bool bin_heap_sift_down(binary_heap *const hp, const size_t ind);

static void int_swap(int *const a, int *const b);

//--------------------------------------------------------------------------------------------------------------------------------
// dsl
//--------------------------------------------------------------------------------------------------------------------------------

#define $data       (hp->data)
#define $size       (hp->size)
#define $capacity   (hp->capacity)

//--------------------------------------------------------------------------------------------------------------------------------
// body
//--------------------------------------------------------------------------------------------------------------------------------

binary_heap *bin_heap_new(const size_t capacity)
{
    binary_heap *hp = (binary_heap *) log_calloc(1, sizeof(binary_heap));
    log_verify(hp != nullptr, nullptr);

    if (!bin_heap_ctor(hp, capacity)) { log_free(hp); return nullptr; }
    return hp;
}

bool bin_heap_ctor(binary_heap *const hp, const size_t capacity)
{
    log_verify(hp != nullptr, false);

    $data = (int *) log_calloc((size_t) capacity + 1 /* fictional */, sizeof(int));
    log_verify($data != nullptr, false);

    $capacity = capacity + 1; // fictional
    $size     =            1; // fictional

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void bin_heap_free(binary_heap *const hp)
{
    bin_heap_dtor(hp);
    log_free     (hp);
}

void bin_heap_dtor(binary_heap *const hp)
{
    if (hp == nullptr) return;
    log_free($data);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool bin_heap_insert(binary_heap *const hp, const int val)
{
    log_verify(hp != nullptr    , false);
    log_verify($size < $capacity, false);

    $data[$size] = val;
    $size++;

    return bin_heap_sift_up(hp, $size - 1);
}

//--------------------------------------------------------------------------------------------------------------------------------

int bin_heap_get_min(binary_heap *const hp)
{
    log_verify(hp    != nullptr, 0);
    log_verify($size !=       0, 0);

    return $data[1];
}

int bin_heap_extract_min(binary_heap *const hp)
{
    log_verify(hp    != nullptr, 0);
    log_verify($size !=       0, 0);

    int ans = $data[1];
    int_swap ($data + 1, $data + $size - 1);
    $size--;

    if ($size == 0) return ans;

    bin_heap_sift_down(hp, 1);
    return ans;
}

//--------------------------------------------------------------------------------------------------------------------------------

void bin_heap_sort(int *const arr, const size_t size)
{
    log_verify(arr != nullptr, false);

    binary_heap *hp = bin_heap_new(size);

    for (size_t i = 0; i < size; ++i)          bin_heap_insert(hp, arr[i]);
    for (size_t i = 0; i < size; ++i) arr[i] = bin_heap_extract_min(hp);

    bin_heap_free(hp);
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool bin_heap_sift_up(binary_heap *const hp, const size_t ind)
{
    log_verify(hp  != nullptr, false);
    log_verify(ind !=       0, false);
    log_verify(ind <=   $size, false);

    if (ind == 1) return true;

    size_t par_ind = ind / 2;
    if ($data[par_ind] > $data[ind])
    {
        int_swap($data + par_ind, $data + ind);
        return bin_heap_sift_up  (hp, par_ind);
    }
    return true;
}

static bool bin_heap_sift_down(binary_heap *const hp, const size_t ind)
{
    log_verify(hp  != nullptr, false);
    log_verify(ind !=       0, false);
    log_verify(ind <=   $size, false);

    int left  = (2*ind     >= $size) ? (int) 2e9 : $data[2*ind];
    int right = (2*ind + 1 >= $size) ? (int) 2e9 : $data[2*ind + 1];

    if      (left  < $data[ind] && left  <= right) { int_swap($data + 2*ind    , $data + ind); return bin_heap_sift_down(hp, 2*ind    ); }
    else if (right < $data[ind]                  ) { int_swap($data + 2*ind + 1, $data + ind); return bin_heap_sift_down(hp, 2*ind + 1); }

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
