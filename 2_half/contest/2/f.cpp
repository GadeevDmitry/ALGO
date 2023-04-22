#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONTEST

#ifndef CONTEST
#include "../../../lib/logs/log.h"
#endif

#ifndef CONTEST
#define clog_verify(condition, ret_val) log_verify(condition, ret_val)
#else
#define clog_verify(condition, ret_val)
#endif

#ifndef CONTEST
#define clog_assert(condition) log_assert(condition)
#else
#define clog_assert(condition)
#endif

//================================================================================================================================
// STRUCT DECART
//================================================================================================================================

typedef struct decart_node
{
    long long sum;
    int     prior;
    int       key;

    struct decart_node *left;
    struct decart_node *right;
}
decart_node;

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

bool decart_node_ctor       (decart_node *const cur_node, const int key, decart_node *const left,
                                                                         decart_node *const right);
decart_node *decart_node_new(                             const int key, decart_node *const left,
                                                                         decart_node *const right);
inline void decart_node_dtor(decart_node *const cur_node);
       void decart_tree_dtor(decart_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------
// decart_tree queries
//--------------------------------------------------------------------------------------------------------------------------------

       decart_node *decart_tree_insert       (decart_node  *const cur_node, const int           key_to_insert);
static decart_node *decart_tree_insert_static(decart_node  *const cur_node, decart_node *const node_to_insert);
static long long    decart_tree_culc_sum     (decart_node **const cur_node, const int l_key, const int r_key );

//--------------------------------------------------------------------------------------------------------------------------------
// descent
//--------------------------------------------------------------------------------------------------------------------------------

bool decart_tree_find_cur (decart_node *const cur_node, const int key_base);

//--------------------------------------------------------------------------------------------------------------------------------
// split, merge
//--------------------------------------------------------------------------------------------------------------------------------

static void         decart_tree_split(decart_node *const cur_node, const int split_key, decart_node **left, decart_node **right);
static decart_node *decart_tree_merge(decart_node *const left, decart_node *const right);

//--------------------------------------------------------------------------------------------------------------------------------
// sum io
//--------------------------------------------------------------------------------------------------------------------------------

static inline void       decart_node_reculc_sum(decart_node *const cur_node);
static inline long long  decart_node_get_sum   (decart_node *const cur_node);

//================================================================================================================================
// MAIN
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// query_handler
//--------------------------------------------------------------------------------------------------------------------------------

enum QUERY_TYPE
{
    ADD = 0 ,
    SUM     ,
};

       bool query_handler(decart_node **root);
static bool query_parser (enum QUERY_TYPE *const type, int *const query_param_1, int *const query_param_2);

//--------------------------------------------------------------------------------------------------------------------------------

signed main()
{
    decart_node *root = NULL;

    int query_num = 0;
    if (scanf("%d", &query_num) != 1) return 0;

    while (query_num--)
    {
        if (!query_handler(&root)) return 0;
    }

    decart_tree_dtor(root);
}

//--------------------------------------------------------------------------------------------------------------------------------
// query_handler
//--------------------------------------------------------------------------------------------------------------------------------

bool query_handler(decart_node **root)
{
    clog_verify(root != NULL, false);

    const  int        mod = (int) 1e9;
    static long long seed = 0;

    enum QUERY_TYPE type = (enum QUERY_TYPE) -1;
    int  query_param_1   = 0;
    int  query_param_2   = 0;

    if (!query_parser(&type, &query_param_1, &query_param_2)) return false;

    switch (type)
    {
        case ADD:   query_param_1 = (query_param_1 + seed) % mod;
                    seed = 0LL;
                    return (*root = decart_tree_insert(*root, query_param_1)) != NULL;

        case SUM:   seed = decart_tree_culc_sum(root, query_param_1, query_param_2);
                    printf("%lld\n", seed);
                    return true;

        default :   return false;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool query_parser(enum QUERY_TYPE *const type, int *const query_param_1, int *const query_param_2)
{
    char query_type[2] = "";
    if (scanf("%1s", query_type) != 1) return false;

    switch (query_type[0])
    {
        case '+':   *type = ADD;
                    return scanf("%d", query_param_1) == 1;
        case '?':   *type = SUM;
                    return scanf("%d %d", query_param_1, query_param_2) == 2;
        default :   return false;
    }

    return false;
}

//================================================================================================================================
// DECART BODY
//================================================================================================================================

#define $cur_sum    (cur_node->sum)
#define $cur_prior  (cur_node->prior)
#define $cur_key    (cur_node->key)

#define $cur_l      (cur_node->left)
#define $cur_r      (cur_node->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $l_sum      ($cur_l->sum)
#define $l_prior    ($cur_l->prior)
#define $l_key      ($cur_l->key)

#define $l_l        ($cur_l->left)
#define $l_r        ($cur_l->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $r_sum      ($cur_r->sum)
#define $r_prior    ($cur_r->prior)
#define $r_key      ($cur_r->key)

#define $r_l        ($cur_r->left)
#define $r_r        ($cur_r->right)

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

bool decart_node_ctor(decart_node *const cur_node, const int key, decart_node *const left,
                                                                  decart_node *const right)
{
    clog_verify(cur_node != NULL, false);

    $cur_key   = key;
    $cur_prior = rand();

    $cur_l =  left;
    $cur_r = right;

    decart_node_reculc_sum(cur_node);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

decart_node *decart_node_new(const int key, decart_node *const left,
                                            decart_node *const right)
{
    decart_node *result = (decart_node *) calloc(1, sizeof(decart_node));
    if          (result == NULL) return NULL;

    decart_node_ctor(result, key, left, right);
    return result;
}

//--------------------------------------------------------------------------------------------------------------------------------

inline void decart_node_dtor(decart_node *const cur_node)
{
    if (cur_node != NULL) free(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------

void decart_tree_dtor(decart_node *const cur_node)
{
    if (cur_node == NULL) return;

    decart_tree_dtor($cur_l);
    decart_tree_dtor($cur_r);
    decart_node_dtor(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------
// decart_tree queries
//--------------------------------------------------------------------------------------------------------------------------------

decart_node *decart_tree_insert(decart_node *const cur_node, const int key_to_insert)
{
    if (decart_tree_find_cur(cur_node, key_to_insert)) return cur_node;

    decart_node *node_to_insert = decart_node_new(key_to_insert, NULL, NULL);
    if          (node_to_insert == NULL) return NULL;

    return decart_tree_insert_static(cur_node, node_to_insert);
}

//--------------------------------------------------------------------------------------------------------------------------------

static decart_node *decart_tree_insert_static(decart_node *const cur_node, decart_node *const node_to_insert)
{
    clog_assert(node_to_insert != NULL);

    decart_node *left  = NULL;
    decart_node *right = NULL;

    decart_tree_split(cur_node, node_to_insert->key, &left, &right);

    return decart_tree_merge(decart_tree_merge(left, node_to_insert), right);
}

//--------------------------------------------------------------------------------------------------------------------------------

static long long decart_tree_culc_sum(decart_node **const cur_node, const int l_key,
                                                                    const int r_key)
{
    decart_node *left  = NULL;
    decart_node *main  = NULL;
    decart_node *right = NULL;

    decart_tree_split(*cur_node, l_key - 1, &left, &right);
    decart_tree_split( right   , r_key    , &main, &right);

    long long result = decart_node_get_sum(main);
    *cur_node  = decart_tree_merge(left, decart_tree_merge(main, right));

    return result;
}

//--------------------------------------------------------------------------------------------------------------------------------
// descent
//--------------------------------------------------------------------------------------------------------------------------------

bool decart_tree_find_cur(decart_node *cur_node, const int key_base)
{
    while (cur_node != NULL)
    {
        if ($cur_key == key_base) return true;
        if ($cur_key <  key_base) cur_node = $cur_r;
        else                      cur_node = $cur_l;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------
// split, merge
//--------------------------------------------------------------------------------------------------------------------------------

#define $L          (*left)

#define $L_size     ($L->size)
#define $L_prior    ($L->prior)
#define $L_key      ($L->key)

#define $L_l        ($L->left)
#define $L_r        ($L->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $R          (*right)

#define $R_size     ($R->size)
#define $R_prior    ($R->prior)
#define $R_key      ($R->key)

#define $R_l        ($R->left)
#define $R_r        ($R->right)

//--------------------------------------------------------------------------------------------------------------------------------

static void decart_tree_split(decart_node *const cur_node, const int split_key, decart_node **left,
                                                                                decart_node **right)
{
    if (cur_node == NULL)
    {
        $L = NULL;
        $R = NULL;

        return;
    }

    if ($cur_key <= split_key)
    {
        decart_tree_split($cur_r, split_key, left, right);

        $cur_r = $L;
        decart_node_reculc_sum(cur_node);

        $L = cur_node;
        return;
    }

    decart_tree_split($cur_l, split_key, left, right);

    $cur_l = $R;
    decart_node_reculc_sum(cur_node);

    $R = cur_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

#undef $L
#undef $R

#define $L left
#define $R right

//--------------------------------------------------------------------------------------------------------------------------------

static decart_node *decart_tree_merge(decart_node *const left, decart_node *const right)
{
    if ($L == NULL) return $R;
    if ($R == NULL) return $L;

    if ($L_prior > $R_prior)
    {
        $L_r = decart_tree_merge($L_r, $R);
        decart_node_reculc_sum  ($L);

        return $L;
    }

    $R_l = decart_tree_merge($L, $R_l);
    decart_node_reculc_sum  ($R);

    return $R;
}

//--------------------------------------------------------------------------------------------------------------------------------
// sum io
//--------------------------------------------------------------------------------------------------------------------------------

static inline void decart_node_reculc_sum(decart_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    const long long l_sum = decart_node_get_sum($cur_l);
    const long long r_sum = decart_node_get_sum($cur_r);

    $cur_sum = l_sum + r_sum + $cur_key;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline long long decart_node_get_sum(decart_node *const cur_node)
{
    if (cur_node == NULL) return 0;

    return $cur_sum;
}