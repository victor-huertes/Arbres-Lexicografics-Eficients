#include "../include/naive.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>

NaiveTrie::NaiveTrie() : root(make_unique<TrieNode>()) {}

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

    for (unsigned char idx : word)
    {
        if (!current->children[idx])
        {
            return positions;
        }
        current = current->children[idx];
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
    autocomplete_aux(current, prefix, results, 5);

    return results;
}

// Función auxiliar para recolectar palabras con posiciones
void NaiveTrie::collect_words_with_positions(TrieNode *node, const string &prefix, vector<pair<string, int>> &results) const
{
    if (!node)
        return;

    // Solo añadir a results si es fin de palabra
    if (node->end_of_word && !node->index.empty())
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
            char next_char = static_cast<char>(i);
            collect_words_with_positions(node->children[i], prefix + next_char, results);
        }
    }
}

void NaiveTrie::autocomplete_aux(TrieNode *node, const string &prefix, vector<pair<string, int>> &results, int limit) const
{
    if (!node || results.size() >= limit)
        return;

    if (node->end_of_word && !node->index.empty())
    {
        // Anadimos la palabra una sola vez con su primera posición
        results.push_back({prefix, node->index[0]});
    }

    for (size_t i = 0; i < node->children.size() && results.size() < limit; ++i)
    {
        if (node->children[i])
        {
            char next_char = static_cast<char>(i);
            collect_words_with_positions(node->children[i], prefix + next_char, results, limit);
        }
    }
}

// Obtener todas las palabras del trie
vector<pair<string, int>> NaiveTrie::get_words() const
{
    vector<pair<string, int>> result;
    return collect_words_with_positions(root.get(), "", result);
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

    // Navegar fins al final del prefix
    for (unsigned char idx : prefix)
    {
        current = current->children[idx];
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
    root = make_unique<TrieNode>();
    root->children = vector<TrieNode *>(128, nullptr);
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
