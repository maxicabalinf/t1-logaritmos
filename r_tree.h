#include <bits/stdc++.h>

#include <array>
#include <vector>

using namespace std;

#define MAX_CHILDS 2

/** Representación de un rectángulo. */
typedef array<double, 4> Rectangle;

/**
 * @brief Representación de un nodo del árbol R.
 *
 */
class Node {
   private:
    vector<Rectangle> keys;
    vector<Node*> children;
    int max_children;
    bool is_leaf;

   public:
    Node(vector<Rectangle> keys, vector<Node*> children, int max_children);

    int children_qty();
};