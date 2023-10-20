#include "r_tree.h"

void generic_tree_builder(vector<Rectangle> initial_associated_rectangles, int M, string file_name, string method_name, function<void(vector<tuple<Point, Node, Rectangle>> &, void *)> sorting_phase) {
    ofstream tree_file; /* Archivo que almacena el árbol resultante. */
    tree_file.open(file_name, ios::out | ios::binary);
    if (!tree_file.is_open()) {
        cout << "Error opening file: " << file_name;
        exit(1);
    }
    cout << method_name << " | File opened" << endl;

    long int written_nodes = 0; /* Contador de nodos escritos en archivo. */
    int first_iteration = true;
    vector<Node> nodes;
    vector<Rectangle> associated_rectangles = initial_associated_rectangles;
    long int n = size(initial_associated_rectangles); /* Cantidad de nodos que se está usando. */
    long int S = ceil(sqrt(ceil((double)n / (double)M)));

    /* Crea nodos hoja con los rectángulos iniciales. */
    for (long int i = 0; i < n; i++) {
        nodes.push_back(Node::toNode(initial_associated_rectangles[i]));
    }
    cout << method_name << " | Leaves created" << endl;

    /* Recursión: crea nodos padres hasta nodo raiz. */
    while (true) {
        /* Generar centros de c/u de los n rectangulos. */
        cout << method_name << " | Next gen with n=" << n << endl;
        vector<tuple<Point, Node, Rectangle>> center_associations;
        for (long int i = 0; i < n; i++) {
            Point center;
            center[0] = (double)((associated_rectangles[i])[0] + (associated_rectangles[i])[2]) / (double)2; /* coord. X */
            center[1] = (double)((associated_rectangles[i])[1] + (associated_rectangles[i])[3]) / (double)2; /* coord. Y */
            center_associations.push_back(make_tuple(center, nodes[i], associated_rectangles[i]));
        }
        cout << method_name << " | Centers calculated" << endl;

        /* Ordenar centros según la abscisa. */
        TreeBuildingVars srt_vars = {n, M, S};
        sorting_phase(center_associations, (void *)&srt_vars);
        cout << method_name << " | Sorted centers" << endl;

        /* Escribe los hijos de la generación actual en archivo. */
        long int children_start_index = written_nodes;
        for (long int i = 0; i < n; i++) {
            tree_file.write((char *)&get<1>(center_associations[i]), sizeof(Node));
            written_nodes += 1;
        }
        cout << method_name << " | written_nodes=" << written_nodes << endl;

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
        cout << method_name << " | Created parents" << endl;

        /* Identifica MBR de cada grupo. */
        vector<Rectangle> parents_rectangle_representations;
        for (long int i = 0; i < n_parents; i++) {
            /* Obtiene datos del i-ésimo padre de esta generación. */
            Node &ith_parent = parent_nodes[i];
            MaxChildrenArray<Rectangle> &keys = ith_parent.keys;

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
            parents_rectangle_representations.push_back(ith_MBR);
        }

        cout << method_name << " | MBRs calculated" << endl;


        /* Si se agrupó todo en un único nodo raíz, registrar raíz y devolver archivo. */
        if (n_parents == 1) {
            /* Escribe el nodo raíz en el archivo */
            cout << method_name << " | Created root !" << endl;
            tree_file.write((char *)&parent_nodes[0], sizeof(Node));
            tree_file.close();
            return;
        }

        /* Si no, repetir el procedimiento usando los MBR recién generados. */
        n = n_parents;
        nodes = parent_nodes;
        associated_rectangles = parents_rectangle_representations;
    }
}