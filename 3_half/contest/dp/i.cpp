#include <iostream>
#include <vector>

int main()
{
    int n = 0;
    scanf("%d", &n);

    int v[n];
    for (int i = 0; i < n; ++i)
        scanf("%d", v+i);

    std::vector<int> more;
    std::vector<int> less;

    more.push_back(v[0]);
    bool next_more = true;

    for (int i = 1; i < n; ++i)
    {
        if (next_more && v[i] > more.back())
        {
            next_more = false;
            more.push_back(v[i]);
        }
        else if (!next_more && v[i] < more.back())
        {
            next_more = true;
            more.push_back(v[i]);
        }
        else
        {
            more.back() = v[i];
        }
    }

    less.push_back(v[0]);
    next_more = false;

    for (int i = 1; i < n; ++i)
    {
        if (next_more && v[i] > less.back())
        {
            next_more = false;
            less.push_back(v[i]);
        }
        else if (!next_more && v[i] < less.back())
        {
            next_more = true;
            less.push_back(v[i]);
        }
        else
        {
            less.back() = v[i];
        }
    }

    if (less.size() > more.size())
    {
        printf("%d\n", less.size());

        for (int i = 0; i < less.size(); ++i)
            printf("%d ", less[i]);
        printf("\n");
        return 0;
    }

    printf("%d\n", more.size());

    for (int i = 0; i < more.size(); ++i)
        printf("%d ", more[i]);
    printf("\n");
}
