/* Implementación de árbol R. */

#include "r_tree.h"

#include <bits/stdc++.h>

#include <tuple>
#include <vector>

Node::Node() {}

Node::Node(MaxChildrenArray<Rectangle> keys, MaxChildrenArray<long int> children, int keys_qty, bool is_leaf) {
    this->keys = keys;
    this->children = children;
    this->keys_qty = keys_qty;
    this->is_leaf = is_leaf;
}

Node Node::toNode(Rectangle rectangle) {
    MaxChildrenArray<Rectangle> keys = {rectangle};
    MaxChildrenArray<long int> children = {0};
    return Node(keys, children, 1, true);
}

template <typename T>
Node Node::readNode(T &file, long int index) {
    Node extracted_node;
    file.seekg(index * sizeof(Node));
    file.read((char *)&extracted_node, sizeof(Node));
    return extracted_node;
}
template Node Node::readNode(fstream &file, long int index);
template Node Node::readNode(ifstream &file, long int index);

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

/**
 * @brief Compara dos asociaciones centro-nodo-rectangulo para ordenar ascendentemente (eje X).
 *
 * @return true
 * @return false
 */
bool sort_by_X(const tuple<Point, Node, Rectangle>& a, const tuple<Point, Node, Rectangle>& b) {
    return get<0>(a)[0] < get<0>(b)[0];
};

/**
 * @brief Compara dos asociaciones centro-nodo-rectangulo para ordenar ascendentemente (eje Y).
 *
 * @return true
 * @return false
 */
bool sort_by_Y(const tuple<Point, Node, Rectangle>& a, const tuple<Point, Node, Rectangle>& b) {
    return get<0>(a)[1] < get<0>(b)[1];
};
