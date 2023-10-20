#pragma once
#include <bits/stdc++.h>

#include <array>
#include <vector>

using namespace std;

#define UNUSED(x) (void)(x)

#define MAX_CHILDREN 204 /* PARA CLUSTER DE 4096 BYTES. */

typedef array<double, 2> Point;

typedef array<long int, 4> Rectangle; /* Representación de un rectángulo como <xmin, ymin, xmax, ymax> */

template <typename T>
using MaxChildrenArray = array<T, MAX_CHILDREN>;

/**
 * @brief Representación de un nodo del árbol R.
 Tamaño nodo: 20 * MAX_CHILDREN + 8 bytes
 *
 */
class Node {
   public:
    MaxChildrenArray<Rectangle> keys;
    MaxChildrenArray<long int> children;
    int keys_qty = 0;
    bool is_leaf = false;

    Node(void);

    Node(MaxChildrenArray<Rectangle> keys, MaxChildrenArray<long int> children, int keys_qty, bool is_leaf = false);

    static void r_tree_nearest_X(vector<Rectangle> initial_associated_rectangles, int M, string file_name);

    static void r_tree_sort_tile_recursive(vector<Rectangle> initial_associated_rectangles, int M, string file_name);

    static void r_tree_hilbert(vector<Rectangle> initial_associated_rectangles, int M, string file_name);

    static tuple<vector<Rectangle>, int> r_tree_rectangle_search(Rectangle rec_to_search, string filename);

    static Node toNode(Rectangle rectangle);

    template <typename T>
    static Node readNode(T& file, long int index);

    string toText(void);
};

bool sort_by_X(const tuple<Point, Node, Rectangle>& a, const tuple<Point, Node, Rectangle>& b);

bool sort_by_Y(const tuple<Point, Node, Rectangle>& a, const tuple<Point, Node, Rectangle>& b);

void generic_tree_builder(vector<Rectangle> initial_associated_rectangles, int M, string file_name, string method_name, function<void(vector<tuple<Point, Node, Rectangle>>&, void*)> sorting_phase);

typedef struct {
    long int n;
    int M;
    long int S;
} TreeBuildingVars;