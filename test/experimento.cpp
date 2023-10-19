#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "../src/exec.h"
#include "../src/hilbert.h"
#include "../src/r_tree.h"
#include "../src/search.h"

int randomValue(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Genera arreglo R
vector<Rectangle> build_set_r(long int R) {
    vector<Rectangle> arregloR;
    for (long int i = 0; i < R; i++) {
        long int xmin = randomValue(0, 500000 - 100);
        long int ymin = randomValue(0, 500000 - 100);
        long int xmax = xmin + randomValue(1, 100);
        long int ymax = ymin + randomValue(1, 100);
        arregloR.push_back((Rectangle){xmin, ymin, xmax, ymax});
    }

    return arregloR;
}

// Genera arreglo Q
vector<Rectangle> build_set_q(int Q) {
    vector<Rectangle> arregloQ;
    for (long int i = 0; i < Q; i++) {
        long int xmin = randomValue(0, 500000 - 100000);
        long int ymin = randomValue(0, 500000 - 100000);
        long int xmax = xmin + randomValue(1, 100000);
        long int ymax = ymin + randomValue(1, 100000);
        arregloQ.push_back((Rectangle){xmin, ymin, xmax, ymax});
    }

    return arregloQ;
}

// Construye los arboles con los 3 constructores y guarda el tiempo que se demoro c/u
vector<double> r_tree_construtor(vector<Rectangle> arregloR, int M, string nearest_file, string hilbert_file, string str_file) {
    vector<double> times;  //{tiempo_nearest_x, tiempo_hilbert, tiempo_str}

    clock_t start = clock();
    Node::r_tree_nearest_X(arregloR, M, nearest_file);
    clock_t end = clock();
    double time_taken = double(end - start) / CLOCKS_PER_SEC;
    times.push_back(time_taken);

    start = clock();
    Node::r_tree_hilbert(arregloR, M, hilbert_file);
    end = clock();
    time_taken = double(end - start) / CLOCKS_PER_SEC;
    times.push_back(time_taken);

    start = clock();
    Node::r_tree_sort_tile_recursive(arregloR, M, str_file);
    end = clock();
    time_taken = double(end - start) / CLOCKS_PER_SEC;
    times.push_back(time_taken);

    return times;
}

int main() {
    long int R;   // numero de rectangulos
    int Q = 100;  // numero de consultas
    int M;

    vector<vector<double>> tiempos_construc_totales;
    vector<double> tiempos_busqueda_prom;

    // Generador de casos de prueba
    for (int i = 10; i <= 25; i++) {
        vector<Rectangle> arregloR = build_set_r(R);
        vector<double> tiempos_i = r_tree_construtor(arregloR, M, "t_rearest_x", "t_hilbert", "t_str");
        tiempos_construc_totales.push_back(tiempos_i);
        vector<Rectangle> arregloQ = build_set_q(Q);

        double tiempo_promedio = 0;

        long int n = pow(2, i);
        for (int j = 0; j < 100; j++) {
            clock_t start = clock();
            Node::r_tree_rectangle_search(arregloQ[i], "t_nearest_x");
            clock_t end = clock();
            double time_taken = double(end - start) / CLOCKS_PER_SEC;
            tiempo_promedio += time_taken;
        }
        tiempos_busqueda_prom.push_back(tiempo_promedio / (double)100);
    }
}