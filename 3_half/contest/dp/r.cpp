#include <iostream>
#include <string.h>
#include <vector>

int main()
{
    unsigned n;
    scanf("%u", &n);

    char g[n + 1][n + 2];
    for (unsigned i = 1; i <= n; ++i)
        scanf("%s", g[i] + 1);

    unsigned pow_n = (1 << (n + 1));
    unsigned dp[pow_n];

    for (unsigned i = 0; i < pow_n; ++i) dp[i] = 0;
    dp[0] = 1;

    int ans = 0;

    for (unsigned i = 1; i < pow_n; ++i)
    {
        if (i & 1) continue;
    
        std::vector <unsigned> v;
        for (unsigned j = 1; j <= n; ++j)
            if (i & (1 << j))
                v.push_back(j);

        for (unsigned cnt1 = 0; cnt1 < v.size(); ++cnt1)
        {
            for (unsigned cnt2 = cnt1 + 1; cnt2 < v.size(); ++cnt2)
            {
                if (g[v[cnt1]][v[cnt2]] != 'Y') continue;

                unsigned left = i;
                left ^= ((unsigned) 1 << v[cnt1]);
                left ^= ((unsigned) 1 << v[cnt2]);

                if (dp[left])
                {
                    dp[i] = 1;
                    break;
                }
            }
            if (dp[i]) break;
        }

        if (dp[i]) ans = std::max(ans, (int) v.size());
    }

    printf("%d\n", ans);
}
