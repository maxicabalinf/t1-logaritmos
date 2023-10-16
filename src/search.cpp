#include <math.h>

#include <algorithm>
#include <tuple>

#include "r_tree.h"

//Busqueda dentro de un R-tree: buscar todos los nodos hojas que intersecten al rectangulo

//Funcion retorna si dos rectangulos se intersectan
bool rectangles_intersect(Rectangle rec1, Rectangle rec2){
    if (rec2[2] < rec1[0] || rec1[2] < rec2[2]){
        return false;
    }

    if (rec2[3] < rec1[1] || rec1[3] < rec1[1]){
        return false;
    }

    return true;
}

vector<Rectangle> the_answer;

void r_tree_rectangle_search(Node root_node, Rectangle rec_to_search, vector<Rectangle> to_fill){
    if (root_node.is_leaf){
        if (rectangles_intersect(root_node.keys[0], rec_to_search)){
            to_fill.push_back(root_node.keys[0]);
        }
    }

    else{
        for (long int i=0; i<root_node.keys_qty; i++){
            if (rectangles_intersect(root_node.keys[i], rec_to_search)){
                //r_tree_rectangle_search() avanzar al nodo hijo
            }
        }
    }
}