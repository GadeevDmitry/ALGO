#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_INDEX(matrix, size, y, x) *(matrix + size*y + x)
#define int    long long
#define size_t long long
typedef long long ull;

const ull MOD = 1000000009;

void matrix_mul(const ull *matrix_1, const ull *matrix_2, ull *const matrix_res, const size_t n)
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

void matrix_pow(const ull *matrix, ull *const matrix_res, const size_t n, const size_t p)
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

long long fib(long long n)
{
    if (n <= 1) return 0;
    if (n == 2) return 1;

    ull dp_base[2][2] = {0, 1, 1, 1};
    ull dp_res [2][2];
    matrix_pow((ull *) dp_base, (ull *) dp_res, 2, n - 1);

    return dp_res[0][1];
}

signed main()
{
    int n, q;
    scanf("%lld %lld", &n, &q);

    long long h_vec[n];
    long long k_vec[n];
    long long c_vec[q];

    for (int i = 0; i < n; ++i)
    {
        long long x, y, k;
        scanf("%lld %lld %lld", &x, &y, &k);

        h_vec[i] = y - x;
        k_vec[i] = k;
    }

//  for (int i = 0; i < n; ++i)
//      printf("h = %lld\nk = %lld\n", h_vec[i], k_vec[i]);

    for (int i = 0; i < q; ++i)
    {
        scanf("%lld", c_vec+i);
    }

    for (int i = 0; i < q; ++i)
    {
        long long ans = 0;

        for (int j = 0; j < n; ++j)
        {
            if (h_vec[j] < c_vec[i])
                continue;

            ans += ((fib((h_vec[j] - c_vec[i]) % MOD + 2) % MOD) * (k_vec[j] % MOD)) % MOD;
            ans %= MOD;
        }
        printf("%lld\n", ans);
    }
}
