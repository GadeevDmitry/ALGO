#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    int N = 0, G = 0, K = 0;
    scanf("%d %d %d", &N, &G, &K);

    std::vector<std::vector<int>> k_vec(G);
    std::vector<std::vector<int>> v_vec(G);

    for (int i = 0; i < N; ++i)
    {
        int k = 0, v = 0, g = 0;
        scanf("%d %d %d", &k, &v, &g);

        k_vec[g - 1].push_back(k);
        v_vec[g - 1].push_back(v);
    }

    int dp[G + 1][K + 1];

    for (int i = 0; i <= G; ++i)
        for (int j = 0; j <= K; ++j)
            dp[i][j] = 0;

    for (int i = 1; i <= G; ++i)
    {
        for (int j = 1; j <= K; ++j)
        {
            dp[i][j] = dp[i - 1][j];

            for (int k = 0; k < k_vec[i - 1].size(); ++k)
            {
                if (k_vec[i - 1][k] > j) continue;

                dp[i][j] = std::max(dp[i][j], dp[i - 1][j - k_vec[i - 1][k]] + v_vec[i - 1][k]);
            }
        }
    }

    int ans = dp[G][K];
    for (int i = 1; i <= K; ++i)
        ans = std::max(ans, dp[G][i]);

    printf("%d\n", ans);
}
