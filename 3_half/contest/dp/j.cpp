#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>

int main()
{
    char s1[1000] = "";
    char s2[1000] = "";

    scanf("%s %s", s1, s2);

    int n = strlen(s1);
    int m = strlen(s2);

    int dp[n + 1][m + 1];
    std::vector<int> v1;
    std::vector<int> v2;

    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= m; ++j)
        {
            dp[i][j] = 0;
        }
    }

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);

            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = std::max(dp[i][j], dp[i - 1][j - 1] + 1);
        }
    }

    int i = n;
    int j = m;

    while (i != 0 && j != 0)
    {
        if ((dp[i][j] == dp[i - 1][j - 1] + 1) && (s1[i - 1] == s2[j - 1]))
        {
            v1.push_back(i);
            v2.push_back(j);

            --i;
            --j;
        }
        else
        {
            if (dp[i][j] == dp[i - 1][j]) --i;
            else                          --j;
        }
    }

    std::reverse(v1.begin(), v1.end());
    std::reverse(v2.begin(), v2.end());
/*
    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= m; ++j)
        {
            printf("%d ", dp[i][j]);
        }
        printf("\n");
    }
*/
    printf("%d\n", dp[n][m]);

    for (int i = 0; i < dp[n][m]; ++i)
        printf("%d ", v1[i]);
    printf("\n");

    for (int i = 0; i < dp[n][m]; ++i)
        printf("%d ", v2[i]);
    printf("\n");
}
