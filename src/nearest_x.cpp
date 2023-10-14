/* Implementación del método Nearest-X para construcción de árboles R. */

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
bool sort_by_X(const tuple<Point, int, Rectangle>& a, const tuple<Point, int, Rectangle>& b) {
    return get<0>(a)[0] < get<0>(b)[0];
};

/**
 * @brief Construye un árbol R a partir de un conjunto de rectángulos usando el
 * método Nearest-X.
 *
 * @param initial_associated_rectangles
 * @param initial_rectangles
 * @param M Cantidad máxima de hijos por nodo
 */
void Node::r_tree_nearest_X(vector<Rectangle> initial_associated_rectangles, int M, string file_name) {
    ofstream tree_file; /* Archivo que almacena el árbol resultante. */
    tree_file.open(file_name, ios::out | ios::binary);

    int written_nodes = 0; /* Contador de nodos escritos en archivo. */
    int first_iteration = true;
    vector<Node> nodes;
    vector<Rectangle> associated_rectangles = initial_associated_rectangles;
    int n = size(initial_associated_rectangles); /* Cantidad de nodos que se está usando. */
    vector<Node*> leaf_references(n);

    /* Crea nodos hoja con los rectángulos iniciales. */
    for (int i = 0; i < n; i++) {
        nodes.push_back(Node::toNode(initial_associated_rectangles[i]));
    }

    /* Recursion: creacion nodos padres hasta nodo raiz. */
    while (true) { /* Generar centros de c/u de los n rectangulos. */
        vector<tuple<Point, Node, Rectangle>> center_associations;
        for (int i = 0; i < n; i++) {
            Point center;
            center[0] = ((associated_rectangles[i])[0] + (associated_rectangles[i])[1]) / 2; /* coord. X */
            center[1] = ((associated_rectangles[i])[2] + (associated_rectangles[i])[3]) / 2; /* coord. Y */
            center_associations.push_back(make_tuple(center, nodes[i], associated_rectangles[i]));
        }

        /* Ordenar centros según la abscisa. */
        sort(center_associations.begin(), center_associations.end(), sort_by_X);

        /* Escribe los hijos de la generación actual en archivo. */
        int children_start_index = written_nodes;
        for (int i = 0; i < n; i++) {
            tree_file.write((char*)&nodes[i], sizeof(Node));
            written_nodes += 1;
        }

        /* Agrupar ordenadamente de a M puntos. */
        /* Formar n/M nodos de tamaño M. El último nodo tiene x<=M nodos. */
        int n_parents = ceil(n / M); /* Cantidad de nuevos nodos. */
        vector<Node> parent_nodes(n_parents);
        for (int i = 0; i < n_parents; i++) {
            /* Extrae los hijos correspondientes al i-esimo padre de esta iteración. */
            int start = i * M;
            int end = ((i + 1) * M <= n) ? (i + 1) * M : n;
            int n_children = end - start;
            vector<tuple<Point, Node, Rectangle>> children_assoc(center_associations.begin() + start, center_associations.begin() + end);

            /* Registra índices de archivo de nodos hijos. */
            vector<int> children_file_indexes;
            vector<Rectangle> keys(n_children);
            for (int j = 0; j < n_children; j++) {
                children_file_indexes[j] = children_start_index + j;
                keys[j] = get<2>(children_assoc[j]);
            }

            /* Crea el i-esimo nodo padre de esta iteración. */
            Node ith_node(keys, children_file_indexes, n_children);
            parent_nodes[i] = ith_node;
        }

        /* Identifica MBR de cada grupo. */
        vector<Rectangle> parents_rectangle_representations;
        for (int i = 0; i < n_parents; i++) {
            /* Obtiene datos del i-ésimo padre de esta generación. */
            Node ith_parent = parent_nodes[i];
            vector<Rectangle> keys = ith_parent.keys;

            /* Calcula mínimos y máximos de cada coordenada dentro del nodo. */
            int X_min = (keys[0])[0];
            int Y_min = (keys[0])[1];
            int X_max = (keys[0])[2];
            int Y_max = (keys[0])[3];
            for (int j = 0; j < size(keys); j++) {
                X_min = min(X_min, (keys[i])[0]);
                Y_min = min(Y_min, (keys[i])[1]);
                X_max = max(X_max, (keys[i])[2]);
                Y_max = max(Y_max, (keys[i])[3]);
            }

            /* Genera el MBR correspondiente al i-esimo padre de esta generación. */
            Rectangle ith_MBR = {X_min, Y_min, X_max, Y_max};
            parents_rectangle_representations.push_back(ith_MBR);
        }

        /* Si se agrupó todo en un único nodo raíz, registrar raíz y devolver archivo. */
        if (n_parents == 1) {
            /* Escribe el nodo raíz en el archivo */
            tree_file.write((char*)&parent_nodes[0], sizeof(Node));
            tree_file.close();
        }

        /* Si no, repetir el procedimiento usando los MBR recién generados. */
        n = n_parents;
        nodes = parent_nodes;
        associated_rectangles = parents_rectangle_representations;
    }
}
