#include <math.h>

#include <algorithm>
#include <tuple>

#include "r_tree.h"

// Busqueda dentro de un R-tree: buscar todos los nodos hojas que intersecten al rectangulo

/**
 * @brief Evalua si dos rectangulos de intersectan
 *
 * @param rec1 Un rectangulo.
 * @param rec2 El otro rectangulo.
 * @return Valor booleano, true si se intersectan y false si no.
 *
 */
bool rectangles_intersect(Rectangle rec1, Rectangle rec2) {
    if (rec1[0] > rec2[2] || rec1[2] < rec2[0] || rec1[1] > rec2[3] || rec1[3] < rec1[1]) {
        return false;
    }

    return true;
}

/**
 * @brief Escribe los nodos de un archivo binario en un vector de nodos.
 *
 * @param file_name El nombre del archivo binario.
 * @return El vector de nodos.
 *
 */
vector<Node> file_to_r_tree(string file_name) {
    fstream nodes_source(file_name, ios::in | ios::binary);
    vector<Node> r_tree;
    long int max_nodes = ceil(sizeof(nodes_source) / sizeof(Node));
    long int i = 0;
    while (i < max_nodes) {
        Node obtained_node = Node::readNode(nodes_source, i);
        r_tree.push_back(obtained_node);
        i++;
    }
    return r_tree;
}

/**
 * @brief Busca en un R-tree todos los rectangulos que intersecten a un rectangulo R.
 *
 * @param rec_to_serach El rectangulo R a intersectar.
 *
 */
tuple<vector<Rectangle>, int> Node::r_tree_rectangle_search(Rectangle rec_to_search, string filename) {
    // TODO clear cache
    queue<int> remaining_nodes_indexes;
    fstream tree_file(filename, ios::in | ios::binary);
    tree_file.seekg(0, ios::end);
    int file_size = tree_file.tellg();
    int n_nodes = file_size / sizeof(Node);
    remaining_nodes_indexes.push(n_nodes - 1);
    int block_accesses = 0;
    vector<Rectangle> results;
    while (!remaining_nodes_indexes.empty()) {
        Node actual_branch = Node::readNode(tree_file, remaining_nodes_indexes.front());
        remaining_nodes_indexes.pop();
        block_accesses += 1;
        int n_keys = actual_branch.keys_qty;
        if (actual_branch.is_leaf) {
            if (rectangles_intersect(rec_to_search, actual_branch.keys[0])) {
                results.push_back(actual_branch.keys[0]);
            }
        } else {
            for (int i = 0; i < n_keys; i++) {
                if (rectangles_intersect(rec_to_search, actual_branch.keys[i])) {
                    remaining_nodes_indexes.push(actual_branch.children[i]);
                }
            }
        }
    }
    tree_file.close();

    return make_tuple(results, block_accesses);
}