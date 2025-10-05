#include "../include/trie.h"
#include <algorithm>

Trie::Trie() : root(std::make_unique<TrieNode>()) {}

void Trie::insert(const std::string& word) {
    if (word.empty()) return;
    
    TrieNode* current = root.get();
    
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = std::make_unique<TrieNode>();
        }
        current = current->children[c].get();
    }
    
    current->is_end_of_word = true;
}

bool Trie::search(const std::string& word) const {
    if (word.empty()) return false;
    
    TrieNode* current = root.get();
    
    for (char c : word) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second.get();
    }
    
    return current->is_end_of_word;
}

bool Trie::starts_with(const std::string& prefix) const {
    if (prefix.empty()) return true;
    
    TrieNode* current = root.get();
    
    for (char c : prefix) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second.get();
    }
    
    return true;
}

bool Trie::remove(const std::string& word) {
    if (word.empty() || !search(word)) {
        return false;
    }
    
    remove_helper(root.get(), word, 0);
    return true;
}

bool Trie::remove_helper(TrieNode* node, const std::string& word, size_t index) {
    if (index == word.length()) {
        // Hem arribat al final de la paraula
        node->is_end_of_word = false;
        
        // Si el node no té fills, pot ser eliminat
        return node->children.empty();
    }
    
    char c = word[index];
    auto it = node->children.find(c);
    
    if (it == node->children.end()) {
        return false;
    }
    
    bool should_delete_child = remove_helper(it->second.get(), word, index + 1);
    
    if (should_delete_child) {
        node->children.erase(it);
    }
    
    // El node actual pot ser eliminat si:
    // 1. No marca el final d'una paraula
    // 2. No té fills
    return !node->is_end_of_word && node->children.empty();
}

std::vector<std::string> Trie::get_words_with_prefix(const std::string& prefix) const {
    std::vector<std::string> results;
    
    if (!starts_with(prefix)) {
        return results;
    }
    
    TrieNode* current = root.get();
    
    // Navegar fins al final del prefix
    for (char c : prefix) {
        current = current->children[c].get();
    }
    
    // Recollir totes les paraules que comencin amb aquest prefix
    collect_words_with_prefix(current, prefix, results);
    
    return results;
}

void Trie::collect_words_with_prefix(TrieNode* node, const std::string& prefix, std::vector<std::string>& results) const {
    if (node->is_end_of_word) {
        results.push_back(prefix);
    }
    
    for (const auto& pair : node->children) {
        collect_words_with_prefix(pair.second.get(), prefix + pair.first, results);
    }
}

std::vector<std::string> Trie::get_all_words() const {
    return get_words_with_prefix("");
}

bool Trie::empty() const {
    return root->children.empty();
}

void Trie::clear() {
    root = std::make_unique<TrieNode>();
}