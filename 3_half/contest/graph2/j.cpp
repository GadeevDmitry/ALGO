#include <stdio.h>
#include <vector>

static bool dfs(
    const int n, const std::vector<std::vector<int>> &g,
    std::vector<int> &mt, std::vector<char> &used, int v)
{
    if (used[v]) return false;
    used[v] = true;

    for (auto to : g[v])
    {
        if (mt[to] == -1 || dfs(n, g, mt, used, mt[to]))
        {
            mt[to] = v;
            return true;
        }
    }

    return false;
}

int main()
{
    int n = 0, m = 0;
    scanf("%d %d", &n, &m);

    std::vector<std::vector<int>> g(n);
    for (int i = 0; i < n; ++i)
    {
        int j = 0;
        scanf("%d", &j);

        while (j != 0)
        {
            g[i].push_back(j - 1);
            scanf("%d", &j);
        }
    }

    std::vector<int> mt(m, -1);
    for (int i = 0; i < n; ++i)
    {
        std::vector<char> used(n, false);
        dfs(n, g, mt, used, i);
    }

    int total = 0;
    for (int i = 0; i < m; ++i)
        total = (mt[i] == -1) ? total : total + 1;

    printf("%d\n", total);
    for (int i = 0; i < m; ++i)
    {
        if (mt[i] != -1)
            printf("%d %d\n", mt[i] + 1, i + 1);
    }
}
