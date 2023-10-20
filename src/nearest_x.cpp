/* Implementación del método Nearest-X para construcción de árboles R. */

#include <math.h>

#include <algorithm>
#include <tuple>

#include "r_tree.h"

using namespace std;

void nearest_x_sorting(vector<tuple<Point, Node, Rectangle>> &associations, void *_) {
    UNUSED(_);
    sort(associations.begin(), associations.end(), sort_by_X);
}

/**
 * @brief Construye un árbol R a partir de un conjunto de rectángulos usando el
 * método Nearest-X.
 *
 * @param initial_associated_rectangles
 * @param initial_rectangles
 * @param M Cantidad máxima de hijos por nodo
 */
void Node::r_tree_nearest_X(vector<Rectangle> initial_associated_rectangles, int M, string file_name) {
    generic_tree_builder(initial_associated_rectangles, M, file_name, "NEAREST-X", nearest_x_sorting);
}