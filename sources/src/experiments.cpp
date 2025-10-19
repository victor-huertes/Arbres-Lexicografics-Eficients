#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cctype>

#ifdef _WIN32 // no se quien lo va a ejecutar al final
    #include <windows.h>
    #include <psapi.h>
#else
    #include <sys/resource.h>
    #include <unistd.h>
#endif

// Incluye tus implementaciones reales
#include "../include/naive.h"
#include "../include/radixDone.h"

using namespace std;

// ===========================================
// FUNCIONES AUXILIARES
// ===========================================

// Carga palabras desde un archivo (una por línea)
vector<string> loadDataset(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        exit(0);
    }
    vector<string> words;
    string word;
    while (file >> word) {
        //preprocess: delete special characters, lowercase letters
        word.erase(remove_if(word.begin(), word.end(), 
            [](char c) { return !isalnum(c); }), word.end());
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        if(!word.empty()) words.push_back(word);
    }
         
    return words;
}
// Mide el uso máximo de memoria (en KB)
long getMemoryUsageKB() {
    #ifdef _WIN32
        _PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        return pmc.WorkingSetSize / 1024; // Convertir bytes a KB
    #else
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        return usage.ru_maxrss;
    #endif
}


// Plantilla para medir tiempo de ejecución de una función arbitraria
template <typename Func>
double measureTime(Func f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    return elapsed.count() * 1000; // milisegundos
}

// ===========================================
// EXPERIMENTO GENERAL
// ===========================================

template <typename Structure>
void runExperiment(
    const string& name,
    Structure& structure,
    const vector<string>& insertWords,
    const vector<string>& searchWords
) {
    cout << "\n=====================================\n";
    cout << " Experimento: " << name << "\n";
    cout << "=====================================\n";

    // --- Inserción ---
    long memBefore = getMemoryUsageKB();
    double insertTime = measureTime([&]() {
        for (const auto& w : insertWords)
            structure.insert(w);
    });
    long memAfter = getMemoryUsageKB();

    // --- Búsqueda exacta ---
    size_t found = 0;
    double searchTime = measureTime([&]() {
        for (const auto& w : searchWords)
            if (structure.search_positions(w).size() != 0) found++;
    });

    // --- Búsqueda por prefijo (opcional) ---
    // double prefixTime = measureTime([&]() {
    //     for (const auto& w : searchWords)
    //         structure.startsWith(w.substr(0, w.size()/2)); // ejemplo
    // });

    // --- Resultados ---
    cout << fixed << setprecision(3);
    cout << "Palabras insertadas: " << insertWords.size() << "\n";
    cout << "Palabras buscadas:   " << searchWords.size() << "\n";
    cout << "Palabras encontradas: " << found << "\n";
    cout << "Tiempo inserción:    " << insertTime << " ms\n";
    cout << "Tiempo búsqueda:     " << searchTime << " ms\n";
    // cout << "Tiempo prefijos:     " << prefixTime << " ms\n";
    cout << "Memoria usada:       " << (memAfter - memBefore) << " KB\n";
}

// ===========================================
// MAIN
// ===========================================

int main() {
    // Rutas a tus datasets
    // Assuming the project structure:
    // project_root/
    //   ├── sources/
    //   │   ├── src/
    //   │   │   └── experiments.cpp
    //   │   └── main/
    //   │       ├── Alice_in_Wonderland.txt
    //   │       └── dataset_busqueda_Alice.txt
    string insertDatasetPath = "sources/main/Alice_in_Wonderland.txt";
    string searchDatasetPath = "sources/main/dataset_busqueda_Alice.txt";


    // Cargar datasets
    vector<string> insertWords = loadDataset(insertDatasetPath);
    vector<string> searchWords = loadDataset(searchDatasetPath);

    // Crear estructuras
    NaiveTrie trie;
    RadixTrie radix;

    // Ejecutar experimentos
    runExperiment("Naive Trie", trie, insertWords, searchWords);
    runExperiment("Radix Tree", radix, insertWords, searchWords);

    return 0;
}
