#include <assert.h>

#include "../src/exec.h"
#include "../src/r_tree.h"

#define N 5

void test_readNode(void) {
    /* Rectangulos iniciales. */
    vector<Rectangle> rectangles{{{1, 1, 1, 1},
                                  {2, 2, 2, 2},
                                  {3, 3, 3, 3},
                                  {4, 4, 4, 4},
                                  {5, 5, 5, 5}}};

    /* Crea archivo con nodos. */
    fstream node_source("test_extraction_source", ios::out | ios::binary);
    MaxChildrenArray<Rectangle> top_floor{{{1, 1, 3, 3}, {4, 4, 5, 5}}};
    vector<Node> expected_nodes = {
        Node({rectangles[0]}, {0}, 0, true),
        Node({rectangles[1]}, {0}, 0, true),
        Node({rectangles[2]}, {0}, 0, true),
        Node({rectangles[3]}, {0}, 0, true),
        Node({rectangles[4]}, {0}, 0, true),
        Node({rectangles[0], rectangles[1], rectangles[2]}, {0, 1, 2}, 3, false),
        Node({rectangles[3], rectangles[4]}, {3, 4}, 2, false),
        Node({top_floor[0], top_floor[1]}, {5, 6}, 2, false),
    };
    for (int i = 0; i < 8; i++) {
        node_source.write((char *)&expected_nodes[i], sizeof(Node));
    }
    node_source.close();

    /* Extracción de nodos. */
    node_source.open("test_extraction_source", ios::in | ios::binary);
    for (int i = 0; i < 8; i++) {
        Node extracted_node = Node::readNode(node_source, i);
        assert(extracted_node.toText() == expected_nodes[i].toText());
    }
    node_source.close();
}

void test_nearestX(void) {
    /* Rectangulos iniciales. */
    vector<Rectangle> rectangles{{
        {5, 5, 5, 5},
        {1, 1, 1, 1},
        {3, 3, 3, 3},
        {2, 2, 2, 2},
        {4, 4, 4, 4},
    }};

    /* Construcción obtenida. */
    Node::r_tree_nearest_X(rectangles, 3, "test_nearest_x_result");

    /* Construcción esperada. */
    fstream expected_file("test_nearest_x_expected", ios::out | ios::binary);
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

    /* Evalua resultados. */
    fstream obtained_file("test_nearest_x_result", ios::in | ios::binary);
    expected_file.open("test_nearest_x_expected", ios::in | ios::binary);
    for (int i = 0; i < 8; i++) {
        Node obtained_node = Node::readNode(obtained_file, i);
        Node expected_node = Node::readNode(expected_file, i);
        assert(obtained_node.toText() == expected_node.toText());
    }
}

int main(int argc, char *argv[]) {
    test_readNode();
    test_nearestX();
    return 0;
}

void test_STR(void) {
    /* Rectangulos iniciales. */
    vector<Rectangle> rectangles{{
        {5, 5, 5, 5},
        {1, 1, 1, 1},
        {3, 3, 3, 3},
        {2, 2, 2, 2},
        {4, 4, 4, 4},
        {6, 6, 6, 6},
        {8, 8, 8, 8},
        {9, 9, 9, 9},
        {7, 7, 7, 7},
        {10, 10, 10, 10}
    }};

    /* Construcción obtenida. */
    Node::r_tree_sort_tile_recursive(rectangles, 3, "test_sort_tile_recursive_result");

    /* Construcción esperada. */
    fstream expected_file("test_sort_tile_recursive_expected", ios::out | ios::binary);
    vector<Node> expected_nodes = {
        Node({{1, 1, 1, 1}}, {0}, 1, true),
        Node({{2, 2, 2, 2}}, {0}, 1, true),
        Node({{3, 3, 3, 3}}, {0}, 1, true),
        Node({{4, 4, 4, 4}}, {0}, 1, true),
        Node({{5, 5, 5, 5}}, {0}, 1, true),
        Node({{6, 6, 6, 6}}, {0}, 1, true),
        Node({{7, 7, 7, 7}}, {0}, 1, true),
        Node({{8, 8, 8, 8}}, {0}, 1, true),
        Node({{9, 9, 9, 9}}, {0}, 1, true),
        Node({{10, 10, 10, 10}}, {0}, 1, true),
        Node({(Rectangle){1, 1, 1, 1},
              (Rectangle){2, 2, 2, 2},
              (Rectangle){3, 3, 3, 3}},
             {0, 1, 2}, 3),
        Node({(Rectangle){4, 4, 4, 4},
              (Rectangle){5, 5, 5, 5},
              (Rectangle){6, 6, 6, 6}},
             {3, 4, 5}, 3),
        Node({(Rectangle){7, 7, 7, 7},
              (Rectangle){8, 8, 8, 8},
              (Rectangle){9, 9, 9, 9}},
             {6, 7, 8}, 3),
        Node({(Rectangle){10, 10, 10, 10}},
             {9}, 1),
        Node({(Rectangle){1, 1, 3, 3},
              (Rectangle){4, 4, 6, 6}},
             {10, 11}, 2),
        Node({(Rectangle){7, 7, 9, 9},
              (Rectangle){10, 10, 10, 10}},
             {12, 13}, 2),
        Node({(Rectangle){1, 1, 6, 6},
              (Rectangle){7, 7, 10, 10}},
             {14, 15}, 2)
    };
    for (int i = 0; i < 8; i++) {
        expected_file.write((char *)&expected_nodes[i], sizeof(Node));
    }
    expected_file.close();

    /* Evalua resultados. */
    fstream obtained_file("test_sort_tile_recursive_result", ios::in | ios::binary);
    expected_file.open("test_sort_tile_recursive_expected", ios::in | ios::binary);
    for (int i = 0; i < 8; i++) {
        Node obtained_node = Node::readNode(obtained_file, i);
        Node expected_node = Node::readNode(expected_file, i);
        assert(obtained_node.toText() == expected_node.toText());
    }
}

int main(int argc, char *argv[]) {
    test_readNode();
    test_nearestX();
    return 0;
}
