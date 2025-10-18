#include "../include/radixDone.h"
#include <algorithm>
#include <iostream>

// Constructor
RadixTrie::RadixTrie() : root(make_unique<RadixNode>()) {}

// Funció auxiliar per trobar la longitud del prefix comú entre dues strings
size_t find_common_prefix(const string& s1, const string& s2) {
    size_t len = 0;
    while (len < s1.length() && len < s2.length() && s1[len] == s2[len]) {
        len++;
    }
    return len;
}

// Inicialitzar el trie amb un text (insertar tots els sufixos)
void RadixTrie::init(const string& text) {
    root = make_unique<RadixNode>(""); // Netejar i reiniciar l'arbre
    for (size_t i = 0; i < text.length(); ++i) {
        // Inserir cada sufix amb la seva posició inicial
        string suffix = text.substr(i);
        insert(suffix, i);
    }
}

// Insertar una paraula i la seva posició
void RadixTrie::insert(const string& word, int position) {
    if (word.empty()) return;

    RadixNode* current = root.get();
    string remaining_word = word;

    while (!remaining_word.empty()) {
        char next_char = remaining_word[0];
        auto it = current->children.find(next_char);
        
        // 1. Cas NO HI HA COINCIDÈNCIA (afegir un nou fill)
        if (it == current->children.end()) {
            auto new_node = make_unique<RadixNode>(remaining_word);
            new_node->is_end_of_key = true;
            if (position != -1) new_node->positions.push_back(position);
            current->children[next_char] = std::move(new_node);
            return;
        }

        // 2. Cas POSSIBLE COINCIDÈNCIA (continuar o dividir)
        RadixNode* child = it->second.get();
        size_t match_len = find_common_prefix(remaining_word, child->label);
        
        // Cas A: Coincidència Parcial (DIVISIÓ DE NODE)
        if (match_len < child->label.length() && match_len < remaining_word.length()) { 
            
            // a) Crear el node SPLIT amb el prefix comú
            string common_prefix = child->label.substr(0, match_len);
            auto split_node = make_unique<RadixNode>(common_prefix);
            
            // b) Reconfigurar el node antic (el fill) per ser fill del node split
            string child_suffix = child->label.substr(match_len);
            child->label = child_suffix;
            split_node->children[child_suffix[0]] = std::move(it->second);
            
            // c) Gestionar la resta de la paraula nova (nou fill)
            string new_suffix = remaining_word.substr(match_len);
            
            // d) Crear i inserir el nou node per al sufix restant
            auto new_node = make_unique<RadixNode>(new_suffix);
            new_node->is_end_of_key = true;
            if (position != -1) new_node->positions.push_back(position);
            
            split_node->children[new_suffix[0]] = std::move(new_node);
            
            // e) Substituir l'antic fill per l'acabat de crear 'split_node'
            current->children[next_char] = std::move(split_node);
            return;
        }
        
        // Cas B: La Paraula Nova és un Prefix del Fill (DIVISIÓ DE NODE)
        if (match_len == remaining_word.length() && match_len < child->label.length()) {
            
            // a) Crear el node SPLIT amb la paraula nova (prefix)
            string new_prefix = remaining_word;
            auto split_node = make_unique<RadixNode>(new_prefix);
            split_node->is_end_of_key = true;
            if (position != -1) split_node->positions.push_back(position);
            
            // b) Reconfigurar el node antic per ser fill del node split
            string child_suffix = child->label.substr(match_len);
            child->label = child_suffix;
            split_node->children[child_suffix[0]] = std::move(it->second);
            
            // c) Substituir l'antic fill
            current->children[next_char] = std::move(split_node);
            return;
        }

        // Cas C: Coincidència Total amb l'Etiqueta del Fill (AVANÇAR)
        if (match_len == child->label.length() && match_len <= remaining_word.length()) {
            remaining_word = remaining_word.substr(match_len);
            current = child;
        } else {
            // Error, no s'hauria d'arribar
            return;
        }
    }
    
    // Si hem consumit tota la paraula, marquem el node actual com a final de clau.
    current->is_end_of_key = true;
    if (position != -1) current->positions.push_back(position);
}


// Cercar paraula i retornar posicions (Implementació const)
vector<int> RadixTrie::search(const string& word) const {
    if (word.empty()) return {};

    RadixNode* current = root.get();
    string remaining_word = word;

    while (!remaining_word.empty()) {
        char next_char = remaining_word[0];
        auto it = current->children.find(next_char);

        if (it == current->children.end()) return {};

        RadixNode* child = it->second.get();
        size_t match_len = find_common_prefix(remaining_word, child->label);

        // Si la paraula que busquem s'acaba
        if (match_len == remaining_word.length()) {
            // La paraula busqueda acaba aquí.
            // Només és vàlida si coincideix amb el prefix de l'etiqueta del fill.
            if (match_len <= child->label.length()) {
                return child->is_end_of_key ? child->positions : vector<int>{};
            } else {
                return {};
            }
        }

        // Si l'etiqueta del fill s'acaba
        if (match_len == child->label.length()) {
            // Coincidència total de l'etiqueta, continuem
            remaining_word = remaining_word.substr(match_len);
            current = child;
        } else {
            // No hi ha coincidència total de l'etiqueta ni de la paraula
            return {};
        }
    }

    // Hem consumit tota la paraula (només possible si la paraula era buida o ja està a l'arrel)
    return current->is_end_of_key ? current->positions : vector<int>{};
}

// Funció auxiliar per recol·lectar totes les posicions del subarbre
void RadixTrie::collect_positions_recursive(RadixNode* node, vector<int>& positions) const {
    if (!node) return;

    if (node->is_end_of_key) {
        positions.insert(positions.end(), node->positions.begin(), node->positions.end());
    }

    for (const auto& pair : node->children) {
        collect_positions_recursive(pair.second.get(), positions);
    }
}

// Encontrar palabras que comienzan con un prefijo (devuelve posiciones - Implementació const)
vector<int> RadixTrie::starts_with(const string& prefix) const {
    vector<int> positions;
    if (prefix.empty()) {
        collect_positions_recursive(root.get(), positions);
        return positions;
    }

    RadixNode* current = root.get();
    string remaining_prefix = prefix;
    RadixNode* final_node = nullptr;

    // 1. Cercar el node on acaba el prefix
    while (!remaining_prefix.empty()) {
        char next_char = remaining_prefix[0];
        auto it = current->children.find(next_char);

        if (it == current->children.end()) return {};

        RadixNode* child = it->second.get();
        size_t match_len = find_common_prefix(remaining_prefix, child->label);

        // El prefix acaba enmig de l'etiqueta del fill
        if (match_len == remaining_prefix.length()) {
            // Coincideix amb un prefix de l'etiqueta del fill, aquest és el node arrel del subarbre
            final_node = child;
            remaining_prefix = "";
            break;
        }
        
        // El prefix és més llarg que l'etiqueta del fill
        if (match_len == child->label.length()) {
            remaining_prefix = remaining_prefix.substr(match_len);
            current = child;
        } else {
            // El prefix no coincideix amb l'etiqueta del fill
            return {};
        }
    }
    
    // 2. Recollir totes les posicions del subarbre a partir del node trobat
    if (final_node == nullptr) final_node = current;
    
    // Si el prefix s'ha trobat, el node final és on comença el subarbre
    if (final_node->is_end_of_key && final_node == current) {
         positions.insert(positions.end(), final_node->positions.begin(), final_node->positions.end());
    }
    
    collect_positions_recursive(final_node, positions);
    return positions;
}


// Funció auxiliar per recol·lectar totes les paraules i posicions del subarbre
void RadixTrie::collect_words_recursive(RadixNode* node, string current_prefix, vector<pair<string, int>>& results) const {
    if (!node) return;

    // Afegim l'etiqueta del node actual al prefix construït
    current_prefix += node->label;

    if (node->is_end_of_key) {
        for (int pos : node->positions) {
            results.push_back({current_prefix, pos});
        }
    }

    // Continuem amb els fills
    for (const auto& pair : node->children) {
        // En la recursió, passem el prefix actualitzat
        collect_words_recursive(pair.second.get(), current_prefix, results);
    }
}

// Autocomplete: devuelve todas las palabras que empiezan por un prefijo (Implementació const)
vector<pair<string, int>> RadixTrie::autocomplete(const string& prefix) const {
    vector<pair<string, int>> results;
    if (prefix.empty()) {
        collect_words_recursive(root.get(), "", results);
        return results;
    }

    RadixNode* current = root.get();
    string remaining_prefix = prefix;
    string current_word_fragment = ""; // La part de la paraula construïda fins al node
    RadixNode* final_node = nullptr;

    // 1. Cercar el node on acaba el prefix
    while (!remaining_prefix.empty()) {
        char next_char = remaining_prefix[0];
        auto it = current->children.find(next_char);

        if (it == current->children.end()) return {};

        RadixNode* child = it->second.get();
        size_t match_len = find_common_prefix(remaining_prefix, child->label);
        
        // Cas A: El prefix s'atura enmig de l'etiqueta del fill (Final de la cerca)
        if (match_len == remaining_prefix.length()) {
            current_word_fragment += child->label.substr(0, match_len);
            final_node = child;
            remaining_prefix = ""; 
            break;
        }
        
        // Cas B: Coincidència total amb l'etiqueta del fill, però el prefix continua (Avançar)
        if (match_len == child->label.length()) {
            current_word_fragment += child->label;
            remaining_prefix = remaining_prefix.substr(match_len);
            current = child;
        } else {
            // El prefix no coincideix
            return {};
        }
    }
    
    // 2. Recollir totes les paraules (amb el prefix completat) a partir del node trobat
    if (final_node == nullptr) final_node = current;
    
    // El prefix base per a la recursivitat és la paraula construïda fins al node pare.
    string prefix_for_recursion = current_word_fragment.substr(0, current_word_fragment.length() - final_node->label.length());
    
    // La funció recursiva s'encarrega de reconstruir la paraula a partir del node final_node
    collect_words_recursive(final_node, prefix_for_recursion, results);
    
    // Necessitem filtrar els resultats, ja que la nostra cerca de prefix pot haver recollit 
    // sufixos que comencen abans de la longitud del prefix.
    vector<pair<string, int>> filtered_results;
    for (const auto& pair : results) {
        // La condició és que la paraula recollida COMMENCI amb el prefix
        if (pair.first.length() >= prefix.length() && pair.first.substr(0, prefix.length()) == prefix) {
            filtered_results.push_back(pair);
        }
    }
    
    return filtered_results;
}


// Obtener todas las palabras del trie (Implementació const)
vector<pair<string, int>> RadixTrie::get_words() const {
    vector<pair<string, int>> results;
    collect_words_recursive(root.get(), "", results);
    return results;
}

// Implementació de clear()
void RadixTrie::clear() {
    // Reemplaçar el unique_ptr actual amb un de nou i buit.
    // L'antic unique_ptr alliberarà automàticament tots els nodes.
    root = make_unique<RadixNode>("");
}

// Implementació de empty()
bool RadixTrie::empty() const {
    // El trie només és buit si l'arrel no té fills.
    // Assumim que l'arrel sempre existeix per simplificar la inserció.
    if (!root) return true; // Cas extrem si l'arrel és nullptr
    
    // Si l'arrel no té fills, el trie està buit.
    return root->children.empty(); 
}