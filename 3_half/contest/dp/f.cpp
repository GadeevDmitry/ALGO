#include <stdio.h>

int main()
{
    int n = 0,
        k = 0;

    scanf("%d %d", &k, &n);

    if (n == 0 && k > 1)
    {
        printf("-1\n");
        return 0;
    }

    if (k == 1)
    {
        printf("0\n");
        return 0;
    }

    k--;
    int dp[n + 1];
    for (int i = 0; i <= n; ++i) dp[i] = 0;

    int m = 0;
    for (; dp[n] < k; ++m)
    {
        for (int x = n; x > 0; --x)
        {
            dp[x] += 1 + dp[x - 1];
        }
    }

    printf("%d\n", m);
}
