#ifndef _SINS_BASE_RBTREE_H
#define _SINS_BASE_RBTREE_H

#include "base.h"

struct rb_node {
	sulong parent_color;
	struct rb_node *right;
	struct rb_node *left;
};

struct rb_root;
typedef int (*rb_cmp_fn_t)(struct rb_node *,
                           struct rb_node *,
                           struct rb_root *);

struct rb_root {
	struct rb_node    *root_node;
	rb_cmp_fn_t        cmp_func;
};

static inline void RB_INIT_ROOT(struct rb_root *root, rb_cmp_fn_t func)
{
    root->root_node = NULL;
    root->cmp_func = func;
}

interface struct rb_node *rb_first(struct rb_root *);
interface struct rb_node *rb_last(struct rb_root *);
interface struct rb_node *rb_prev(struct rb_node *);
interface struct rb_node *rb_next(struct rb_node *);
interface struct rb_node *rb_find(struct rb_node *, struct rb_root *);
interface struct rb_node *rb_findx(struct rb_node *,
                                   struct rb_root *,
                                   struct rb_node **,
                                   struct rb_node **);

interface struct rb_node *rb_lower(struct rb_node *, struct rb_root *);
interface struct rb_node *rb_upper(struct rb_node *, struct rb_root *);

interface int             rb_insert(struct rb_node *, struct rb_root *);
interface int             rb_minsert(struct rb_node *, struct rb_root *);
interface void            rb_erase(struct rb_node *, struct rb_root *);
interface struct rb_node *rb_finsert(struct rb_node *, struct rb_root *);

#endif

