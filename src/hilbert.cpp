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

/**
 * @brief Construye un árbol R a partir de un conjunto de rectángulos usando el
 * método de curva de Hilbert.
 *
 * @param initial_associated_rectangles
 * @param initial_rectangles
 * @param M Cantidad máxima de hijos por nodo
 */
void Node::r_tree_hilbert(vector<Rectangle> initial_associated_rectangles, int M, string file_name) {
    ofstream tree_file; /* Archivo que almacena el árbol resultante. */
    tree_file.open(file_name, ios::out | ios::binary);

    int written_nodes = 0; /* Contador de nodos escritos en archivo. */
    int first_iteration = true;
    vector<Node> nodes;
    vector<Rectangle> associated_rectangles = initial_associated_rectangles;
    long int n = size(initial_associated_rectangles); /* Cantidad de nodos que se está usando. */

    /* Crea nodos hoja con los rectángulos iniciales. */
    for (long int i = 0; i < n; i++) {
        nodes.push_back(Node::toNode(initial_associated_rectangles[i]));
    }

    /* Recursion: creacion nodos padres hasta nodo raiz. */
    while (true) { /* Generar centros de c/u de los n rectangulos. */
        vector<tuple<Point, Node, Rectangle>> center_associations;
        for (long int i = 0; i < n; i++) {
            Point center;
            center[0] = (double)((associated_rectangles[i])[0] + (associated_rectangles[i])[2]) / (double)2; /* coord. X */
            center[1] = (double)((associated_rectangles[i])[1] + (associated_rectangles[i])[3]) / (double)2; /* coord. Y */
            center_associations.push_back(make_tuple(center, nodes[i], associated_rectangles[i]));
        }

        /* Ordenar centros según hilbert. */
        sort(center_associations.begin(), center_associations.end(), sort_by_hilbert);

        /* Escribe los hijos de la generación actual en archivo. */
        int children_start_index = written_nodes;
        for (long int i = 0; i < n; i++) {
            tree_file.write((char*)&get<1>(center_associations[i]), sizeof(Node));
            // tree_file << get<1>(center_associations[i]).toText() << endl;
            written_nodes += 1;
        }

        /* Agrupar ordenadamente de a M puntos. */
        /* Formar n/M nodos de tamaño M. El último nodo tiene x<=M nodos. */
        long int n_parents = ceil((double)n / (double)M); /* Cantidad de nuevos nodos. */

        vector<Node> parent_nodes;
        for (long int i = 0; i < n_parents; i++) {
            /* Extrae los hijos correspondientes al i-esimo padre de esta iteración. */
            long int start = i * M;
            long int end = ((i + 1) * M <= n) ? (i + 1) * M : n;
            long int n_children = end - start;
            vector<tuple<Point, Node, Rectangle>> children_assoc(center_associations.begin() + start, center_associations.begin() + end);

            /* Registra índices de archivo de nodos hijos. */
            MaxChildrenArray<long int> children_file_indexes;
            MaxChildrenArray<Rectangle> keys;
            fill_n(children_file_indexes.begin(), MAX_CHILDREN, 0);
            fill_n(keys.begin(), MAX_CHILDREN, (Rectangle){0, 0, 0, 0});
            for (long int j = 0; j < n_children; j++) {
                children_file_indexes[j] = children_start_index + start + j;
                keys[j] = get<2>(children_assoc[j]);
            }

            /* Crea el i-esimo nodo padre de esta iteración. */
            Node ith_node(keys, children_file_indexes, n_children, false);
            parent_nodes.push_back(ith_node);
        }

        /* Identifica MBR de cada grupo. */
        vector<Rectangle> parents_rectangle_representations(MAX_CHILDREN, (Rectangle){0, 0, 0, 0});
        for (long int i = 0; i < n_parents; i++) {
            /* Obtiene datos del i-ésimo padre de esta generación. */
            Node ith_parent = parent_nodes[i];
            MaxChildrenArray<Rectangle> keys = ith_parent.keys;

            /* Calcula mínimos y máximos de cada coordenada dentro del nodo. */
            long int X_min = (keys[0])[0];
            long int Y_min = (keys[0])[1];
            long int X_max = (keys[0])[2];
            long int Y_max = (keys[0])[3];
            for (int j = 0; j < ith_parent.keys_qty; j++) {
                X_min = min(X_min, (keys[j])[0]);
                Y_min = min(Y_min, (keys[j])[1]);
                X_max = max(X_max, (keys[j])[2]);
                Y_max = max(Y_max, (keys[j])[3]);
            }

            /* Genera el MBR correspondiente al i-esimo padre de esta generación. */
            Rectangle ith_MBR = {X_min, Y_min, X_max, Y_max};
            parents_rectangle_representations[i] = (ith_MBR);
        }

        /* Si se agrupó todo en un único nodo raíz, registrar raíz y devolver archivo. */
        if (n_parents == 1) {
            /* Escribe el nodo raíz en el archivo */
            tree_file.write((char*)&parent_nodes[0], sizeof(Node));
            // tree_file << parent_nodes[0].toText() << endl;
            tree_file.close();
            return;
        }

        /* Si no, repetir el procedimiento usando los MBR recién generados. */
        n = n_parents;
        nodes = parent_nodes;
        associated_rectangles = parents_rectangle_representations;
    }
}