/**
 * @file test_performance.cpp
 * @brief Programa de testing para comparar el rendimiento de Naive Trie y Radix Trie
 * @author Performance Testing
 * @date 2025-10-18
 */

#include "../include/naive.h"
#include "../include/radix.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <cctype>

using namespace std;
using namespace chrono;

// Colores ANSI para terminal
namespace Color
{
    const string RESET = "\033[0m";
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string BLUE = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN = "\033[36m";
    const string BOLD = "\033[1m";
}

/**
 * @brief Imprime un encabezado formateado
 */
void print_header(const string &text)
{
    cout << "\n"
         << Color::MAGENTA << Color::BOLD;
    cout << string(80, '=') << "\n";
    cout << setw(40 + text.length() / 2) << text << "\n";
    cout << string(80, '=') << Color::RESET << "\n\n";
}

/**
 * @brief Imprime un subencabezado formateado
 */
void print_subheader(const string &text)
{
    cout << "\n"
         << Color::CYAN << Color::BOLD;
    cout << string(80, '-') << "\n";
    cout << text << "\n";
    cout << string(80, '-') << Color::RESET << "\n";
}

/**
 * @brief Imprime un resultado de prueba formateado
 */
void print_result(const string &operation, double time_ms, const string &extra_info = "")
{
    cout << Color::GREEN << "✓ " << left << setw(45) << operation << Color::RESET;
    cout << Color::YELLOW << right << setw(12) << fixed << setprecision(3) << time_ms << " ms" << Color::RESET;
    if (!extra_info.empty())
    {
        cout << "  " << Color::BLUE << extra_info << Color::RESET;
    }
    cout << "\n";
}

/**
 * @brief Lee un archivo y retorna su contenido como string
 */
string read_file(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        throw runtime_error("No se pudo abrir el archivo: " + filename);
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

/**
 * @brief Obtiene el tamaño de un archivo en KB
 */
double get_file_size_kb(const string &filename)
{
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open())
        return 0.0;

    streamsize size = file.tellg();
    file.close();
    return static_cast<double>(size) / 1024.0;
}

/**
 * @brief Cuenta las palabras aproximadas en un texto
 */
int count_words(const string &text)
{
    int count = 0;
    bool in_word = false;

    for (char c : text)
    {
        if (isspace(c))
        {
            in_word = false;
        }
        else if (!in_word)
        {
            in_word = true;
            count++;
        }
    }

    return count;
}

/**
 * @brief Limpia y normaliza el texto para el Naive Trie
 *
 * El Naive Trie usa c - '0' para calcular índices, lo que significa que solo
 * puede manejar caracteres en el rango 0-9 (ASCII 48-57).
 *
 * Esta función convierte el texto para que sea compatible:
 * - Convierte letras a dígitos (a->0, b->1, ..., z->25, luego módulo 10)
 * - Mantiene espacios como '0'
 * - Elimina puntuación
 *
 * @param text Texto original
 * @return Texto limpio con solo dígitos 0-9
 */
string clean_text_for_naive_trie(const string &text)
{
    string cleaned;
    cleaned.reserve(text.length());

    for (char c : text)
    {
        if (isalpha(c))
        {
            // Convertir letra a dígito: a/A->0, b/B->1, ..., j/J->9, k/K->0, ...
            char lower = tolower(c);
            int digit = (lower - 'a') % 10;
            cleaned += ('0' + digit);
        }
        else if (isspace(c))
        {
            // Mantener espacios como '0' para separar palabras
            cleaned += '0';
        }
        else if (isdigit(c))
        {
            // Los dígitos ya son válidos
            cleaned += c;
        }
        // Ignorar puntuación y otros caracteres
    }

    return cleaned;
}

/**
 * @brief Analiza y muestra estadísticas de caracteres en el texto
 */
void analyze_text_characters(const string &text)
{
    map<char, int> char_count;
    int letters = 0, digits = 0, spaces = 0, punctuation = 0, others = 0;

    for (char c : text)
    {
        char_count[c]++;
        if (isalpha(c))
            letters++;
        else if (isdigit(c))
            digits++;
        else if (isspace(c))
            spaces++;
        else if (ispunct(c))
            punctuation++;
        else
            others++;
    }

    cout << Color::CYAN << "Análisis de caracteres del texto:" << Color::RESET << "\n";
    cout << "  Letras: " << letters << "\n";
    cout << "  Dígitos: " << digits << "\n";
    cout << "  Espacios: " << spaces << "\n";
    cout << "  Puntuación: " << punctuation << "\n";
    cout << "  Otros: " << others << "\n";
    cout << "  Caracteres únicos: " << char_count.size() << "\n";
}

/**
 * @brief Prueba el rendimiento del Naive Trie
 */
void test_naive_trie(const string &filename)
{
    print_subheader("PRUEBA: Naive Trie con " + filename);

    try
    {
        double file_size = get_file_size_kb(filename);

        // 1. Inicialización con archivo
        cout << "\n"
             << Color::BOLD << "1. Inicialización con archivo ("
             << fixed << setprecision(2) << file_size << " KB)" << Color::RESET << "\n";

        auto start = high_resolution_clock::now();

        string text_content = read_file(filename);
        int word_count = count_words(text_content);

        NaiveTrie naive_trie;
        naive_trie.init(text_content);

        auto end = high_resolution_clock::now();
        double init_time = duration_cast<microseconds>(end - start).count() / 1000.0;

        print_result("Carga e inicialización del archivo", init_time,
                     "~" + to_string(word_count) + " palabras");

        // 2. Inserción de palabras
        cout << "\n"
             << Color::BOLD << "2. Inserción de palabras" << Color::RESET << "\n";

        // 2.1 Insertar varias palabras normales
        vector<string> new_words = {"testing", "performance", "algorithm"};
        for (const auto &word : new_words)
        {
            start = high_resolution_clock::now();
            naive_trie.insert(word, text_content.length());
            end = high_resolution_clock::now();
            double insert_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            print_result("Inserción palabra nueva: '" + word + "'", insert_time);
        }

        // 2.2 Insertar palabras que ya están en el texto
        vector<string> existing_words = {"Lorem", "ipsum", "dolor"};
        for (const auto &word : existing_words)
        {
            start = high_resolution_clock::now();
            naive_trie.insert(word, 999); // posición ficticia
            end = high_resolution_clock::now();
            double insert_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            print_result("Inserción palabra existente: '" + word + "'", insert_time);
        }

        // 3. Búsqueda de palabras
        cout << "\n"
             << Color::BOLD << "3. Búsqueda de palabras" << Color::RESET << "\n";

        vector<string> test_searches = {
            "Lorem",               // Palabra que aparece en el texto
            "ipsum",               // Palabra que aparece en el texto
            "dolor",               // Palabra que aparece en el texto
            "testing",             // Palabra insertada nueva
            "hello",               // Palabra que NO aparece
            "nonexistent_word_xyz" // Palabra que NO aparece
        };

        double total_search_time = 0.0;
        int found_count = 0;

        for (const auto &search_word : test_searches)
        {
            start = high_resolution_clock::now();

            auto positions = naive_trie.search_positions(search_word);

            end = high_resolution_clock::now();
            double search_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            total_search_time += search_time;

            string status;
            if (!positions.empty())
            {
                found_count++;
                status = "encontrada (" + to_string(positions.size()) + " ocurrencias)";
            }
            else
            {
                status = "no encontrada";
            }

            print_result("Búsqueda de '" + search_word + "'", search_time, status);
        }

        double avg_search_time = total_search_time / test_searches.size();
        cout << "\n"
             << Color::GREEN << "Tiempo promedio de búsqueda: "
             << fixed << setprecision(3) << avg_search_time << " ms" << Color::RESET << "\n";
        cout << Color::GREEN << "Palabras encontradas: " << found_count << "/"
             << test_searches.size() << Color::RESET << "\n";

        // 4. Autocompletado de palabras
        cout << "\n"
             << Color::BOLD << "4. Autocompletado de palabras" << Color::RESET << "\n";

        vector<string> autocomplete_prefixes = {
            "Lo",   // Prefijo que aparece (Lorem)
            "ip",   // Prefijo que aparece (ipsum)
            "dol",  // Prefijo que aparece (dolor, dolore, etc.)
            "test", // Prefijo de palabra insertada (testing)
            "xyz"   // Prefijo que NO aparece
        };

        double total_autocomplete_time = 0.0;
        int prefixes_with_results = 0;

        for (const auto &prefix : autocomplete_prefixes)
        {
            start = high_resolution_clock::now();

            auto completions = naive_trie.autocomplete(prefix);

            end = high_resolution_clock::now();
            double autocomplete_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            total_autocomplete_time += autocomplete_time;

            string status;
            if (!completions.empty())
            {
                prefixes_with_results++;
                // Limitar a mostrar máximo 5 sugerencias
                int show_count = min(5, (int)completions.size());
                status = to_string(completions.size()) + " sugerencias";
                if (completions.size() > 5)
                {
                    status += " (mostrando " + to_string(show_count) + ")";
                }
                status += ": ";
                for (int i = 0; i < show_count; i++)
                {
                    status += completions[i].first;
                    if (i < show_count - 1)
                        status += ", ";
                }
            }
            else
            {
                status = "sin sugerencias";
            }

            print_result("Autocompletado de '" + prefix + "'", autocomplete_time, status);
        }

        double avg_autocomplete_time = total_autocomplete_time / autocomplete_prefixes.size();
        cout << "\n"
             << Color::GREEN << "Tiempo promedio de autocompletado: "
             << fixed << setprecision(3) << avg_autocomplete_time << " ms" << Color::RESET << "\n";
        cout << Color::GREEN << "Prefijos con resultados: " << prefixes_with_results << "/"
             << autocomplete_prefixes.size() << Color::RESET << "\n";
    }
    catch (const exception &e)
    {
        cout << Color::RED << "✗ Error: " << e.what() << Color::RESET << "\n";
    }
}

/**
 * @brief Prueba el rendimiento del Radix Trie
 */
void test_radix_trie(const string &filename)
{
    print_subheader("PRUEBA: Radix Trie con " + filename);

    try
    {
        double file_size = get_file_size_kb(filename);

        // 1. Inicialización con archivo
        cout << "\n"
             << Color::BOLD << "1. Inicialización con archivo ("
             << fixed << setprecision(2) << file_size << " KB)" << Color::RESET << "\n";

        auto start = high_resolution_clock::now();

        string text_content = read_file(filename);
        int word_count = count_words(text_content);

        RadixTrie radix_trie;
        radix_trie.init(text_content);

        auto end = high_resolution_clock::now();
        double init_time = duration_cast<microseconds>(end - start).count() / 1000.0;

        print_result("Carga e inicialización del archivo", init_time,
                     "~" + to_string(word_count) + " palabras");

        // 2. Inserción de palabras
        cout << "\n"
             << Color::BOLD << "2. Inserción de palabras" << Color::RESET << "\n";

        // 2.1 Insertar varias palabras normales
        vector<string> new_words = {"testing", "performance", "algorithm"};
        for (const auto &word : new_words)
        {
            start = high_resolution_clock::now();
            radix_trie.insert(word, text_content.length());
            end = high_resolution_clock::now();
            double insert_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            print_result("Inserción palabra nueva: '" + word + "'", insert_time);
        }

        // 2.2 Insertar palabras que ya están en el texto
        vector<string> existing_words = {"Lorem", "ipsum", "dolor"};
        for (const auto &word : existing_words)
        {
            start = high_resolution_clock::now();
            radix_trie.insert(word, 999); // posición ficticia
            end = high_resolution_clock::now();
            double insert_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            print_result("Inserción palabra existente: '" + word + "'", insert_time);
        }

        // 3. Búsqueda de palabras
        cout << "\n"
             << Color::BOLD << "3. Búsqueda de palabras" << Color::RESET << "\n";

        vector<string> test_searches = {
            "Lorem",               // Palabra que aparece en el texto
            "ipsum",               // Palabra que aparece en el texto
            "dolor",               // Palabra que aparece en el texto
            "testing",             // Palabra insertada nueva
            "hello",               // Palabra que NO aparece
            "nonexistent_word_xyz" // Palabra que NO aparece
        };

        double total_search_time = 0.0;
        int found_count = 0;

        for (const auto &search_word : test_searches)
        {
            start = high_resolution_clock::now();

            auto positions = radix_trie.search(search_word);

            end = high_resolution_clock::now();
            double search_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            total_search_time += search_time;

            string status;
            if (!positions.empty())
            {
                found_count++;
                status = "encontrada (" + to_string(positions.size()) + " ocurrencias)";
            }
            else
            {
                status = "no encontrada";
            }

            print_result("Búsqueda de '" + search_word + "'", search_time, status);
        }

        double avg_search_time = total_search_time / test_searches.size();
        cout << "\n"
             << Color::GREEN << "Tiempo promedio de búsqueda: "
             << fixed << setprecision(3) << avg_search_time << " ms" << Color::RESET << "\n";
        cout << Color::GREEN << "Palabras encontradas: " << found_count << "/"
             << test_searches.size() << Color::RESET << "\n";

        // 4. Autocompletado de palabras
        cout << "\n"
             << Color::BOLD << "4. Autocompletado de palabras" << Color::RESET << "\n";

        vector<string> autocomplete_prefixes = {
            "Lo",   // Prefijo que aparece (Lorem)
            "ip",   // Prefijo que aparece (ipsum)
            "dol",  // Prefijo que aparece (dolor, dolore, etc.)
            "test", // Prefijo de palabra insertada (testing)
            "xyz"   // Prefijo que NO aparece
        };

        double total_autocomplete_time = 0.0;
        int prefixes_with_results = 0;

        for (const auto &prefix : autocomplete_prefixes)
        {
            start = high_resolution_clock::now();

            auto completions = radix_trie.autocomplete(prefix);

            end = high_resolution_clock::now();
            double autocomplete_time = duration_cast<microseconds>(end - start).count() / 1000.0;
            total_autocomplete_time += autocomplete_time;

            string status;
            if (!completions.empty())
            {
                prefixes_with_results++;
                // Limitar a mostrar máximo 5 sugerencias
                int show_count = min(5, (int)completions.size());
                status = to_string(completions.size()) + " sugerencias";
                if (completions.size() > 5)
                {
                    status += " (mostrando " + to_string(show_count) + ")";
                }
                status += ": ";
                for (int i = 0; i < show_count; i++)
                {
                    status += completions[i].first;
                    if (i < show_count - 1)
                        status += ", ";
                }
            }
            else
            {
                status = "sin sugerencias";
            }

            print_result("Autocompletado de '" + prefix + "'", autocomplete_time, status);
        }

        double avg_autocomplete_time = total_autocomplete_time / autocomplete_prefixes.size();
        cout << "\n"
             << Color::GREEN << "Tiempo promedio de autocompletado: "
             << fixed << setprecision(3) << avg_autocomplete_time << " ms" << Color::RESET << "\n";
        cout << Color::GREEN << "Prefijos con resultados: " << prefixes_with_results << "/"
             << autocomplete_prefixes.size() << Color::RESET << "\n";
    }
    catch (const exception &e)
    {
        cout << Color::RED << "✗ Error: " << e.what() << Color::RESET << "\n";
    }
}

/**
 * @brief Función principal
 */
int main(int argc, char *argv[])
{
    print_header("PRUEBAS DE RENDIMIENTO: NAIVE TRIE vs RADIX TRIE");

    // Determinar archivo de entrada
    string input_file = "input/lorem_ipsum.txt";

    if (argc > 1)
    {
        input_file = argv[1];
    }

    cout << Color::BOLD << "Archivo de entrada: " << Color::RESET << input_file << "\n";

    // Probar Naive Trie
    test_naive_trie(input_file);

    cout << "\n\n";

    // Probar Radix Trie
    test_radix_trie(input_file);

    // Resumen comparativo
    print_header("RESUMEN COMPARATIVO");
    cout << Color::BOLD << "Archivo probado: " << Color::RESET << input_file << "\n";
    cout << "\n"
         << Color::GREEN << "✓ Pruebas completadas exitosamente" << Color::RESET << "\n";
    cout << "\n"
         << Color::CYAN << "Nota: " << Color::RESET
         << "Las mediciones están en milisegundos (ms)\n";
    cout << Color::CYAN << "      " << Color::RESET
         << "Los tiempos incluyen todas las operaciones de inicialización y búsqueda.\n";

    return 0;
}
