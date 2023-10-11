// Implementación de árbol R.

#include "r_tree.h"

#include <bits/stdc++.h>

#include <tuple>
#include <vector>

Node::Node(vector<Rectangle *> keys, vector<Node *> children, int max_children) {
    this->keys = keys;
    this->children = children;
    this->max_children = max_children;
}
int Node::children_qty() {
    return size(children);
}
Node *Node::toNode(Rectangle rectangle) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->keys.push_back(&rectangle);
    return node;
}
