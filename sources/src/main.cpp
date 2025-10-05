#include "../include/trie.h"
#include <iostream>
#include <vector>

void print_vector(const std::vector<std::string>& words, const std::string& title) {
    std::cout << title << ":\n";
    for (const auto& word : words) {
        std::cout << "  - " << word << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== Demostració del Trie ===" << std::endl;
    
    Trie trie;
    
    // Inserir paraules
    std::vector<std::string> words_to_insert = {
        "gat", "gats", "gatet", "casa", "cases", "casat", 
        "cotxe", "cotxes", "col", "color", "colors"
    };
    
    std::cout << "Inserint paraules al Trie...\n";
    for (const auto& word : words_to_insert) {
        trie.insert(word);
        std::cout << "  Inserit: " << word << "\n";
    }
    std::cout << "\n";
    
    // Mostrar totes les paraules
    print_vector(trie.get_all_words(), "Totes les paraules al Trie");
    
    // Cercar paraules
    std::cout << "Cercant paraules:\n";
    std::vector<std::string> words_to_search = {"gat", "gato", "casa", "cases", "cotxe"};
    for (const auto& word : words_to_search) {
        bool found = trie.search(word);
        std::cout << "  '" << word << "': " << (found ? "TROBADA" : "NO TROBADA") << "\n";
    }
    std::cout << "\n";
    
    // Cercar prefixos
    std::cout << "Cercant prefixos:\n";
    std::vector<std::string> prefixes = {"ga", "cas", "co", "xyz"};
    for (const auto& prefix : prefixes) {
        bool has_prefix = trie.starts_with(prefix);
        std::cout << "  Prefix '" << prefix << "': " << (has_prefix ? "EXISTEIX" : "NO EXISTEIX") << "\n";
    }
    std::cout << "\n";
    
    // Buscar paraules amb prefix específic
    std::cout << "Paraules que comencen amb 'ga':\n";
    auto words_with_ga = trie.get_words_with_prefix("ga");
    for (const auto& word : words_with_ga) {
        std::cout << "  - " << word << "\n";
    }
    std::cout << "\n";
    
    std::cout << "Paraules que comencen amb 'co':\n";
    auto words_with_co = trie.get_words_with_prefix("co");
    for (const auto& word : words_with_co) {
        std::cout << "  - " << word << "\n";
    }
    std::cout << "\n";
    
    // Eliminar paraules
    std::cout << "Eliminant la paraula 'gats'...\n";
    bool removed = trie.remove("gats");
    std::cout << "  Eliminació " << (removed ? "EXITOSA" : "FALLIDA") << "\n\n";
    
    print_vector(trie.get_all_words(), "Paraules després d'eliminar 'gats'");
    
    // Verificar que la paraula ja no existeix
    std::cout << "Cercant 'gats' després d'eliminar-la: " << (trie.search("gats") ? "TROBADA" : "NO TROBADA") << "\n";
    std::cout << "Però 'gat' encara existeix: " << (trie.search("gat") ? "TROBADA" : "NO TROBADA") << "\n\n";
    
    // Comprovar si el Trie està buit
    std::cout << "El Trie està buit? " << (trie.empty() ? "SÍ" : "NO") << "\n";
    
    // Netejar el Trie
    std::cout << "\nNetejant el Trie...\n";
    trie.clear();
    std::cout << "El Trie està buit després de netejar? " << (trie.empty() ? "SÍ" : "NO") << "\n";
    
    std::cout << "\n=== Fi de la demostració ===" << std::endl;
    
    return 0;
}