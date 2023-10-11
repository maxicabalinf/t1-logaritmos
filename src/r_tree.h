#include <bits/stdc++.h>

#include <array>
#include <vector>

using namespace std;

#define MAX_CHILDS 2

/** Representación de un rectángulo. */
typedef array<double, 4> Rectangle; /* <xmin, ymin, xmax, ymax> */

/**
 * @brief Representación de un nodo del árbol R.
 *
 */
class Node {
   public:
    vector<Rectangle *> keys;
    vector<Node *> children;
    int max_children;
    bool is_leaf;

    Node(vector<Rectangle *> keys, vector<Node *> children, int max_children);

    Node *r_tree_nearest_X(vector<Rectangle *> initial_associated_rectangles, int M);

    int children_qty();

    static Node *toNode(Rectangle rectangle);
};