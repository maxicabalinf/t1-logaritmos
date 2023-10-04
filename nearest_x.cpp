#include <tuple>

#include "r_tree.h"

typedef array<double, 2> Point;
using namespace std;
// Implementación del método Nearest-X para construcción de árboles R.

/**
 * @brief Compara dos asociaciones centro-rectangulo para ordenar ascendentemente.
 *
 * @return true
 * @return false
 */
bool sorting_criterium(const pair<Point, Rectangle>& a, const pair<Point, Rectangle>& b) {
    return a.first[0] < b.first[0];
};

Node r_tree_nearest_X(vector<Rectangle> rectangles) {
    // Generar centros de c/u de los n rectangulos.
    vector<pair<Point, Rectangle>> center_associations;
    for (int i = 0; i < size(rectangles); i++) {
        Point center;
        center[0]= (rectangles[i][0] + rectangles[i][1]) / 2;
        center[1] = (rectangles[i][2] + rectangles[i][3]) / 2;
        center_associations.push_back(make_pair(center, rectangles[i]));
    }
    // Ordenar centros según la abscisa.
    sort(center_associations.begin(), center_associations.end(), sorting_criterium);
    // Agrupar ordenadamente de a M puntos.

    // Formar n/M nodos de tamaño M. El último nodo tiene x<=M nodos.
    // Identificar MBR de cada grupo
    // Si se agrupó todo en un único nodo raíz, terminar. Si no, repetir el procedimiento usando los MBR generados en el paso anterior.
}
