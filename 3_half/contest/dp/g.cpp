#include <stdio.h>

#define int long long

signed main()
{
    int n = 0;
    scanf("%d", &n);

    int dp[n + 1][n + 1];
    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= n; ++j)
        {
            if (i == j) dp[i][j] = 1;
            else        dp[i][j] = 0;
        }
    }

    for (int i = 1; i <= n; ++i)
    {
        for (int j = i - 1; j > 0; --j)
        {
            for (int k = 2*j; k <= i - j; ++k)
                dp[i][j] += dp[i - j][k];
        }
    }
/*
    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= n; ++j)
            printf("%d ", dp[i][j]);
        printf("\n");
    }
*/
    int ans = 0;
    for (int j = 0; j <= n; ++j) ans += dp[n][j];

    printf("%lld\n", ans);
}
