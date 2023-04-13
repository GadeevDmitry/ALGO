#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CONTEST

#ifndef CONTEST
#include "../../../lib/logs/log.h"
#endif

#ifndef CONTEST
#define clog_assert(condition) log_assert(condition)
#else
#define clog_assert(condition)
#endif

//================================================================================================================================
// STRUCT AVL
//================================================================================================================================

struct avl_node
{
    int key;
    int height;

    struct avl_node *left;
    struct avl_node *right;
};

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

void inline avl_node_ctor    (struct avl_node *const node, const int key,
                                                           const int height, struct avl_node *const left,
                                                                             struct avl_node *const right);
struct avl_node *avl_node_new(                             const int key,
                                                           const int height, struct avl_node *const left,
                                                                             struct avl_node *const right);
void inline avl_node_dtor(struct avl_node *const node);
void        avl_tree_dtor(struct avl_node *const root);

//--------------------------------------------------------------------------------------------------------------------------------
// descent
//--------------------------------------------------------------------------------------------------------------------------------

int              avl_lower_bound(struct avl_node *const     node, const int key_to_find  );
struct avl_node *avl_insert     (struct avl_node *const cur_node, const int key_to_insert);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------
#ifndef CONTEST
void        avl_node_dump       (avl_node *const cur_node);
void        avl_tree_dump       (avl_node *const     root);
static void avl_tree_dump_static(avl_node *const cur_node);
#endif
//--------------------------------------------------------------------------------------------------------------------------------
// possible rotate
//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *possible_rotate(struct avl_node *const node);
//--------------------------------------------------------------------------------------------------------------------------------
static struct avl_node *possible_small_rotate_left (struct avl_node *const cur_node);
static struct avl_node *possible_small_rotate_right(struct avl_node *const cur_node);
//--------------------------------------------------------------------------------------------------------------------------------
static struct avl_node *possible_big_rotate_left   (struct avl_node *const cur_node);
static struct avl_node *possible_big_rotate_right  (struct avl_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------
// rotate
//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *avl_small_rotate_left (struct avl_node *const cur_node);
static struct avl_node *avl_small_rotate_right(struct avl_node *const cur_node);
//--------------------------------------------------------------------------------------------------------------------------------
static struct avl_node *avl_big_rotate_left   (struct avl_node *const cur_node);
static struct avl_node *avl_big_rotate_right  (struct avl_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------
// height io
//--------------------------------------------------------------------------------------------------------------------------------

static inline int  avl_get_balance  (struct avl_node *const cur_node);
static inline int  avl_get_height   (struct avl_node *const cur_node);
static inline void avl_reculc_height(struct avl_node *const cur_node);

//================================================================================================================================
// MAIN
//================================================================================================================================

bool query_handler(struct avl_node **root);

//--------------------------------------------------------------------------------------------------------------------------------

int main()
{
    int query_num = 0;
    if (scanf("%d", &query_num) != 1) return 0;

    struct avl_node *root = NULL;

    while (query_num--)
    {
        if (!query_handler(&root)) { avl_tree_dtor(root); return 0; }

        #ifndef CONTEST
        avl_tree_dump(root);
        #endif
    }

    avl_tree_dtor(root);
}

//--------------------------------------------------------------------------------------------------------------------------------

enum QUERY_TYPE
{
    ADD = 0 ,
    NEXT    ,
    ERROR   ,
};

enum QUERY_TYPE determine_query_type(int *const query_param);

//--------------------------------------------------------------------------------------------------------------------------------

bool query_handler(struct avl_node **root)
{
    clog_assert(root != NULL);

    static int seed = 0;
    const  int mod  = (int) 1e9;

    int             query_param = 0;
    enum QUERY_TYPE query_type  = determine_query_type(&query_param);

    switch (query_type)
    {
        case ADD:   query_param = (query_param + seed) % mod;

                   *root = avl_insert(*root, query_param);
                    seed = 0;

                    return true;

        case NEXT:  seed = avl_lower_bound(*root, query_param);
                    printf("%d\n", seed);

                    return  true;
        case ERROR:
        default:    return false;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

enum QUERY_TYPE determine_query_type(int *const query_param)
{
    clog_assert(query_param != NULL);

    char query_type[2] = "";
    if (scanf("%1s", query_type) != 1) return ERROR;

    switch (query_type[0])
    {
        case '+':   if (scanf("%d", query_param) != 1) return ERROR;
                    return ADD;

        case '?':   if (scanf("%d", query_param) != 1) return ERROR;
                    return NEXT;

        default:    return ERROR;
    }

    return ERROR;
}

//================================================================================================================================
// AVL BODY
//================================================================================================================================

#define $cur_key        (cur_node->key)
#define $cur_height     (cur_node->height)
#define $cur_l          (cur_node->left)
#define $cur_r          (cur_node->right)

#define $l_key          ($cur_l->key)
#define $l_height       ($cur_l->height)
#define $l_l            ($cur_l->left)
#define $l_r            ($cur_l->right)

#define $r_key          ($cur_r->key)
#define $r_height       ($cur_r->height)
#define $r_l            ($cur_r->left)
#define $r_r            ($cur_r->right)

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

void inline avl_node_ctor(struct avl_node *const cur_node, const int key,
                                                           const int height /* = 1 */, struct avl_node *const left  /* = NULL */,
                                                                                       struct avl_node *const right /* = NULL */)
{
    clog_assert(cur_node != NULL);

    $cur_key    = key;
    $cur_height = height;

    $cur_l = left;
    $cur_r = right;
}

//--------------------------------------------------------------------------------------------------------------------------------

struct avl_node *avl_node_new(const int key,
                              const int height /* = 1 */, struct avl_node *const left  /* = NULL */,
                                                          struct avl_node *const right /* = NULL */)
{
    struct avl_node *new_node = (struct avl_node *) calloc(1, sizeof(struct avl_node));
    if              (new_node == NULL) return NULL;

    avl_node_ctor(new_node, key, height, left, right);

    return new_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

void inline avl_node_dtor(struct avl_node *const node)
{
    if (node != NULL) free(node);
}

//--------------------------------------------------------------------------------------------------------------------------------

void avl_tree_dtor(struct avl_node *const root)
{
    if (root == NULL) return;

    avl_tree_dtor(root->left );
    avl_tree_dtor(root->right);

    avl_node_dtor(root);
}

//--------------------------------------------------------------------------------------------------------------------------------
// descent
//--------------------------------------------------------------------------------------------------------------------------------

int avl_lower_bound(struct avl_node *const node, const int key_to_find)
{
    struct avl_node *cur_node = node;
    int              cur_ans  =   -1;

    while (cur_node != NULL)
    {
        if ($cur_key == key_to_find) return key_to_find;
        if ($cur_key >  key_to_find) cur_ans = $cur_key;

        if      ($cur_key <  key_to_find) cur_node = $cur_r;
        else if ($cur_l   !=        NULL) cur_node = $cur_l;

        else return cur_ans;
    }

    return cur_ans;
}

//--------------------------------------------------------------------------------------------------------------------------------

struct avl_node *avl_insert(struct avl_node *const cur_node, const int key_to_insert)
{
    if (cur_node == NULL) return avl_node_new(key_to_insert, 1, NULL, NULL);

    if ($cur_key == key_to_insert) return cur_node;

    if ($cur_key <  key_to_insert) $cur_r = avl_insert($cur_r, key_to_insert);
    else                           $cur_l = avl_insert($cur_l, key_to_insert);

    avl_reculc_height(cur_node);
    return possible_rotate(cur_node);
}

#ifndef CONTEST
//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void avl_node_dump(avl_node *const cur_node)
{
    log_tab_message(HTML_COLOR_MEDIUM_BLUE
                    "avl_node (addr: %p)\n"
                    "{"
                    HTML_COLOR_CANCEL  "\n", cur_node);

    if (cur_node == NULL) { log_tab_message(HTML_COLOR_MEDIUM_BLUE "}" HTML_COLOR_CANCEL "\n"); }

    LOG_TAB++;

    log_tab_message(HTML_COLOR_DARK_ORANGE
                    "key    = %d\n"
                    "height = %d\n\n"
                    "left   = %p\n"
                    "right  = %p"
                    HTML_COLOR_CANCEL "\n", $cur_key, $cur_height, $cur_l, $cur_r);

    LOG_TAB--;
    log_tab_message(HTML_COLOR_MEDIUM_BLUE "}" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void avl_tree_dump(avl_node *const root)
{
    log_tab_message(HTML_COLOR_GOLD "============AVL TREE DUMP============\n" HTML_COLOR_CANCEL);
    avl_tree_dump_static(root);
    log_tab_message(HTML_COLOR_GOLD "=====================================\n" HTML_COLOR_CANCEL);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void avl_tree_dump_static(avl_node *const cur_node)
{
    if (cur_node == NULL) return;

    avl_tree_dump_static($cur_l  );
    avl_node_dump       (cur_node);
    avl_tree_dump_static($cur_r  );
}
#endif //!CONTEST

//--------------------------------------------------------------------------------------------------------------------------------
// possible rotate
//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *possible_rotate(struct avl_node *const node)
{
    clog_assert(node != NULL);

    struct avl_node *new_root = NULL;

    if ((new_root = possible_small_rotate_left (node)) != NULL) return new_root;
    if ((new_root = possible_small_rotate_right(node)) != NULL) return new_root;

    if ((new_root = possible_big_rotate_left (node)) != NULL) return new_root;
    if ((new_root = possible_big_rotate_right(node)) != NULL) return new_root;

    return node;
}

//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *possible_small_rotate_left(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    const int balance_cur = avl_get_balance(cur_node);
    const int balance_r   = avl_get_balance($cur_r  );

    if (balance_cur == -2 && (balance_r == -1 || balance_r == 0))
    {
        return avl_small_rotate_left(cur_node);
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *possible_small_rotate_right(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    const int balance_cur = avl_get_balance(cur_node);
    const int balance_l   = avl_get_balance($cur_l  );

    if (balance_cur == 2 && (balance_l == 1 || balance_l == 0))
    {
        return avl_small_rotate_right(cur_node);
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *possible_big_rotate_left(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    const int balance_cur = avl_get_balance(cur_node);
    const int balance_r   = avl_get_balance($cur_r  );

    if (balance_cur == -2 && balance_r == 1)
    {
        return avl_big_rotate_left(cur_node);
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *possible_big_rotate_right(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    const int balance_cur = avl_get_balance(cur_node);
    const int balance_l   = avl_get_balance($cur_l  );

    if (balance_cur == 2 && balance_l == -1)
    {
        return avl_big_rotate_right(cur_node);
    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------
// rotate
//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *avl_small_rotate_left(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    struct avl_node *const new_root = $cur_r;

    $cur_r = $r_l;
    new_root->left = cur_node;

    avl_reculc_height(cur_node);
    avl_reculc_height(new_root);

    return new_root;
}

//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *avl_small_rotate_right(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    struct avl_node *const new_root = $cur_l;

    $cur_l = $l_r;
    new_root->right = cur_node;

    avl_reculc_height(cur_node);
    avl_reculc_height(new_root);

    return new_root;
}

//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *avl_big_rotate_left(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    $cur_r = avl_small_rotate_right($cur_r);

    return avl_small_rotate_left(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------

static struct avl_node *avl_big_rotate_right(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    $cur_l = avl_small_rotate_left($cur_l);

    return avl_small_rotate_right(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------
// height io
//--------------------------------------------------------------------------------------------------------------------------------

static inline int avl_get_balance(struct avl_node *const cur_node)
{
    if (cur_node == NULL) return 0;

    return avl_get_height($cur_l) - avl_get_height($cur_r);
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline int avl_get_height(struct avl_node *const cur_node)
{
    return (cur_node == NULL) ? 0 : $cur_height;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void avl_reculc_height(struct avl_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    const int l_height = avl_get_height($cur_l);
    const int r_height = avl_get_height($cur_r);

    $cur_height = (l_height > r_height) ? l_height : r_height;
    $cur_height++;
}