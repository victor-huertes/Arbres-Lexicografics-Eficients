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
// ESTRUCTURAS PARA ESTADÍSTICAS
// ===========================================

struct Statistics {
    double mean;
    double median;
    double std_dev;
    double min;
    double max;
    double q1;  // Primer cuartil
    double q3;  // Tercer cuartil
    
    Statistics() : mean(0), median(0), std_dev(0), min(0), max(0), q1(0), q3(0) {}
};

// Calcula estadísticas de un vector de valores
Statistics calculateStatistics(vector<double> values) {
    Statistics stats;
    
    if (values.empty()) return stats;
    
    // Ordenar para calcular mediana y cuartiles
    sort(values.begin(), values.end());
    
    // Mínimo y máximo
    stats.min = values.front();
    stats.max = values.back();
    
    // Media
    stats.mean = accumulate(values.begin(), values.end(), 0.0) / values.size();
    
    // Mediana
    size_t n = values.size();
    if (n % 2 == 0) {
        stats.median = (values[n/2 - 1] + values[n/2]) / 2.0;
    } else {
        stats.median = values[n/2];
    }
    
    // Cuartiles
    size_t q1_pos = n / 4;
    size_t q3_pos = 3 * n / 4;
    stats.q1 = values[q1_pos];
    stats.q3 = values[q3_pos];
    
    // Desviación estándar
    double variance = 0.0;
    for (double val : values) {
        variance += (val - stats.mean) * (val - stats.mean);
    }
    variance /= values.size();
    stats.std_dev = sqrt(variance);
    
    return stats;
}

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

// Estructura para almacenar resultados con estadísticas de múltiples ejecuciones
struct ExperimentResultWithStats {
    string dataset;
    int mode;
    string structure;
    
    Statistics nodos_totales_stats;
    Statistics profundidad_maxima_stats;
    Statistics profundidad_mediana_stats;
    Statistics palabras_buscadas_stats;
    Statistics palabras_encontradas_stats;
    Statistics tiempo_insercion_stats;
    Statistics tiempo_busqueda_stats;
    Statistics memoria_stats;
    Statistics percentage_mem_stats;
    Statistics nodos_visitados_stats;
    
    int num_runs;
    bool success;
    
    ExperimentResultWithStats() : mode(0), num_runs(0), success(false) {}
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

// Función para ejecutar múltiples corridas y calcular estadísticas
template <typename Structure>
ExperimentResultWithStats runMultipleExperiments(
    const string &structureName,
    const string &datasetName,
    const string &textToInsert,
    const vector<string> &searchWords,
    int initMode = 0,
    int numRuns = 5)
{
    ExperimentResultWithStats finalResult;
    finalResult.dataset = datasetName;
    finalResult.mode = initMode;
    finalResult.structure = structureName;
    finalResult.num_runs = numRuns;
    finalResult.success = false;
    
    vector<double> nodos_totales_vec, profundidad_maxima_vec, profundidad_mediana_vec;
    vector<double> palabras_buscadas_vec, palabras_encontradas_vec;
    vector<double> tiempo_insercion_vec, tiempo_busqueda_vec;
    vector<double> memoria_vec, percentage_mem_vec, nodos_visitados_vec;
    
    int successful_runs = 0;
    
    for (int i = 0; i < numRuns; ++i) {
        cerr << "  Corrida " << (i+1) << "/" << numRuns << " - " << structureName 
             << " - " << datasetName << "..." << endl;
        
        {  // Bloque de scope para forzar destrucción inmediata
            Structure structure;  // Crear nueva instancia para cada corrida
            ExperimentResult result = runExperimentCSV(
                structureName, datasetName, structure, textToInsert, searchWords, initMode
            );
            
            if (result.success) {
                successful_runs++;
                nodos_totales_vec.push_back(static_cast<double>(result.nodos_totales));
                profundidad_maxima_vec.push_back(static_cast<double>(result.profundidad_maxima));
                profundidad_mediana_vec.push_back(result.profundidad_mediana);
                palabras_buscadas_vec.push_back(static_cast<double>(result.palabras_buscadas));
                palabras_encontradas_vec.push_back(static_cast<double>(result.palabras_encontradas));
                tiempo_insercion_vec.push_back(result.tiempo_insercion_ms);
                tiempo_busqueda_vec.push_back(result.tiempo_busqueda_ms);
                memoria_vec.push_back(result.memoria_kb);
                percentage_mem_vec.push_back(result.percentage_mem);
                nodos_visitados_vec.push_back(static_cast<double>(result.nodos_visitados));
            }
            // 'structure' se destruye aquí al salir del scope
        }
        
        // Pequeña pausa para permitir que el sistema libere memoria
        #ifndef _WIN32
        usleep(100000);  // 100ms de pausa
        #endif
    }
    
    if (successful_runs > 0) {
        finalResult.success = true;
        finalResult.num_runs = successful_runs;
        
        finalResult.nodos_totales_stats = calculateStatistics(nodos_totales_vec);
        finalResult.profundidad_maxima_stats = calculateStatistics(profundidad_maxima_vec);
        finalResult.profundidad_mediana_stats = calculateStatistics(profundidad_mediana_vec);
        finalResult.palabras_buscadas_stats = calculateStatistics(palabras_buscadas_vec);
        finalResult.palabras_encontradas_stats = calculateStatistics(palabras_encontradas_vec);
        finalResult.tiempo_insercion_stats = calculateStatistics(tiempo_insercion_vec);
        finalResult.tiempo_busqueda_stats = calculateStatistics(tiempo_busqueda_vec);
        finalResult.memoria_stats = calculateStatistics(memoria_vec);
        finalResult.percentage_mem_stats = calculateStatistics(percentage_mem_vec);
        finalResult.nodos_visitados_stats = calculateStatistics(nodos_visitados_vec);
    }
    
    return finalResult;
}

// Función para guardar estadísticas en CSV
void saveStatsToCSV(const vector<ExperimentResultWithStats>& results, 
                    const string& outputFile = "results_stats.csv") {
    ofstream file(outputFile);
    
    // Encabezado
    file << "Dataset,Modo,Estructura,Metric,Mean,Median,StdDev,Min,Max,Q1,Q3,NumRuns\n";
    
    file << fixed << setprecision(3);
    
    for (const auto& r : results) {
        if (!r.success) continue;
        
        // Tiempo de inserción
        file << r.dataset << "," << r.mode << "," << r.structure << ",Tiempo_Insercion_ms,"
             << r.tiempo_insercion_stats.mean << ","
             << r.tiempo_insercion_stats.median << ","
             << r.tiempo_insercion_stats.std_dev << ","
             << r.tiempo_insercion_stats.min << ","
             << r.tiempo_insercion_stats.max << ","
             << r.tiempo_insercion_stats.q1 << ","
             << r.tiempo_insercion_stats.q3 << ","
             << r.num_runs << "\n";
        
        // Tiempo de búsqueda
        file << r.dataset << "," << r.mode << "," << r.structure << ",Tiempo_Busqueda_ms,"
             << r.tiempo_busqueda_stats.mean << ","
             << r.tiempo_busqueda_stats.median << ","
             << r.tiempo_busqueda_stats.std_dev << ","
             << r.tiempo_busqueda_stats.min << ","
             << r.tiempo_busqueda_stats.max << ","
             << r.tiempo_busqueda_stats.q1 << ","
             << r.tiempo_busqueda_stats.q3 << ","
             << r.num_runs << "\n";
        
        // Memoria
        file << r.dataset << "," << r.mode << "," << r.structure << ",Memoria_KB,"
             << r.memoria_stats.mean << ","
             << r.memoria_stats.median << ","
             << r.memoria_stats.std_dev << ","
             << r.memoria_stats.min << ","
             << r.memoria_stats.max << ","
             << r.memoria_stats.q1 << ","
             << r.memoria_stats.q3 << ","
             << r.num_runs << "\n";
        
        // Nodos visitados
        file << r.dataset << "," << r.mode << "," << r.structure << ",Nodos_Visitados,"
             << r.nodos_visitados_stats.mean << ","
             << r.nodos_visitados_stats.median << ","
             << r.nodos_visitados_stats.std_dev << ","
             << r.nodos_visitados_stats.min << ","
             << r.nodos_visitados_stats.max << ","
             << r.nodos_visitados_stats.q1 << ","
             << r.nodos_visitados_stats.q3 << ","
             << r.num_runs << "\n";
        
        // Nodos totales
        file << r.dataset << "," << r.mode << "," << r.structure << ",Nodos_Totales,"
             << r.nodos_totales_stats.mean << ","
             << r.nodos_totales_stats.median << ","
             << r.nodos_totales_stats.std_dev << ","
             << r.nodos_totales_stats.min << ","
             << r.nodos_totales_stats.max << ","
             << r.nodos_totales_stats.q1 << ","
             << r.nodos_totales_stats.q3 << ","
             << r.num_runs << "\n";
    }
    
    file.close();
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
    // Verificar si se debe usar formato CSV o modo estadísticas
    bool useCSV = false;
    bool useStats = false;
    int numRuns = 5;  // Número de corridas por defecto para estadísticas
    
    if (argc > 1) {
        string arg1(argv[1]);
        if (arg1 == "--csv") {
            useCSV = true;
        } else if (arg1 == "--stats") {
            useStats = true;
            if (argc > 2) {
                numRuns = atoi(argv[2]);
                if (numRuns < 1) numRuns = 5;
            }
        }
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
        "data/dna_genome.txt",
        "data/wikipedia_titles.txt"
    };
    vector<string> searchDatasetPath = {
        "data/lorem_ipsum_search.txt",
        "data/bee_movie_search.txt",
        "data/alice_wonderland_search.txt",
        "data/moby_dick_search.txt",
        "data/words_alpha_search.txt",
        "data/dna_genome_search.txt",
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

            cerr << "Procesando dataset: " << datasetName << " (modo " << mode << ")..." << endl;
            
            // Ejecutar NaiveTrie en un scope separado para liberar memoria inmediatamente
            {
                cerr << "  - Ejecutando NaiveTrie..." << endl;
                NaiveTrie trie;
                results.push_back(runExperimentCSV("NaiveTrie", datasetName, trie, textToInsert, searchWords, mode));
                // trie se destruye aquí
            }
            
            // Pequeña pausa para permitir liberación de memoria
            #ifndef _WIN32
            usleep(50000);  // 50ms
            #endif
            
            // Ejecutar RadixTrie en un scope separado
            {
                cerr << "  - Ejecutando RadixTrie..." << endl;
                RadixTrie radix;
                results.push_back(runExperimentCSV("RadixTrie", datasetName, radix, textToInsert, searchWords, mode));
                // radix se destruye aquí
            }
            
            // Pausa entre datasets
            #ifndef _WIN32
            usleep(50000);  // 50ms
            #endif
            
            cerr << "  ✓ Completado" << endl << endl;
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
                 << (r.success ? "1" : "0") << "\n";
        }
    }
    else if (useStats) {
        // ===============================================
        // MODO ESTADÍSTICAS: Múltiples corridas con análisis estadístico
        // ===============================================
        
        cout << "=============================================================\n";
        cout << "  MODO ESTADÍSTICAS: Ejecutando " << numRuns << " corridas por experimento\n";
        cout << "=============================================================\n\n";
        
        vector<ExperimentResultWithStats> statsResults;
        
        for (size_t i = 0; i < insertDatasetPath.size(); ++i)
        {
            string textToInsert = loadTextFile(insertDatasetPath[i]);
            vector<string> searchWords = loadDataset(searchDatasetPath[i]);
            
            // Extraer nombre del dataset
            string datasetName = insertDatasetPath[i];
            size_t lastSlash = datasetName.find_last_of("/\\");
            if (lastSlash != string::npos) {
                datasetName = datasetName.substr(lastSlash + 1);
            }
            size_t dotPos = datasetName.find_last_of(".");
            if (dotPos != string::npos) {
                datasetName = datasetName.substr(0, dotPos);
            }
            
            // Determinar modo
            int mode = (datasetName == "wikipedia_titles") ? 1 : 0;
            
            cout << "Dataset: " << datasetName << " (Modo " << mode << ")\n";
            cout << "  Ejecutando NaiveTrie... ";
            cout.flush();
            
            auto naiveStats = runMultipleExperiments<NaiveTrie>(
                "NaiveTrie", datasetName, textToInsert, searchWords, mode, numRuns
            );
            statsResults.push_back(naiveStats);
            cout << (naiveStats.success ? "✓" : "✗") << "\n";
            
            cout << "  Ejecutando RadixTrie... ";
            cout.flush();
            
            auto radixStats = runMultipleExperiments<RadixTrie>(
                "RadixTrie", datasetName, textToInsert, searchWords, mode, numRuns
            );
            statsResults.push_back(radixStats);
            cout << (radixStats.success ? "✓" : "✗") << "\n\n";
        }
        
        // Guardar resultados estadísticos
        saveStatsToCSV(statsResults, "output/results_stats.csv");
        
        cout << "\n=============================================================\n";
        cout << "  Resultados guardados en output/results_stats.csv\n";
        cout << "=============================================================\n";
        
        // Mostrar resumen
        cout << "\nRESUMEN DE ESTADÍSTICAS:\n";
        cout << fixed << setprecision(3);
        for (const auto& r : statsResults) {
            if (!r.success) continue;
            cout << "\n" << r.dataset << " - " << r.structure << ":\n";
            cout << "  Tiempo Inserción: " << r.tiempo_insercion_stats.mean 
                 << " ± " << r.tiempo_insercion_stats.std_dev << " ms\n";
            cout << "  Tiempo Búsqueda:  " << r.tiempo_busqueda_stats.mean 
                 << " ± " << r.tiempo_busqueda_stats.std_dev << " ms\n";
            cout << "  Memoria:          " << r.memoria_stats.mean 
                 << " ± " << r.memoria_stats.std_dev << " KB\n";
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
