#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static void dfs(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &cutpoints, std::vector<unsigned> &tin, std::vector<unsigned> &dp, unsigned &time, unsigned cur, int par);

//==================================================================================================

int main()
{
    unsigned N, M;
    scanf("%u %u", &N, &M);

    std::vector<std::vector<unsigned>> g(N);
    for (unsigned i = 1; i <= M; ++i)
    {
        unsigned a, b;
        scanf("%u %u", &a, &b);

        g[a - 1].push_back(b - 1);
        g[b - 1].push_back(a - 1);
    }

    std::vector<unsigned> cutpoints;
    std::vector<unsigned> tin(N, 0);
    std::vector<unsigned> dp (N, 0);
    unsigned time = 1;

    for (unsigned cur = 0; cur < N; ++cur)
    {
        if (!tin[cur])
            dfs(g, cutpoints, tin, dp, time, cur, -1);
    }
    std::sort(cutpoints.begin(), cutpoints.end());

    printf("%lu\n", cutpoints.size());
    for (unsigned cutpoint: cutpoints)
        printf("%u ", cutpoint + 1);
    printf("\n");
}

//--------------------------------------------------------------------------------------------------

static void dfs(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &cutpoints, std::vector<unsigned> &tin, std::vector<unsigned> &dp, unsigned &time, unsigned cur, int par)
{
    tin[cur] = time++;
    dp [cur] = tin[cur];

    unsigned children = 0;
    bool  is_cutpoint = false;
 
    for (unsigned next: g[cur])
    {
        if (tin[next] && next != par)
        {
            dp[cur] = std::min(dp[cur], tin[next]);
        }
        else if (!tin[next])
        {
            dfs(g, cutpoints, tin, dp, time, next, cur);
            dp[cur] = std::min(dp[cur], dp[next]);

            if (dp[next] >= tin[cur] && !is_cutpoint && par != -1)
            {
                is_cutpoint = true;
                cutpoints.push_back(cur);
            }
            ++children;
        }
    }

    if (par == -1 && children > 1)
        cutpoints.push_back(cur);
}
