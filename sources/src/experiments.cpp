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
#include "../include/radix.h"

using namespace std;

// ===========================================
// FUNCIONES AUXILIARES
// ===========================================

// Carga palabras desde un archivo (una por línea)
vector<string> loadDataset(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        exit(0);
    }
    vector<string> words;
    string word;
    while (file >> word)
    {
        // preprocess: delete special characters, lowercase letters
        word.erase(remove_if(word.begin(), word.end(),
                             [](char c)
                             { return !isalnum(c); }),
                   word.end());
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (!word.empty())
            words.push_back(word);
    }

    return words;
}
// Mide el uso máximo de memoria (en KB)
long getMemoryUsageKB()
{
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
double measureTime(Func f)
{
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
    const string &name,
    Structure &structure,
    const vector<string> &insertWords,
    const vector<string> &searchWords)
{
    cout << "\n=====================================\n";
    cout << " Experimento: " << name << "\n";
    cout << "=====================================\n";

    // --- Inserción ---
    double insertTime = measureTime([&]()
                                    {
        for (const auto& w : insertWords)
            structure.insert(w); });

    // Calcular memoria del Trie
    size_t memoryBytes = structure.get_memory_usage();
    double memoryKB = memoryBytes / 1024.0;

    // --- Búsqueda exacta ---
    size_t found = 0;
    double searchTime = measureTime([&]()
                                    {
        for (const auto& w : searchWords)
            if (structure.search_positions(w).size() != 0) found++; });

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
    cout << "Memoria usada:       " << memoryKB << " KB (" << memoryBytes << " bytes)\n";
    // cout << "Tiempo prefijos:     " << prefixTime << " ms\n";
}

// ===========================================
// MAIN
// ===========================================

int main()
{
    // Rutas a tus datasets
    // Assuming the project structure:
    // project_root/
    //   ├── sources/
    //   │   ├── src/
    //   │   │   └── experiments.cpp
    //   │   └── main/
    //   │       ├── Alice_in_Wonderland.txt
    //   │       └── dataset_busqueda_Alice.txt
    vector  <string> insertDatasetPath = {"input/lorem_ipsum.txt",
        "input/bee_movie_script.txt",
        "input/Alice_in_Wonderland.txt",
        "input/words_alpha.txt"};
    vector  <string> searchDatasetPath = {"input/dataset_busqueda_loremipsum.txt",
        "input/dataset_busqueda_bee_movie.txt",
        "input/dataset_busqueda_Alice.txt",
        "input/dataset_busqueda_words_alpha.txt"};

    // Cargar datasets
    for(size_t i = 0; i < insertDatasetPath.size(); ++i){
    
        vector<string> insertWords = loadDataset(insertDatasetPath[i]);
        vector<string> searchWords = loadDataset(searchDatasetPath[i]);

        // Crear estructuras
        NaiveTrie trie;
        RadixTrie radix;

        cout << "DATASET: " << insertDatasetPath[i] << endl;
        // Ejecutar experimentos
        runExperiment("Naive Trie", trie, insertWords, searchWords);
        runExperiment("Radix Tree", radix, insertWords, searchWords);

        cout << endl;
    }
    return 0;
}
