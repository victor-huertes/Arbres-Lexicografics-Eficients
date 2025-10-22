#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <numeric>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "../include/naive.h"
#include "../include/radix.h"

using namespace std;

struct Statistics {
    double mean, median, std_dev, min, max, q1, q3;
};

Statistics calculateStatistics(vector<double> values) {
    Statistics stats = {0, 0, 0, 0, 0, 0, 0};
    if (values.empty()) return stats;
    
    sort(values.begin(), values.end());
    size_t n = values.size();
    
    stats.min = values.front();
    stats.max = values.back();
    stats.mean = accumulate(values.begin(), values.end(), 0.0) / n;
    stats.median = (n % 2 == 0) ? (values[n/2 - 1] + values[n/2]) / 2.0 : values[n/2];
    stats.q1 = values[n / 4];
    stats.q3 = values[3 * n / 4];
    
    double variance = 0.0;
    for (double val : values) variance += (val - stats.mean) * (val - stats.mean);
    stats.std_dev = sqrt(variance / n);
    
    return stats;
}

string loadTextFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open " << filename << endl;
        exit(1);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

vector<string> loadDataset(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open " << filename << endl;
        exit(1);
    }
    
    vector<string> words;
    string word;
    while (file >> word) {
        word.erase(remove_if(word.begin(), word.end(), [](char c) { return !isalnum(c); }), word.end());
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (!word.empty()) words.push_back(word);
    }
    return words;
}

template <typename Func>
double measureTime(Func f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double>(end - start).count() * 1000.0;
}

struct Result {
    string dataset, structure;
    int mode;
    size_t nodos, profundidad, palabras_buscadas, palabras_encontradas, nodos_visitados;
    double profundidad_mediana, tiempo_insercion, tiempo_busqueda, memoria_kb;
    bool success;
};

struct ResultStats {
    string dataset, structure;
    int mode, num_runs;
    Statistics tiempo_insercion, tiempo_busqueda, memoria, nodos_visitados;
    bool success;
};

template <typename Structure>
Result runExperiment(const string &structure, const string &dataset, const string &text,
                     const vector<string> &words, int mode) {
    Result r = {dataset, structure, mode, 0, 0, words.size(), 0, 0, 0.0, 0.0, 0.0, 0.0, false};
    
    try {
        Structure s;
        r.tiempo_insercion = measureTime([&]() { s.init(text, mode); });
        
        r.nodos = s.get_total_nodes();
        auto depth = s.calculate_depth_metrics();
        r.profundidad = depth.first;
        r.profundidad_mediana = depth.second;
        r.memoria_kb = s.get_memory_usage() / 1024.0;
        
        size_t found = 0, visited = 0;
        r.tiempo_busqueda = measureTime([&]() {
            for (const auto& w : words) {
                if (!s.search_positions(w).empty()) {
                    found++;
                    visited += s.get_last_nodes_visited();
                }
            }
        });
        
        r.palabras_encontradas = found;
        r.nodos_visitados = visited;
        r.success = true;
    } catch (...) {}
    
    return r;
}

template <typename Structure>
ResultStats runMultiple(const string &structure, const string &dataset, const string &text,
                        const vector<string> &words, int mode, int runs) {
    ResultStats rs = {dataset, structure, mode, 0, {}, {}, {}, {}, false};
    vector<double> tiempo_ins, tiempo_bus, mem, visited;
    
    for (int i = 0; i < runs; ++i) {
        Result r = runExperiment<Structure>(structure, dataset, text, words, mode);
        if (r.success) {
            tiempo_ins.push_back(r.tiempo_insercion);
            tiempo_bus.push_back(r.tiempo_busqueda);
            mem.push_back(r.memoria_kb);
            visited.push_back(r.nodos_visitados);
        }
        #ifndef _WIN32
        usleep(100000);
        #endif
    }
    
    if (!tiempo_ins.empty()) {
        rs.num_runs = tiempo_ins.size();
        rs.tiempo_insercion = calculateStatistics(tiempo_ins);
        rs.tiempo_busqueda = calculateStatistics(tiempo_bus);
        rs.memoria = calculateStatistics(mem);
        rs.nodos_visitados = calculateStatistics(visited);
        rs.success = true;
    }
    
    return rs;
}

void saveCSV(const vector<Result>& results, const string& file) {
    ofstream out(file);
    out << "Dataset,Mode,Structure,Nodes,Depth,DepthMedian,WordsSearched,WordsFound,"
        << "InsertTime,SearchTime,MemoryKB,NodesVisited,Success\n" << fixed << setprecision(3);
    
    for (const auto& r : results) {
        out << r.dataset << "," << r.mode << "," << r.structure << "," << r.nodos << ","
            << r.profundidad << "," << r.profundidad_mediana << "," << r.palabras_buscadas << ","
            << r.palabras_encontradas << "," << r.tiempo_insercion << "," << r.tiempo_busqueda << ","
            << r.memoria_kb << "," << r.nodos_visitados << "," << (r.success ? "1" : "0") << "\n";
    }
}

void saveStats(const vector<ResultStats>& results, const string& file) {
    ofstream out(file);
    out << "Dataset,Mode,Structure,Metric,Mean,Median,StdDev,Min,Max,Q1,Q3,Runs\n" 
        << fixed << setprecision(3);
    
    auto write = [&](const ResultStats& r, const string& metric, const Statistics& s) {
        out << r.dataset << "," << r.mode << "," << r.structure << "," << metric << ","
            << s.mean << "," << s.median << "," << s.std_dev << "," << s.min << ","
            << s.max << "," << s.q1 << "," << s.q3 << "," << r.num_runs << "\n";
    };
    
    for (const auto& r : results) {
        if (!r.success) continue;
        write(r, "InsertTime", r.tiempo_insercion);
        write(r, "SearchTime", r.tiempo_busqueda);
        write(r, "Memory", r.memoria);
        write(r, "NodesVisited", r.nodos_visitados);
    }
}

string extractName(const string& path) {
    size_t slash = path.find_last_of("/\\");
    size_t dot = path.find_last_of(".");
    string name = (slash != string::npos) ? path.substr(slash + 1) : path;
    return (dot != string::npos) ? name.substr(0, dot - (slash != string::npos ? slash + 1 : 0)) : name;
}

int main(int argc, char* argv[]) {
    bool csv = false, stats = false;
    int runs = 5;
    
    if (argc > 1) {
        string arg(argv[1]);
        if (arg == "--csv") csv = true;
        else if (arg == "--stats") {
            stats = true;
            if (argc > 2) runs = max(1, atoi(argv[2]));
        }
    }

    vector<string> insertPaths = {
        "data/lorem_ipsum.txt", "data/bee_movie.txt", "data/alice_wonderland.txt",
        "data/moby_dick.txt", "data/words_alpha.txt", "data/dna_genome.txt", "data/wikipedia_titles.txt"
    };
    vector<string> searchPaths = {
        "data/lorem_ipsum_search.txt", "data/bee_movie_search.txt", "data/alice_wonderland_search.txt",
        "data/moby_dick_search.txt", "data/words_alpha_search.txt", "data/dna_genome_search.txt",
        "data/wikipedia_titles_search.txt"
    };

    if (csv) {
        vector<Result> results;
        for (size_t i = 0; i < insertPaths.size(); ++i) {
            string text = loadTextFile(insertPaths[i]);
            vector<string> words = loadDataset(searchPaths[i]);
            string name = extractName(insertPaths[i]);
            int mode = (name == "wikipedia_titles") ? 1 : 0;
            
            results.push_back(runExperiment<NaiveTrie>("NaiveTrie", name, text, words, mode));
            results.push_back(runExperiment<RadixTrie>("RadixTrie", name, text, words, mode));
        }
        saveCSV(results, "output/results.csv");
    }
    else if (stats) {
        vector<ResultStats> results;
        for (size_t i = 0; i < insertPaths.size(); ++i) {
            string text = loadTextFile(insertPaths[i]);
            vector<string> words = loadDataset(searchPaths[i]);
            string name = extractName(insertPaths[i]);
            int mode = (name == "wikipedia_titles") ? 1 : 0;
            
            results.push_back(runMultiple<NaiveTrie>("NaiveTrie", name, text, words, mode, runs));
            results.push_back(runMultiple<RadixTrie>("RadixTrie", name, text, words, mode, runs));
        }
        saveStats(results, "output/results_stats.csv");
    }
    else {
        for (size_t i = 0; i < insertPaths.size(); ++i) {
            string text = loadTextFile(insertPaths[i]);
            vector<string> words = loadDataset(searchPaths[i]);
            string name = extractName(insertPaths[i]);
            int mode = (name == "wikipedia_titles") ? 1 : 0;
            
            cout << "\n=== " << name << " ===\n";
            Result r1 = runExperiment<NaiveTrie>("NaiveTrie", name, text, words, mode);
            Result r2 = runExperiment<RadixTrie>("RadixTrie", name, text, words, mode);
            
            cout << "NaiveTrie: Insert=" << r1.tiempo_insercion << "ms Search=" << r1.tiempo_busqueda 
                 << "ms Mem=" << r1.memoria_kb << "KB\n";
            cout << "RadixTrie: Insert=" << r2.tiempo_insercion << "ms Search=" << r2.tiempo_busqueda 
                 << "ms Mem=" << r2.memoria_kb << "KB\n";
        }
    }
    
    return 0;
}
