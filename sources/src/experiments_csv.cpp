#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#include <unistd.h>
#endif

#include "../include/naive.h"
#include "../include/radix.h"

using namespace std;

// ===========================================
// FUNCIONES AUXILIARES
// ===========================================

string loadTextFile(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        exit(1);
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

vector<string> loadDataset(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        exit(1);
    }
    vector<string> words;
    string word;
    while (file >> word)
    {
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

long getMemoryUsageKB()
{
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize / 1024;
#else
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
#endif
}

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
// EXPERIMENTO PARA CSV
// ===========================================

struct ExperimentResult {
    string dataset;
    int mode;
    string structure;
    size_t palabras_buscadas;
    size_t palabras_encontradas;
    double tiempo_insercion_ms;
    double tiempo_busqueda_ms;
    double memoria_kb;
    double percentage_mem;
    bool success;
};

template <typename Structure>
ExperimentResult runExperimentCSV(
    const string &structureName,
    const string &datasetName,
    Structure &structure,
    const string &textToInsert,
    const vector<string> &searchWords,
    int initMode = 0)
{
    ExperimentResult result;
    result.dataset = datasetName;
    result.mode = initMode;
    result.structure = structureName;
    result.palabras_buscadas = searchWords.size();
    result.success = false;

    try {
        // Inicialización
        result.tiempo_insercion_ms = measureTime([&]()
                                        { structure.init(textToInsert, initMode); });

        // Memoria
        size_t memoryString = textToInsert.size() * 8; //medida total del dataset
        size_t memoryBytes = structure.get_memory_usage();
        result.memoria_kb = memoryBytes / 1024.0;
        result.percentage_mem = memoryBytes/memoryString * 100;

        // Búsqueda
        size_t found = 0;
        result.tiempo_busqueda_ms = measureTime([&]()
                                        {
            for (const auto& w : searchWords)
                if (structure.search_positions(w).size() != 0) found++; });

        result.palabras_encontradas = found;
        result.success = true;
    }
    catch (const std::bad_alloc& e) {
        result.palabras_encontradas = 0;
        result.tiempo_insercion_ms = 0;
        result.tiempo_busqueda_ms = 0;
        result.memoria_kb = 0;
    }
    catch (...) {
        result.palabras_encontradas = 0;
        result.tiempo_insercion_ms = 0;
        result.tiempo_busqueda_ms = 0;
        result.memoria_kb = 0;
    }

    return result;
}

// ===========================================
// MAIN
// ===========================================

int main()
{
    vector<string> insertDatasetPath = {"input/lorem_ipsum.txt",
                                        "input/bee_movie_script.txt",
                                        "input/Alice_in_Wonderland.txt",
                                        "input/words_alpha.txt"};
    vector<string> searchDatasetPath = {"input/dataset_busqueda_loremipsum.txt",
                                        "input/dataset_busqueda_bee_movie.txt",
                                        "input/dataset_busqueda_Alice.txt",
                                        "input/dataset_busqueda_words_alpha.txt"};

    // Encabezado CSV
    cout << "Dataset,Modo,Estructura,Palabras_Buscadas,Palabras_Encontradas,Tiempo_Insercion_ms,Tiempo_Busqueda_ms,Memoria_KB,Porcentaje_Memoria,Exito" << endl;

    // Recopilar resultados
    vector<ExperimentResult> results;

    for (size_t i = 0; i < insertDatasetPath.size(); ++i)
    {
        string textToInsert = loadTextFile(insertDatasetPath[i]);
        vector<string> searchWords = loadDataset(searchDatasetPath[i]);
        
        // Extraer nombre del dataset (sin ruta y extensión)
        string datasetName = insertDatasetPath[i];
        size_t lastSlash = datasetName.find_last_of("/\\");
        if (lastSlash != string::npos) {
            datasetName = datasetName.substr(lastSlash + 1);
        }
        size_t dotPos = datasetName.find_last_of(".");
        if (dotPos != string::npos) {
            datasetName = datasetName.substr(0, dotPos);
        }

        // Modo 0
        NaiveTrie trie_mode0;
        RadixTrie radix_mode0;
        
        results.push_back(runExperimentCSV("NaiveTrie", datasetName, trie_mode0, textToInsert, searchWords, 0));
        results.push_back(runExperimentCSV("RadixTrie", datasetName, radix_mode0, textToInsert, searchWords, 0));

        // Modo 1
        NaiveTrie trie_mode1;
        RadixTrie radix_mode1;
        
        results.push_back(runExperimentCSV("NaiveTrie", datasetName, trie_mode1, textToInsert, searchWords, 1));
        results.push_back(runExperimentCSV("RadixTrie", datasetName, radix_mode1, textToInsert, searchWords, 1));
    }

    // Imprimir resultados en formato CSV
    cout << fixed << setprecision(3);
    for (const auto& r : results) {
        cout << r.dataset << ","
             << r.mode << ","
             << r.structure << ","
             << r.palabras_buscadas << ","
             << r.palabras_encontradas << ","
             << r.tiempo_insercion_ms << ","
             << r.tiempo_busqueda_ms << ","
             << r.memoria_kb << ","
             << r.percentage_mem << ","
             << (r.success ? "1" : "0")
             << endl;
    }

    return 0;
}
