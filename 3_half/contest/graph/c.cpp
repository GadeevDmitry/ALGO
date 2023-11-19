#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static bool dfs(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &top_sort, std::vector<unsigned> &colors, unsigned cur);

//==================================================================================================

int main()
{
    unsigned N, M;
    scanf("%u %u", &N, &M);

    std::vector<std::vector<unsigned>> g(N);
    for (unsigned i = 0; i < M; ++i)
    {
        unsigned a, b;
        scanf("%u %u", &a, &b);

        g[a - 1].push_back(b - 1);
    }

    std::vector<unsigned> top_sort;
    std::vector<unsigned> colors(N, 0);

    bool res = true;
    for (unsigned cur = 0; cur < N; ++cur)
    {
        if (colors[cur] != 0)
            continue;

        if (!dfs(g, top_sort, colors, cur))
        {
            res = false;
            break;
        }
    }

    if (!res)
    {
        printf("-1\n");
        return 0;
    }

    std::reverse(top_sort.begin(), top_sort.end());
    for (auto elem: top_sort)
        printf("%u ", elem + 1);
    printf("\n");
}

//--------------------------------------------------------------------------------------------------

static bool dfs(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &top_sort, std::vector<unsigned> &colors, unsigned cur)
{
    colors[cur] = 1;
    for (unsigned next: g[cur])
    {
        switch (colors[next])
        {
            case 0 : if (!dfs(g, top_sort, colors, next)) return false;
                     break;

            case 1 : return false;
            case 2 : break;
            default: return false;
        }
    }
    colors[cur] = 2;

    top_sort.push_back(cur);
    return true;
}
