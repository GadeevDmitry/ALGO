#include <stdio.h>
#include <vector>

#define int long long

const int INF       = 100000;
const int MINUS_INF = -1e9;

static void recovery(int from, int to, std::vector<std::vector<int>> &p, std::vector<int> &ans);

signed main()
{
    int n = 0;
    scanf("%lld", &n);

    std::vector<std::vector<int>> d(n, std::vector<int> (n));
    std::vector<std::vector<int>> p(n, std::vector<int> (n, -1));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            scanf("%lld", &(d[i][j]));
        }
    }

    for (int k = 0; k < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (i == k || j == k) continue;

                if (d[i][k] < INF && d[k][j] < INF)
                {
                    if (d[i][j] >= d[i][k] + d[k][j])
                    {
                        d[i][j] = d[i][k] + d[k][j];
                        d[i][j] = std::max(d[i][j], MINUS_INF);
                        p[i][j] = (p[i][k] == -1) ? k : p[i][k];
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (d[i][i] < 0)
        {
            printf("YES\n");
/*
            printf("i = %d\n", i);

            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    printf("%d ", d[i][j]);
                }
                printf("\n");
            }

            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    printf("%d ", p[i][j]);
                }
                printf("\n");
            }
*/
            std::vector<int> ans;
            ans.push_back(i);
            for (int next = p[i][i]; next != i && next != -1; next = p[next][i])
                ans.push_back(next);
            ans.push_back(i);

            printf("%d\n", ans.size());
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
