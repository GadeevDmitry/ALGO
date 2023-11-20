#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static void dfs (std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &top_sort, std::vector<unsigned> &used, unsigned cur);
static void dfs2(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &components, unsigned cur, unsigned cur_component);

//==================================================================================================

int main()
{
    unsigned N, M;
    scanf("%u %u", &N, &M);

    std::vector<std::vector<unsigned>> g (N);
    std::vector<std::vector<unsigned>> gt(N);
    for (unsigned i = 0; i < M; ++i)
    {
        unsigned a, b;
        scanf("%u %u", &a, &b);

        g [a - 1].push_back(b - 1);
        gt[b - 1].push_back(a - 1);
    }

    std::vector<unsigned> top_sort;
    std::vector<unsigned> components(N, 0);

    for (unsigned cur = 0; cur < N; ++cur)
    {
        if (!components[cur])
            dfs(g, top_sort, components, cur);
    }
    std::reverse(top_sort.begin(), top_sort.end());

    components.assign(N, 0);
    unsigned component_cnt = 1;

    for (unsigned elem: top_sort)
    {
        if (!components[elem])
        {
            dfs2(gt, components, elem, component_cnt);
            component_cnt++;
        }
    }

    printf("%u\n", component_cnt - 1);
    for (auto elem: components)
        printf("%u ", elem);
    printf("\n");
}

//--------------------------------------------------------------------------------------------------

static void dfs(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &top_sort, std::vector<unsigned> &used, unsigned cur)
{
    used[cur] = 1;
    for (unsigned next: g[cur])
    {
        if (!used[next])
            dfs(g, top_sort, used, next);
    }

    top_sort.push_back(cur);
}

//--------------------------------------------------------------------------------------------------

static void dfs2(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &components, unsigned cur, unsigned cur_component)
{
    components[cur] = cur_component;
    for (unsigned next: g[cur])
    {
        if (!components[next])
            dfs2(g, components, next, cur_component);
    }
}
