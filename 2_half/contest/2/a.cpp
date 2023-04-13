#include <stdio.h>
#include <stdlib.h>

#include "../../../lib/logs/log.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

struct avl_node
{
    int key;
    int height;

    avl_node *left;
    avl_node *right;
};

//--------------------------------------------------------------------------------------------------------------------------------

void inline avl_node_ctor(avl_node *const node, const int key,
                                                const int height = 1, avl_node *const left  = nullptr,
                                                                      avl_node *const right = nullptr);
avl_node *avl_node_new(const int key,
                       const int height = 1, avl_node *const left  = nullptr,
                                             avl_node *const right = nullptr);

void inline avl_node_dtor(avl_node *const node);
void        avl_tree_dtor(avl_node *const root);

//--------------------------------------------------------------------------------------------------------------------------------

int avl_lower_bound(avl_node *const node, const int key_to_find);

//--------------------------------------------------------------------------------------------------------------------------------

void                    avl_insert  (avl_node *const node, const int key_to_insert);
static inline avl_node *avl_find_key(avl_node *const node, const int key_to_find  );

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

void inline avl_node_ctor(avl_node *const cur_node, const int key,
                                                    const int height /* = 1 */, avl_node *const left  /* = nullptr */,
                                                                                avl_node *const right /* = nullptr */)
{
    log_assert(cur_node != nullptr);

    $cur_key    = key;
    $cur_height = height;

    $cur_l = left;
    $cur_r = right;
}

//--------------------------------------------------------------------------------------------------------------------------------

avl_node *avl_node_new(const int key,
                       const int height /* = 1 */, avl_node *const left  /* = nullptr */,
                                                   avl_node *const right /* = nullptr */)
{
    avl_node *new_node = (avl_node *) calloc(1, sizeof(avl_node));
    if       (new_node == nullptr) return nullptr;

    avl_node_ctor(new_node, key, height, left, right);

    return new_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

void inline avl_node_dtor(avl_node *const node)
{
    if (node != nullptr) free(node);
}

//--------------------------------------------------------------------------------------------------------------------------------

void avl_tree_dtor(avl_node *const root)
{
    if (root == nullptr) return;

    avl_tree_dtor(root->left );
    avl_tree_dtor(root->right);

    avl_node_dtor(root);
}

//--------------------------------------------------------------------------------------------------------------------------------
// lower_bound
//--------------------------------------------------------------------------------------------------------------------------------

int avl_lower_bound(avl_node *const node, const int key_to_find)
{
    avl_node *cur_node = node;

    while (cur_node != nullptr)
    {
        if($cur_key == key_to_find) return key_to_find;

        if($cur_key <  key_to_find) cur_node = $cur_r;
        else                        cur_node = $cur_l;
    }

    return -1;
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert
//--------------------------------------------------------------------------------------------------------------------------------

void avl_insert(avl_node *const node, const int key_to_insert)
{
    log_assert(node != nullptr);

    avl_node *cur_node = avl_find_key(node, key_to_insert);
    if ($cur_key == key_to_insert) return;

    avl_node *kid_node = avl_node_new(key_to_insert);
    if ($cur_key < key_to_insert) $cur_l = kid_node;
    else                          $cur_r = kid_node;

    possible_rotate(cur_node);
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline avl_node *avl_find_key(avl_node *cur_node, const int key_to_find)
{
    log_assert(cur_node != nullptr);

    while (true)
    {
        if ($cur_key == key_to_find) break;

        avl_node *node_cpy = cur_node;

        if ($cur_key < key_to_find) cur_node = $cur_l;
        else                        cur_node = $cur_r;

        if (cur_node == nullptr) { cur_node = node_cpy; break; }
    }

    return cur_node;
}


//--------------------------------------------------------------------------------------------------------------------------------
// rotate
//--------------------------------------------------------------------------------------------------------------------------------

static bool possible_rotate(avl_node *const node)
{
    log_assert(node != nullptr);

    if (possible_big_rotate_left (node)) return true;
    if (possible_big_rotate_right(node)) return true;

    if (possible_small_rotate_left (node)) return true;
    if (possible_small_rotate_right(node)) return true;

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool possible_small_rotate_left(avl_node *const node)
{
    log_assert(node != nullptr);

}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void avl_get_height(avl_node *const node)
{
    if ()
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void avl_reculc_height(avl_node *const cur_node)
{
    log_assert(cur_node != nullptr);
    log_assert($cur_l   != nullptr);
    log_assert($cur_r  != nullptr);

    $cur_height = ($l_height > $r_height) ? $l_height : $r_height;
    $cur_height++;
}