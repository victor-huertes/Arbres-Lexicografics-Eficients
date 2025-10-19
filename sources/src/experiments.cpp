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

// Carga el contenido completo de un archivo como string
string loadTextFile(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        exit(0);
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Carga palabras desde un archivo (una por línea) - para búsquedas
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
bool runExperiment(
    const string &name,
    Structure &structure,
    const string &textToInsert,
    const vector<string> &searchWords,
    int initMode = 0)
{
    cout << "\n=====================================\n";
    cout << " Experimento: " << name << "\n";
    cout << "=====================================\n";

    // Límite de seguridad para Modo 2: máximo 50KB de texto
    const size_t MAX_TEXT_SIZE_MODE2 = 50000;
    if (initMode == 2 && textToInsert.length() > MAX_TEXT_SIZE_MODE2) {
        cout << "⚠️  OMITIDO: Texto demasiado grande para Modo 2 (" 
             << textToInsert.length() << " chars > " << MAX_TEXT_SIZE_MODE2 << " límite)\n";
        cout << "    El Modo 2 genera ~" << textToInsert.length() * 20 
             << " substrings, lo cual excedería la memoria disponible.\n";
        return false;
    }

    try {
        // --- Inicialización ---
        double insertTime = measureTime([&]()
                                        { structure.init(textToInsert, initMode); });

        // Calcular memoria del Trie
        size_t memoryBytes = structure.get_memory_usage();
        double memoryKB = memoryBytes / 1024.0;

        // --- Búsqueda exacta ---
        size_t found = 0;
        double searchTime = measureTime([&]()
                                        {
            for (const auto& w : searchWords)
                if (structure.search_positions(w).size() != 0) found++; });

        // --- Resultados ---
        cout << fixed << setprecision(3);
        cout << "Palabras buscadas:   " << searchWords.size() << "\n";
        cout << "Palabras encontradas: " << found << "\n";
        cout << "Tiempo inserción:    " << insertTime << " ms\n";
        cout << "Tiempo búsqueda:     " << searchTime << " ms\n";
        cout << "Memoria usada:       " << memoryKB << " KB\n";
        
        return true;
    }
    catch (const std::bad_alloc& e) {
        cout << "❌ ERROR: Memoria insuficiente - " << e.what() << "\n";
        cout << "    Experimento omitido.\n";
        return false;
    }
    catch (const std::exception& e) {
        cout << "❌ ERROR: " << e.what() << "\n";
        cout << "    Experimento omitido.\n";
        return false;
    }
    catch (...) {
        cout << "❌ ERROR: Excepción desconocida\n";
        cout << "    Experimento omitido.\n";
        return false;
    }
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
    vector<string> insertDatasetPath = {"input/lorem_ipsum.txt",
                                        "input/bee_movie_script.txt",
                                        "input/Alice_in_Wonderland.txt",
                                        "input/words_alpha.txt"};
    vector<string> searchDatasetPath = {"input/dataset_busqueda_loremipsum.txt",
                                        "input/dataset_busqueda_bee_movie.txt",
                                        "input/dataset_busqueda_Alice.txt",
                                        "input/dataset_busqueda_words_alpha.txt"};

    // Cargar datasets
    for (size_t i = 0; i < insertDatasetPath.size(); ++i)
    {
        // Cargar el texto completo para inserción
        string textToInsert = loadTextFile(insertDatasetPath[i]);
        
        // Cargar palabras para búsqueda
        vector<string> searchWords = loadDataset(searchDatasetPath[i]);

        cout << "DATASET: " << insertDatasetPath[i] << endl;
        
        // Modo 0: Insertar palabras con posición en el texto
        cout << "\n=== MODO 0: Palabras con posición en texto ===" << endl;
        
        // Crear estructuras
        NaiveTrie trie_mode0;
        RadixTrie radix_mode0;

        // Ejecutar experimentos con modo 0
        runExperiment("Naive Trie (Modo 0)", trie_mode0, textToInsert, searchWords, 0);
        runExperiment("Radix Tree (Modo 0)", radix_mode0, textToInsert, searchWords, 0);

        // Modo 1: Insertar palabras con número de línea
        cout << "\n=== MODO 1: Palabras con número de línea ===" << endl;
        
        // Crear estructuras
        NaiveTrie trie_mode1;
        RadixTrie radix_mode1;

        // Ejecutar experimentos con modo 1
        runExperiment("Naive Trie (Modo 1)", trie_mode1, textToInsert, searchWords, 1);
        runExperiment("Radix Tree (Modo 1)", radix_mode1, textToInsert, searchWords, 1);

        // Modo 2: Insertar todos los substrings (1 a 20 chars)
        cout << "\n=== MODO 2: Todos los substrings (1-20 chars) ===" << endl;
        
        // Crear estructuras
        NaiveTrie trie_mode2;
        RadixTrie radix_mode2;

        // Ejecutar experimentos con modo 2
        runExperiment("Naive Trie (Modo 2)", trie_mode2, textToInsert, searchWords, 2);
        runExperiment("Radix Tree (Modo 2)", radix_mode2, textToInsert, searchWords, 2);

        cout << endl;
    }
    return 0;
}
