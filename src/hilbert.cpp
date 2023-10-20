// Implementación del método Hilbert para construcción de árboles R.
#include <math.h>

#include <algorithm>
#include <tuple>

#include "r_tree.h"

// rotar/voltear un cuadrante apropiadamente
void rot(long int n, long int* x, long int* y, long int rx, long int ry) {
    long int t;
    if (ry == 0) {
        if (rx == 1) {
            *x = n - 1 - *x;
            *y = n - 1 - *y;
        }
        t = *x;
        *x = *y;
        *y = t;
    }
}

// convierte (x,y) a d
long int xy2d(long int n, long int x, long int y) {
    long int rx, ry, s, d = 0;
    for (s = n / 2; s > 0; s /= 2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}

/**
 * @brief Compara dos asociaciones centro-nodo-rectangulo para ordenar ascendentemente (eje Y).
 *
 * @return true
 * @return false
 */
bool sort_by_hilbert(const tuple<Point, Node, Rectangle>& a, const tuple<Point, Node, Rectangle>& b) {
    long int d_a = xy2d(524288, long(get<0>(a)[0]), long(get<0>(a)[1]));
    long int d_b = xy2d(524288, long(get<0>(b)[0]), long(get<0>(b)[1]));
    return d_a < d_b;
};

void hilbert_sorting(vector<tuple<Point, Node, Rectangle>>& associations, void* _) {
    UNUSED(_);
    sort(associations.begin(), associations.end(), sort_by_hilbert);
}

/**
 * @brief Construye un árbol R a partir de un conjunto de rectángulos usando el
 * método de curva de Hilbert.
 *
 * @param initial_associated_rectangles
 * @param initial_rectangles
 * @param M Cantidad máxima de hijos por nodo
 */
void Node::r_tree_hilbert(vector<Rectangle> initial_associated_rectangles, int M, string file_name) {
    generic_tree_builder(initial_associated_rectangles, M, file_name, "HILBERT", hilbert_sorting);
}