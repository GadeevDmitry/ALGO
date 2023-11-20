#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static void dfs(std::vector<std::vector<std::pair<unsigned, unsigned>>> &g, std::vector<unsigned> &bridges, std::vector<unsigned> &tin, std::vector<unsigned> &dp, unsigned &time, unsigned cur, int par);

//==================================================================================================

int main()
{
    unsigned N, M;
    scanf("%u %u", &N, &M);

    std::vector<std::vector<std::pair<unsigned, unsigned>>> g (N);
    for (unsigned i = 1; i <= M; ++i)
    {
        unsigned a, b;
        scanf("%u %u", &a, &b);

        g[a - 1].push_back(std::pair(b - 1, i));
        g[b - 1].push_back(std::pair(a - 1, i));
    }

    std::vector<unsigned> bridges;
    std::vector<unsigned> tin(N, 0);
    std::vector<unsigned> dp (N, 0);
    unsigned time = 1;

    for (unsigned cur = 0; cur < N; ++cur)
    {
        if (!tin[cur])
            dfs(g, bridges, tin, dp, time, cur, -1);
    }
    std::sort(bridges.begin(), bridges.end());

    printf("%lu\n", bridges.size());
    for (unsigned bridge: bridges)
        printf("%u ", bridge);
    printf("\n");
}

//--------------------------------------------------------------------------------------------------

static void dfs(std::vector<std::vector<std::pair<unsigned, unsigned>>> &g, std::vector<unsigned> &bridges, std::vector<unsigned> &tin, std::vector<unsigned> &dp, unsigned &time, unsigned cur, int par)
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
