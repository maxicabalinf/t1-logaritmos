/* Implementación de árbol R. */

#include "r_tree.h"

#include <bits/stdc++.h>

#include <tuple>
#include <vector>

Node::Node(vector<Rectangle> keys, vector<int> children, int children_qty) {
    this->keys = keys;
    this->children = children;
    this->children_qty = children_qty;
}

Node Node::toNode(Rectangle rectangle) {
    vector<Rectangle> keys(1, rectangle);
    vector<int> children(1, 0);
    return Node(keys, children, 1);
}
