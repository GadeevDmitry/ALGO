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
    int size;
    int prior;
    int key;

    struct decart_node *left;
    struct decart_node *right;
}
decart_node;

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

inline bool decart_node_ctor(decart_node *const cur_node, const int key ,
                                                          const int size, decart_node *const left,
                                                                          decart_node *const right);
decart_node *decart_node_new(                             const int key ,
                                                          const int size, decart_node *const left,
                                                                          decart_node *const right);
inline void decart_node_dtor(decart_node *const cur_node);
       void decart_tree_dtor(decart_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------
// insert, erase
//--------------------------------------------------------------------------------------------------------------------------------

       decart_node *decart_tree_insert       (decart_node *const cur_node, const int           key_to_insert);
static decart_node *decart_tree_insert_static(decart_node *const cur_node, decart_node *const node_to_insert);
       decart_node *decart_tree_erase        (decart_node *const cur_node, const int           key_to_erase );

//--------------------------------------------------------------------------------------------------------------------------------
// descent
//--------------------------------------------------------------------------------------------------------------------------------

bool decart_tree_find_next(decart_node *const cur_node, const int key_base, int *const key_next);
bool decart_tree_find_prev(decart_node *const cur_node, const int key_base, int *const key_prev);
bool decart_tree_find_cur (decart_node *const cur_node, const int key_base                     );
bool decart_tree_find_kth (decart_node *const cur_node,       int k       , int *const kth_key );

//--------------------------------------------------------------------------------------------------------------------------------
// split, merge
//--------------------------------------------------------------------------------------------------------------------------------

static void         decart_tree_split(decart_node *const cur_node, const int split_key, decart_node **left, decart_node **right);
static decart_node *decart_tree_merge(decart_node *const left, decart_node *const right);

//--------------------------------------------------------------------------------------------------------------------------------
// size io
//--------------------------------------------------------------------------------------------------------------------------------

static inline void decart_node_reculc_size(decart_node *const cur_node);
static inline int  decart_node_get_size   (decart_node *const cur_node);

//================================================================================================================================
// MAIN
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// query_handler
//--------------------------------------------------------------------------------------------------------------------------------

enum QUERY_TYPE
{
    INSERT = 0  ,
    DELETE      ,
    EXISTS      ,
    NEXT        ,
    PREV        ,
    KTH         ,
};

bool query_handler(decart_node **root);

static inline bool query_insert(decart_node **root, const int key_to_insert);
static inline bool query_erase (decart_node **root, const int key_to_erase);

static inline bool query_exists(decart_node **root, const int key_base);
static inline bool query_next  (decart_node **root, const int key_base);
static inline bool query_prev  (decart_node **root, const int key_base);
static inline bool query_kth   (decart_node **root, const int k);

//--------------------------------------------------------------------------------------------------------------------------------
// query_parser
//--------------------------------------------------------------------------------------------------------------------------------

static bool query_parser(enum QUERY_TYPE *const type, int *const query_param);

//--------------------------------------------------------------------------------------------------------------------------------

int main()
{
    decart_node *root = NULL;

    while (query_handler(&root)) {}

    decart_tree_dtor(root);
}

//--------------------------------------------------------------------------------------------------------------------------------
// query_handler
//--------------------------------------------------------------------------------------------------------------------------------

bool query_handler(decart_node **root)
{
    clog_verify(root != NULL, false);

    enum QUERY_TYPE type = (enum QUERY_TYPE) -1;
    int query_param = 0;

    if (!query_parser(&type, &query_param)) return false;

    switch (type)
    {
        case INSERT:    return query_insert(root, query_param);
        case DELETE:    return query_erase (root, query_param);
        case EXISTS:    return query_exists(root, query_param);
        case NEXT  :    return query_next  (root, query_param);
        case PREV  :    return query_prev  (root, query_param);
        case KTH   :    return query_kth   (root, query_param);

        default    :    return false;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------
// query insert, erase
//--------------------------------------------------------------------------------------------------------------------------------

static inline bool query_insert(decart_node **root, const int key_to_insert)
{
    clog_assert(root != NULL);

    return (*root = decart_tree_insert(*root, key_to_insert)) != NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline bool query_erase(decart_node **root, const int key_to_erase)
{
    clog_assert(root != NULL);

    *root = decart_tree_erase(*root, key_to_erase);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// query descent
//--------------------------------------------------------------------------------------------------------------------------------

static inline bool query_exists(decart_node **root, const int key_base)
{
    clog_assert(root != NULL);

    if (decart_tree_find_cur(*root, key_base)) printf("true\n");
    else                                       printf("false\n");

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline bool query_next(decart_node **root, const int key_base)
{
    clog_assert(root != NULL);

    int key_next = 0;

    if (decart_tree_find_next(*root, key_base, &key_next)) printf("%d\n", key_next);
    else                                                   printf("none\n");

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline bool query_prev(decart_node **root, const int key_base)
{
    clog_assert(root != NULL);

    int key_prev = 0;

    if (decart_tree_find_prev(*root, key_base, &key_prev)) printf("%d\n", key_prev);
    else                                                   printf("none\n");

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline bool query_kth(decart_node **root, const int k)
{
    clog_assert(root != NULL);

    int key_kth = 0;

    if (decart_tree_find_kth(*root, k, &key_kth)) printf("%d\n", key_kth);
    else                                          printf("none\n");

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// query_parser
//--------------------------------------------------------------------------------------------------------------------------------

static bool query_parser(enum QUERY_TYPE *const type, int *const query_param)
{
    static const char *query_name[] =
    {
        "insert"    ,
        "delete"    ,
        "exists"    ,
        "next"      ,
        "prev"      ,
        "kth"       ,
    };

    char query_type[20] = "";

    if (scanf("%19s %d", query_type, query_param) != 2) return false;

    for (size_t i = 0; i * sizeof(char *) < sizeof(query_name); ++i)
    {
        if (strcmp(query_type, query_name[i]) == 0)
        {
            *type = (enum QUERY_TYPE) i;
            return true;
        }
    }

    return false;
}

//================================================================================================================================
// DECART BODY
//================================================================================================================================

#define $cur_size   (cur_node->size)
#define $cur_prior  (cur_node->prior)
#define $cur_key    (cur_node->key)

#define $cur_l      (cur_node->left)
#define $cur_r      (cur_node->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $l_size     ($cur_l->size)
#define $l_prior    ($cur_l->prior)
#define $l_key      ($cur_l->key)

#define $l_l        ($cur_l->left)
#define $l_r        ($cur_l->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $r_size     ($cur_r->size)
#define $r_prior    ($cur_r->prior)
#define $r_key      ($cur_r->key)

#define $r_l        ($cur_r->left)
#define $r_r        ($cur_r->right)

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

inline bool decart_node_ctor(decart_node *const cur_node, const int key ,
                                                          const int size, decart_node *const left,
                                                                          decart_node *const right)
{
    clog_verify(cur_node != NULL, false);

    $cur_size  = size;
    $cur_key   =  key;

    $cur_prior = rand();

    $cur_l =  left;
    $cur_r = right;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

decart_node *decart_node_new(const int key ,
                             const int size, decart_node *const left,
                                             decart_node *const right)
{
    decart_node *result = (decart_node *) calloc(1, sizeof(decart_node));
    if          (result == NULL) return NULL;

    decart_node_ctor(result, key, size, left, right);
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
// insert, erase
//--------------------------------------------------------------------------------------------------------------------------------

decart_node *decart_tree_insert(decart_node *const cur_node, const int key_to_insert)
{
    if (decart_tree_find_cur(cur_node, key_to_insert)) return cur_node;

    decart_node *node_to_insert = decart_node_new(key_to_insert, 1, NULL, NULL);
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

decart_node *decart_tree_erase(decart_node *const cur_node, const int key_to_erase)
{
    if (!decart_tree_find_cur(cur_node, key_to_erase)) return cur_node;

    decart_node *left     = NULL;
    decart_node *right    = NULL;
    decart_node *to_erase = NULL;

    decart_tree_split(cur_node, key_to_erase    , &left, &right   );
    decart_tree_split(left    , key_to_erase - 1, &left, &to_erase);

    decart_node_dtor(to_erase);

    return decart_tree_merge(left, right);
}

//--------------------------------------------------------------------------------------------------------------------------------
// descent
//--------------------------------------------------------------------------------------------------------------------------------

bool decart_tree_find_next(decart_node *cur_node, const int key_base, int *const key_next)
{
    int result = key_base;

    while (cur_node != NULL)
    {
        decart_node *const left  = $cur_l;
        decart_node *const right = $cur_r;

        bool condition = $cur_key <= key_base;

        result   = condition ? result : $cur_key;
        cur_node = condition ? right  :     left;
    }

    if (key_next != NULL) *key_next = result;

    return result > key_base;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool decart_tree_find_prev(decart_node *cur_node, const int key_base, int *const key_prev)
{
    int result = key_base;

    while (cur_node != NULL)
    {
        decart_node *const left  = $cur_l;
        decart_node *const right = $cur_r;

        bool condition = $cur_key >= key_base;

        result   = condition ? result : $cur_key;
        cur_node = condition ?   left :    right;
    }

    if (key_prev != NULL) *key_prev = result;

    return result < key_base;
}

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

bool decart_tree_find_kth(decart_node *const cur_node, int k, int *const kth_key)
{
    clog_verify(kth_key != NULL, false);

    if (cur_node == NULL) return false;

    const int l_size = decart_node_get_size($cur_l);

    if (l_size < k) { k -= (l_size + 1); return decart_tree_find_kth($cur_r, k, kth_key); }
    if (l_size > k) {                    return decart_tree_find_kth($cur_l, k, kth_key); }

    *kth_key = $cur_key;

    return true;
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
        decart_node_reculc_size(cur_node);

        $L = cur_node;
        return;
    }

    decart_tree_split($cur_l, split_key, left, right);

    $cur_l = $R;
    decart_node_reculc_size(cur_node);

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
        decart_node_reculc_size ($L);

        return $L;
    }

    $R_l = decart_tree_merge($L, $R_l);
    decart_node_reculc_size($R);

    return $R;
}

//--------------------------------------------------------------------------------------------------------------------------------
// size io
//--------------------------------------------------------------------------------------------------------------------------------

static inline void decart_node_reculc_size(decart_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    const int l_size = decart_node_get_size($cur_l);
    const int r_size = decart_node_get_size($cur_r);

    $cur_size = l_size + r_size + 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline int decart_node_get_size(decart_node *const cur_node)
{
    if (cur_node == NULL) return 0;

    return $cur_size;
}