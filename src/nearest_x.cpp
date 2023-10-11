// Implementación del método Nearest-X para construcción de árboles R.

#include <math.h>

#include <tuple>

#include "r_tree.h"

typedef array<double, 2> Point;
using namespace std;

/**
 * @brief Compara dos asociaciones centro-nodo-rectangulo para ordenar ascendentemente.
 *
 * @return true
 * @return false
 */
bool sort_by_X(const tuple<Point, Node*, Rectangle*>& a, const tuple<Point, Node*, Rectangle*>& b) {
    return get<0>(a)[0] < get<0>(b)[0];
};

/**
 * @brief Construye un árbol R a partir de un conjunto de rectángulos usando el
 * método Nearest-X.
 *
 * @param initial_associated_rectangles
 * @param initial_rectangles
 * @param M Cantidad máxima de hijos por nodo
 * @return Puntero a la raiz del árbol
 */
Node* Node::r_tree_nearest_X(vector<Rectangle*> initial_associated_rectangles, int M) {
    vector<Node*> nodes;
    vector<Rectangle*> associated_rectangles = initial_associated_rectangles;
    int n = size(initial_associated_rectangles); /* Cantidad de nodos que se está usando. */
    for (int i = 0; i < n; i++) {
        nodes.push_back(Node::toNode(*initial_associated_rectangles[i]));
    }

    while (true) { /* Generar centros de c/u de los n rectangulos. */
        vector<tuple<Point, Node*, Rectangle*>> center_associations;
        for (int i = 0; i < n; i++) {
            Point center;
            center[0] = ((*associated_rectangles[i])[0] + (*associated_rectangles[i])[1]) / 2; /* coord. X */
            center[1] = ((*associated_rectangles[i])[2] + (*associated_rectangles[i])[3]) / 2; /* coord. Y */
            center_associations.push_back(make_tuple(center, nodes[i], associated_rectangles[i]));
        }

        /* Ordenar centros según la abscisa. */
        sort(center_associations.begin(), center_associations.end(), sort_by_X);

        /* Agrupar ordenadamente de a M puntos. */
        int n_parents = ceil(n / M); /* Cantidad de nuevos nodos. */
        /* Formar n/M nodos de tamaño M. El último nodo tiene x<=M nodos. */
        vector<Node*> parent_nodes(n_parents);
        for (int i = 0; i < n_parents; i++) {
            int start = i * M;
            int end = ((i + 1) * M <= n) ? (i + 1) * M : n;
            int n_children = end - start;
            vector<tuple<Point, Node*, Rectangle*>> children_assoc(center_associations.begin() + start, center_associations.begin() + end);
            vector<Node*> children(n_children);
            vector<Rectangle*> keys(n_children);
            for (int j = 0; j < n_children; j++) {
                children[j] = get<1>(children_assoc[j]);
                keys[j] = get<2>(children_assoc[j]);
            }
            Node ith_node(keys, children, M);
            parent_nodes[i] = &ith_node;
        }

        /* Identificar MBR de cada grupo. */
        /* Obtener mínimos y máximos de cada coordenada dentro del nodo. */
        vector<Rectangle*> parent_rec_assoc;
        for (int i = 0; i < n_parents; i++) {
            Node ith_parent = *parent_nodes[i];
            vector<Rectangle*>& keys = ith_parent.keys;
            double X_min = (*keys[0])[0];
            double Y_min = (*keys[0])[1];
            double X_max = (*keys[0])[2];
            double Y_max = (*keys[0])[3];
            for (int j = 0; j < size(keys); j++) {
                X_min = min(X_min, (*keys[i])[0]);
                Y_min = min(Y_min, (*keys[i])[1]);
                X_max = max(X_max, (*keys[i])[2]);
                Y_max = max(Y_max, (*keys[i])[3]);
            }
            Rectangle ith_MBR = {X_min, Y_min, X_max, Y_max};  // TODO preservar referencia
            parent_rec_assoc.push_back(&ith_MBR);
        }
        /* Si se agrupó todo en un único nodo raíz, terminar. Si no, repetir el procedimiento usando los MBR generados en el paso anterior. */
        if (n_parents <= 1) {
            return parent_nodes[0];
        }
        n = n_parents;
        nodes = parent_nodes;
        associated_rectangles = parent_rec_assoc;
    }
}
