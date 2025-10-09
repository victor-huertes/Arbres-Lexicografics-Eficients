#include "../include/naive.h"
#include "../include/patricia.h"
#include "../include/radix.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
using namespace std;

// Wrapper para PatriciaTrie que implementa la interfaz Trie
class PatriciaTrieWrapper : public Trie {
private:
    PatriciaTrie trie;
    
public:
    void insert(const string& word) override {
        trie.insert(word);
    }
    
    bool search(const string& word) const override {
        // PatriciaTrie no tiene const search, pero podemos hacer un cast
        return const_cast<PatriciaTrie&>(trie).search(word);
    }
    
    bool starts_with(const string& prefix) const override {
        // Usamos search_positions para verificar si hay coincidencias
        auto positions = const_cast<PatriciaTrie&>(trie).starts_with_positions(prefix);
        return !positions.empty();
    }
    
    vector<string> get_words_with_prefix(const string& prefix) const override {
        auto words_with_pos = const_cast<PatriciaTrie&>(trie).autocomplete(prefix);
        vector<string> result;
        for (const auto& pair : words_with_pos) {
            result.push_back(pair.first);
        }
        return result;
    }
    
    vector<string> get_all_words() const override {
        auto words_with_pos = const_cast<PatriciaTrie&>(trie).get_words();
        vector<string> result;
        for (const auto& pair : words_with_pos) {
            result.push_back(pair.first);
        }
        return result;
    }
    
    bool empty() const override {
        return const_cast<PatriciaTrie&>(trie).isEmpty();
    }
    
    void clear() override {
        trie.makeEmpty();
    }
};

// Wrapper para RadixTrie que implementa la interfaz Trie
class RadixTrieWrapper : public Trie {
private:
    RadixTrie trie;
    
public:
    void insert(const string& word) override {
        trie.insert(word, 0); // Posición 0 por defecto
    }
    
    bool search(const string& word) const override {
        auto positions = const_cast<RadixTrie&>(trie).search(word);
        return !positions.empty();
    }
    
    bool starts_with(const string& prefix) const override {
        auto positions = const_cast<RadixTrie&>(trie).starts_with(prefix);
        return !positions.empty();
    }
    
    vector<string> get_words_with_prefix(const string& prefix) const override {
        auto words_with_pos = const_cast<RadixTrie&>(trie).autocomplete(prefix);
        vector<string> result;
        for (const auto& pair : words_with_pos) {
            result.push_back(pair.first);
        }
        return result;
    }
    
    vector<string> get_all_words() const override {
        auto words_with_pos = const_cast<RadixTrie&>(trie).get_words();
        vector<string> result;
        for (const auto& pair : words_with_pos) {
            result.push_back(pair.first);
        }
        return result;
    }
    
    bool empty() const override {
        // RadixTrie no tiene método empty, comprobamos si get_words está vacío
        return const_cast<RadixTrie&>(trie).get_words().empty();
    }
    
    void clear() override {
        // RadixTrie no tiene clear, crear uno nuevo
        trie.~RadixTrie();
        new (&trie) RadixTrie();
    }
};

void print_vector(const vector<string>& words, const string& title) {
    cout << title << ":\n";
    for (const auto& word : words) {
        cout << "  - " << word << "\n";
    }
    cout << "\n";
}

void demostration_trie(Trie* trie){
    cout << "=== Demostració del Trie ===" << endl;
    
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
        trie->insert(word);
        cout << "  Inserit: " << word << "\n";
    }
    cout << "\n";
    
    // Mostrar totes les paraules
    print_vector(trie->get_all_words(), "Totes les paraules al Trie");
    
    // Cercar paraules
    cout << "Cercant paraules:\n";
    vector<string> words_to_search; /* = {"gat", "gato", "casa", "cases", "cotxe"}; */
    while(cin >> x){
        words_to_search.push_back(x);
    }
    for (const auto& word : words_to_search) {
        bool found = trie->search(word);
        cout << "  '" << word << "': " << (found ? "TROBADA" : "NO TROBADA") << "\n";
    }
    cout << "\n";
    
    // Cercar prefixos
    cout << "Cercant prefixos:\n";
    vector<string> prefixes = {"ga", "cas", "co", "xyz"};
    for (const auto& prefix : prefixes) {
        bool has_prefix = trie->starts_with(prefix);
        cout << "  Prefix '" << prefix << "': " << (has_prefix ? "EXISTEIX" : "NO EXISTEIX") << "\n";
    }
    cout << "\n";
    
    // Buscar paraules amb prefix específic
    cout << "Paraules que comencen amb 'ga':\n";
    auto words_with_ga = trie->get_words_with_prefix("ga");
    for (const auto& word : words_with_ga) {
        cout << "  - " << word << "\n";
    }
    cout << "\n";
    
    cout << "Paraules que comencen amb 'co':\n";
    auto words_with_co = trie->get_words_with_prefix("co");
    for (const auto& word : words_with_co) {
        cout << "  - " << word << "\n";
    }
    cout << "\n";
    
    
    // Comprovar si el Trie està buit
    cout << "El Trie està buit? " << (trie->empty() ? "SÍ" : "NO") << "\n";
    
    // Netejar el Trie
    cout << "\nNetejant el Trie...\n";
    trie->clear();
    cout << "El Trie està buit després de netejar? " << (trie->empty() ? "SÍ" : "NO") << "\n";
    
    cout << "\n=== Fi de la demostració ===" << endl;
}

void demo_interactive(Trie* trie){
    cout << "=== Interfície interactiva del Trie ===" << endl;
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
                trie->insert(paraula);
                cout << "  Inserida: " << paraula << "\n";
            }
        } else if (comando == "b") {
            cout << "Introdueix la paraula a buscar: ";
            cin >> entrada;
            bool trobat = trie->search(entrada);
            cout << "  '" << entrada << "': " << (trobat ? "TROBADA" : "NO TROBADA") << "\n";
        } else if (comando == "p") {
            cout << "Introdueix el prefix a buscar: ";
            cin >> entrada;
            bool existeix = trie->starts_with(entrada);
            cout << "  Prefix '" << entrada << "': " << (existeix ? "EXISTEIX" : "NO EXISTEIX") << "\n";
        } else if (comando == "t") {
            cout << "Introdueix el prefix per mostrar paraules: ";
            cin >> entrada;
            auto paraules = trie->get_words_with_prefix(entrada);
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
    
    cout << "=== Selector de tipus de Trie ===" << endl;
    cout << "Escull el tipus de Trie que vols utilitzar:" << endl;
    cout << "  1 - NaiveTrie (Trie bàsic)" << endl;
    cout << "  2 - PatriciaTrie (PATRICIA)" << endl;
    cout << "  3 - RadixTrie (Radix Tree)" << endl;
    cout << "Introdueix l'opció (1/2/3): ";
    
    int opcio;
    cin >> opcio;
    
    unique_ptr<Trie> trie;
    
    switch(opcio) {
        case 1:
            cout << "\n=== Has seleccionat NaiveTrie ===" << endl;
            trie = make_unique<NaiveTrie>();
            break;
        case 2:
            cout << "\n=== Has seleccionat PatriciaTrie ===" << endl;
            trie = make_unique<PatriciaTrieWrapper>();
            break;
        case 3:
            cout << "\n=== Has seleccionat RadixTrie ===" << endl;
            trie = make_unique<RadixTrieWrapper>();
            break;
        default:
            cout << "Opció no vàlida. Utilitzant NaiveTrie per defecte." << endl;
            trie = make_unique<NaiveTrie>();
            break;
    }
    
    // Executar demo interactiu amb el Trie seleccionat
    demo_interactive(trie.get());
    
    //NO HACE FALTA IMPLEMENTAR PARA CADENAS NO ESTRUCTURADAS, SI JUSTIFICAMOS LO SUFICIENTEMENTE BIEN EN LA MEMORIA
    // esta bien lo de cambiar un alfabeto a otro -> {0,1} para hacerlo todo en binario. 
    //posibilidad de buscar por palabras que estén mal escritas y indexar frases enteras. 
    //entender tema typos y todo eso, no cal implementarlo, pero si explicarlo en la memoria.

    return 0;
}