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
    if (rec2[2] < rec1[0] || rec1[2] < rec2[2] || rec2[3] < rec1[1] || rec1[3] < rec1[1]) {
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
    long int i = 0;
    while (true) {
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
 * @param to_fill El vector de rectangulos a llenar con los rectangulos que intersectan a R.
 * @param r_tree El R-tree en donde buscar, representado como un vector de nodos.
 * @param index La llave del nodo a evaluar, se parte con el nodo raiz.
 *
 */
void Node::r_tree_rectangle_search(Rectangle rec_to_search, vector<Rectangle> to_fill, vector<Node> r_tree, long int index) {
    Node obtained_node = r_tree[index];

    // Si el nodo es una hoja, evaluamos interseccion y insertamos o no en to_fill
    if (obtained_node.is_leaf) {
        if (rectangles_intersect(obtained_node.keys[0], rec_to_search)) {
            to_fill.push_back(obtained_node.keys[0]);
        }
    }

    // Si el nodo no es hoja, evaluamos interseccion y aplicamos o no recursion con nuevo indice.
    else {
        for (long int i = 0; i < obtained_node.keys_qty; i++) {
            if (rectangles_intersect(obtained_node.keys[i], rec_to_search)) {
                long int index_wanted = obtained_node.children[i];
                r_tree_rectangle_search(rec_to_search, to_fill, r_tree, index_wanted);
            }
        }
    }
}