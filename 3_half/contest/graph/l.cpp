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

static unsigned N, M;
std::vector<edge_t> edges;
std::vector<unsigned> dsu;

static unsigned crascal();

//==================================================================================================

int main()
{
    scanf("%llu %llu", &N, &M);

    edges.resize(M);
    for (unsigned i = 0; i < M; ++i)
    {
        edge_t edge;
        scanf("%llu %llu %llu", &edge.a, &edge.b, &edge.w);

        edge.a--;
        edge.b--;
        edges[i] = edge;
    }
    std::sort(edges.begin(), edges.end(), edge_cmp);
/*
    for (edge_t edge: edges)
        printf("{%llu, %llu, %llu}\n", edge.a, edge.b, edge.w);
*/
    dsu.resize(N);
    for (unsigned i = 0; i < N; ++i)
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

    for (unsigned i = 0; i < M; ++i)
    {
        unsigned anc_a = get_anc(edges[i].a);
        unsigned anc_b = get_anc(edges[i].b);

        if (anc_a != anc_b)
        {
            dsu[anc_a] = anc_b;
            res += edges[i].w;
        }
    }

    return res;
}
