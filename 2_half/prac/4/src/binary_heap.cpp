#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "math.h"

#include "../../../../lib/logs/log.h"

//--------------------------------------------------------------------------------------------------------------------------------
// function declaration
//--------------------------------------------------------------------------------------------------------------------------------

#include "binary_heap.h"

static bool bin_heap_sift_up  (binary_heap *const hp, const int ind);
static bool bin_heap_sift_down(binary_heap *const hp, const int ind);

//--------------------------------------------------------------------------------------------------------------------------------
// dsl
//--------------------------------------------------------------------------------------------------------------------------------

#define $data       hp->data
#define $size       hp->size
#define $capacity   hp->capacity

//--------------------------------------------------------------------------------------------------------------------------------
// body
//--------------------------------------------------------------------------------------------------------------------------------

bool bin_heap_ctor(binary_heap *const hp, const size_t capacity)
{
    log_verify(hp != nullptr, false);
    log_verify(capacity > 0 , false);

    $data = (int *) log_calloc(capacity + 1 /* fictional */, sizeof(int));
    if ($data == nullptr)
    {
        log_error("Can't allocate memory for binary_heap.data:\n"
                  "log_calloc(capacity + 1 (fictional) = %lu, sizeof(int) = %lu) returns nullptr\n", capacity + 1, sizeof(int));
        return false;
    }

    $capacity = capacity;
    $size     =        0;

    return true;
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

    $size++;
    $data[$size] = val;

    return bin_heap_sift_up(hp, (int) $size);
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
    int_swap ($data + 1, $data + $size);
    $size--;

    if ($size == 0) return true;

    bin_heap_sift_down(hp, 1);
    return ans;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool bin_heap_sort(binary_heap *const hp, int *const arr, const int arr_size)
{
    log_verify(arr != nullptr, false);
    log_verify(arr_size   > 0, false);

    for (int i = 0; i < arr_size; ++i)          bin_heap_insert     (hp, arr[i]);
    for (int i = 0; i < arr_size; ++i) arr[i] = bin_heap_extract_min(hp);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool bin_heap_sift_up(binary_heap *const hp, const int ind)
{
    log_verify(hp  !=     nullptr, false);
    log_verify(ind >            0, false);
    log_verify(ind <= (int) $size, false);

    if (ind == 1) return true;

    if ($data[ind / 2] > $data[ind])
    {
        int_swap($data + ind/2, $data + ind);
        return bin_heap_sift_up(hp, ind/2);
    }

    return true;
}

static bool bin_heap_sift_down(binary_heap *const hp, const int ind)
{
    log_verify(hp !=      nullptr, false);
    log_verify(ind >            0, false);
    log_verify(ind <= (int) $size, false);

    int left  = (2*ind     >= (int) $size) ? 0 : $data[2*ind];
    int right = (2*ind + 1 >= (int) $size) ? 0 : $data[2*ind + 1];

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

    unsigned temp = *a;
    *a =   *b;
    *b = temp;
}