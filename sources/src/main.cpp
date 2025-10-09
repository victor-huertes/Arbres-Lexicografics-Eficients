#include "../include/trie.h"
#include <iostream>
#include <vector>
#include <sstream>
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

void demo_naive2(){
    cout << "=== Interfície interactiva del Trie ===" << endl;
    Trie trie;
    string comando, entrada;

    cout << "Opcions disponibles:\n";
    cout << "  i - Introduir paraules (separades per espais)\n";
    cout << "  b - Buscar una paraula\n";
    cout << "  p - Buscar si existeix un prefix\n";
    cout << "  t - Mostrar totes les paraules amb un prefix\n";
    cout << "  q - Sortir\n";

    while (true) {
        cout << "\nIntrodueix una opció (i/b/p/t/q): ";
        cin >> comando;
        if (comando == "q") {
            cout << "Sortint de la demostració...\n";
            break;
        }
        if (comando == "i") {
            cout << "Introdueix les paraules a inserir (separades per espais): ";
            cin.ignore();
            getline(cin, entrada);
            istringstream iss(entrada);
            string paraula;
            while (iss >> paraula) {
                trie.insert(paraula);
                cout << "  Inserida: " << paraula << "\n";
            }
        } else if (comando == "b") {
            cout << "Introdueix la paraula a buscar: ";
            cin >> entrada;
            bool trobat = trie.search(entrada);
            cout << "  '" << entrada << "': " << (trobat ? "TROBADA" : "NO TROBADA") << "\n";
        } else if (comando == "p") {
            cout << "Introdueix el prefix a buscar: ";
            cin >> entrada;
            bool existeix = trie.starts_with(entrada);
            cout << "  Prefix '" << entrada << "': " << (existeix ? "EXISTEIX" : "NO EXISTEIX") << "\n";
        } else if (comando == "t") {
            cout << "Introdueix el prefix per mostrar paraules: ";
            cin >> entrada;
            auto paraules = trie.get_words_with_prefix(entrada);
            if (paraules.empty()) {
                cout << "  No hi ha paraules amb el prefix '" << entrada << "'.\n";
            } else {
                cout << "  Paraules amb el prefix '" << entrada << "':\n";
                for (const auto& p : paraules) {
                    cout << "    - " << p << "\n";
                }
            }
        } else {
            cout << "Opció no reconeguda. Torna-ho a intentar.\n";
        }
    }
    cout << "=== Fi de la demostració interactiva ===" << endl;
}

int main() {
    
    //demostration_naive();
    demo_naive2();
    //NO HACE FALTA IMPLEMENTAR PARA CADENAS NO ESTRUCTURADAS, SI JUSTIFICAMOS LO SUFICIENTEMENTE BIEN EN LA MEMORIA
    // esta bien lo de cambiar un alfabeto a otro -> {0,1} para hacerlo todo en binario. 
    //posibilidad de buscar por palabras que estén mal escritas y indexar frases enteras. 
    //entender tema typos y todo eso, no cal implementarlo, pero si explicarlo en la memoria.

    return 0;
}