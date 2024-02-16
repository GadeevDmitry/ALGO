#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//==================================================================================================

#define MATRIX_INDEX(matrix, size, y, x) *(matrix + size*y + x)

typedef unsigned long long ull;
typedef unsigned char      uch;

const ull MOD = 1000003;

//==================================================================================================

static ull get_path_num(const ull n);

static void matrix_mul(const ull *matrix_1, const ull *matrix_2, ull *const matrix_res, const size_t n);
static void matrix_pow(const ull *matrix, ull *const matrix_res, const size_t n, const size_t p);

//==================================================================================================

int main()
{
    ull n = 0;
    if (scanf("%llu", &n) != 1)
        return 1;

    printf("%llu\n", get_path_num(n));
}

//--------------------------------------------------------------------------------------------------

static ull get_path_num(const ull n)
{
    if (n == 1)
        return 1;

    const uch max_jmp_len = 5;
    const ull dp_base[max_jmp_len][max_jmp_len] = {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0}};

    ull dp_res[max_jmp_len][max_jmp_len] = {};
    matrix_pow((ull *) dp_base, (ull *) dp_res, 5, n - 1);

    return dp_res[0][0];
}

//--------------------------------------------------------------------------------------------------

static void matrix_mul(const ull *matrix_1, const ull *matrix_2, ull *const matrix_res, const size_t n)
{
    assert(matrix_1 != matrix_res);
    assert(matrix_2 != matrix_res);

    for (size_t y = 0; y < n; ++y) {
    for (size_t x = 0; x < n; ++x)
        {
            MATRIX_INDEX(matrix_res, n, y, x) = 0;

            for (size_t i = 0; i < n; ++i)
                MATRIX_INDEX(matrix_res, n, y, x) += MATRIX_INDEX(matrix_1, n, y, i) *
                                                     MATRIX_INDEX(matrix_2, n, i, x);
                MATRIX_INDEX(matrix_res, n, y, x) %= MOD;
        }
    }
}

//--------------------------------------------------------------------------------------------------

static void matrix_pow(const ull *matrix, ull *const matrix_res, const size_t n, const size_t p)
{
    assert(matrix != matrix_res);
    assert(p > 0);

    if (p == 1)
    {
        memcpy(matrix_res, matrix, n*n*sizeof(ull));
        return;
    }

    ull matrix_same[n][n];
    memcpy(matrix_same, matrix, n*n*sizeof(ull));

    ull matrix_square[n][n];
    matrix_mul(matrix, matrix, (ull *) matrix_square, n);

    ull matrix_recursive[n][n];
    matrix_pow((ull *) matrix_square, (ull *) matrix_recursive, n, p/2);

    if (p % 2) matrix_mul((ull *) matrix_same, (ull *) matrix_recursive, matrix_res, n);
    else       memcpy    (matrix_res, matrix_recursive, n*n*sizeof(ull));
}
