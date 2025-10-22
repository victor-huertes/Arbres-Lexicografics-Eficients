#include <iostream>
#include <fstream>
#include <sstream>
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

// Función para guardar resultados en formato CSV
void saveToCSV(const string& datasetName, 
               const string& structureName,
               int mode,
               size_t wordsSearched,
               size_t wordsFound,
               double insertTime,
               double searchTime,
               double memoryKB,
               const string& outputFile = "results.csv") {
    
    static bool headerWritten = false;
    ofstream file(outputFile, ios::app);
    
    if (!headerWritten) {
        file << "Dataset,Structure,Mode,WordsSearched,WordsFound,InsertTime,SearchTime,MemoryKB\n";
        headerWritten = true;
    }
    
    file << datasetName << ","
         << structureName << ","
         << mode << ","
         << wordsSearched << ","
         << wordsFound << ","
         << fixed << setprecision(3) << insertTime << ","
         << searchTime << ","
         << memoryKB << "\n";
}

// ===========================================
// ESTRUCTURA Y FUNCIÓN PARA CSV
// ===========================================

struct ExperimentResult {
    string dataset;
    int mode;
    string structure;
    size_t nodos_totales;
    size_t profundidad_maxima;
    double profundidad_mediana;
    size_t palabras_buscadas;
    size_t palabras_encontradas;
    double tiempo_insercion_ms;
    double tiempo_busqueda_ms;
    double memoria_kb;
    double percentage_mem;
    size_t nodos_visitados;
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

        // Métricas estructurales
        result.nodos_totales = structure.get_total_nodes();
        pair<size_t, double> depth_metrics = structure.calculate_depth_metrics();
        result.profundidad_maxima = depth_metrics.first;
        result.profundidad_mediana = depth_metrics.second;

        // Memoria
        size_t memoryString = textToInsert.size() * 8; //medida total del dataset
        size_t memoryBytes = structure.get_memory_usage();
        result.memoria_kb = memoryBytes / 1024.0;
        result.percentage_mem = memoryBytes/memoryString * 100;

        // Búsqueda
        size_t found = 0;
        size_t nodes_visited_total = 0;
        result.tiempo_busqueda_ms = measureTime([&]()
                                        {
            for (const auto& w : searchWords) {
                if (structure.search_positions(w).size() != 0) {
                    found++;
                    nodes_visited_total += structure.get_last_nodes_visited();
                }
            } });

        result.palabras_encontradas = found;
        result.nodos_visitados = nodes_visited_total;
        result.success = true;
    }
    catch (const std::bad_alloc& e) {
        result.palabras_encontradas = 0;
        result.tiempo_insercion_ms = 0;
        result.tiempo_busqueda_ms = 0;
        result.memoria_kb = 0;
        result.nodos_totales = 0;
        result.profundidad_maxima = 0;
        result.profundidad_mediana = 0;
        result.nodos_visitados = 0;
    }
    catch (...) {
        result.palabras_encontradas = 0;
        result.tiempo_insercion_ms = 0;
        result.tiempo_busqueda_ms = 0;
        result.memoria_kb = 0;
        result.nodos_totales = 0;
        result.profundidad_maxima = 0;
        result.profundidad_mediana = 0;
        result.nodos_visitados = 0;
    }

    return result;
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

    try {
        // --- Inicialización ---
        double insertTime = measureTime([&]()
                                        { structure.init(textToInsert, initMode); });

        // ----------------------------------------------------
        // NUEVAS MÉTRICAS ESTRUCTURALES
        // ----------------------------------------------------
        cout << "\n--- Métrica Estructural ---\n";
        cout << "Nodos Totales Creados: " << structure.get_total_nodes() << endl;

        pair<size_t, double> depth_metrics = structure.calculate_depth_metrics();
        cout << "Profundidad Máxima:    " << depth_metrics.first << endl;
        cout << "Profundidad Mediana:   " << fixed << setprecision(2) << depth_metrics.second << endl;

        // Calcular memoria del Trie
        size_t memoryBytes = structure.get_memory_usage();
        double memoryKB = memoryBytes / 1024.0;

        // --- Búsqueda exacta ---
        size_t found = 0;
        size_t nodes_visited_total = 0;
        double searchTime = measureTime([&]()
                                        {
            for (const auto& w : searchWords)
                if (structure.search_positions(w).size() != 0) {
                    found++;
                    nodes_visited_total += structure.get_last_nodes_visited(); // Para medir nodos visitados
                } });

        // --- Resultados ---
        cout << fixed << setprecision(3);
        cout << "Palabras buscadas:   " << searchWords.size() << "\n";
        cout << "Palabras encontradas: " << found << "\n";
        cout << "Tiempo inserción:    " << insertTime << " ms\n";
        cout << "Tiempo búsqueda:     " << searchTime << " ms\n";
        cout << "Memoria usada:       " << memoryKB << " KB\n";
        cout << "Nodos Visitados:     " << nodes_visited_total << endl;
        
        saveToCSV(name, " ", initMode, searchWords.size(), found, insertTime, searchTime, memoryKB);

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

int main(int argc, char* argv[])
{
    // Verificar si se debe usar formato CSV
    bool useCSV = false;
    if (argc > 1 && string(argv[1]) == "--csv") {
        useCSV = true;
    }

    // Rutas a tus datasets
    // Assuming the project structure:
    // project_root/
    //   ├── sources/
    //   │   ├── src/
    //   │   │   └── experiments.cpp
    //   │   └── main/
    //   │       ├── Alice_in_Wonderland.txt
    //   │       └── dataset_busqueda_Alice.txt
    vector<string> insertDatasetPath = {
        "data/lorem_ipsum.txt",
        "data/bee_movie.txt",
        "data/alice_wonderland.txt",
        "data/moby_dick.txt",
        "data/words_alpha.txt",
        "data/dna_genome.txt"
        "data/wikipedia_titles.txt"
    };
    vector<string> searchDatasetPath = {
        "data/lorem_ipsum_search.txt",
        "data/bee_movie_search.txt",
        "data/alice_wonderland_search.txt",
        "data/moby_dick_search.txt",
        "data/words_alpha_search.txt",
        "data/dna_genome_search.txt"
        "data/wikipedia_titles_search.txt"
    };

    if (useCSV) {
        // ===============================================
        // MODO CSV: Salida en formato CSV
        // ===============================================
        
        // Encabezado CSV
        cout << "Dataset,Modo,Estructura,Nodos_Totales,Profundidad_Maxima,Profundidad_Mediana,Palabras_Buscadas,Palabras_Encontradas,Tiempo_Insercion_ms,Tiempo_Busqueda_ms,Memoria_KB,Porcentaje_Memoria,Nodos_Visitados,Exito" << endl;

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

            // Determinar modo: wikipedia usa modo 1, el resto modo 0
            int mode = (datasetName == "wikipedia_titles") ? 1 : 0;

            NaiveTrie trie;
            RadixTrie radix;
            
            results.push_back(runExperimentCSV("NaiveTrie", datasetName, trie, textToInsert, searchWords, mode));
            results.push_back(runExperimentCSV("RadixTrie", datasetName, radix, textToInsert, searchWords, mode));
        }

        // Imprimir resultados en formato CSV
        cout << fixed << setprecision(3);
        for (const auto& r : results) {
            cout << r.dataset << ","
                 << r.mode << ","
                 << r.structure << ","
                 << r.nodos_totales << ","
                 << r.profundidad_maxima << ","
                 << r.profundidad_mediana << ","
                 << r.palabras_buscadas << ","
                 << r.palabras_encontradas << ","
                 << r.tiempo_insercion_ms << ","
                 << r.tiempo_busqueda_ms << ","
                 << r.memoria_kb << ","
                 << r.percentage_mem << ","
                 << r.nodos_visitados << ","
                 << (r.success ? "1" : "0");
        }
    }
    else {
        // ===============================================
        // MODO NORMAL: Salida detallada
        // ===============================================
        
        // Cargar datasets
        for (size_t i = 0; i < insertDatasetPath.size(); ++i)
        {        
            string dataset_name = insertDatasetPath[i].substr(insertDatasetPath[i].find_last_of("/") + 1);
            // Cargar el texto completo para inserción
            string textToInsert = loadTextFile(insertDatasetPath[i]);
            
            // Cargar palabras para búsqueda
            vector<string> searchWords = loadDataset(searchDatasetPath[i]);

            cout << "DATASET: " << insertDatasetPath[i] << endl;
            
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

            // Determinar modo: wikipedia usa modo 1, el resto modo 0
            int mode = (datasetName == "wikipedia_titles") ? 1 : 0;
            
            cout << "\n=== MODO " << mode << ": " 
                 << (mode == 0 ? "Palabras con posición en texto" : "Palabras con número de línea") 
                 << " ===" << endl;
            
            // Crear estructuras
            NaiveTrie trie;
            RadixTrie radix;

            // Ejecutar experimentos con el modo correspondiente
            runExperiment("Naive Trie", trie, textToInsert, searchWords, mode);
            runExperiment("Radix Tree", radix, textToInsert, searchWords, mode);

            cout << endl;
        }
    }
    return 0;
}
