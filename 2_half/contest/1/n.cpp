#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#include "../../../lib/logs/log.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

struct heap
{
    unsigned *data;

    size_t size;
    size_t capacity;
};

//--------------------------------------------------------------------------------------------------------------------------------
// DSL
//--------------------------------------------------------------------------------------------------------------------------------

#define $data     hp->data
#define $size     hp->size
#define $capacity hp->capacity

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// solve
//--------------------------------------------------------------------------------------------------------------------------------

void     solve           (struct heap *const hp, const int n, const int k, const unsigned a0, const unsigned x, const unsigned y);
unsigned get_another_elem(const unsigned prev, const unsigned x, const unsigned y);

//--------------------------------------------------------------------------------------------------------------------------------
// output
//--------------------------------------------------------------------------------------------------------------------------------

void output(struct heap *const hp);

//--------------------------------------------------------------------------------------------------------------------------------
// heap
//--------------------------------------------------------------------------------------------------------------------------------

int      heap_ctor       (struct heap *const hp, const size_t capacity);
void     heap_dtor       (struct heap *const hp);
int      heap_insert     (struct heap *const hp, const unsigned val);
unsigned heap_get_max    (struct heap *const hp);
int      heap_extract_max(struct heap *const hp);
int      heap_sift_up    (struct heap *const hp, const int ind);
int      heap_sift_down  (struct heap *const hp, const int ind);
//void     heap_dump (const heap *const hp);

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

void unsigned_swap(unsigned *const a, unsigned *const b);

//================================================================================================================================
// MAIN
//================================================================================================================================

const int min_inf = 0;

int main()
{
    int      n  = 0, k = 0;
    unsigned a0 = 0, x = 0, y = 0;

    if (scanf("%d %d %u %u %u", &n, &k, &a0, &x, &y) != 5) { /* log_error(); */ return 0; }
    if (k > n) k = n;

    struct heap hp;
    if (!heap_ctor(&hp, (size_t) k)) { /* log_error(...); */ return 0; }
    
    //log_tab_message("AFTER CTOR\n");
    //heap_dump(&hp);

    solve (&hp, n, k, a0, x, y);
    output(&hp);
}

//--------------------------------------------------------------------------------------------------------------------------------
// solve
//--------------------------------------------------------------------------------------------------------------------------------

void solve(struct heap *const hp, const int n, const int k, const unsigned a0, const unsigned x, const unsigned y)
{
    //log_verify(hp != nullptr, ;);
    //log_verify(k <= n, ;);

    unsigned cur = a0;

    for (int i = 0; i < k; ++i)
    {
        cur = get_another_elem(cur, x, y);
        heap_insert(hp, cur);
    }

    //log_tab_message("FIRST k = %d elemets:\n", k);
    //heap_dump(hp);

    for (int i = k; i < n; ++i)
    {
        cur = get_another_elem(cur, x, y);
        if (cur < heap_get_max(hp))
        {
            heap_extract_max(hp);
            heap_insert(hp, cur);
        }
    }
}

unsigned get_another_elem(const unsigned prev, const unsigned x, const unsigned y)
{
    unsigned long long cur = (x * prev + y) % (1ull << 30);
    return (unsigned)  cur;
}


//--------------------------------------------------------------------------------------------------------------------------------
// output
//--------------------------------------------------------------------------------------------------------------------------------

void output(struct heap *const hp)
{
    int size = (int) $size;
    unsigned *arr = (unsigned *) calloc((size_t) size, sizeof(unsigned));

    for (int i = 0; i < size; ++i)
    {
        arr[i] = heap_get_max(hp);
        heap_extract_max(hp);
    }

    for (int i = size - 1; i >= 0; --i) { printf("%u ", arr[i]); }
    printf("\n");

    heap_dtor(hp);
    free(arr);
}

//--------------------------------------------------------------------------------------------------------------------------------
// heap
//--------------------------------------------------------------------------------------------------------------------------------

int heap_ctor(struct heap *const hp, const size_t capacity)
{
    //log_verify(hp != nullptr, 0);

    $data = (unsigned *) calloc(capacity + 1, sizeof(unsigned));  // 1-indexing
    if ($data == NULL) { /* log_error(...); */ return 0; }

    $capacity = capacity;
    $size     =        0;

    return 1;
}

void heap_dtor(struct heap *const hp)
{
    if (hp == NULL) return;

    free($data);
}

//--------------------------------------------------------------------------------------------------------------------------------

int heap_insert(struct heap *const hp, const unsigned val)
{
    //log_verify(hp != nullptr, 0);
    //log_verify($size < $capacity, 0);

    $size++;
    $data[$size] = val;

    return heap_sift_up(hp, (int) $size);
}

//--------------------------------------------------------------------------------------------------------------------------------

unsigned heap_get_max(struct heap *const hp)
{
    //log_verify(hp != nullptr, 0);
    //log_verify($size != 0, 0);

    return $data[1];
}

int heap_extract_max(struct heap *const hp)
{
    //log_verify(hp != nullptr, 0);
    //log_verify($size != 0, 0);

    unsigned_swap($data + 1, $data + $size);
    $size--;

    if ($size == 0) return 1;
    return heap_sift_down(hp, 1);
}

//--------------------------------------------------------------------------------------------------------------------------------

int heap_sift_up(struct heap *const hp, const int ind)
{
    //log_verify(hp != nullptr, 0);
    //log_verify(ind > 0, 0);
    //log_verify(ind <= (int) $size, 0);

    if (ind == 1) return 1;

    if ($data[ind / 2] < $data[ind])
    {
        unsigned_swap($data + ind/2, $data + ind);
        return heap_sift_up(hp, ind/2);
    }
    return 1;
}

int heap_sift_down(struct heap *const hp, const int ind)
{
    //log_verify(hp != nullptr, 0);
    //log_verify(ind > 0, 0);
    //log_verify(ind <= (int) $size, 0);

    unsigned left  = (2*ind     > (int) $size) ? 0 : $data[2*ind];
    unsigned right = (2*ind + 1 > (int) $size) ? 0 : $data[2*ind + 1];

    if (left  > $data[ind] && left  >= right) { unsigned_swap($data + 2*ind    , $data + ind); return heap_sift_down(hp, 2*ind    ); }
    if (right > $data[ind] && right >=  left) { unsigned_swap($data + 2*ind + 1, $data + ind); return heap_sift_down(hp, 2*ind + 1); }

    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
void heap_dump(const heap *const hp)
{
    log_tab_message("heap(addrr: %p)\n"
                    "{\n", hp);

    if (hp == nullptr) { log_tab_message("}\n"); return; }

    LOG_TAB++;
    log_tab_message("size    : %llu\n"
                    "capacity: %llu\n"
                    "data    : ", $size, $capacity);

    for (int i = 1; i <= (int) $size; ++i) { log_message("%llu ", $data[i]); }

    LOG_TAB--;
    log_tab_message("\n}\n");
}
*/

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

void unsigned_swap(unsigned *const a, unsigned *const b)
{
    //log_verify(a != nullptr, ;);
    //log_verify(b != nullptr, ;);

    unsigned temp = *a;
    *a =   *b;
    *b = temp;
}
