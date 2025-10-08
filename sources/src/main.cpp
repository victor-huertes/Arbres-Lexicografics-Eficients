#include "../include/trie.h"
#include <iostream>
#include <vector>
using namespace std;

void print_vector(const vector<string>& words, const string& title) {
    cout << title << ":\n";
    for (const auto& word : words) {
        cout << "  - " << word << "\n";
    }
    cout << "\n";
}

int main() {
    cout << "=== Demostració del Trie ===" << endl;
    
    Trie trie;
    
    // Inserir paraules
    vector<string> words_to_insert = {
        "gat", "gats", "gatet", "casa", "cases", "casat", 
        "cotxe", "cotxes", "col", "color", "colors"
    };
    
    cout << "Inserint paraules al Trie...\n";
    for (const auto& word : words_to_insert) {
        trie.insert(word);
        cout << "  Inserit: " << word << "\n";
    }
    cout << "\n";
    
    // Mostrar totes les paraules
    print_vector(trie.get_all_words(), "Totes les paraules al Trie");
    
    // Cercar paraules
    cout << "Cercant paraules:\n";
    vector<string> words_to_search = {"gat", "gato", "casa", "cases", "cotxe"};
    for (const auto& word : words_to_search) {
        bool found = trie.search(word);
        cout << "  '" << word << "': " << (found ? "TROBADA" : "NO TROBADA") << "\n";
    }
    cout << "\n";
    
    // Cercar prefixos
    cout << "Cercant prefixos:\n";
    vector<string> prefixes = {"ga", "cas", "co", "xyz"};
    for (const auto& prefix : prefixes) {
        bool has_prefix = trie.starts_with(prefix);
        cout << "  Prefix '" << prefix << "': " << (has_prefix ? "EXISTEIX" : "NO EXISTEIX") << "\n";
    }
    cout << "\n";
    
    // Buscar paraules amb prefix específic
    cout << "Paraules que comencen amb 'ga':\n";
    auto words_with_ga = trie.get_words_with_prefix("ga");
    for (const auto& word : words_with_ga) {
        cout << "  - " << word << "\n";
    }
    cout << "\n";
    
    cout << "Paraules que comencen amb 'co':\n";
    auto words_with_co = trie.get_words_with_prefix("co");
    for (const auto& word : words_with_co) {
        cout << "  - " << word << "\n";
    }
    cout << "\n";
    
    // Eliminar paraules
    cout << "Eliminant la paraula 'gats'...\n";
    bool removed = trie.remove("gats");
    cout << "  Eliminació " << (removed ? "EXITOSA" : "FALLIDA") << "\n\n";
    
    print_vector(trie.get_all_words(), "Paraules després d'eliminar 'gats'");
    
    // Verificar que la paraula ja no existeix
    cout << "Cercant 'gats' després d'eliminar-la: " << (trie.search("gats") ? "TROBADA" : "NO TROBADA") << "\n";
    cout << "Però 'gat' encara existeix: " << (trie.search("gat") ? "TROBADA" : "NO TROBADA") << "\n\n";
    
    // Comprovar si el Trie està buit
    cout << "El Trie està buit? " << (trie.empty() ? "SÍ" : "NO") << "\n";
    
    // Netejar el Trie
    cout << "\nNetejant el Trie...\n";
    trie.clear();
    cout << "El Trie està buit després de netejar? " << (trie.empty() ? "SÍ" : "NO") << "\n";
    
    cout << "\n=== Fi de la demostració ===" << endl;
    
    return 0;
}