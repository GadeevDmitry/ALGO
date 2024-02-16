#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static void input(
    unsigned &N, unsigned &M,
    std::vector<std::vector<std::pair<unsigned, unsigned>>> &g);

static void output(const std::vector<unsigned> &bridges);

static void solve(
    const unsigned N, const unsigned M,
    const std::vector<std::vector<std::pair<unsigned, unsigned>>> &g,

    std::vector<unsigned> &bridges);

static void dfs(
    const std::vector<std::vector<std::pair<unsigned, unsigned>>> &g,
    std::vector<unsigned> &bridges,
    std::vector<unsigned> &tin, std::vector<unsigned> &dp, unsigned &time, unsigned cur, int par);

//==================================================================================================

int main()
{
    unsigned N = 0, M = 0;
    std::vector<std::vector<std::pair<unsigned, unsigned>>> g;
    input(N, M, g);

    std::vector<unsigned> bridges;
    solve(N, M, g, bridges);

    output(bridges);
}

//--------------------------------------------------------------------------------------------------

static void input(
    unsigned &N, unsigned &M,
    std::vector<std::vector<std::pair<unsigned, unsigned>>> &g)
{
    scanf("%u %u", &N, &M);

    g.resize(N);
    for (unsigned i = 1; i <= M; ++i)
    {
        unsigned a, b;
        scanf("%u %u", &a, &b);

        g[a - 1].push_back(std::pair(b - 1, i));
        g[b - 1].push_back(std::pair(a - 1, i));
    }
}

//--------------------------------------------------------------------------------------------------

static void output(const std::vector<unsigned> &bridges)
{
    printf("%lu\n", bridges.size());
    for (unsigned bridge: bridges)
        printf("%u ", bridge);
    printf("\n");
}

//--------------------------------------------------------------------------------------------------

static void solve(
    const unsigned N, const unsigned M,
    const std::vector<std::vector<std::pair<unsigned, unsigned>>> &g,

    std::vector<unsigned> &bridges)
{
    std::vector<unsigned> tin(N, 0);
    std::vector<unsigned> dp (N, 0);
    unsigned time = 1;

    for (unsigned cur = 0; cur < N; ++cur)
    {
        if (!tin[cur])
            dfs(g, bridges, tin, dp, time, cur, -1);
    }
    std::sort(bridges.begin(), bridges.end());
}

//--------------------------------------------------------------------------------------------------

static void dfs(
    const std::vector<std::vector<std::pair<unsigned, unsigned>>> &g,
    std::vector<unsigned> &bridges,
    std::vector<unsigned> &tin, std::vector<unsigned> &dp, unsigned &time, unsigned cur, int par)
{
    tin[cur] = time++;
    dp [cur] = tin[cur];

    for (std::pair<unsigned, unsigned> edge: g[cur])
    {
        if (tin[edge.first] && edge.first != par)
        {
            dp[cur] = std::min(dp[cur], tin[edge.first]);
        }
        else if (!tin[edge.first])
        {
            dfs(g, bridges, tin, dp, time, edge.first, cur);
            dp[cur] = std::min(dp[cur], dp[edge.first]);

            if (dp[edge.first] > tin[cur])
                bridges.push_back(edge.second);
        }
    }
}
