#include "../src/radix.cpp"
#include <fstream>
#include <iostream>
#include <string>

void readFromFile(RadixTrie& trie, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    string word;
    int position = 0;
    while (file >> word) {  // Lee palabra por palabra
        trie.insert(word, position);
        ++position;
    }

    cout << "Se han insertado " << position << " palabras del archivo." << endl;
    file.close();
}

int main() {
    RadixTrie trie;

    int option;
    while(true) {
    cout << "\nMenu principal del RadixTrie:" << endl;
    cout << "[1] Insertar palabra" << endl;
    cout << "[2] Buscar palabra" << endl;
    cout << "[3] Leer archivo de texto" << endl;
    cout << "[9] Exit" << endl;

    int posicio = 0;
    cin >> option;
    if(option == 9) return 0;
    else if(option == 1) {
        string word;
        trie.insert(word, posicio);
        ++posicio;
        //cout << "Paraula inserida." << endl;
    } else if(option == 2) {
        string word;
        cout << "Introdueix la paraula a cercar: ";
        cin >> word;
        vector<int> positions = trie.search(word);
        cout << "Positions of word '" << word << "': ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    } else if(option == 3) {
        string filename;
        cout << "Introduce el nombre del archivo (debe estar en el directorio actual): ";
        cin >> filename;
        readFromFile(trie, filename);
    } else {
        cout << "Opción no válida." << endl;
        return 0;
    }
    cout << endl;
    }
}