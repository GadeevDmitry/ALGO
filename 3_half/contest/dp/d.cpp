#include <stdio.h>

const int MIN = 1 << 31;

int bin_search(int val, int *v, int size)
{
    int l = -1, r = size - 1;

    while (r - l > 1)
    {
        int bin = (r + l) / 2;

        if (val > v[bin]) r = bin;
        else              l = bin;
    }

    if (val > v[r]) return r;
    return size;
}

int main()
{
    int n = 0;
    scanf("%d", &n);

    int v    [n];
    int dp   [n];
    int dp_el[n];
    int p    [n];

    for (int i = 0; i < n; ++i)
    {
        scanf("%d", v+i);
        dp[i]    = MIN;
        dp_el[i] = n;
        p [i]    = n;
    }

    for (int i = 0; i < n; ++i)
    {
        int pos = bin_search(v[i], dp, n);

        if (pos == n) continue;
        dp   [pos] = v[i];
        dp_el[pos] = i;

        if (pos != 0)
            p[i] = dp_el[pos - 1];
    }

    int ans = n;
    for (int i = n - 1; i >= 0; --i)
    {
        if (dp[i] != MIN)
        {
            ans = i;
            break;
        }
    }

    printf("%d\n", ans + 1);
    int cur = 0;

    int p_rev[n];
    int ind = 0;
    for (int i = dp_el[ans]; i != n; i = p[i])
    {
        p_rev[ind] = i;
        ++ind;
    }

    for (int i = ind - 1; i >= 0; --i)
        printf("%d ", p_rev[i] + 1);
    printf("\n");
}
