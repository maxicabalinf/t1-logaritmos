#include <math.h>

#include <algorithm>
#include <tuple>

#include "r_tree.h"

void srt_sorting(vector<tuple<Point, Node, Rectangle>> &associations, void *ptr) {
    TreeBuildingVars vars = *((TreeBuildingVars *)ptr);
    long int n = vars.n;
    long int M = vars.M;
    long int S = vars.S;

    /* Ordena centros según la abscisa. */
    sort(associations.begin(), associations.end(), sort_by_X);
    // cout << "STR | Sorted by X" << endl;

    /* Agrupa en n/M nodos. */
    for (long int i = 0; i < S; i++) {
        /* Caso borde de última franja vertical vacía. */
        if (i * S * M >= n) {
            break;
        }
        /* Identifica franja vertical. */
        long int start = i * S * M;
        long int end = ((i + 1) * S * M <= n) ? (i + 1) * S * M : n;
        /* Ordena franja vertical según la ordenada. */
        sort(associations.begin() + start, associations.begin() + end, sort_by_Y);
    }
    // cout << "STR | Sorted by Y" << endl;

    return;
}

void Node::r_tree_sort_tile_recursive(vector<Rectangle> initial_associated_rectangles, int M, string file_name) {
    generic_tree_builder(initial_associated_rectangles, M, file_name, "STR", srt_sorting);
}