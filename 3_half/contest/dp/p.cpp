#include <algorithm>
#include <iostream>
#include <vector>

const int INF = 2e9;

signed main()
{
    unsigned n = 0;
    scanf("%u", &n);

    unsigned d[n + 1][n + 1];
    for (unsigned i = 1; i <= n; ++i)
    {
        for (unsigned j = 1; j <= n; ++j)
        {
            scanf("%u", &d[i][j]);
        }
    }

    unsigned pow_n = (1 << (n + 1));

    unsigned dp[pow_n][n + 1];
    unsigned p [pow_n][n + 1];

    for (unsigned i = 0; i < pow_n; ++i)
    {
        for (unsigned j = 0; j <= n; ++j)
        {
            dp[i][j] = INF;
            p [i][j] = INF;
        }
    }

    dp[0][0] = 0;

    for (unsigned i = 0; i < pow_n; ++i)
    {
        if (i & 1) continue;

        for (unsigned j = 1; j <= n; ++j)
        {
            if (((1 << j) & i) == 0) continue;

            unsigned left = i ^ (1 << j);
            if (left == 0)
            {
                dp[i][j] = 0;
                continue;
            }

            for (unsigned k = 1; k <= n; ++k)
            {
                if (((1 << k) & left) == 0) continue;

                if (dp[i][j] > dp[left][k] + d[k][j])
                {
                    dp[i][j] = dp[left][k] + d[k][j];
                    p [i][j] = k;
                }
            }
        }
    }

    unsigned ans_end = n;
    unsigned pow_end = pow_n - 2;

    for (unsigned j = 1; j <= n; ++j)
    {
        if (dp[pow_end][j] <
            dp[pow_end][ans_end])
        {
            ans_end = j;
        }
    }

    unsigned path = dp[pow_end][ans_end];
    std::vector <unsigned> ans;
    ans.push_back(ans_end);

    while (pow_end != 0)
    {
        unsigned left = pow_end ^ (1 << ans_end);
        if (left == 0) break;

        for (unsigned i = 1; i <= n; ++i)
        {
            if (((1 << i) & left) == 0) continue;

            if (dp[pow_end][ans_end] == dp[left][i] + d[i][ans_end])
            {
                ans.push_back(i);
                pow_end = left;
                ans_end = i;
                break;
            }
        }
//      printf("ERROR in answer restoring\n");
    }

    std::reverse(ans.begin(), ans.end());

    printf("%u\n", path);
    for (unsigned i = 0; i < ans.size(); ++i)
        printf("%u ", ans[i]);
    printf("\n");
}
