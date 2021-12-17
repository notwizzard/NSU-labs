#include <stdio.h>
#include <malloc.h>

typedef struct node node;

struct node {
    char height;
    int value;
    node *left;
    node *right;
};

char max(char a, char b) {
    return a > b ? a : b;
}

char CheckHeight(node *n) {
    return n ? n->height : 0;
}

char GetHeight(node *n) {
    return max(CheckHeight(n->left), CheckHeight(n->right)) + 1;
}

char GetHeightDifference(node *root) {
    return CheckHeight(root->left) - CheckHeight(root->right);
}

node *LeftRotation(node *root) {
    node *newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;

    root->height = GetHeight(root);
    newRoot->height = GetHeight(newRoot);

    return newRoot;
}

node *RightRotation(node *root) {
    node *newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;

    root->height = GetHeight(root);
    newRoot->height = GetHeight(newRoot);

    return newRoot;
}

node *BalanceTree(node *root) {
    root->height = GetHeight(root);

    if(GetHeightDifference(root) == -2) {
        if(root->right && GetHeightDifference(root->right) > 0)
            root->right = RightRotation(root->right);
        return LeftRotation(root);
    }

    if(GetHeightDifference(root) == 2) {
        if(root->left && GetHeightDifference(root->left) < 0)
            root->left = LeftRotation(root->left);
        return RightRotation(root);
    }

    return root;
}

node *Insert(node *root, int value, node *tree, int ind) {
    if (!root) {
        tree[ind].value = value;
        tree[ind].height = 1;
        tree[ind].left = NULL;
        tree[ind].right = NULL;
        root = &tree[ind];
        return root;
    }

    if (value > root->value)
        root->right = Insert(root->right, value, tree, ind);
    else
        root->left = Insert(root->left, value, tree, ind);

    return BalanceTree(root);
}

int main() {

    int n = 0;
    if (!scanf("%d", &n))
        return 0;

    if (n <= 0) {
        printf("0");
        return 0;
    }

    node *tree = malloc(sizeof(node) * n);
    node *root = NULL;
    int v;
    for (int i = 0; i < n; i++) {
        if (!scanf("%d", &v))
            return 0;

        root = Insert(root, v, tree, i);
    }

    printf("%d", root->height);
    free(tree);
}
