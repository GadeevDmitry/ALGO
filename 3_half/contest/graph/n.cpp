#include <stdio.h>
#include <vector>
#include <algorithm>

#define unsigned unsigned long long

//==================================================================================================

static unsigned N, M;
static unsigned r_max = 0;

std::vector<unsigned> dsu;
std::vector<unsigned> r;

static unsigned get_anc(unsigned elem);
static void     merge  (unsigned a, unsigned b);

//==================================================================================================

int main()
{
    scanf("%llu %llu", &N, &M);

    r.resize(N);
    for (unsigned i = 0; i < N; ++i)
    {
        scanf("%llu", &r[i]);
        r_max = std::max(r_max, r[i]);
    }

    dsu.resize(N);
    for (unsigned i = 0; i < N; ++i)
        dsu[i] = i;

    while (M--)
    {
        unsigned a, b;
        scanf("%llu %llu", &a, &b);

        merge(a - 1, b - 1);
        printf("%llu\n", r_max);
    }
}

//--------------------------------------------------------------------------------------------------

static unsigned get_anc(unsigned elem)
{
    if (dsu[elem] == elem)
        return elem;

    return dsu[elem] = get_anc(dsu[elem]);
}

//--------------------------------------------------------------------------------------------------

static void merge(unsigned src, unsigned dest)
{
    unsigned anc_src  = get_anc(src);
    unsigned anc_dest = get_anc(dest);

    if (anc_src != anc_dest)
    {
        dsu[anc_src] = anc_dest;

        r[anc_dest] += r[anc_src];
        r_max = std::max(r_max, r[anc_dest]);
    }
}
