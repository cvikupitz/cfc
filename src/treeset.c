/**
 * MIT License
 *
 * Copyright (c) 2020 Cole Vikupitz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <math.h>
#include <stdlib.h>
#include "boundedstack.h"
#include "treeset.h"

/*
 * Struct for a node inside the tree.
 */
typedef struct node {
    struct node *parent;        /* Pointer to the parent node */
    struct node *left;          /* Pointer to the left child node */
    struct node *right;         /* Pointer to the right child node */
    char color;                 /* The node's current color (RED or BLACK) */
    void *data;                 /* Pointer to the element */
} Node;

/*
 * Struct for the treeset ADT.
 */
struct treeset {
    int (*cmp)(void *, void *); /* Function for comparing elements in the tree */
    Node *root;                 /* Pointer to the tree's root node */
    long size;                  /* The treeset's current size */
};

/*
 * Struct iterator for populating tree elements via tree traversal.
 */
typedef struct {
    void **items;       /* Array of treeset items */
    long next;          /* Next index in iteration */
} TreeIter;

/* Value for painting a node RED */
#define RED 0
/* Value for painting a node BLACK */
#define BLACK 1
/* Macro for evaluating the color of the given node */
#define COLOR(x) ( ( (x) != NULL ) ? x->color : BLACK )

Status treeset_new(TreeSet **tree, int (*comparator)(void *, void *)) {

    /* Allocate the struct, check for allocation failures */
    TreeSet *temp = (TreeSet *)malloc(sizeof(TreeSet));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initialize remaining struct members */
    temp->cmp = comparator;
    temp->root = NULL;
    temp->size = 0L;
    *tree = temp;

    return OK;
}

/* Macro to check if the treeset is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )

static Node *allocNode(void *item) {

    /* Allocates memory for the tree node */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        /* Initializes the tree node's members */
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;
        node->color = RED;
        node->data = item;
    }

    return node;
}

/*
 * Searches the tree and returns the node with the specified item.
 */
static Node *findNode(TreeSet *tree, void *item) {

    Node *temp = tree->root;
    while (temp != NULL) {
        int cmp = (*tree->cmp)(item, temp->data);
        if (cmp == 0)  /* Node is found */
            break;
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    return temp;
}

/*
 * Performs a single left rotation on the specified node.
 */
static void rotateLeft(TreeSet *tree, Node *node) {

    /* Links min node in right subtree to node */
    Node *temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL)
        temp->left->parent = node;

    /* Links the nodes to perform the left rotation */
    temp->parent = node->parent;
    if (node->parent == NULL) {
        tree->root = temp;
    } else {
        if (node->parent->left == node)
            node->parent->left = temp;
        else
            node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}

/*
 * Performs a single right rotation on the specified node
 */
static void rotateRight(TreeSet *tree, Node *node) {

    /* Links max node in left subtree to node */
    Node *temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL)
        temp->right->parent = node;

    /* Links the nodes to perform the right rotation */
    temp->parent = node->parent;
    if (node->parent == NULL) {
        tree->root = temp;
    } else {
        if (node->parent->left == node)
            node->parent->left = temp;
        else
            node->parent->right = temp;
    }

    temp->right = node;
    node->parent = temp;
}

/*
 * Inserts the specified node into the tree.
 */
static void insertNode(TreeSet *tree, Node *node) {

    Node *temp = tree->root, *parent = NULL;
    int cmp = 0;
    tree->size++;

    /* Traverse down to the NIL node where the node is to be placed */
    while (temp != NULL) {
        parent = temp;
        cmp = (*tree->cmp)(node->data, temp->data);
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    /* Links the node into place with the tree */
    node->parent = parent;
    if (parent == NULL) {
        tree->root = node;
    } else {
        if (cmp < 0)
            parent->left = node;
        else
            parent->right = node;
    }
}

/*
 * Performs recoloring and rotations on the tree after an insertion to
 * ensure the red-black tree properties are upheld.
 */
static void insertFixup(TreeSet *tree, Node *node) {

    /* Rebalance needed only if the parent is red */
    while (COLOR(node->parent) == RED && node->parent->parent != NULL) {
        if (node->parent == node->parent->parent->left) {
            Node *uncle = node->parent->parent->right;
            if (COLOR(uncle) == RED) {
                /* Case: uncle is red, so need to color parent, uncle and grandparent */
                /* Advance up to grandparent node */
                node->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            } else {
                /* Case: uncle is black and node is right child */
                /* Advance up to parent node and perform left rotation */
                if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(tree, node);
                }
                /* Case: uncle is black and node is left child */
                /* Recolor parent, grandparent, and rotate grandparent right */
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateRight(tree, node->parent->parent);
            }
        } else {
            Node *uncle = node->parent->parent->left;
            if (COLOR(uncle) == RED) {
                /* Case: uncle is red, so need to color parent, uncle and grandparent */
                /* Advance up to grandparent node */
                node->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            } else {
                /* Case: uncle is black and node is left child */
                /* Advance up to parent node and perform right rotation */
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(tree, node);
                }
                /* Case: uncle is black and node is left child */
                /* Recolor parent, grandparent, and rotate grandparent left */
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateLeft(tree, node->parent->parent);
            }
        }
    }

    /* Tree root node must always be black */
    tree->root->color = BLACK;
}

Status treeset_add(TreeSet *tree, void *item) {

    /* Checks if the item is already present */
    if (findNode(tree, item) != NULL)
        return ALREADY_EXISTS;

    /* Allocates memory for the new node */
    Node *node = allocNode(item);
    if (node == NULL)
        return ALLOC_FAILURE;

    /* Inserts the new item in the tree */
    insertNode(tree, node);
    insertFixup(tree, node);

    return OK;
}

Boolean treeset_contains(TreeSet *tree, void *item) {
    return ( findNode(tree, item) != NULL ) ? TRUE : FALSE;
}

/*
 * Fetches and returns the node with the minimum value in the given subtree.
 */
static Node *getMin(Node *node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

/*
 * Fetches and returns the node with the maximum value in the given subtree.
 */
static Node *getMax(Node *node) {
    while (node->right != NULL)
        node = node->right;
    return node;
}

Status treeset_first(TreeSet *tree, void **first) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;
    /* Fetches the first item, saves into pointer */
    Node *minNode = getMin(tree->root);
    *first = minNode->data;

    return OK;
}

Status treeset_last(TreeSet *tree, void **last) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;
    /* Fetches the last item, saves into pointer */
    Node *maxNode = getMax(tree->root);
    *last = maxNode->data;

    return OK;
}

Status treeset_floor(TreeSet *tree, void *item, void **floor) {

    Node *temp = NULL;
    Node *current = tree->root;

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;

    while (current != NULL) {
        int cmp = (*tree->cmp)(item, current->data);
        if (cmp == 0) {
            /* Value found, return as floor */
            temp = current;
            break;
        }
        if (cmp < 0) {
            current = current->left;
        } else {
            /* Potential floor value found */
            temp = current;
            current = current->right;
        }
    }

    /* If floor value found, save into pointer */
    if (temp == NULL)
        return NOT_FOUND;
    *floor = temp->data;

    return OK;
}

Status treeset_ceiling(TreeSet *tree, void *item, void **ceiling) {

    Node *temp = NULL;
    Node *current = tree->root;

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;

    while (current != NULL) {
        int cmp = (*tree->cmp)(item, current->data);
        if (cmp == 0) {
            /* Value found, return as ceiling */
            temp = current;
            break;
        }
        if (cmp > 0) {
            current = current->right;
        } else {
            /* Potential ceiling value found */
            temp = current;
            current = current->left;
        }
    }

    /* If ceiling value found, save into pointer */
    if (temp == NULL)
        return NOT_FOUND;
    *ceiling = temp->data;

    return OK;
}

Status treeset_lower(TreeSet *tree, void *item, void **lower) {

    Node *temp = NULL;
    Node *current = tree->root;

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;

    while (current != NULL) {
        int cmp = (*tree->cmp)(item, current->data);
        if (cmp <= 0) {
            current = current->left;
        } else {
            /* Potential lower value found */
            temp = current;
            current = current->right;
        }
    }

    /* If lower value found, save into pointer */
    if (temp == NULL)
        return NOT_FOUND;
    *lower = temp->data;

    return OK;
}

Status treeset_higher(TreeSet *tree, void *item, void **higher) {

    Node *temp = NULL;
    Node *current = tree->root;

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;

    while (current != NULL) {
        int cmp = (*tree->cmp)(item, current->data);
        if (cmp >= 0) {
            current = current->right;
        } else {
            /* Potential higher value found */
            temp = current;
            current = current->left;
        }
    }

    /* If higher value found, save into pointer */
    if (temp == NULL)
        return NOT_FOUND;
    *higher = temp->data;

    return OK;
}

/*
 * Performs recoloring and rotations on the tree after a deletion to
 * ensure the red-black tree properties are upheld.
 */
static void deleteFixup(TreeSet *tree, Node *node, Node *parent) {

    /* Fixup required only if node deleted is black */
    while (node != tree->root && COLOR(node) == BLACK) {

        if (node == parent->left) {
            Node *sibling = parent->right;
            /* Case: sibling is red */
            /* Recolor sibling and parent, then rotate parent left */
            if (COLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateLeft(tree, parent);
                sibling = parent->right;
            }

            if (COLOR(sibling->left) == BLACK && COLOR(sibling->right) == BLACK) {
                /* Case: sibling has no red children */
                /* Recolor sibling, advance to parent node */
                sibling->color = RED;
                node = parent;
                parent = parent->parent;
            } else {
                if (COLOR(sibling->right) == BLACK) {
                    /* Case: sibling has left red child */
                    /* Recolor sibling & sibling's left child, rotate sibling right */
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(tree, sibling);
                    sibling = parent->right;
                }
                /* Case: sibling has right red child */
                /* Recolor parent, sibling, & sibling's right child */
                /* Rotate the parent node left */
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(tree, parent);
                node = tree->root;
            }
        } else {
            Node *sibling = parent->left;
            /* Case: sibling is red */
            /* Recolor sibling and parent, then rotate parent right */
            if (COLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateRight(tree, parent);
                sibling = parent->left;
            }
            if (COLOR(sibling->right) == BLACK && COLOR(sibling->left) == BLACK) {
                /* Case: sibling has no red children */
                /* Recolor sibling, advance to parent node */
                sibling->color = RED;
                node = parent;
                parent = parent->parent;
            } else {
                if (COLOR(sibling->left) == BLACK) {
                    /* Case: sibling has right red child */
                    /* Recolor sibling & sibling's right child, rotate sibling left */
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(tree, sibling);
                    sibling = parent->left;
                }
                /* Case: sibling has left red child */
                /* Recolor parent, sibling, & sibling's left child */
                /* Rotate the parent node right */
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(tree, parent);
                node = tree->root;
            }
        }
    }

    node->color = BLACK;
}

/*
 * Deletes the specified node from the tree, then performs a fixup to ensure the
 * red-black tree properties are upheld. The node to be destroyed is then stored
 * into '*src'.
 */
static void deleteNode(TreeSet *tree, Node *node, Node **src) {

    Node *splice, *child;
    tree->size--;

    /* Finds the node to be removed, swaps elements within predecessor */
    if (node->left == NULL) {
        splice = node;
        child = node->right;
    } else if (node->right == NULL) {
        splice = node;
        child = node->left;
    } else {
        splice = node->left;
        while (splice->right != NULL)
            splice = splice->right;
        child = splice->left;
        node->data = splice->data;
    }

    /* Unlinks the node from the tree */
    Node *parent = splice->parent;
    if (child != NULL)
        child->parent = parent;
    if (parent == NULL) {
        tree->root = child;
        *src = splice;
        return;
    }

    /* Links up the remaining nodes */
    if (splice == parent->left)
        parent->left = child;
    else
        parent->right = child;

    /* Performs a fixup on the tree after removal */
    if (splice->color == BLACK)
        deleteFixup(tree, child, parent);
    *src = splice;
}

Status treeset_pollFirst(TreeSet *tree, void **first) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;
    /* Retrieves the minimum node, saves the item into pointer */
    Node *node = getMin(tree->root);
    *first = node->data;

    /* Remove node from tree, free allocated memory */
    Node *temp;
    deleteNode(tree, node, &temp);
    free(temp);

    return OK;
}

Status treeset_pollLast(TreeSet *tree, void **last) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;
    /* Retrieves the maximum node, saves the item into pointer */
    Node *node = getMax(tree->root);
    *last = node->data;

    /* Remove node from tree, free allocated memory */
    Node *temp;
    deleteNode(tree, node, &temp);
    free(temp);

    return OK;
}

Status treeset_remove(TreeSet *tree, void *item, void (*destructor)(void *)) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the item is present */
    Node *node = findNode(tree, item);
    if (node == NULL)
        return NOT_FOUND;

    /* Removed node from tree, free allocated memory */
    Node *temp;
    void *toDelete = node->data;
    deleteNode(tree, node, &temp);
    if (destructor != NULL)
        (*destructor)(toDelete);
    free(temp);

    return OK;
}

/*
 * Clears out the treeset of all its elements via a post-order traversal. Frees up all
 * reserved memory back to the heap.
 */
static void clearTree(Node *node, void (*destructor)(void *)) {

    Node *parent;

    while (node != NULL) {
        parent = node->parent;
        /* Node is a leaf node, safe to delete */
        if (node->left == NULL && node->right == NULL) {
            /* Need to nullify parent's correct child node */
            if (parent != NULL) {
                if (parent->left == node)
                    parent->left = NULL;
                else
                    parent->right = NULL;
            }
            /* Deallocate node and stored item */
            if (*destructor != NULL)
                (*destructor)(node->data);
            free(node);
            node = parent;
        } else if (node->left != NULL) {
            /* Left child exists, traverse left subtree */
            node = node->left;
        } else {
            /* Right child exists, traverse right subtree */
            node = node->right;
        }
    }
}

void treeset_clear(TreeSet *tree, void (*destructor)(void *)) {
    clearTree(tree->root, destructor);
    tree->root = NULL;
    tree->size = 0L;
}

long treeset_size(TreeSet *tree) {
    return tree->size;
}

Boolean treeset_isEmpty(TreeSet *tree) {
    return IS_EMPTY(tree);
}

/*
 * Populates the iteration of treeset elements via an in-order traversal.
 */
static void populateArray(TreeIter *iter, BoundedStack *toVisit, Node *node) {

    while (node != NULL || boundedstack_isEmpty(toVisit) == FALSE) {

        /* Traverse down to the left-most node */
        while (node != NULL) {
            /* Pushes nodes onto visiting stack during left traversal */
            (void)boundedstack_push(toVisit, node);
            node = node->left;
        }

        /* Once leaf node reached, process and move to right subtree */
        (void)boundedstack_pop(toVisit, (void **)&node);
        iter->items[iter->next++] = node->data;
        node = node->right;
    }
}

/*
 * Allocates and creates an array representation of the treeset.
 */
static void **generateArray(TreeSet *tree) {

    BoundedStack *stack;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( tree->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Create stack for iterative approach */
    long capacity = (long)( 2 * ceil( log2(tree->size + 1) ) );  /* Max depth should be <= 2*log2(N + 1) */
    if (boundedstack_new(&stack, capacity) != OK) {
        free(items);
        return NULL;
    }

    /* Collects the array of treeset items */
    TreeIter iter = {items, 0L};
    populateArray(&iter, stack, tree->root);
    boundedstack_destroy(stack, NULL);

    return iter.items;
}

Status treeset_toArray(TreeSet *tree, Array **array) {

    /* Does not create the array if empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;

    /* Generate the array of treeset items */
    void **items = generateArray(tree);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    /* Initialize the remaining struct members */
    temp->items = items;
    temp->len = tree->size;
    *array = temp;

    return OK;
}

Status treeset_iterator(TreeSet *tree, Iterator **iter) {

    Iterator *temp = NULL;

    /* Does not create the array if empty */
    if (IS_EMPTY(tree) == TRUE)
        return STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    void **items = generateArray(tree);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Creates a new iterator with the items */
    Status status = iterator_new(&temp, items, tree->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void treeset_destroy(TreeSet *tree, void (*destructor)(void *)) {
    clearTree(tree->root, destructor);
    free(tree);
}
