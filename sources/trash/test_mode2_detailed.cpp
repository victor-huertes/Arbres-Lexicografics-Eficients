#include <iostream>
#include <string>
#include "sources/include/naive.h"

using namespace std;

int main() {
    // Ejemplo simple para verificar el Modo 2
    string test = "Hello";
    
    cout << "=== Texto de prueba: \"" << test << "\" ===" << endl;
    cout << "\nSubstrings esperados (con límite 4 para visualización):" << endl;
    cout << "Posición 0: 'H', 'He', 'Hel', 'Hell'" << endl;
    cout << "Posición 1: 'e', 'el', 'ell', 'ello'" << endl;
    cout << "Posición 2: 'l', 'll', 'llo'" << endl;
    cout << "Posición 3: 'l', 'lo'" << endl;
    cout << "Posición 4: 'o'" << endl;
    
    cout << "\n=== Probando NaiveTrie Modo 2 ===" << endl;
    NaiveTrie trie;
    trie.init(test, 2);
    
    // Probar algunas búsquedas
    vector<string> searches = {"h", "he", "hel", "hell", "hello", "e", "el", "ell", "ello", "l", "ll", "llo", "lo", "o"};
    
    for (const auto& search : searches) {
        auto positions = trie.search_positions(search);
        cout << "'" << search << "': ";
        if (positions.empty()) {
            cout << "No encontrado";
        } else {
            cout << "Encontrado en posiciones: ";
            for (int pos : positions) {
                cout << pos << " ";
            }
        }
        cout << endl;
    }
    
    return 0;
}
