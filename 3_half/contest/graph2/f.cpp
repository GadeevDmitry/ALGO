#include <stdio.h>
#include <vector>

int main()
{
    int n = 0;
    scanf("%d", &n);

    std::vector<std::vector<int>> d(n, std::vector<int> (n));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            scanf("%d", &(d[i][j]));
        }
    }

    for (int k = 0; k < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (d[i][k] == 0 || d[k][j] == 0) continue;
                d[i][j] = 1;
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            printf("%d ", d[i][j]);
        }
        printf("\n");
    }
}
