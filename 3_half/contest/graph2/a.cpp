#include <stdio.h>

#include <set>
#include <vector>

const int INF = 2009000999;

static void dijkstra(const int n, const int s, std::vector<std::vector<std::pair<int, int>>> &g, std::vector<int> &d);

int main()
{
    int k = 0;
    scanf("%d", &k);

    for (int i = 0; i < k; ++i)
    {
        int n = 0, m = 0;
        scanf("%d %d", &n, &m);

        std::vector<std::vector<std::pair<int, int>>> g(n);
        for (int j = 0; j < m; ++j)
        {
            int a = 0, b = 0, len = 0;
            scanf("%d %d %d", &a, &b, &len);

            g[a].push_back(std::make_pair(b, len));
            g[b].push_back(std::make_pair(a, len));
        }

        int s = 0;
        scanf("%d", &s);

        std::vector<int> dist;
        dijkstra(n, s, g, dist);

        for (auto elem : dist)
        {
            printf("%d ", elem);
        }
        printf("\n");
    }
}

static void dijkstra(const int n, const int s, std::vector<std::vector<std::pair<int, int>>> &g, std::vector<int> &d)
{
    d.resize(n, INF);
    d[s] = 0;

    std::set<std::pair<int, int>> q;
    q.insert(std::make_pair(d[s], s));

    while (!q.empty())
    {
        int v = q.begin()->second;
        q.erase(q.begin());

        for (auto elem : g[v])
        {
            int to  = elem.first;
            int len = elem.second;

            if (d[v] + len < d[to])
            {
                q.erase(std::make_pair(d[to], to));
                d[to] = d[v] + len;
                q.insert(std::make_pair(d[to], to));
            }
        }
    }
}
