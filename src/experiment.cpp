#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "exec.h"
#include "hilbert.h"
#include "r_tree.h"
#include "search.h"

const string EXPERIMENTS_FOLDER = "./experiments";

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
vector<Rectangle> build_set_q(void) {
    vector<Rectangle> arregloQ;
    for (long int i = 0; i < 100; i++) {
        long int xmin = randomValue(0, 500000 - 100000);
        long int ymin = randomValue(0, 500000 - 100000);
        long int xmax = xmin + randomValue(1, 100000);
        long int ymax = ymin + randomValue(1, 100000);
        arregloQ.push_back((Rectangle){xmin, ymin, xmax, ymax});
    }

    return arregloQ;
}

typedef tuple<
    double,
    double,
    pair<double, double>,
    pair<double, double>>
    stats;

map<int, double> critical_values{{95, 1.96}, {99, 2.576}};

pair<double, double> confidence_interval(int percentage, double mean, double stdev, int n = 100) {
    double sqrt_n = sqrt(n);
    double offset = critical_values[percentage] * stdev / sqrt_n;
    return make_pair(mean - offset, mean + offset);
}

stats calculate_stats(vector<double> samples) {
    double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
    double mean = sum / samples.size();

    std::vector<double> diff(samples.size());
    std::transform(samples.begin(), samples.end(), diff.begin(),
                   std::bind2nd(std::minus<double>(), mean));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / samples.size());
    pair<double, double> conf_in_99 = confidence_interval(99, mean, stdev);
    pair<double, double> conf_in_95 = confidence_interval(95, mean, stdev);

    return make_tuple(mean, stdev,
                      confidence_interval(99, mean, stdev),
                      confidence_interval(95, mean, stdev));
}

void save_results(fstream &file, int N, string method, map<string, stats> measurements) {
    file << N + " " + method + " ";
    vector<string> measure_names{"time", "access"};
    for (int i = 0; i < 2; i++) {
        file << get<0>(measurements[measure_names[i]]) << " ";
        file << get<1>(measurements[measure_names[i]]) << " ";
        file << "[";
        file << get<2>(measurements[measure_names[i]]).first << ", ";
        file << get<2>(measurements[measure_names[i]]).second << " ";
        file << "] ";
        file << "[";
        file << get<3>(measurements[measure_names[i]]).first << ", ";
        file << get<3>(measurements[measure_names[i]]).second << " ";
        file << "] ";
    }
    file << endl;
}

// Construye los arboles con los 3 constructores y guarda el tiempo que se demoro c/u
void r_tree_builder(vector<Rectangle> arregloR, string nearest_file, string hilbert_file, string str_file) {
    long int exponent = (long)log2(size(arregloR));
    cout << "exponent=" << exponent << endl;

    Node::r_tree_nearest_X(arregloR, MAX_CHILDREN, nearest_file);
    cout << "Finished building Nearest-X for n=2^" << exponent << endl;

    Node::r_tree_hilbert(arregloR, MAX_CHILDREN, hilbert_file);
    cout << "Finished building Hilbert for n=2^" << exponent << endl;

    Node::r_tree_sort_tile_recursive(arregloR, MAX_CHILDREN, str_file);
    cout << "Finished building STR for n=2^" << exponent << endl;
}

int main() {
    vector<double> tiempos_busqueda_prom;
    string build_methods[3] = {
        "nearest_x_",
        "hibert_",
        "str_",
    };
    string filenames[3] = {
        EXPERIMENTS_FOLDER + "/exp_" + build_methods[0],
        EXPERIMENTS_FOLDER + "/exp_" + build_methods[1],
        EXPERIMENTS_FOLDER + "/exp_" + build_methods[2],
    };
    fstream results(EXPERIMENTS_FOLDER + "/results", ios::out | ios::binary);
    if (!results.is_open()) {
        cout << "Error opening file: " << EXPERIMENTS_FOLDER + "/results";
        exit(1);
    }

    // Generador de casos de prueba
    for (int i = 17; i <= 25; i++) {
        cout << "n=2^" << i << endl;
        long int n = pow(2, i);
        cout << "n=" << n << endl;
        vector<Rectangle> arregloR = build_set_r(n);
        vector<Rectangle> arregloQ = build_set_q();
        cout << "Built sets !" << endl;
        r_tree_builder(arregloR,
                       filenames[0] + to_string(i),
                       filenames[1] + to_string(i),
                       filenames[2] + to_string(i));
        cout << "Built trees !" << endl;

        for (int k = 0; k < 3; k++) {
            vector<double> time_samples;
            vector<double> access_samples;
            map<string, stats> measurement_stats;
            cout << build_methods[k] << endl;
            for (int j = 0; j < 100; j++) {
                clock_t start = clock();
                tuple<vector<Rectangle>, int> search_result = Node::r_tree_rectangle_search(
                    arregloQ[i],
                    filenames[k] + to_string(i));
                clock_t end = clock();
                double time_taken = double(end - start) / CLOCKS_PER_SEC;
                time_samples.push_back(time_taken);
                access_samples.push_back((double)get<1>(search_result));
                cout << "Done " << j << endl;
            }

            /* Almacena resultado de este experimento. */
            measurement_stats["time"] = calculate_stats(time_samples);
            measurement_stats["access"] = calculate_stats(access_samples);
            save_results(results, n, build_methods[k], measurement_stats);
        }
        cout << "DONE n=2^" << i << endl;
    }
    results.close();
}