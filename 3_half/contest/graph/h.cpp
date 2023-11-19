#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static unsigned N, M;
static std::vector<std::vector<unsigned>> g;

static std::vector<unsigned> tin;
static std::vector<unsigned> dp;
static unsigned timer = 1;

static std::vector<unsigned> components;
static unsigned max_components = 1;

static void dfs (unsigned cur, int par);
static void dfs2(unsigned cur, unsigned cur_component);

//==================================================================================================

int main()
{
    scanf("%u %u", &N, &M);

    g         .resize(N);
    tin       .resize(N, 0);
    dp        .resize(N, 0);
    components.resize(N, 0);

    for (unsigned i = 0; i < M; ++i)
    {
        unsigned a, b;
        scanf("%u %u", &a, &b);

        g[a - 1].push_back(b - 1);
        g[b - 1].push_back(a - 1);
    }

    dfs (0, -1);
    dfs2(0,  1);
/*
    for (unsigned i = 0; i < N; ++i)
        printf("%u ", components[i]);
    printf("\n");
*/
    std::vector<unsigned> components_edges(max_components);
    for (unsigned i = 0; i < N; ++i)
    {
        for (unsigned j: g[i])
        {
            if (components[i] == components[j]) continue;
            components_edges[components[i] - 1]++;
        }
    }

    unsigned ans = 0;
    for (unsigned elem: components_edges)
        ans += (elem == 1) ? 1 : 0;

    printf("%u\n", (ans + 1) / 2);
}

//--------------------------------------------------------------------------------------------------

static void dfs(unsigned cur, int par)
{
    tin[cur] = timer++;
    dp [cur] = tin[cur];

    for (unsigned next: g[cur])
    {
        if (tin[next] && next != par)
        {
            dp[cur] = std::min(dp[cur], tin[next]);
        }
        else if (!tin[next])
        {
            dfs(next, cur);
            dp[cur] = std::min(dp[cur], dp[next]);
        }
    }
}

//--------------------------------------------------------------------------------------------------

static void dfs2(unsigned cur, unsigned cur_component)
{
    components[cur] = cur_component;

    for (unsigned next: g[cur])
    {
        if (!components[next])
        {
            if (dp[next] > tin[cur])
            {
                max_components++;
                dfs2(next, max_components);
            }
            else
            {
                dfs2(next, cur_component);
            }
        }
    }
}
