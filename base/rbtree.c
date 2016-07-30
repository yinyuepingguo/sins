#include "base/rbtree.h"

static inline void __rb_set_parent(struct rb_node *rb, struct rb_node *p)
{	rb->parent_color = ((rb->parent_color & 1) | (sulong)p);  }

static inline void __rb_set_color(struct rb_node *rb, sulong color)
{	rb->parent_color = ((rb->parent_color & ~1) | color);  }

static inline struct rb_node* __rb_parent( struct rb_node *r )
{	return ((struct rb_node *)((r)->parent_color & ~1));  }

static inline sulong __rb_color( struct rb_node *r )
{	return ((r)->parent_color & 3);  }

static inline void __rb_set_red( struct rb_node *r )
{	__rb_set_color(r, 0);  }

static inline void __rb_set_black( struct rb_node *r )
{	__rb_set_color(r, 1);  }

static inline int __rb_is_red( struct rb_node *r )
{	return __rb_color(r) == 0;  }

static inline int __rb_is_black( struct rb_node *r )
{	return __rb_color(r) != 0;  }


static void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
	/**
           Z :parent                     Z
            \                             \
			 A :node         ==>           X
			/ \                           / \
           F   X :right                  A   Y
		      / \                       / \
			 B   Y                     F   B
  	**/

	struct rb_node *right = node->right;
	struct rb_node *parent = __rb_parent(node);

	if ((node->right = right->left))
		__rb_set_parent(right->left, node);

	right->left = node;
	__rb_set_parent(right, parent);

	if(parent)
	{
		if (node == parent->left)
			parent->left = right;
		else
			parent->right = right;
	}
	else
		root->root_node = right;

	__rb_set_parent(node, right);
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->left;
	struct rb_node *parent = __rb_parent(node);

	if ((node->left = left->right))
		__rb_set_parent(left->right, node);
	left->right = node;

	__rb_set_parent(left, parent);

	if(parent)
	{
		if (node == parent->right)
			parent->right = left;
		else
			parent->left = left;
	}
	else
		root->root_node = left;

	__rb_set_parent(node, left);
}

static void __rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;

	while((parent = __rb_parent(node)) && __rb_is_red(parent))
	{
		gparent = __rb_parent(parent);

		if(parent == gparent->left)
		{
			{
				register struct rb_node *uncle = gparent->right;
				if(uncle && __rb_is_red(uncle))
				{
					__rb_set_black(uncle);
					__rb_set_black(parent);
					__rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if(parent->right == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			__rb_set_black(parent);
			__rb_set_red(gparent);
			__rb_rotate_right(gparent, root);
		} else {
			{
				register struct rb_node *uncle = gparent->left;
				if(uncle && __rb_is_red(uncle))
				{
					__rb_set_black(uncle);
					__rb_set_black(parent);
					__rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->left == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			__rb_set_black(parent);
			__rb_set_red(gparent);
			__rb_rotate_left(gparent, root);
		}
	}
	__rb_set_black(root->root_node);
}


static
struct rb_node * __rb_find( struct rb_node *node, struct rb_root *root, struct rb_node **par )
{
	struct rb_node *p, *n;
    n = p = root->root_node;
	if(p)
		for(;;) {
			int cr;
			cr = root->cmp_func(node, p, root);

			if(cr < 0)
				n = p->left;
			else if(cr > 0)
				n = p->right;
			else
				break;

			if(n == NULL)
				break;

			p = n;
		}

    if(par)
        *par = p;

	return n;
}

struct rb_node *rb_find( struct rb_node *node, struct rb_root *root )
{
	return __rb_find(node, root, NULL);
}

struct rb_node *__rb_find_left( struct rb_node *node,
							     struct rb_root *root )
{
	struct rb_node *n = node->left;
    for(;n;)
	{
		if( root->cmp_func(n, node, root) )
		{
			// then n < node
			n = n->right;
		}
		else
		{
			// n == node
			node = n;
			n = node->left;
		}
	}
	return node;
}

struct rb_node *__rb_find_right( struct rb_node *node,
							      struct rb_root *root )
{
	struct rb_node *n = node->right;
    for(;n;)
	{
		if( root->cmp_func(n, node, root) )
		{
			// then n > node
			n = n->left;
		}
		else
		{
			// n == node
			node = n;
			n = node->right;
		}
	}
	return node;
}

struct rb_node *
rb_findx( struct rb_node *node, struct rb_root *root, struct rb_node **pl, struct rb_node **pr )
{
	struct rb_node *aa;
    int cc;

	__rb_find(node, root, &aa);
	if(!aa) {
		if(pl)
			*pl = 0;
		if(pr)
			*pr = 0;
		return 0;
	}

	cc = root->cmp_func(node, aa, root);
	if(pl) {
		if(cc < 0)
		    *pl = rb_prev(aa);
		else if(cc == 0)
			*pl = __rb_find_left(aa, root);
		else
			*pl = aa;
	}
	if(pr) {
		if(cc > 0)
		    *pr = rb_next(aa);
        else if(cc == 0)
			*pr = __rb_find_right(aa, root);
		else
			*pr = aa;
	}
    return aa;
}

struct rb_node * rb_lower( struct rb_node *node, struct rb_root *root )
{
	struct rb_node *n;
	n = rb_find(node, root);
	if(n)
		n = __rb_find_left(n, root);

	return n;
}

struct rb_node * rb_upper( struct rb_node *node, struct rb_root *root )
{
	struct rb_node *n;
	n = rb_find(node, root);
	if(n)
		n = rb_next( __rb_find_right(n, root) );

    return n;
}

static inline void __rb_link_node( struct rb_node *node,
                                   struct rb_node *parent,
								   struct rb_node **rblink )
{
	node->parent_color = (sulong)parent;
	node->left = node->right = 0;
	*rblink = node;
}

/**
return:
        return node if found a val. return 0 and link-in if not found.
**/
static inline struct rb_node *__rb_insert( struct rb_node *node, struct rb_root *root )
{
	struct rb_node ** p = &(root->root_node);
    struct rb_node * parent = 0;
	int cr;
	while (*p) {
		parent = *p;
		cr = root->cmp_func(node, parent, root);
		if(cr < 0)
    		p = & parent->left;
        else if(cr > 0)
    		p = & parent->right;
		else
		    return parent;
	}
	__rb_link_node(node, parent, p);

    return 0;
}

struct rb_node *rb_finsert( struct rb_node * node, struct rb_root *root )
{
    struct rb_node *r;
	if((r = __rb_insert(node, root)))
		return r;

	__rb_insert_color(node, root);
    return node;
}

int rb_insert( struct rb_node * node, struct rb_root *root )
{
	return rb_finsert(node, root) == node;
}

int rb_minsert( struct rb_node * node, struct rb_root *root )
{
	struct rb_node *n;

	if((n = __rb_insert( node, root )))	{
		// find the last one
		n = __rb_find_right(n, root);

	    for(;;)
		{
			// if n->right == 0 then link to n->right
			if(! n->right) {
				__rb_link_node( node, n, & n->right );
				break;
			}

			// find the min node of tree nn->right
			n = n->right;
			while(n->left)
				n = n->left;

			if( root->cmp_func(node, n, root) )	{
				__rb_link_node( node, n, & n->left );
				break;
			}
		}
	}
	__rb_insert_color(node, root);
	return n == 0;
}

static void __rb_erase_color( struct rb_node *node,
                              struct rb_node *parent,
			                  struct rb_root *root )
{
	struct rb_node *other;

	while ((!node || __rb_is_black(node)) && node != root->root_node)
	{
		if (parent->left == node)
		{
			other = parent->right;
			if (__rb_is_red(other))
			{
				__rb_set_black(other);
				__rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->right;
			}
			if ((!other->left || __rb_is_black(other->left)) &&
			    (!other->right || __rb_is_black(other->right)))
			{
				__rb_set_red(other);
				node = parent;
				parent = __rb_parent(node);
			}
			else
			{
				if (!other->right || __rb_is_black(other->right))
				{
					struct rb_node *o_left;
					if ((o_left = other->left))
						__rb_set_black(o_left);
					__rb_set_red(other);
					__rb_rotate_right(other, root);
					other = parent->right;
				}
				__rb_set_color(other, __rb_color(parent));
				__rb_set_black(parent);
				if (other->right)
					__rb_set_black(other->right);
				__rb_rotate_left(parent, root);
				node = root->root_node;
				break;
			}
		}
		else
		{
			other = parent->left;
			if(__rb_is_red(other))
			{
				__rb_set_black(other);
				__rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->left;
			}
			if ((!other->left || __rb_is_black(other->left)) &&
			    (!other->right || __rb_is_black(other->right)))
			{
				__rb_set_red(other);
				node = parent;
				parent = __rb_parent(node);
			}
			else
			{
				if (!other->left || __rb_is_black(other->left))
				{
					register struct rb_node *o_right;
					if ((o_right = other->right))
						__rb_set_black(o_right);
					__rb_set_red(other);
					__rb_rotate_left(other, root);
					other = parent->left;
				}
				__rb_set_color(other, __rb_color(parent));
				__rb_set_black(parent);
				if (other->left)
					__rb_set_black(other->left);
				__rb_rotate_right(parent, root);
				node = root->root_node;
				break;
			}
		}
	}
	if (node)
		__rb_set_black(node);
}

void rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	sulong color;

	if (!node->left)
		child = node->right;
	else if (!node->right)
		child = node->left;
	else
	{
		struct rb_node *old = node, *left;

		node = node->right;
		while ((left = node->left) != 0)
			node = left;
		child = node->right;
		parent = __rb_parent(node);
		color = __rb_color(node);

		if (child)
			__rb_set_parent(child, parent);
		if (parent == old) {
			parent->right = child;
			parent = node;
		} else
			parent->left = child;

		node->parent_color = old->parent_color;
		node->right = old->right;
		node->left = old->left;

		if (__rb_parent(old))
		{
			if (__rb_parent(old)->left == old)
				__rb_parent(old)->left = node;
			else
				__rb_parent(old)->right = node;
		} else
			root->root_node = node;

		__rb_set_parent(old->left, node);
		if (old->right)
			__rb_set_parent(old->right, node);
		goto bcolor;
	}

	parent = __rb_parent(node);
	color = __rb_color(node);

	if (child)
		__rb_set_parent(child, parent);
	if (parent)
	{
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root->root_node = child;

 bcolor:
	if (color == 1)
		__rb_erase_color(child, parent, root);
}

struct rb_node *rb_first(struct rb_root *root)
{
	struct rb_node	*n;

	n = root->root_node;
	if (!n)
		return 0;

	while (n->left)
		n = n->left;

	return n;
}

struct rb_node *rb_last(struct rb_root *root)
{
	struct rb_node	*n;

	n = root->root_node;
	if (!n)
		return 0;

	while (n->right)
		n = n->right;

	return n;
}

struct rb_node *rb_next(struct rb_node *node)
{
	struct rb_node *parent;

	if (__rb_parent(node) == node)
		return 0;

	if (node->right) {
		node = node->right;
		while (node->left)
			node = node->left;
		return node;
	}

	while ((parent = __rb_parent(node)) && node == parent->right)
		node = parent;

	return parent;
}

struct rb_node *rb_prev(struct rb_node *node)
{
	struct rb_node *parent;

	if (__rb_parent(node) == node)
		return 0;

	if (node->left) {
		node = node->left;
		while (node->right)
			node = node->right;
		return node;
	}

	while ((parent = __rb_parent(node)) && node == parent->left)
		node = parent;

	return parent;
}





