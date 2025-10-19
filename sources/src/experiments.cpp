#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#else
    #include <sys/resource.h>
    #include <unistd.h>
#endif

// Incluye tus implementaciones reales
#include "../include/naive.h"
#include "../include/radixDone.h"

// ===========================================
// FUNCIONES AUXILIARES
// ===========================================

// Carga palabras desde un archivo (una por línea)
std::vector<std::string> loadDataset(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> words;
    std::string word;
    while (file >> word)
        words.push_back(word);
    return words;
}
// Mide el uso máximo de memoria (en KB)
long getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize / 1024; // Convertir bytes a KB
}


// Plantilla para medir tiempo de ejecución de una función arbitraria
template <typename Func>
double measureTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count() * 1000; // milisegundos
}

// ===========================================
// EXPERIMENTO GENERAL
// ===========================================

template <typename Structure>
void runExperiment(
    const std::string& name,
    Structure& structure,
    const std::vector<std::string>& insertWords,
    const std::vector<std::string>& searchWords
) {
    std::cout << "\n=====================================\n";
    std::cout << " Experimento: " << name << "\n";
    std::cout << "=====================================\n";

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
            if (structure.search(w)) found++;
    });

    // --- Búsqueda por prefijo (opcional) ---
    // double prefixTime = measureTime([&]() {
    //     for (const auto& w : searchWords)
    //         structure.startsWith(w.substr(0, w.size()/2)); // ejemplo
    // });

    // --- Resultados ---
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Palabras insertadas: " << insertWords.size() << "\n";
    std::cout << "Palabras buscadas:   " << searchWords.size() << "\n";
    std::cout << "Palabras encontradas: " << found << "\n";
    std::cout << "Tiempo inserción:    " << insertTime << " ms\n";
    std::cout << "Tiempo búsqueda:     " << searchTime << " ms\n";
    // std::cout << "Tiempo prefijos:     " << prefixTime << " ms\n";
    std::cout << "Memoria usada:       " << (memAfter - memBefore) << " KB\n";
}

// ===========================================
// MAIN
// ===========================================

int main() {
    // Rutas a tus datasets
    std::string insertDatasetPath = "../main/Alice_in_Wonderland.txt";       // dataset grande para construcción
    std::string searchDatasetPath = "../main/dataset_busqueda_Alice.txt"; // dataset mixto de búsqueda

    // Cargar datasets
    std::vector<std::string> insertWords = loadDataset(insertDatasetPath);
    std::vector<std::string> searchWords = loadDataset(searchDatasetPath);

    // Crear estructuras
    NaiveTrie trie;
    RadixTrie radix;

    // Ejecutar experimentos
    runExperiment("Naive Trie", trie, insertWords, searchWords);
    runExperiment("Radix Tree", radix, insertWords, searchWords);

    return 0;
}
