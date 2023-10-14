#include <assert.h>

#include "../src/exec.h"
#include "../src/r_tree.h"

#define N 5

int main(int argc, char *argv[]) {
    /* Rectangulos iniciales. */
    vector<Rectangle> rectangles{{1, 1, 1, 1},
                                 {2, 2, 2, 2},
                                 {3, 3, 3, 3},
                                 {4, 4, 4, 4},
                                 {5, 5, 5, 5}};

    /* Resultado esperado. */
    ofstream expected_file;
    expected_file.open("test_nearest_x_expected", ios::out | ios::binary);
    vector<Node> expected_nodes = {
        Node({rectangles[0]}, {0}, 0),
        Node({rectangles[1]}, {0}, 0),
        Node({rectangles[2]}, {0}, 0),
        Node({rectangles[3]}, {0}, 0),
        Node({rectangles[4]}, {0}, 0),
        Node({{1, 1, 1, 1}, {2, 2, 2, 2}, {3, 3, 3, 3}}, {0, 1, 2}, 3),
        Node({{4, 4, 4, 4}, {5, 5, 5, 5}}, {3, 4}, 2),
        Node({{1, 1, 3, 3}, {4, 4, 5, 5}}, {5, 6}, 2),
    };
    for (int i = 0; i < 8; i++) {
        expected_file.write((char *)&expected_nodes[i], sizeof(Node));
        // cout.write((char *)&expected_nodes[i], sizeof(Node));
    }
    expected_file.close();

    /* Resultado obtenido. */
    Node::r_tree_nearest_X(rectangles, 3, "test_nearest_x_result");

    /* Evalua resultados. */
    // string test_result = exec("diff test_nearest_x_expected test_nearest_x_result");
    // cout << "nearest-x: " << test_result << endl;
}
