#include <bits/stdc++.h>

#include <array>
#include <vector>

using namespace std;

#define MAX_CHILDREN 2

typedef array<int, 4> Rectangle; /* Representación de un rectángulo como <xmin, ymin, xmax, ymax> */

/**
 * @brief Representación de un nodo del árbol R.
 *
 */
class Node {
   public:
    vector<Rectangle> keys;
    vector<int> children;
    int max_children;
    bool is_leaf;

    Node(vector<Rectangle> keys, vector<int> children, int max_children);

    static void r_tree_nearest_X(vector<Rectangle> initial_associated_rectangles, int M, string file_name);

    int children_qty();

    static Node toNode(Rectangle rectangle);
};