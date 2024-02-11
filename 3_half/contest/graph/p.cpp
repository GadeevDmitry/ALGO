#include <stdio.h>
#include <assert.h>

#include <vector>

int n, l, timer = 0;
std::vector<int> tin, tout;
std::vector<std::vector<int>> g, up;

static void dfs(const int cur, const int par)
{
    tin[cur] = timer++;

    up[cur][0] = par;
    for (int i = 1; i <= l; ++i)
        up[cur][i] = up[up[cur][i - 1]][i - 1];

    for (auto next : g[cur])
        if (next != par)
            dfs(next, cur);

    tout[cur] = timer++;
}

static bool is_parent(const int a, const int b)
{
    return tin[a] <= tin[b] && tout[a] >= tout[b];
}

static int get_lca(int a, int b)
{
    if (is_parent(a, b)) return a;
    if (is_parent(b, a)) return b;

    for (int i = l; i >= 0; --i)
        if (!is_parent(up[a][i], b))
            a = up[a][i];

    return up[a][0];
}

static unsigned get_dist(int a, int b)
{
    if (a == b) return 0;

    if (is_parent(a, b))
    {
        unsigned dist = 0;
        for (int i = l; i >= 0; --i)
        {
            if (!is_parent(up[b][i], a))
            {
                dist += (1 << (unsigned) i);
                b = up[b][i];
            }
        }

        return dist + 1;
    }

    if (is_parent(b, a))
    {
        unsigned dist = 0;
        for (int i = l; i >= 0; --i)
        {
            if (!is_parent(up[a][i], b))
            {
                dist += (1 << (unsigned) i);
                a = up[a][i];
            }
        }

        return dist + 1;
    }

    return get_dist(a, get_lca(a, b)) + get_dist(b, get_lca(a, b));
}

int main()
{
    scanf("%d", &n);

    g.resize(n);
    for (int i = 0; i < n - 1; ++i)
    {
        int a = 0, b = 0;
        scanf("%d %d", &a, &b);

        g[a - 1].push_back(b - 1);
        g[b - 1].push_back(a - 1);
    }

    l = 1;
    while ((1 << (unsigned) l) <= n) ++l;

    up.resize(n);
    for (int i = 0; i < n; ++i)
        up[i].resize(l + 1);

    tin .resize(n);
    tout.resize(n);

    dfs(0, 0);

    int q = 0;
    scanf("%d", &q);

    while (q--)
    {
        int a = 0, b = 0;
        scanf("%d %d", &a, &b);

        printf("%u\n", get_dist(a - 1, b - 1));
    }
}
