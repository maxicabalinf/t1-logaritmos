#pragma once
#include <bits/stdc++.h>

#include <array>
#include <vector>

using namespace std;

#define MAX_CHILDREN 5

typedef array<int, 4> Rectangle; /* Representación de un rectángulo como <xmin, ymin, xmax, ymax> */

template <typename T>
using MaxChildrenArray = array<T, MAX_CHILDREN>;

/**
 * @brief Representación de un nodo del árbol R.
 *
 */
class Node {
   public:
    MaxChildrenArray<Rectangle> keys;
    MaxChildrenArray<int> children;
    int keys_qty = 0;
    bool is_leaf = false;

    Node(void);

    Node(MaxChildrenArray<Rectangle> keys, MaxChildrenArray<int> children, int keys_qty, bool is_leaf = false);

    static void r_tree_nearest_X(vector<Rectangle> initial_associated_rectangles, int M, string file_name);

    static Node toNode(Rectangle rectangle);

    template <typename T>
    static Node readNode(T& file, int index);

    string toText(void);
};