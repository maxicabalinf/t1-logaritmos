/* Implementación de árbol R. */

#include "r_tree.h"

#include <bits/stdc++.h>

#include <tuple>
#include <vector>

Node::Node() {}

Node::Node(MaxChildrenArray<Rectangle> keys, MaxChildrenArray<int> children, int keys_qty, bool is_leaf) {
    this->keys = keys;
    this->children = children;
    this->keys_qty = keys_qty;
    this->is_leaf = is_leaf;
}

Node Node::toNode(Rectangle rectangle) {
    MaxChildrenArray<Rectangle> keys = {rectangle};
    MaxChildrenArray<int> children = {0};
    return Node(keys, children, 1, true);
}

template <typename T>
Node Node::readNode(T &file, int index) {
    Node extracted_node;
    file.seekg(index * sizeof(Node));
    file.read((char *)&extracted_node, sizeof(Node));
    return extracted_node;
}
template Node Node::readNode(fstream &file, int index);
template Node Node::readNode(ifstream &file, int index);

string Node::toText(void) {
    string node_text;
    node_text += "keys=[";
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node_text += "{";
        for (int j = 0; j < 4; j++) {
            node_text += to_string(this->keys[i][j]) + ", ";
        }
        node_text += "},";
    }
    node_text += "];children=[";
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node_text += to_string((this->children)[i]) + ", ";
    }
    node_text += "];";
    node_text += "keys_qty=" + to_string(this->keys_qty) + ";";
    node_text += "is_leaf=" + to_string(this->is_leaf) + ";";

    return node_text;
}