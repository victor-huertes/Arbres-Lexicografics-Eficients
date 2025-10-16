#include "../include/naive.h"
#include <algorithm>

NaiveTrie::NaiveTrie() : root(make_unique<TrieNode>()) {}

// Inicializa el trie con un texto (inserta todos los sufijos)
void NaiveTrie::init(const string& text) {
    clear();
    for (size_t i = 0; i < text.length(); ++i) {
        string suffix = text.substr(i);
        insert(suffix, i);
    }
}

// Insertar una palabra y su posición
void NaiveTrie::insert(const string& word, int position) {
    if (word.empty()) return;

    TrieNode* current = root.get();

    for (char c : word) {
        int idx = c - '0';
        if (idx < 0 || idx >= (int)current->children.size()) {
            current->children.resize(idx + 1, nullptr);
        }
        if (!current->children[idx]) {
            current->children[idx] = new TrieNode();
        }
        current = current->children[idx];
    }

    current->index.push_back(position);
    current->end_of_word = true;
}

// Buscar palabra y devolver posiciones
vector<int> NaiveTrie::search_positions(const string& word) const {
    vector<int> positions;
    if (word.empty()) return positions;

    TrieNode* current = root.get();

    for (char c : word) {
        int idx = c - '0';
        if (idx < 0 || idx >= (int)current->children.size() || !current->children[idx]) {
            return positions;
        }
        current = current->children[idx];
    }

    if (!current->index.empty()) {
        positions = current->index;
    }
    return positions;
}

// Encontrar palabras que comienzan con un prefijo (devuelve posiciones)
vector<int> NaiveTrie::starts_with_positions(const string& prefix) const {
    vector<int> positions;
    if (prefix.empty()) return positions;
    
    TrieNode* current = root.get();
    
    for (char c : prefix) {
        int idx = c - '0';
        if(idx >= (int)current->children.size() || idx < 0 || !current->children[idx]) {
            return positions;
        }
        current = current->children[idx];
    }
    
    // Recolectar todas las posiciones del subárbol
    collect_positions_from_subtree(current, positions);
    return positions;
}

// Función auxiliar para recolectar posiciones
void NaiveTrie::collect_positions_from_subtree(TrieNode* node, vector<int>& positions) const {
    if (!node) return;
    
    if (!node->index.empty()) {
        positions.insert(positions.end(), node->index.begin(), node->index.end());
    }
    
    for (size_t i = 0; i < node->children.size(); ++i) {
        if (node->children[i]) {
            collect_positions_from_subtree(node->children[i], positions);
        }
    }
}

// Autocomplete: devuelve todas las palabras que empiezan por un prefijo
vector<pair<string, int>> NaiveTrie::autocomplete(const string& prefix) const {
    vector<pair<string, int>> results;
    
    TrieNode* current = root.get();
    
    // Navegar hasta el final del prefix
    for (char c : prefix) {
        int idx = c - '0';
        if (idx >= (int)current->children.size() || idx < 0 || !current->children[idx]) {
            return results;
        }
        current = current->children[idx];
    }
    
    // Recolectar todas las palabras que comienzan con este prefix
    collect_words_with_positions(current, prefix, results);
    
    return results;
}

// Función auxiliar para recolectar palabras con posiciones
void NaiveTrie::collect_words_with_positions(TrieNode* node, const string& prefix, vector<pair<string, int>>& results) const {
    if (!node) return;
    
    if (!node->index.empty()) {
        for (int pos : node->index) {
            results.push_back({prefix, pos});
        }
    }
    
    for (size_t i = 0; i < node->children.size(); ++i) {
        if (node->children[i]) {
            char next_char = static_cast<char>(i + '0');
            collect_words_with_positions(node->children[i], prefix + next_char, results);
        }
    }
}

// Obtener todas las palabras del trie
vector<pair<string, int>> NaiveTrie::get_words() const {
    return autocomplete("");
}

// Método legacy: insertar sin posición
void NaiveTrie::insert(const string& word) {

    if (word.empty()) return;

    TrieNode* current = root.get();

    for (char c : word) {
        int idx = c - '0';
        if (idx < 0 || idx >= (int)current->children.size()) {
            // Resize vector if needed
            current->children.resize(idx + 1, nullptr);
        }
        if (!current->children[idx]) {
            current->children[idx] = new TrieNode();
        }
        current = current->children[idx];
    }

    current->index.push_back(1); // cambiar despues por la heuristica para decidir como indexar
    current->end_of_word = true;
    /* if (word.empty()) return;
    
    TrieNode* current = root.get();
    
    for (char c : word) {
        if (current->children[c-'0'] == current->children.end()) {
            current->children[c-'0'] = make_unique<TrieNode>();
        }
        current = current->children[c-'0'].get();
    }
    
    current->index.push_back(1); */ // cambiar despues por la heuristica para decidir como indexar
    //para indexacion, si es un conjunto de datos no estructurados, se puede hacer indexando cada sufijo existente en el texto. 
}

bool NaiveTrie::search(const string& word) const {
    if (word.empty()) return false;

    TrieNode* current = root.get();

    for (char c : word) {
        int idx = c - '0';
        if (idx < 0 || idx >= (int)current->children.size() || !current->children[idx]) {
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

bool NaiveTrie::starts_with(const string& prefix) const {
    if (prefix.empty()) return true;
    
    TrieNode* current = root.get();
    
    for (char c : prefix) {
        int idx = c - '0';
        if(idx > (int)current->children.size() || idx < 0 || !current->children[idx]) return false;
        
        current = current->children[idx];
    }
    
    return true;
}

vector<string> NaiveTrie::get_words_with_prefix(const string& prefix) const {

    vector<string> results;
    
    if (!starts_with(prefix)) {
        return results;
    }
    
    TrieNode* current = root.get();
    
    // Navegar fins al final del prefix
    for (char c : prefix) {
        current = current->children[c-'0'];
    }
    
    // Recollir totes les paraules que comencin amb aquest prefix
    collect_words_with_prefix(current, prefix, results);
    
    return results;
}

void NaiveTrie::collect_words_with_prefix(TrieNode* node, const string& prefix, vector<string>& results) const {
    if (!node->index.empty()) {
        results.push_back(prefix);
    }
    
    for (size_t i = 0; i < node->children.size(); ++i) {
        if (node->children[i]) {
            char next_char = static_cast<char>(i + '0');
            collect_words_with_prefix(node->children[i], prefix + next_char, results);
        }
    }
}

vector<string> NaiveTrie::get_all_words() const {
    return get_words_with_prefix("");
}

bool NaiveTrie::empty() const {
    return root->children.empty();
}

void NaiveTrie::clear() {
    root = make_unique<TrieNode>();
}