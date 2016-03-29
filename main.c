/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: francesco
 *
 * Created on 29 marzo 2016, 11.51
 */

#include <stdio.h>
#include <stdlib.h>

struct s_btree_node {
    char key;
    struct s_btree_node *left;
    struct s_btree_node *right;
    int visited;
};

typedef struct s_btree_node btree_node;

btree_node * createBTreeNode(char key, btree_node *left, btree_node *right) {
    btree_node *r;
    r = (btree_node *) malloc(sizeof (btree_node));
    r->key = key;
    r->left = left;
    r->right = right;
    return r;
}

//---------------------------------------------------

struct s_stack {
    int size;
    btree_node **els;
    int top;
};

typedef struct s_stack stack;

stack * createStack(int size) {
    stack *s = NULL;
    s = (stack *) malloc(sizeof (stack));
    if (s) {
        s->size = 0;
        s->els = (btree_node **) malloc(size * sizeof (btree_node *));
        if (s->els) {
            s->size = size;
            s->top = -1;
        } else {
            free(s);
            s = NULL;
        }
    }
    return s;
}

int stackIsEmpty(stack *s) {
    return s->top == -1;

}

int stackIsFull(stack *s) {
    return s->top == (s->size - 1);
}

int push(stack *s, btree_node *e) {
    int r = 0;
    if (!stackIsFull(s)) {
        s->top++;
        s->els[s->top] = e;
    }
    return r;
}

btree_node * pop(stack *s) {
    btree_node *r = NULL;
    if (!stackIsEmpty(s)) {
        r = s->els[s->top];
        s->top--;
    }
    return r;
}

btree_node * top(stack *s) {
    btree_node *r = NULL;
    if (!stackIsEmpty(s)) {
        r = s->els[s->top];
    }
    return r;
}

//-------------------------------------------------------------------------

struct s_queue {
    int size;
    btree_node **els;
    int first;
    int last;
    int lastAdded;
};

typedef struct s_queue queue;

queue * createQueue(int size) {
    queue *q = NULL;
    q = (queue *) malloc(sizeof (queue));
    if (q) {
        q->els = (btree_node **) malloc(size * sizeof (btree_node *));
        if (q->els) {
            q->size = size;
            q->first = 0;
            q->last = -1;
            q->lastAdded = 0;
        } else {
            free(q);
            q = NULL;
        }
    }
    return q;
}

int queueIsFull(queue *q) {
    return ((q->last + 1) % q->size == q->first) && q->lastAdded;
}

int queueIsEmpty(queue *q) {
    return ((q->last + 1) % q->size == q->first) && !(q->lastAdded);
}

int enqueue(queue *q, btree_node *e) {
    int r = 0;
    if (!queueIsFull(q)) {
        q->last = (q->last + 1) % q->size;
        q->els[q->last] = e;
        q->lastAdded = 1;
        r = 1;
    }
    return r;
}

btree_node * dequeue(queue *q) {
    btree_node *r = NULL;
    if (!queueIsEmpty(q)) {
        r = q->els[q->first];
        q->first = (q->first + 1) % q->size;
        q->lastAdded = 0;
    }
    return r;
}

btree_node * front(queue *q) {
    btree_node *r = NULL;
    if (!queueIsEmpty(q)) {
        r = q->els[q->first];
    }
    return r;
}

//---------------------------------------------------------

void DFS_in(btree_node *root) {
    if (root) {
        DFS_in(root->left);
        printf("%c ", root->key);
        DFS_in(root->right);
    }
}

void DFS_pre(btree_node *root) {
    if (root) {
        printf("%c ", root->key);
        DFS_pre(root->left);
        DFS_pre(root->right);
    }
}

void DFS_post(btree_node *root) {
    if (root) {
        DFS_post(root->left);
        DFS_post(root->right);
        printf("%c ", root->key);
    }
}

// ------------------------------------------------------

void BFS(btree_node *root) {
    queue *q = createQueue(20);
    btree_node *current = NULL;
    if (q) {
        enqueue(q, root);
        while (!queueIsEmpty(q)) {
            current = dequeue(q);
            printf("%c ", current->key);
            if (current->left)
                enqueue(q, current->left);
            if (current->right)
                enqueue(q, current->right);
        }
        free(q);
    } else {
        fprintf(stderr, "Unable to create queue\n");
    }
}

//-------------------------------------------------------

void bTreeVisitedReset(btree_node *root) {
    stack *s = createStack(20);
    btree_node *current = NULL;

    if (s) {
        push(s, root);
        while (!stackIsEmpty(s)) {
            current = pop(s);
            current->visited = 0;
            if (current->left) {
                push(s, current->left);
            }
            if (current->right) {
                push(s, current->right);
            }
        }
        free(s);
    } else {
        fprintf(stderr, "Unable to create stack\n");
    }

}

void DFS_pre_iterative(btree_node *root) {
    stack *s = createStack(20);
    btree_node *current = NULL;

    //bTreeVisitedReset(root);

    if (s) {
        push(s, root);
        while (!stackIsEmpty(s)) {
            current = pop(s);
            printf("%c ", current->key);
            if (current->right) {
                push(s, current->right);
            }
            if (current->left) {
                push(s, current->left);
            }
        }
        free(s);
    } else {
        fprintf(stderr, "Unable to create stack\n");
    }
}

void DFS_in_iterative(btree_node *root) {
    stack *s = createStack(20);
    btree_node *current = NULL;

    bTreeVisitedReset(root);

    if (s) {
        push(s, root);
        while (!stackIsEmpty(s)) {
            current = pop(s);
            if (!current->left || current->left->visited) {
                printf("%c ", current->key);
                current->visited = 1;
                if (current->right) { // it is not possible that the right child is visited
                    push(s, current->right);
                }
            } else {
                push(s, current);
                push(s, current->left);
            }
        }
        free(s);
    } else {
        fprintf(stderr, "Unable to create stack\n");
    }
}

void DFS_post_iterative(btree_node *root) {
    stack *s = createStack(20);
    btree_node *current = NULL;

    bTreeVisitedReset(root);

    if (s) {
        push(s, root);
        while (!stackIsEmpty(s)) {
            current = pop(s);
            if (!current->left || current->left->visited) {
                if (!current->right || current->right->visited) {
                    printf("%c ", current->key);
                    current->visited = 1;
                } else {
                    push(s, current);
                    if (current->right) {
                        push(s, current->right);
                    }
                }
            } else {
                push(s, current);
                if (current->left) {
                    push(s, current->left);
                }
            }
            /*
                        if (current->left) {
                            if (current->left->visited) {
                                if (current->right) {
                                    if (current->right->visited) {
                                        printf("%c ", current->key);
                                        current->visited = 1;
                                    } else {
                                        push(s, current);
                                        push(s, current->right);
                                    }
                                } else {
                                    printf("%c ", current->key);
                                    current->visited = 1;
                                }
                            } else {
                                push(s, current);
                                push(s, current->left);
                            }
                        } else {
                            if (current->right) {
                                if (current->right->visited) {
                                    printf("%c ", current->key);
                                    current->visited = 1;
                                } else {
                                    push(s, current);
                                    push(s, current->right);
                                }
                            } else {
                                printf("%c ", current->key);
                                current->visited = 1;
                            }
                        }
             */
        }
        free(s);
    } else {
        fprintf(stderr, "Unable to create stack\n");
    }
}

int main(int argc, char** argv) {
    printf("Evviva Tom LondRa!\n");

    btree_node *root = createBTreeNode('A',
            createBTreeNode('B',
            createBTreeNode('C', NULL, NULL),
            createBTreeNode('D', NULL, NULL)
            ),
            createBTreeNode('E',
            createBTreeNode('F', NULL, NULL)
            , NULL)
            );

    printf("\nDFS_pre:\t");
    DFS_pre(root);
    printf("\nDFS_pre_i:\t");
    DFS_pre_iterative(root);
    printf("\n---\nDFS_in:\t\t");
    DFS_in(root);
    printf("\nDFS_in_i:\t");
    DFS_in_iterative(root);
    printf("\n---\nDFS_post:\t");
    DFS_post(root);
    printf("\nDFS_post_i:\t");
    DFS_post_iterative(root);
    printf("\n---\nBFS:\t\t");
    BFS(root);

    return (EXIT_SUCCESS);
}

