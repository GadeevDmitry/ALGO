#include <stdio.h>
#include <vector>
#include <algorithm>

#define unsigned unsigned long long

//==================================================================================================

typedef struct
{
    unsigned a;
    unsigned b;
    unsigned w;
}
edge_t;

bool edge_cmp(edge_t &a, edge_t &b)
{
    return a.w < b.w;
}

//==================================================================================================

static unsigned N;
std::vector<edge_t> edges;
std::vector<unsigned> dsu;

static unsigned crascal();

//==================================================================================================

int main()
{
    scanf("%llu", &N);

    for (unsigned i = 1; i <= N; ++i) {
    for (unsigned j = 1; j <= N; ++j)
        {
            unsigned w;
            scanf("%llu", &w);

            if (j < i)
                edges.push_back({.a = i, .b = j, .w = w});
        }
    }

    for (unsigned i = 1; i <= N; ++i)
    {
        unsigned w;
        scanf("%llu", &w);

        edges.push_back({.a = 0, .b = i, .w = w});
    }

    std::sort(edges.begin(), edges.end(), edge_cmp);
/*
    for (edge_t edge: edges)
        printf("{%llu, %llu, %llu}\n", edge.a, edge.b, edge.w);
*/

    dsu.resize(N + 1);
    for (unsigned i = 0; i <= N; ++i)
        dsu[i] = i;

    printf("%llu\n", crascal());
}

//--------------------------------------------------------------------------------------------------

static unsigned get_anc(unsigned elem)
{
    if (dsu[elem] == elem)
        return elem;

    return dsu[elem] = get_anc(dsu[elem]);
}

//--------------------------------------------------------------------------------------------------

static unsigned crascal()
{
    unsigned res = 0;

    for (edge_t edge: edges)
    {
        unsigned anc_a = get_anc(edge.a);
        unsigned anc_b = get_anc(edge.b);

        if (anc_a != anc_b)
        {
            dsu[anc_a] = anc_b;
            res += edge.w;
        }
    }

    return res;
}
