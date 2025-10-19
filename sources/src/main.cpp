#include "../include/naive.h"
#include "../include/radix.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
using namespace std;

void show_help() {
    cout << "\n=== Comandos disponibles ===" << endl;
    cout << "  /insert <palabra1> <palabra2> ... - Inserta una o mas palabras" << endl;
    cout << "  /search <palabra>                  - Busca si existe una palabra" << endl;
    cout << "  /prefix <prefijo>                  - Muestra palabras con el prefijo" << endl;
    cout << "  /all                               - Muestra todas las palabras" << endl;
    cout << "  /clear                             - Limpia el trie" << endl;
    cout << "  /help                              - Muestra esta ayuda" << endl;
    cout << "  /quit                              - Sale del programa" << endl;
    cout << endl;
}

void process_naive_command(NaiveTrie& trie, const string& comando, const vector<string>& args) {
    if (comando == "/insert") {
        if (args.empty()) {
            cout << "Error: Debes especificar al menos una palabra" << endl;
            return;
        }
        for (const auto& palabra : args) {
            trie.insert(palabra);
            cout << "Insertada: " << palabra << endl;
        }
    }
    else if (comando == "/search") {
        if (args.empty()) {
            cout << "Error: Debes especificar una palabra" << endl;
            return;
        }
        bool encontrada = trie.search(args[0]);
        cout << "Palabra '" << args[0] << "': " << (encontrada ? "ENCONTRADA" : "NO ENCONTRADA") << endl;
    }
    else if (comando == "/prefix") {
        if (args.empty()) {
            cout << "Error: Debes especificar un prefijo" << endl;
            return;
        }
        vector<string> palabras = trie.get_words_with_prefix(args[0]);
        cout << "Palabras con prefijo '" << args[0] << "': " << palabras.size() << endl;
        for (const auto& palabra : palabras) {
            cout << "  - " << palabra << endl;
        }
    }
    else if (comando == "/all") {
        vector<string> palabras = trie.get_all_words();
        cout << "Total de palabras: " << palabras.size() << endl;
        for (const auto& palabra : palabras) {
            cout << "  - " << palabra << endl;
        }
    }
    else if (comando == "/clear") {
        trie.clear();
        cout << "Trie limpiado" << endl;
    }
    else if (comando == "/help") {
        show_help();
    }
    else {
        cout << "Comando desconocido. Usa /help para ver los comandos disponibles" << endl;
    }
}

void process_radix_command(RadixTrie& trie, const string& comando, const vector<string>& args) {
    if (comando == "/insert") {
        if (args.empty()) {
            cout << "Error: Debes especificar al menos una palabra" << endl;
            return;
        }
        for (const auto& palabra : args) {
            trie.insert(palabra);
            cout << "Insertada: " << palabra << endl;
        }
    }
    else if (comando == "/search") {
        if (args.empty()) {
            cout << "Error: Debes especificar una palabra" << endl;
            return;
        }
        bool encontrada = trie.search_bool(args[0]);
        cout << "Palabra '" << args[0] << "': " << (encontrada ? "ENCONTRADA" : "NO ENCONTRADA") << endl;
    }
    else if (comando == "/prefix") {
        if (args.empty()) {
            cout << "Error: Debes especificar un prefijo" << endl;
            return;
        }
        auto palabras_con_pos = trie.autocomplete(args[0]);
        cout << "Palabras con prefijo '" << args[0] << "': " << palabras_con_pos.size() << endl;
        for (const auto& par : palabras_con_pos) {
            cout << "  - " << par.first << endl;
        }
    }
    else if (comando == "/all") {
        auto palabras_con_pos = trie.get_words();
        cout << "Total de palabras: " << palabras_con_pos.size() << endl;
        for (const auto& par : palabras_con_pos) {
            cout << "  - " << par.first << endl;
        }
    }
    else if (comando == "/clear") {
        trie.clear();
        cout << "Trie limpiado" << endl;
    }
    else if (comando == "/help") {
        show_help();
    }
    else {
        cout << "Comando desconocido. Usa /help para ver los comandos disponibles" << endl;
    }
}

int main() {
    cout << "=== Selector de tipo de Trie ===" << endl;
    cout << "Elige el tipo de Trie que quieres utilizar:" << endl;
    cout << "  1 - NaiveTrie (Trie basico)" << endl;
    cout << "  2 - RadixTrie (Radix Tree)" << endl;
    cout << "Introduce la opcion (1/2): ";
    
    int opcion;
    cin >> opcion;
    cin.ignore(); // Limpiar el buffer
    
    if (opcion == 1) {
        cout << "\n=== Has seleccionado NaiveTrie ===" << endl;
        NaiveTrie trie;
        show_help();
        
        string linea;
        while (true) {
            cout << "NaiveTrie> ";
            if (!getline(cin, linea)) break;
            
            if (linea.empty()) continue;
            
            // Parsear comando y argumentos
            istringstream iss(linea);
            string comando;
            iss >> comando;
            
            if (comando == "/quit") {
                cout << "Saliendo..." << endl;
                break;
            }
            
            vector<string> args;
            string arg;
            while (iss >> arg) {
                args.push_back(arg);
            }
            
            process_naive_command(trie, comando, args);
        }
    }
    else if (opcion == 2) {
        cout << "\n=== Has seleccionado RadixTrie ===" << endl;
        RadixTrie trie;
        show_help();
        
        string linea;
        while (true) {
            cout << "RadixTrie> ";
            if (!getline(cin, linea)) break;
            
            if (linea.empty()) continue;
            
            // Parsear comando y argumentos
            istringstream iss(linea);
            string comando;
            iss >> comando;
            
            if (comando == "/quit") {
                cout << "Saliendo..." << endl;
                break;
            }
            
            vector<string> args;
            string arg;
            while (iss >> arg) {
                args.push_back(arg);
            }
            
            process_radix_command(trie, comando, args);
        }
    }
    else {
        cout << "Opcion no valida." << endl;
        return 1;
    }
    
    return 0;
}
