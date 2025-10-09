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

void demostration_naive(){
    cout << "=== Demostració del Trie ===" << endl;
    
    Trie trie;
    
    cout << "inserta las palabras que quieres introducir al trie" << endl;
    // Inserir paraules
    vector<string> words_to_insert; /* = {
        "gat", "gats", "gatet", "casa", "cases", "casat", 
        "cotxe", "cotxes", "col", "color", "colors"
    };
 */
    string x;
    while(cin >> x) words_to_insert.push_back(x);
    
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
    vector<string> words_to_search; /* = {"gat", "gato", "casa", "cases", "cotxe"}; */
    while(cin >> x){
        words_to_search.push_back(x);
    }
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
    
    
    // Comprovar si el Trie està buit
    cout << "El Trie està buit? " << (trie.empty() ? "SÍ" : "NO") << "\n";
    
    // Netejar el Trie
    cout << "\nNetejant el Trie...\n";
    trie.clear();
    cout << "El Trie està buit després de netejar? " << (trie.empty() ? "SÍ" : "NO") << "\n";
    
    cout << "\n=== Fi de la demostració ===" << endl;
}

int main() {
    
    demostration_naive();
    //NO HACE FALTA IMPLEMENTAR PARA CADENAS NO ESTRUCTURADAS, SI JUSTIFICAMOS LO SUFICIENTEMENTE BIEN EN LA MEMORIA
    
    return 0;
}