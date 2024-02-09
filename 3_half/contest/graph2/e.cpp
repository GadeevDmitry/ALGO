#include <stdio.h>
#include <vector>

const int INF       = 100000;
const int MINUS_INF = -1e9;

static void recovery(int from, int to, std::vector<std::vector<int>> &p, std::vector<int> &ans);

signed main()
{
    int n = 0;
    scanf("%d", &n);

    std::vector<std::vector<int>> d1(n, std::vector<int> (n));
    std::vector<std::vector<int>> d2(n, std::vector<int> (n));
    std::vector<std::vector<int>> p (n, std::vector<int> (n, -1));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            scanf("%d", &(d1[i][j]));
            d2[i][j] = d1[i][j];
        }
    }

    for (int k = 0; k < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (i == k || j == k) continue;

                auto &last = (k % 2 == 0) ? d2 : d1;
                auto &next = (k % 2 == 0) ? d1 : d2;

                if (last[i][k] < INF && last[k][j] < INF)
                {
                    if (last[i][j] >= last[i][k] + last[k][j])
                    {
                        next[i][j] = last[i][k] + last[k][j];
                        next[i][j] = std::max(next[i][j], MINUS_INF);

                        p[i][j] = k;
                    }
                    else
                    {
                        next[i][j] = last[i][j];
                    }
                }
            }
        }
    }

    auto &cur = (n % 2 == 0) ? d2 : d1;

    for (int i = 0; i < n; ++i)
    {
        if (cur[i][i] < 0)
        {
            printf("YES\n");
/*
            printf("i = %d\n\n", i);

            printf("dist:\n");
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    printf("%d ", cur[i][j]);
                }
                printf("\n");
            }

            printf("\npar\n");
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    printf("%d ", p[i][j]);
                }
                printf("\n");
            }
            printf("\n");
*/
            std::vector<int> ans;
            ans.push_back(i);
            recovery(i, i, p, ans);
            ans.push_back(i);

            printf("%lu\n", ans.size());
            for (auto elem : ans)
                printf("%d ", elem + 1);
            printf("\n");

            return 0;
        }
    }

    printf("NO\n");
}

static void recovery(int from, int to, std::vector<std::vector<int>> &p, std::vector<int> &ans)
{
    if (p[from][to] == -1)
        return;

    recovery(from, p[from][to], p, ans);
    ans.push_back(p[from][to]);
    recovery(p[from][to], to, p, ans);
}
