#include <iostream>

const int INF = 2e9;

int main()
{
    int n = 0, m = 0;
    scanf("%d %d", &n, &m);

    int a[n + 1];
    int b[m + 1];

    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    for (int j = 1; j <= m; ++j) scanf("%d", b + j);

    int dp[n + 1][m + 1];
    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= m; ++j)
            dp[i][j] = 0;

    for (int i = 1; i <= n; ++i)
    {
        int best = 0;
        for (int j = 1; j <= m; ++j)
        {
            dp[i][j] = dp[i - 1][j];
            if (a[i] == b[j]) dp[i][j] = std::max(dp[i][j], best + 1);

            if (b[j] < a[i]) best = std::max(best, dp[i - 1][j]);
        }
    }

    int ans = dp[n][m];
    for (int i = 1; i <= m; ++i)
        ans = std::max(ans, dp[n][i]);

    printf("%d\n", ans);
}
