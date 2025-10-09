// #include "../include/trie.h"
// #include <algorithm>

// Trie::Trie() : root(make_unique<TrieNode>()) {}

// void Trie::insert(const string& word) {

//     if (word.empty()) return;

//     TrieNode* current = root.get();

//     for (char c : word) {
//         int idx = c - '0';
//         if (idx < 0 || idx >= current->children.size()) {
//             // Resize vector if needed
//             current->children.resize(idx + 1, nullptr);
//         }
//         if (!current->children[idx]) {
//             current->children[idx] = new TrieNode();
//         }
//         current = current->children[idx];
//     }

//     current->index.push_back(1); // cambiar despues por la heuristica para decidir como indexar
//     current->end_of_word = true;
//     /* if (word.empty()) return;
    
//     TrieNode* current = root.get();
    
//     for (char c : word) {
//         if (current->children[c-'0'] == current->children.end()) {
//             current->children[c-'0'] = make_unique<TrieNode>();
//         }
//         current = current->children[c-'0'].get();
//     }
    
//     current->index.push_back(1); */ // cambiar despues por la heuristica para decidir como indexar
//     //para indexacion, si es un conjunto de datos no estructurados, se puede hacer indexando cada sufijo existente en el texto. 
// }

// bool Trie::search(const string& word) const {
//     if (word.empty()) return false;

//     TrieNode* current = root.get();

//     for (char c : word) {
//         int idx = c - '0';
//         if (idx < 0 || idx >= current->children.size() || !current->children[idx]) {
//             return false;
//         }
//         current = current->children[idx];
//     }

//     return !current->index.empty();
//     /* if (word.empty()) return false;
    
//     TrieNode* current = root.get();
    
//     for (char c : word) {
//         auto it = current->children.find(c);
//         if (it == current->children.end()) {
//             return false;
//         }
//         current = it->second.get();
//     }
    
//     return current->index; */
// }

// bool Trie::starts_with(const string& prefix) const {
//     if (prefix.empty()) return true;
    
//     TrieNode* current = root.get();
    
//     for (char c : prefix) {
//         int idx = c - '0';
//         if(idx > current->children.size() || idx < 0 || !current->children[idx]) return false;
        
//         current = current->children[idx];
//     }
    
//     return true;
// }

// // no hace falta
// /* bool Trie::remove(const string& word) {
//     if (word.empty() || !search(word)) {
//         return false;
//     }
    
//     remove_helper(root.get(), word, 0);
//     return true;
// }

// bool Trie::remove_helper(TrieNode* node, const string& word, size_t index) {
//     if (index == word.length()) {
//         // Hem arribat al final de la paraula
//         node->is_end_of_word = false;
        
//         // Si el node no té fills, pot ser eliminat
//         return node->children.empty();
//     }
    
//     char c = word[index];
//     auto it = node->children.find(c);
    
//     if (it == node->children.end()) {
//         return false;
//     }
    
//     bool should_delete_child = remove_helper(it->second.get(), word, index + 1);
    
//     if (should_delete_child) {
//         node->children.erase(it);
//     }
    
//     // El node actual pot ser eliminat si:
//     // 1. No marca el final d'una paraula
//     // 2. No té fills
//     return !node->is_end_of_word && node->children.empty();
// } */

// vector<string> Trie::get_words_with_prefix(const string& prefix) const {

//     vector<string> results;
    
//     if (!starts_with(prefix)) {
//         return results;
//     }
    
//     TrieNode* current = root.get();
    
//     // Navegar fins al final del prefix
//     for (char c : prefix) {
//         current = current->children[c-'0'];
//     }
    
//     // Recollir totes les paraules que comencin amb aquest prefix
//     collect_words_with_prefix(current, prefix, results);
    
//     return results;
// }

// void Trie::collect_words_with_prefix(TrieNode* node, const string& prefix, vector<string>& results) const {
//     if (!node->index.empty()) {
//         results.push_back(prefix);
//     }
    
//     for (size_t i = 0; i < node->children.size(); ++i) {
//         if (node->children[i]) {
//             char next_char = static_cast<char>(i + '0');
//             collect_words_with_prefix(node->children[i], prefix + next_char, results);
//         }
//     }
// }

// vector<string> Trie::get_all_words() const {
//     return get_words_with_prefix("");
// }

// bool Trie::empty() const {
//     return root->children.empty();
// }

// void Trie::clear() {
//     root = make_unique<TrieNode>();
// }