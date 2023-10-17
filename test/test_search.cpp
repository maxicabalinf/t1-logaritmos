#include <assert.h>

#include "../src/exec.h"
#include "../src/hilbert.h"
#include "../src/r_tree.h"
#include "../src/search.h"

void test_search(void){
    Rectangle to_search = {1,1,2,2};

    fstream expected_file("r_tree", ios::out | ios::binary);
    vector<Node> expected_nodes = {
        Node({{1, 1, 1, 1}}, {0}, 1, true),
        Node({{2, 2, 2, 2}}, {0}, 1, true),
        Node({{3, 3, 3, 3}}, {0}, 1, true),
        Node({{4, 4, 4, 4}}, {0}, 1, true),
        Node({{5, 5, 5, 5}}, {0}, 1, true),
        Node({(Rectangle){1, 1, 1, 1},
              (Rectangle){2, 2, 2, 2},
              (Rectangle){3, 3, 3, 3}},
             {0, 1, 2}, 3),
        Node({(Rectangle){4, 4, 4, 4},
              (Rectangle){5, 5, 5, 5}},
             {3, 4}, 2),
        Node({(Rectangle){1, 1, 3, 3},
              (Rectangle){4, 4, 5, 5}},
             {5, 6}, 2),
    };
    for (int i = 0; i < 8; i++) {
        expected_file.write((char *)&expected_nodes[i], sizeof(Node));
    }
    expected_file.close();

    vector<Node> r_tree = file_to_r_tree("r_tree");
    vector<Node> intersections;




}