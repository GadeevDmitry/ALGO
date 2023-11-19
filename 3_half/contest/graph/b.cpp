#include <stdio.h>
#include <vector>
#include <algorithm>

//==================================================================================================

static void dfs(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &component, std::vector<unsigned> &colors, unsigned cur, unsigned cur_color);

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
        g[b - 1].push_back(a - 1);
    }

    unsigned color = 1;
    std::vector<unsigned> colors(N, 0);
    std::vector<std::vector<unsigned>> components;

    for (unsigned cur = 0; cur < N; ++cur)
    {
        if (colors[cur] != 0)
            continue;

        std::vector<unsigned> component;
        dfs(g, component, colors, cur, color);

        components.push_back(component);
        ++color;
    }

    printf("%u\n", color - 1);
    for (unsigned cnt = 1; cnt < color; ++cnt)
    {
        std::vector<unsigned> &cur = components[cnt - 1];
        printf("%lu\n", cur.size());

        for (unsigned elem: cur)
            printf("%u ", elem + 1);
        printf("\n");
    }
}

//--------------------------------------------------------------------------------------------------

static void dfs(std::vector<std::vector<unsigned>> &g, std::vector<unsigned> &component, std::vector<unsigned> &colors, unsigned cur, unsigned cur_color)
{
    colors[cur] = cur_color;
    component.push_back(cur);

    for (unsigned next: g[cur])
    {
        if (colors[next] == 0)
            dfs(g, component, colors, next, cur_color);
    }
}
