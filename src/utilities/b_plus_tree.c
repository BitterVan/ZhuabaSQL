#include <stdio.h>
#include <stdlib.h>
#define MAXM 10
// m is the maximal number of sons
struct _BpNode;
struct _BpTree;
typedef struct _BpNode BpNode;
typedef struct _BpTree BpTree;

struct _BpNode {
    BpNode* parent;
    BpNode* sons[MAXM];
    int pivots[MAXM];
    int m;
    int end;
    int is_leaf;
};

struct _BpTree {
    BpNode* root;
    int m;
};

BpTree* createBpTree(int);
void insert(BpTree*, int);
BpNode* split(BpNode*);
void fixInsert(BpTree*, BpNode*);
void printNode(BpNode*);
void displayTree(BpTree*);
BpNode* merge(BpNode*, BpNode*);
void delete(BpTree*, int);
void fixDelete(BpTree*, BpNode*);

int main() {
    int n;
    scanf("%d", &n);
    int temp;
    BpTree* test = createBpTree(4);
    for (int i = 0; i < n; i++) {
        scanf("%d", &temp);
        insert(test, temp);
        displayTree(test);
    }
    return 0;
}

BpTree* createBpTree(int m) {
    BpTree* ret = (BpTree*)malloc(sizeof(BpTree));
    ret->root = NULL;
    ret->m = m;
    return ret;
}

void insert(BpTree* tree, int val) {
    if (tree->root == NULL) {
        tree->root = (BpNode*)malloc(sizeof(BpNode));
        tree->root->parent = NULL;
        tree->root->pivots[0] = val;
        tree->root->is_leaf = 1;
        tree->root->end = 1;
        tree->root->m = tree->m;
        return;
    } else {
        BpNode* pos = tree->root;
        while (!(pos->is_leaf)) {
            int i;
            for (i = 0; i < pos->end; i++) {
                if (pos->pivots[i] == val) {
                    printf("Duplication!\n");
                    return;
                } else if (pos->pivots[i] > val) {
                    break;
                }
            }
            pos = pos->sons[i];
        }
        int i;
        for (i = 0; i < pos->end; i++) {
            if (pos->pivots[i] == val) {
                printf("Duplication!\n");
                return;
            } else if (pos->pivots[i] > val) {
                break;
            }
        }
        for (int j = pos->end; j > i; j--) {
            pos->pivots[j] = pos->pivots[j-1];
        }
        pos->pivots[i] = val;
        pos->end++;
        fixInsert(tree, pos);
    }
}

void fixInsert(BpTree* tree, BpNode* node) {
    if (node->end > tree->m - 1 + node->is_leaf) {
        BpNode* right = split(node);
        BpNode* left = node;
        int new_pivot = (node->is_leaf) ? (right->pivots[0]) : (left->pivots[MAXM-1]);
        if (!right->parent) {
            tree->root = (BpNode*)malloc(sizeof(BpNode));
            tree->root->end = 1;
            tree->root->is_leaf = 0;
            tree->root->m = tree->m;
            tree->root->pivots[0] = new_pivot;
            tree->root->sons[0] = left;
            tree->root->sons[1] = right;
            tree->root->parent = NULL;
            left->parent = tree->root;
            right->parent = tree->root;
            return;
        } else {
            BpNode* pre_parent = left->parent;
            int i;
            for (i = 0; i < pre_parent->end; i++) {
                if (pre_parent->pivots[i] > new_pivot)
                    break;
            }
            for (int j = pre_parent->end; j > i; j--) {
                pre_parent->pivots[j] = pre_parent->pivots[j-1];
                pre_parent->sons[j+1] = pre_parent->sons[j];
            }
            pre_parent->pivots[i] = new_pivot;
            pre_parent->sons[i+1] = right;
            pre_parent->end++;
            fixInsert(tree, pre_parent);
        }
    } else {
        return;
    }
}

BpNode* split(BpNode* node) {
    BpNode* ret = (BpNode*)malloc(sizeof(BpNode));
    ret->is_leaf = node->is_leaf;
    ret->m = node->m;
    ret->parent = node->parent;
    if (ret->is_leaf) {
        int lsize = ret->m/2+1;
        int rsize = (ret->m+1)/2;
        for (int i = 0; i < rsize; i++) {
            ret->pivots[i] = node->pivots[i+lsize];
        }
        ret->end = rsize;
        node->end = lsize;
    } else {
        int lsize = (ret->m)/2;
        int rsize = (ret->m-1)/2;
        for (int i = 0; i < rsize; i++) {
            ret->pivots[i] = node->pivots[i+lsize+1];
        }
        for (int i = 0; i < rsize+1; i++) {
            ret->sons[i] = node->sons[i+lsize+1];
            ret->sons[i]->parent = ret;
        }
        node->pivots[MAXM-1] = node->pivots[lsize];
        node->end = lsize;
        ret->end = rsize;
    }
    return ret;
}

void displayTree(BpTree* tree) {
    if (!tree->root) {
        printf("Empty Tree!\n");
        return;
    }
    BpNode* queue[10000];
    int begin = 0;
    int end = 1;
    queue[0] = tree->root;
    while (begin < end) {
        int new_end = end;
        for (int i = begin; i < end; i++) {
            printNode(queue[i]);
            if (!queue[i]->is_leaf) {
                for (int j = 0; j < queue[i]->end+1; j++) {
                    queue[new_end++] = queue[i]->sons[j];
                }
            }
        }
        printf("\n");
        begin = end;
        end = new_end;
    }
    printf("----------------\n");
}

void printNode(BpNode* node) {
    printf("[");
    for (int i = 0; i < node->end; i++) {
        printf("%d", node->pivots[i]);
        printf("%c", ",]"[i == node->end-1]);
    }
}

// BpNode* merge(BpNode* a, BpNode* b) {
//     if (a->is_leaf) {
//         for (int i = 0; i < b->end; i++) {
//             a->pivots[a->end+i] = b->pivots[i];
//         }
//         a->end = a->end + b->end;
//         free(b);
//     } else {
//         BpNode* pre_parent = a->parent;
//         int i;
//         for(i = 0; i < pre_parent->end; i++) {
//             if (pre_parent->sons[i] == a) break;
//         }
//         a->pivots[a->end] = pre_parent->pivots[i];
//         for (int j = 0; j < b->end; j++) {
//             a->pivots[a->end+1+j] = b->pivots[j];
//         }
//         for (int j = 0; j < b->end+1; j++) {
//             a->sons[a->end+1+j] = b->sons[j];
//         }
//         a->end = a->end + 1 + b->end;
//         free(b);
//     }
//     return a;
// }

// void delete(BpTree* tree, int val) {
//     BpNode* pos = tree->root;
//     if (!pos) {
//         printf("Empty Tree!\n");
//         return;
//     }
//     while (!pos->is_leaf) {
//         int i = 0;
//         while (pos->pivots[i] <= val) {
//             i++;
//         }
//         pos = pos->sons[i];
//     }
//     int i = 0;
//     for (i = 0; i < pos->end; i++) {
//         if (pos->pivots[i] == val) {
//             for (int j = i; j < pos->end-1; j++) {
//                 pos->pivots[i] = pos->pivots[i+1];
//             }
//             pos->end--;
//             if((pos->end < pos->m/2 && pos->parent) || pos->end == 0) {
//                 fixDelete(tree, pos);
//             }
//             return;
//         }
//     }
//     printf("No such element!\n");
//     return;
// }

// void fixDelete(BpTree* tree, BpNode* node) {
//     BpNode* pre_parent = node->parent;
//     if (!pre_parent) {
//         tree->root = NULL;
//         free(node);
//         return;
//     }
//     if (node == node->parent->sons[node->end]) {
//         merge(pre_parent->sons[pre_parent->end-1], pre_parent->sons[pre_parent->end]);
//         pre_parent->end--;
//         if (pre_parent->end < pre_parent->m/2) {
//             fixDelete(tree, pre_parent);
//         }
//     } else {
//         int i = 0;
//         for (int i = 0; i < pre_parent->end; i++) {
//             if (pre_parent->sons[i] == node) {
//                 break;
//             }
//         }
//         merge(node, pre_parent->sons[i+1]);
//         for (int j = i; j < pre_parent->end-1; j++) {
//             pre_parent->pivots[j] = pre_parent->pivots[j+1];
//             pre_parent->sons[j+1] = pre_parent->sons[j+2];
//         }
//         pre_parent->end--;
//         if (pre_parent->end < (pre_parent->m/2)) {
//             fixDelete(tree, pre_parent);
//         }
//     }
// }