#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static void input(
    unsigned &N, unsigned &M,
    std::vector<std::vector<unsigned>> &g,
    std::vector<std::vector<unsigned>> &gt);

static void output(
    const unsigned component_cnt,
    const std::vector<unsigned> &components);

static unsigned solve(
    const unsigned N, const unsigned M,
    const std::vector<std::vector<unsigned>> &g,
    const std::vector<std::vector<unsigned>> &gt,

    std::vector<unsigned> &components);

static void dfs (const std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &top_sort, std::vector<unsigned> &used, unsigned cur);
static void dfs2(const std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &components, unsigned cur_component, unsigned cur);

//==================================================================================================

int main()
{
    unsigned N = 0, M = 0;
    std::vector<std::vector<unsigned>> g;
    std::vector<std::vector<unsigned>> gt;
    input(N, M, g, gt);

    std::vector<unsigned> components;
    unsigned component_cnt = solve(N, M, g, gt, components);

    output(component_cnt, components);
}

//--------------------------------------------------------------------------------------------------

static void input(
    unsigned &N, unsigned &M,
    std::vector<std::vector<unsigned>> &g,
    std::vector<std::vector<unsigned>> &gt)
{
    scanf("%u %u", &N, &M);

    g .resize(N);
    gt.resize(N);
    for (unsigned i = 0; i < M; ++i)
    {
        unsigned a, b;
        scanf("%u %u", &a, &b);

        g [a - 1].push_back(b - 1);
        gt[b - 1].push_back(a - 1);
    }
}

//--------------------------------------------------------------------------------------------------

static void output(
    const unsigned component_cnt,
    const std::vector<unsigned> &components)
{
    printf("%u\n", component_cnt - 1);
    for (auto elem: components)
        printf("%u ", elem);
    printf("\n");
}

//--------------------------------------------------------------------------------------------------

static unsigned solve(
    const unsigned N, const unsigned M,
    const std::vector<std::vector<unsigned>> &g,
    const std::vector<std::vector<unsigned>> &gt,

    std::vector<unsigned> &components)
{
    components.resize(N, 0);
    std::vector<unsigned> top_sort;

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
            dfs2(gt, components, component_cnt, elem);
            component_cnt++;
        }
    }

    return component_cnt;
}

//--------------------------------------------------------------------------------------------------

static void dfs(const std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &top_sort, std::vector<unsigned> &used, unsigned cur)
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

static void dfs2(const std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &components, unsigned cur_component, unsigned cur)
{
    components[cur] = cur_component;
    for (unsigned next: g[cur])
    {
        if (!components[next])
            dfs2(g, components, cur_component, next);
    }
}
