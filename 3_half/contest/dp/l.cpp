#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    int N = 0,
        M = 0;
    scanf("%d %d", &N, &M);

    int m[N + 1];
    int c[N + 1];

    m[0] = 0;
    c[0] = 0;

    for (int i = 1; i <= N; ++i) scanf("%d", m+i);
    for (int i = 1; i <= N; ++i) scanf("%d", c+i);

    int dp[N + 1][M + 1];
    for (int i = 0; i <= N; ++i)
        for (int j = 0; j <= M; ++j)
            dp[i][j] = 0;

    for (int i = 1; i <= N; ++i)
    {
        for (int j = 1; j <= M; ++j)
        {
            dp[i][j] = dp[i - 1][j];

            if (j < m[i]) continue;
            dp[i][j] = std::max(dp[i][j], dp[i - 1][j - m[i]] + c[i]);
        }
    }
/*
    for (int i = 0; i <= N; ++i)
    {
        for (int j = 0; j <= M; ++j)
        {
            printf("%d ", dp[i][j]);
        }
        printf("\n");
    }
*/
    int i_ans = N;
    int j_ans = M;
    std::vector <int> ans;

    for (int j = 0; j <= M; ++j)
    {
        if (dp[N][j] > dp[N][j_ans])
            j_ans = j;
    }

    while (i_ans > 0 && j_ans > 0)
    {
        if (m[i_ans] > j_ans)
        {
            i_ans--;
            continue;
        }
        if (dp[i_ans - 1][j_ans - m[i_ans]] + c[i_ans] >
            dp[i_ans - 1][j_ans])
        {
            ans.push_back(i_ans);
            j_ans -= m[i_ans];
            i_ans--;
        }
        else
        {
            i_ans--;
        }
    }

    std::reverse(ans.begin(), ans.end());
    for (int i = 0; i < ans.size(); ++i)
        printf("%d ", ans[i]);
    printf("\n");
}
