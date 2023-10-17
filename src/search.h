#pragma once
#include "r_tree.h"

bool rectangles_intersect(Rectangle rec1, Rectangle rec2);

vector<Node> file_to_r_tree(string file_name);