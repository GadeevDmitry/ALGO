#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static bool dfs(std::vector<std::vector<unsigned>> &g, unsigned cur, std::vector<unsigned> &color, std::vector<unsigned> &parent, unsigned &cycle_begin, unsigned &cycle_end);

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

    std::vector<unsigned> color (N, 0);
    std::vector<unsigned> parent(N, 0);

    for (unsigned zero = 0; zero < N; ++zero)
    {
        if (color[zero] != 0)
            continue;

        unsigned cycle_begin;
        unsigned cycle_end;
        if (dfs(g, zero, color, parent, cycle_begin, cycle_end))
        {
            std::vector<unsigned> ans;
            for (unsigned i = cycle_end; i != cycle_begin; i = parent[i])
            {
                ans.push_back(i);
            }
            ans.push_back(cycle_begin);
            std::reverse(ans.begin(), ans.end());

            printf("YES\n");
            for (auto elem : ans)
                printf("%u ", elem + 1);
            printf("\n");

            return 0;
        }
    }

    printf("NO\n");
}

//--------------------------------------------------------------------------------------------------

static bool dfs(std::vector<std::vector<unsigned>> &g, unsigned cur, std::vector<unsigned> &color, std::vector<unsigned> &parent, unsigned &cycle_begin, unsigned &cycle_end)
{
    color[cur] = 1;
    for (unsigned next: g[cur])
    {
        switch (color[next])
        {
            case 0:
            {
                parent[next] = cur;
                bool res = dfs(g, next, color, parent, cycle_begin, cycle_end);
                if (res) return true;
                continue;
            } break;
            case 1:
            {
                cycle_begin = next;
                cycle_end   = cur;
                return true;
            } break;
            case 2:
            {
            } break;
            default:
            {
                return false;
            } break;
        }
    }

    color[cur] = 2;
    return false;
}
