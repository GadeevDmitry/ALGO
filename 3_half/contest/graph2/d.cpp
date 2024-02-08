#include <stdio.h>
#include <vector>

const int INF = 30000;

int main()
{
    int n = 0, m = 0;
    scanf("%d %d", &n, &m);

    std::vector<std::pair<int, std::pair<int, int>>> e(m);
    for (int i = 0; i < m; ++i)
    {
        int a = 0, b = 0, len = 0;
        scanf("%d %d %d", &a, &b, &len);

        e[i] = {a - 1, {b - 1, len}};
    }

    std::vector<int> d(n, INF);
    d[0] = 0;

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (d[e[j].first] < INF)
            {
                d[e[j].second.first] = std::min(d[e[j].second.first], d[e[j].first] + e[j].second.second);
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        printf("%d ", d[i]);
    }
    printf("\n");
}
