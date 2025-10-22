#include "../src/naive.cpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

void readFromFile(NaiveTrie &trie, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    string word;
    int position = 0;
    while (file >> word)
    {   // Lee palabra por palabra
        // clean word and remove punctuation
        word.erase(remove_if(word.begin(), word.end(),
                             [](char c)
                             { return !isalnum(c); }),
                   word.end());
        // convert to lowercase for
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        // Only insert non-empty words
        if (!word.empty())
        {
            trie.insert(word, position);
            ++position;
        }
    }

    cout << "Se han insertado " << position << " palabras del archivo." << endl;
    file.close();
}
void search(NaiveTrie &trie, string word)
{
    vector<int> positions = trie.search_positions(word);
    if (positions.size() == 0)
    {
        cout << "No se ha encontrado la palabra " << word << endl;
        return;
    }
    cout << "Positions of word '" << word << "': ";
    for (int pos : positions)
    {
        cout << pos << " ";
    }
    cout << endl;
}

void searchFromFile(NaiveTrie &trie, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }
    string word;
    while (file >> word)
    {
        search(trie, word);
    }
    file.close();
}

void showAutocomplete(NaiveTrie &trie) {
    string prefix;
    cout << "Introduce el prefijo para autocompletar: ";
    cin >> prefix;
    
    vector<pair<string, int>> suggestions = trie.autocomplete(prefix);
    if(suggestions.empty()) {
        cout << "No se encontraron sugerencias para '" << prefix << "'" << endl;
        return;
    }
    
    cout << "Sugerencias para '" << prefix << "':" << endl;
    for(const auto &pair : suggestions) {
        cout << "- " << pair.first << " (posición: " << pair.second << ")" << endl;
    }
}

void showAllWords(NaiveTrie &trie) {
    vector<pair<string, int>> words = trie.get_words();
    if(words.empty()) {
        cout << "El trie está vacío" << endl;
        return;
    }
    
    cout << "Todas las palabras en el trie:" << endl;
    for(const auto &pair : words) {
        cout << "- " << pair.first << " (posición: " << pair.second << ")" << endl;
    }
}

int main()
{
    NaiveTrie trie;

    int option;
    while (true)
    {
        cout << "\nMenu principal del NaiveTrie:" << endl;
        cout << "[1] Insertar palabra" << endl;
        cout << "[2] Buscar palabra" << endl;
        cout << "[3] Leer archivo de texto" << endl;
        cout << "[4] Buscar dataset" << endl;
        cout << "[5] Autocomplete" << endl;
        cout << "[6] All words" << endl;
        cout << "[9] Exit" << endl;

        int posicio = 0;
        cin >> option;
        if (option == 9)
            return 0;
        else if (option == 1)
        {
            string word;
            trie.insert(word, posicio);
            ++posicio;
            // cout << "Paraula inserida." << endl;
        }
        else if (option == 2)
        {
            string word;
            cout << "Introdueix la paraula a cercar: ";
            cin >> word;
            search(trie, word);
        }
        else if (option == 3)
        {
            string filename;
            cout << "Introduce el nombre del archivo (debe estar en el directorio actual): ";
            cin >> filename;
            readFromFile(trie, filename);
        }
        else if (option == 4)
        {
            string filename;
            cout << "Introduce el nombre del archivo con el dataset a buscar: ";
            cin >> filename;
            searchFromFile(trie, filename);
        }
        else if (option == 5)
        {
            showAutocomplete(trie);
        }
        else if (option == 6)
        {
            showAllWords(trie);
        }
        else
        {
            cout << "Opción no válida." << endl;
        }
        cout << endl << endl;
    }
}