#include "../include/naive.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>

NaiveTrie::NaiveTrie() : root(make_unique<TrieNode>()) {total_nodes = 1;}

NaiveTrie::~NaiveTrie() {
    // Liberar todos los nodos hijos del root antes de que unique_ptr lo destruya
    if (root) {
        for (auto child : root->children) {
            if (child) {
                delete_subtree(child);
            }
        }
    }
}

void NaiveTrie::delete_subtree(TrieNode *node) {
    if (!node) return;
    
    // Recursivamente eliminar todos los hijos
    for (auto child : node->children) {
        if (child) {
            delete_subtree(child);
        }
    }
    
    // Eliminar el nodo actual
    delete node;
}

// Inicializa el trie con un texto
void NaiveTrie::init(const string &text, int mode)
{
    clear();
    
    if (mode == 0) {
        // Modo 0: Insertar palabras separadas por espacios/caracteres especiales
        // Similar al procesamiento del loadDataset en experiments.cpp
        string word;
        int position = 0;
        
        for (size_t i = 0; i < text.length(); ++i) {
            char c = text[i];
            
            // Si es alfanumérico, añadirlo a la palabra actual
            if (isalnum(c)) {
                if (word.empty()) {
                    position = i; // Guardar la posición inicial de la palabra
                }
                word += tolower(c); // Convertir a minúscula
            } 
            // Si no es alfanumérico y tenemos una palabra, insertarla
            else if (!word.empty()) {
                insert(word, position);
                word.clear();
            }
        }
        
        // Insertar la última palabra si existe
        if (!word.empty()) {
            insert(word, position);
        }
    }
    else if (mode == 1) {
        // Modo 1: Insertar palabras de cada línea con el número de línea como posición
        istringstream iss(text);
        string line;
        int line_number = 0;
        
        while (getline(iss, line)) {
            // Procesar cada palabra de la línea
            string word;
            for (char c : line) {
                // Si es alfanumérico, añadirlo a la palabra actual
                if (isalnum(c)) {
                    word += tolower(c);
                } 
                // Si no es alfanumérico y tenemos una palabra, insertarla
                else if (!word.empty()) {
                    insert(word, line_number);
                    word.clear();
                }
            }
            
            // Insertar la última palabra de la línea si existe
            if (!word.empty()) {
                insert(word, line_number);
            }
            
            line_number++;
        }
    }
    else if (mode == 2) {
        // Modo 2: Insertar todos los substrings de longitud 1 a 20
        // Optimización: Convertir todo el texto a minúsculas una sola vez
        const int MAX_SUBSTRING_LENGTH = 20;
        
        // Convertir a minúsculas de forma más eficiente
        string lower_text = text;
        for (char &c : lower_text) {
            c = tolower(static_cast<unsigned char>(c));
        }
        
        // Insertar substrings de forma incremental (construcción carácter por carácter)
        const size_t text_len = lower_text.length();
        for (size_t i = 0; i < text_len; ++i) {
            // Calcular el máximo de caracteres que podemos tomar desde esta posición
            const int max_len = min(MAX_SUBSTRING_LENGTH, static_cast<int>(text_len - i));
            
            // Construir substrings incrementalmente
            string substring;
            substring.reserve(max_len);
            
            for (int len = 1; len <= max_len; ++len) {
                substring += lower_text[i + len - 1];
                insert(substring, i);
            }
        }
    }
}

// Insertar una palabra y su posición
void NaiveTrie::insert(const string &word, int position)
{
    if (word.empty())
        return;

    TrieNode *current = root.get();

    for (unsigned char idx : word)
    {
        if (!current->children[idx])
        {
            current->children[idx] = new TrieNode();
            total_nodes++; // Contar el nuevo nodo
        }
        current = current->children[idx];
    }

    current->index.push_back(position);
    current->end_of_word = true;
}

// Buscar palabra y devolver posiciones
vector<int> NaiveTrie::search_positions(const string &word) const
{
    vector<int> positions;
    if (word.empty())
        return positions;

    TrieNode *current = root.get();
    last_nodes_visited = 0;

    for (unsigned char idx : word)
    {
        if (!current->children[idx])
        {
            return positions;
        }
        current = current->children[idx];
        last_nodes_visited++; // Incrementar por cada nodo visitado
    }

    if (!current->index.empty())
    {
        positions = current->index;
    }
    return positions;
}

// Encontrar palabras que comienzan con un prefijo (devuelve posiciones)
vector<int> NaiveTrie::starts_with_positions(const string &prefix) const
{
    vector<int> positions;
    if (prefix.empty())
        return positions;

    TrieNode *current = root.get();

    for (unsigned char idx : prefix)
    {
        if (!current->children[idx])
        {
            return positions;
        }
        current = current->children[idx];
    }

    // Recolectar todas las posiciones del subárbol
    collect_positions_from_subtree(current, positions);
    return positions;
}

// Función auxiliar para recolectar posiciones
void NaiveTrie::collect_positions_from_subtree(TrieNode *node, vector<int> &positions) const
{
    if (!node)
        return;

    if (!node->index.empty())
    {
        positions.insert(positions.end(), node->index.begin(), node->index.end());
    }

    for (size_t i = 0; i < node->children.size(); ++i)
    {
        if (node->children[i])
        {
            collect_positions_from_subtree(node->children[i], positions);
        }
    }
}

// Autocomplete: devuelve todas las palabras que empiezan por un prefijo
vector<pair<string, int>> NaiveTrie::autocomplete(const string &prefix) const
{
    vector<pair<string, int>> results;

    TrieNode *current = root.get();

    // Navegar hasta el final del prefix
    for (unsigned char idx : prefix)
    {
        if (!current->children[idx])
        {
            return results;
        }
        current = current->children[idx];
    }

    // Recolectar todas las palabras que comienzan con este prefix
    collect_words_with_positions(current, prefix, results);

    return results;
}

// Función auxiliar para recolectar palabras con posiciones
void NaiveTrie::collect_words_with_positions(TrieNode *node, const string &prefix, vector<pair<string, int>> &results) const
{
    if (!node)
        return;

    if (!node->index.empty())
    {
        for (int pos : node->index)
        {
            results.push_back({prefix, pos});
        }
    }

    for (size_t i = 0; i < node->children.size(); ++i)
    {
        if (node->children[i])
        {
            char next_char = static_cast<char>(i); // Ya no sumamos '0'
            collect_words_with_positions(node->children[i], prefix + next_char, results);
        }
    }
}

// Obtener todas las palabras del trie
vector<pair<string, int>> NaiveTrie::get_words() const
{
    return autocomplete("");
}

void NaiveTrie::insert(const string &word)
{
    if (word.empty())
        return;

    TrieNode *current = root.get();

    for (unsigned char idx : word)
    {
        if (!current->children[idx])
        {
            current->children[idx] = new TrieNode();
        }
        current = current->children[idx];
    }

    current->index.push_back(1); // Por defecto, posición 1 para inserciones directas
    current->end_of_word = true;
}

bool NaiveTrie::search(const string &word) const
{
    if (word.empty())
        return false;

    TrieNode *current = root.get();

    for (unsigned char idx : word)
    {
        if (!current->children[idx])
        {
            return false;
        }
        current = current->children[idx];
    }

    return !current->index.empty();
    /* if (word.empty()) return false;

    TrieNode* current = root.get();

    for (char c : word) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second.get();
    }

    return current->index; */
}

bool NaiveTrie::starts_with(const string &prefix) const
{
    if (prefix.empty())
        return true;

    TrieNode *current = root.get();

    for (unsigned char idx : prefix)
    {
        if (!current->children[idx])
            return false;
        current = current->children[idx];
    }

    return true;
}

vector<string> NaiveTrie::get_words_with_prefix(const string &prefix) const
{
    vector<string> results;

    if (!starts_with(prefix))
    {
        return results;
    }

    TrieNode *current = root.get();
    last_nodes_visited = 0;

    // Navegar fins al final del prefix
    for (unsigned char idx : prefix)
    {
        current = current->children[idx];
        last_nodes_visited++; // Incrementar por cada nodo visitado
        if (!current)
            return results;
    }

    // Recollir totes les paraules que comencin amb aquest prefix
    collect_words_with_prefix(current, prefix, results);

    return results;
}

void NaiveTrie::collect_words_with_prefix(TrieNode *node, const string &prefix, vector<string> &results) const
{
    if (!node->index.empty())
    {
        results.push_back(prefix);
    }

    for (size_t i = 0; i < node->children.size(); ++i)
    {
        if (node->children[i])
        {
            char next_char = static_cast<char>(i); // Ya no sumamos '0'
            collect_words_with_prefix(node->children[i], prefix + next_char, results);
        }
    }
}

vector<string> NaiveTrie::get_all_words() const
{
    return get_words_with_prefix("");
}

bool NaiveTrie::empty() const
{
    return root->children.empty();
}

void NaiveTrie::clear()
{
    // Liberar todos los nodos hijos existentes antes de resetear
    if (root) {
        for (auto child : root->children) {
            if (child) {
                delete_subtree(child);
            }
        }
    }
    
    // Crear nuevo root
    root = make_unique<TrieNode>();
    root->children = vector<TrieNode *>(128, nullptr);
    total_nodes = 1; // Contar la raíz
    last_nodes_visited = 0; // Resetear contador de nodos visitados
    root->end_of_word = false;
}

size_t NaiveTrie::calculate_node_memory(TrieNode *node) const
{
    if (!node)
        return 0;

    size_t memory = 0;

    // Memoria del nodo mismo
    memory += sizeof(TrieNode);

    // Memoria del vector de hijos (128 punteros)
    memory += sizeof(TrieNode *) * 128;

    // Memoria del vector de índices
    memory += sizeof(int) * node->index.capacity();

    // Recursivamente calcular memoria de los hijos
    for (auto *child : node->children)
    {
        if (child)
        {
            memory += calculate_node_memory(child);
        }
    }

    return memory;
}

size_t NaiveTrie::get_memory_usage() const
{
    return calculate_node_memory(root.get());
}

// =========================================================
// IMPLEMENTACIÓN DE calculate_depth_metrics
// =========================================================

void NaiveTrie::calculate_depth_metrics_recursive(TrieNode *node, size_t current_depth,
                                                  size_t &total_depth_sum, size_t &num_words,
                                                  size_t &max_depth) const
{
    if (!node) return;

    if (node->end_of_word)
    {
        num_words++;
        total_depth_sum += current_depth;
        if (current_depth > max_depth) max_depth = current_depth;
    }

    for (size_t i = 0; i < node->children.size(); ++i)
    {
        if (node->children[i])
        {
            // La profundidad se incrementa al bajar a un hijo
            calculate_depth_metrics_recursive(node->children[i], current_depth + 1, total_depth_sum, num_words, max_depth);
        }
    }
}

pair<size_t, double> NaiveTrie::calculate_depth_metrics() const
{
    size_t total_depth_sum = 0;
    size_t num_words = 0;
    size_t max_depth = 0;

    // Llamar a la función recursiva comenzando desde la raíz (profundidad 0)
    calculate_depth_metrics_recursive(root.get(), 0, total_depth_sum, num_words, max_depth);

    double median_depth = (num_words > 0) ? (double)total_depth_sum / num_words : 0.0;

    // Retorna (Profundidad Máxima, Profundidad Mediana)
    return {max_depth, median_depth};

}
