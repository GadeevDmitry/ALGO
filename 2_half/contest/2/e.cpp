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
#define clog_assert(condition)          log_assert(condition)
#else
#define clog_verify(condition, ret_val)
#define clog_assert(condition)
#endif

//================================================================================================================================
// STRUCT SPLAY
//================================================================================================================================

typedef struct splay_node
{
    const char *name_this;
    const char *name_belong;

    struct splay_node *par;
    struct splay_node *left;
    struct splay_node *right;
}
splay_node;

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

inline bool splay_node_ctor(splay_node *const cur_node, const char *const name_this,
                                                        const char *const name_belong, splay_node *const par,
                                                                                       splay_node *const left,
                                                                                       splay_node *const right, const bool is_left_son);
splay_node *splay_node_new (                            const char *const name_this,
                                                        const char *const name_belong, splay_node *const par,
                                                                                       splay_node *const left,
                                                                                       splay_node *const right, const bool is_left_son);
inline void splay_node_dtor(splay_node *const cur_node);
void        splay_tree_dtor(splay_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------
// queries
//--------------------------------------------------------------------------------------------------------------------------------

bool splay_tree_insert(splay_node **const root_node, const char *const name_this, const char *const name_belong);
bool splay_tree_find  (splay_node **const root_node, const char *const name_this);

static inline void splay_tree(splay_node *const cur_node);

#ifndef CONTEST
//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void        splay_node_dump       (const splay_node *const cur_node);
void        splay_tree_dump       (const splay_node *const root);
static void splay_tree_dump_static(const splay_node *const cur_node);

#endif //!CONTEST

//--------------------------------------------------------------------------------------------------------------------------------
// possible rotate
//--------------------------------------------------------------------------------------------------------------------------------

static bool rotate_handler(splay_node *const cur_node);

static bool possible_rotate_double_rr(splay_node *const cur_node);
static bool possible_rotate_double_lr(splay_node *const cur_node);
static bool possible_rotate_double_ll(splay_node *const cur_node);
static bool possible_rotate_double_rl(splay_node *const cur_node);
static bool possible_rotate_single_r (splay_node *const cur_node);
static bool possible_rotate_single_l (splay_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------
// rotate
//--------------------------------------------------------------------------------------------------------------------------------

static inline void rotate_double_rr(splay_node *const cur_node);
static inline void rotate_double_lr(splay_node *const cur_node);
static inline void rotate_double_ll(splay_node *const cur_node);
static inline void rotate_double_rl(splay_node *const cur_node);
static void        rotate_single_r (splay_node *const cur_node);
static void        rotate_single_l (splay_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------
// relative
//--------------------------------------------------------------------------------------------------------------------------------

static inline bool is_root     (const splay_node *const cur_node);
static inline bool is_left_son (const splay_node *const cur_node);
static inline bool is_right_son(const splay_node *const cur_node);

static inline splay_node *get_left_son (const splay_node *const cur_node);
static inline splay_node *get_right_son(const splay_node *const cur_node);

//================================================================================================================================
// MAIN
//================================================================================================================================

void solve();

static bool tree_init    (splay_node **nick_root, splay_node **name_root, char **_str_store, const size_t STR_SIZE);
static bool query_handler(splay_node **nick_root, splay_node **name_root,                    const size_t STR_SIZE);

//--------------------------------------------------------------------------------------------------------------------------------

int main()
{
    solve();
}

//--------------------------------------------------------------------------------------------------------------------------------

void solve()
{
    const size_t STR_SIZE = 601;

    splay_node *nick_root = NULL;
    splay_node *name_root = NULL;

    char       *str_store = NULL;

    if (!tree_init    (&nick_root, &name_root, &str_store, STR_SIZE)) return;
    if (!query_handler(&nick_root, &name_root,             STR_SIZE)) return;

    free(str_store);

    splay_tree_dtor(nick_root);
    splay_tree_dtor(name_root);
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool tree_init(splay_node **nick_root,
                      splay_node **name_root, char **_str_store, const size_t STR_SIZE)
{
    clog_assert( nick_root != NULL);
    clog_assert( name_root != NULL);
    clog_assert(_str_store != NULL);

    size_t n = 0;
    if (scanf("%lu", &n) != 1) return false;

    char *str_store = (char *) calloc(2*n * STR_SIZE, sizeof(char));
    char *name_ptr  = str_store;
    char *nick_ptr  = str_store;

    while (n--)
    {
        if (scanf("%600s", nick_ptr) != 1) { return false; } name_ptr = nick_ptr + strlen(nick_ptr) + 1;
        if (scanf("%600s", name_ptr) != 1) { return false; }

        splay_tree_insert(nick_root, nick_ptr, name_ptr);
        splay_tree_insert(name_root, name_ptr, nick_ptr);

        nick_ptr = name_ptr + strlen(name_ptr) + 1;
    }

    *(_str_store) = str_store;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool query_handler(splay_node **nick_root,
                          splay_node **name_root, const size_t STR_SIZE)
{
    clog_assert(nick_root != NULL);
    clog_assert(name_root != NULL);

    size_t q = 0;
    if (scanf("%lu", &q) != 1) return false;

    char name_buff[STR_SIZE];

    while (q--)
    {
        if (scanf("%600s", name_buff) != 1) return false;

        if      (splay_tree_find(nick_root, name_buff)) printf("%s\n", (*nick_root)->name_belong);
        else if (splay_tree_find(name_root, name_buff)) printf("%s\n", (*name_root)->name_belong);
        else    return false;
    }

    return true;
}

//================================================================================================================================
// BODY SPLAY
//================================================================================================================================

#define $cur_this   (cur_node->name_this)
#define $cur_belong (cur_node->name_belong)

#define $cur_p      (cur_node->par)
#define $cur_l      (cur_node->left)
#define $cur_r      (cur_node->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $p_this     ($cur_p->name_this)
#define $p_belong   ($cur_p->name_belong)

#define $p_p        ($cur_p->par)
#define $p_l        ($cur_p->left)
#define $p_r        ($cur_p->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $l_this     ($cur_l->name_this)
#define $l_belong   ($cur_l->name_belong)

#define $l_p        ($cur_l->par)
#define $l_l        ($cur_l->left)
#define $l_r        ($cur_l->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $r_this     ($cur_r->name_this)
#define $r_belong   ($cur_r->name_belong)

#define $r_p        ($cur_r->par)
#define $r_l        ($cur_r->left)
#define $r_r        ($cur_r->right)

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

inline bool splay_node_ctor(splay_node *const cur_node, const char *const name_this,
                                                        const char *const name_belong, splay_node *const par,
                                                                                       splay_node *const left,
                                                                                       splay_node *const right, const bool is_left_son)
{
    clog_verify(cur_node    != NULL, false);
    clog_verify(name_this   != NULL, false);
    clog_verify(name_belong != NULL, false);

    $cur_this   = name_this;
    $cur_belong = name_belong;

    $cur_p = par;
    $cur_l = left;
    $cur_r = right;

    if (par == NULL) return true;

    if (is_left_son) $p_l = cur_node;
    else             $p_r = cur_node;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

splay_node *splay_node_new(const char *const name_this,
                           const char *const name_belong, splay_node *const par,
                                                          splay_node *const left,
                                                          splay_node *const right, const bool is_left_son)
{
    clog_verify(name_this   != NULL, NULL);
    clog_verify(name_belong != NULL, NULL);

    splay_node *result = (splay_node *) calloc(1, sizeof(splay_node));
    if         (result == NULL) return NULL;

    splay_node_ctor(result, name_this, name_belong, par, left, right, is_left_son);
    return result;
}

//--------------------------------------------------------------------------------------------------------------------------------

inline void splay_node_dtor(splay_node *const cur_node)
{
    if (cur_node != NULL) free(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------

void splay_tree_dtor(splay_node *const cur_node)
{
    if (cur_node == NULL) return;

    splay_tree_dtor($cur_l);
    splay_tree_dtor($cur_r);
    splay_node_dtor(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------
// queries
//--------------------------------------------------------------------------------------------------------------------------------

bool splay_tree_insert(splay_node **const root_node, const char *const name_this,
                                                     const char *const name_belong)
{
    clog_verify(root_node   != NULL, false);
    clog_verify(name_this   != NULL, false);
    clog_verify(name_belong != NULL, false);

    splay_node *cur_node = *root_node;
    splay_node *par_node =       NULL;

    bool is_left_son = false;

    while (cur_node != NULL)
    {
        int cmp = strcmp(name_this, $cur_this);

        if (cmp == 0) return true;

        if (cmp < 0) { par_node = cur_node; cur_node = $cur_l; is_left_son = true;  }
        else         { par_node = cur_node; cur_node = $cur_r; is_left_son = false; }
    }

    splay_node *result = splay_node_new(name_this, name_belong, par_node, NULL, NULL, is_left_son);
    splay_tree (result);

    if    (result != NULL) *root_node = result;
    return result != NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool splay_tree_find(splay_node **const root_node, const char *const name_this)
{
    clog_verify(root_node != NULL, false);
    clog_verify(name_this != NULL, false);

    splay_node *cur_node = *root_node;

    while (cur_node != NULL)
    {
        int cmp = strcmp(name_this, $cur_this);

        if (cmp == 0) break;
        if (cmp <  0) cur_node = $cur_l;
        else          cur_node = $cur_r;
    }

    splay_tree(cur_node);

    if    (cur_node != NULL) *root_node = cur_node;
    return cur_node != NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void splay_tree(splay_node *const cur_node)
{
    if (cur_node == NULL) return;

    while (rotate_handler(cur_node)) ;
}

#ifndef CONTEST
//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void splay_node_dump(const splay_node *const cur_node)
{
    log_tab_message(HTML_COLOR_GOLD
                    "splay_node (addr: %p)\n"
                    "{"
                    HTML_COLOR_CANCEL    "\n", cur_node);

    if (cur_node == nullptr) { log_tab_message(HTML_COLOR_GOLD "}" HTML_COLOR_CANCEL "\n"); return; }

    ++LOG_TAB;

    log_tab_message(HTML_COLOR_DARK_ORANGE
                    "name_this   = %s\n"
                    "name_belong = %s\n\n"
                    HTML_COLOR_CANCEL
                    HTML_COLOR_MEDIUM_BLUE
                    "par   = %p\n"
                    "left  = %p\n"
                    "right = %p"
                    HTML_COLOR_CANCEL "\n", $cur_this, $cur_belong, $cur_p, $cur_l, $cur_r);

    --LOG_TAB;
    log_tab_message(HTML_COLOR_GOLD "}" HTML_COLOR_CANCEL "\n");

}

//--------------------------------------------------------------------------------------------------------------------------------

void splay_tree_dump(const splay_node *const root)
{
    log_tab_message(HTML_COLOR_LIME_GREEN "===========TREE_DUMP===========" HTML_COLOR_CANCEL "\n");

    splay_tree_dump_static(root);

    log_tab_message(HTML_COLOR_LIME_GREEN "===============================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void splay_tree_dump_static(const splay_node *const cur_node)
{
    if (cur_node == NULL) return;

    splay_tree_dump_static($cur_l);
    splay_node_dump       (cur_node);
    splay_tree_dump_static($cur_r);
}

#endif //!CONTEST

//--------------------------------------------------------------------------------------------------------------------------------
// possible rotate
//--------------------------------------------------------------------------------------------------------------------------------

static bool rotate_handler(splay_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    if (possible_rotate_double_rr(cur_node)) return true;
    if (possible_rotate_double_lr(cur_node)) return true;
    if (possible_rotate_double_ll(cur_node)) return true;
    if (possible_rotate_double_rl(cur_node)) return true;

    if (possible_rotate_single_r (cur_node)) return true;
    if (possible_rotate_single_l (cur_node)) return true;

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool possible_rotate_double_rr(splay_node *const cur_node)
{
    if (cur_node == NULL || $cur_p == NULL) return false;

    if (is_left_son(cur_node) && is_left_son($cur_p))
    {
        rotate_double_rr(cur_node);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool possible_rotate_double_lr(splay_node *const cur_node)
{
    if (cur_node == NULL || $cur_p == NULL) return false;

    if (is_right_son(cur_node) && is_left_son($cur_p))
    {
        rotate_double_lr(cur_node);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool possible_rotate_double_ll(splay_node *const cur_node)
{
    if (cur_node == NULL || $cur_p == NULL) return false;

    if (is_right_son(cur_node) && is_right_son($cur_p))
    {
        rotate_double_ll(cur_node);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool possible_rotate_double_rl(splay_node *const cur_node)
{
    if (cur_node == NULL || $cur_p == NULL) return false;

    if (is_left_son(cur_node) && is_right_son($cur_p))
    {
        rotate_double_rl(cur_node);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool possible_rotate_single_r(splay_node *const cur_node)
{
    if (cur_node == NULL || $cur_p == NULL) return false;

    if (is_left_son(cur_node) && is_root($cur_p))
    {
        rotate_single_r(cur_node);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool possible_rotate_single_l(splay_node *const cur_node)
{
    if (cur_node == NULL || $cur_p == NULL) return false;

    if (is_right_son(cur_node) && is_root($cur_p))
    {
        rotate_single_l(cur_node);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------
// rotate
//--------------------------------------------------------------------------------------------------------------------------------

static inline void rotate_double_rr(splay_node *const cur_node)
{
    rotate_single_r($cur_p);
    rotate_single_r(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void rotate_double_lr(splay_node *const cur_node)
{
    rotate_single_l(cur_node);
    rotate_single_r(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void rotate_double_ll(splay_node *const cur_node)
{
    rotate_single_l($cur_p);
    rotate_single_l(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void rotate_double_rl(splay_node *const cur_node)
{
    rotate_single_r(cur_node);
    rotate_single_l(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void rotate_single_r(splay_node *const cur_node)
{
    splay_node *const par_node = $cur_p;

    if      (is_left_son (par_node)) par_node->par->left  = cur_node;
    else if (is_right_son(par_node)) par_node->par->right = cur_node;

    cur_node->par = par_node->par;
    par_node->par = cur_node;

    par_node->left = cur_node->right;
    if (par_node->left != NULL) par_node->left->par = par_node;

    cur_node->right = par_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void rotate_single_l(splay_node *const cur_node)
{
    splay_node *const par_node = $cur_p;

    if      (is_left_son (par_node)) par_node->par->left  = cur_node;
    else if (is_right_son(par_node)) par_node->par->right = cur_node;

    cur_node->par = par_node->par;
    par_node->par = cur_node;

    par_node->right = cur_node->left;
    if (par_node->right != NULL) par_node->right->par = par_node;

    cur_node->left = par_node;
}

//--------------------------------------------------------------------------------------------------------------------------------
// relative
//--------------------------------------------------------------------------------------------------------------------------------

static inline bool is_root(const splay_node *const cur_node)
{
    if (cur_node == NULL) return false;

    return $cur_p == NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline bool is_left_son(const splay_node *const cur_node)
{
    if (cur_node == NULL) return false;

    return cur_node == get_left_son($cur_p);
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline bool is_right_son(const splay_node *const cur_node)
{
    if (cur_node == NULL) return false;

    return cur_node == get_right_son($cur_p);
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline splay_node *get_left_son(const splay_node *const cur_node)
{
    return (cur_node == NULL) ? NULL : $cur_l;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline splay_node *get_right_son(const splay_node *const cur_node)
{
    return (cur_node == NULL) ? NULL : $cur_r;
}