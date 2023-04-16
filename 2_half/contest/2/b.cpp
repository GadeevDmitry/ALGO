#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
// STRUCT DECART_NODE
//================================================================================================================================

typedef struct decart_node
{
    int key;
    int prior;

    struct decart_node *par;
    struct decart_node *left;
    struct decart_node *right;
}
decart_node;

//--------------------------------------------------------------------------------------------------------------------------------
// DSL
//--------------------------------------------------------------------------------------------------------------------------------

#define $cur_key    (cur_node->key)
#define $cur_prior  (cur_node->prior)

#define $cur_p      (cur_node->par)
#define $cur_l      (cur_node->left)
#define $cur_r      (cur_node->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $p_key      ($cur_p->key)
#define $p_prior    ($cur_p->prior)

#define $p_p        ($cur_p->par)
#define $p_l        ($cur_p->left)
#define $p_r        ($cur_p->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $l_key      ($cur_l->key)
#define $l_prior    ($cur_l->prior)

#define $l_p        ($cur_l->par)
#define $l_l        ($cur_l->left)
#define $l_r        ($cur_l->right)

//--------------------------------------------------------------------------------------------------------------------------------

#define $r_key      ($cur_r->key)
#define $r_prior    ($cur_r->prior)

#define $r_p        ($cur_r->par)
#define $r_l        ($cur_r->left)
#define $r_r        ($cur_r->right)

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

inline bool decart_node_ctor(decart_node *const cur_node, const int key,
                                                          const int prior, decart_node *const par,
                                                                           decart_node *const left,
                                                                           decart_node *const right, const bool is_left_son);
decart_node *decart_node_new(                             const int key,
                                                          const int prior, decart_node *const par,
                                                                           decart_node *const left,
                                                                           decart_node *const right, const bool is_left_son);
void decart_node_dtor(decart_node *const cur_node);
void decart_tree_dtor(decart_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------
// task
//--------------------------------------------------------------------------------------------------------------------------------

decart_node *decart_tree_init();
decart_node *decart_tree_add(decart_node *cur_node, const int key,
                                                    const int prior);

#ifndef CONTEST
//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void decart_tree_dump       (const decart_node *const cur_node);
void decart_tree_dump_static(const decart_node *const cur_node);
void decart_node_dump       (const decart_node *const cur_node);

#endif //!CONTEST

//================================================================================================================================
// MAIN
//================================================================================================================================

bool input (      decart_node *const root);
void output(const decart_node *const root);

static void tree_output(const decart_node *const cur_node);
static void node_output(const decart_node *const cur_node);

//--------------------------------------------------------------------------------------------------------------------------------

int main()
{
    decart_node *root = decart_tree_init();
    if (!input(root)) return 0;

    output(root);
    decart_tree_dtor(root);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool input(decart_node *const root)
{
    int n = 0;
    if (scanf("%d", &n) != 1) return false;

    decart_node *max_key_node = root;

    for (int i = 1; i <= n; ++i)
    {
        int prior = 0;
        if (scanf("%*d %d", &prior) != 1) return false;

        max_key_node = decart_tree_add(max_key_node, i, prior);
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void output(const decart_node *const root)
{
    printf("YES\n");

    tree_output(root->left);
    tree_output(root->right);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void tree_output(const decart_node *const cur_node)
{
    if (cur_node == NULL) return;

    tree_output($cur_l);
    node_output(cur_node);
    tree_output($cur_r);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void node_output(const decart_node *const cur_node)
{
    clog_assert(cur_node != NULL);

    int num_par   = ($cur_p == NULL) ? 0 : $p_key;
    int num_left  = ($cur_l == NULL) ? 0 : $l_key;
    int num_right = ($cur_r == NULL) ? 0 : $r_key;

    printf("%d %d %d\n", num_par, num_left, num_right);
}

//================================================================================================================================
// BODY DECART
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

inline bool decart_node_ctor(decart_node *const cur_node, const int key,
                                                          const int prior, decart_node *const par,
                                                                           decart_node *const left,
                                                                           decart_node *const right, const bool is_left_son)
{
    clog_verify(cur_node != NULL, false);

    $cur_key   = key;
    $cur_prior = prior;

    $cur_p = par;
    $cur_l = left;
    $cur_r = right;

    if (par == NULL) return true;

    if (is_left_son) par->left  = cur_node;
    else             par->right = cur_node;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

decart_node *decart_node_new(const int key,
                             const int prior, decart_node *const par,
                                              decart_node *const left,
                                              decart_node *const right, const bool is_left_son)
{
    decart_node *result = (decart_node *) calloc(1, sizeof(decart_node));
    if          (result == NULL) return NULL;

    decart_node_ctor(result, key, prior, par, left, right, is_left_son);
    return result;
}

//--------------------------------------------------------------------------------------------------------------------------------

void decart_node_dtor(decart_node *const cur_node)
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
// task
//--------------------------------------------------------------------------------------------------------------------------------

decart_node *decart_tree_init()
{
    const int min_prior = (int) -2e9;

    return decart_node_new(0, min_prior, NULL, NULL, NULL, false);
}

//--------------------------------------------------------------------------------------------------------------------------------

decart_node *decart_tree_add(decart_node *cur_node, const int key,
                                                    const int prior)
{
    clog_verify(cur_node != NULL, NULL);

    while ($cur_prior > prior) cur_node = $cur_p;

    decart_node *old_cur_r = $cur_r;
    decart_node *new_node  = decart_node_new(key, prior, cur_node, $cur_r, NULL, false);
    if (old_cur_r != NULL) old_cur_r->par = new_node;

    return new_node;
}

#ifndef CONTEST
//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void decart_tree_dump(const decart_node *const cur_node)
{
    log_message(HTML_COLOR_LIME_GREEN "=====TREE_DUMP=====" HTML_COLOR_CANCEL "\n");

    decart_tree_dump_static(cur_node);

    log_message(HTML_COLOR_LIME_GREEN "===================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void decart_tree_dump_static(const decart_node *const cur_node)
{
    if (cur_node == NULL) return;

    decart_tree_dump_static($cur_l);
    decart_node_dump(cur_node);
    decart_tree_dump_static($cur_r);
}

//--------------------------------------------------------------------------------------------------------------------------------

void decart_node_dump(const decart_node *const cur_node)
{
    if (cur_node == NULL) return;

    log_tab_message(HTML_COLOR_GOLD
                    "decart_node (addr: %p)\n"
                    "{"
                    HTML_COLOR_CANCEL     "\n", cur_node);
    ++LOG_TAB;

    int num_par   = ($cur_p == NULL) ? 0 : $p_key;
    int num_left  = ($cur_l == NULL) ? 0 : $l_key;
    int num_right = ($cur_r == NULL) ? 0 : $r_key;

    log_tab_message(HTML_COLOR_DARK_ORANGE
                    "key   = %d\n"
                    "prior = %d\n\n"
                    HTML_COLOR_CANCEL
                    HTML_COLOR_MEDIUM_BLUE
                    "par   = %d\n"
                    "left  = %d\n"
                    "right = %d\n", $cur_key, $cur_prior, num_par, num_left, num_right);

    --LOG_TAB;
    log_tab_message(HTML_COLOR_GOLD "}" HTML_COLOR_CANCEL "\n");
}

#endif //!CONTEST