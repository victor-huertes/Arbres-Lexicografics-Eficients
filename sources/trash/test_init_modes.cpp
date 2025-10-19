#include <iostream>
#include <string>
#include "sources/include/naive.h"
#include "sources/include/radix.h"

using namespace std;

int main() {
    string test_text = "Hello world!\nThis is a test.\nHello again!";
    
    cout << "=== Test Texto ===" << endl;
    cout << test_text << endl << endl;
    
    // Test NaiveTrie - Modo 0
    cout << "=== NaiveTrie - Modo 0 (palabras con posición) ===" << endl;
    NaiveTrie naive_mode0;
    naive_mode0.init(test_text, 0);
    
    vector<int> positions = naive_mode0.search_positions("hello");
    cout << "Búsqueda 'hello': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = naive_mode0.search_positions("world");
    cout << "Búsqueda 'world': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    cout << endl;
    
    // Test NaiveTrie - Modo 1
    cout << "=== NaiveTrie - Modo 1 (líneas con número de línea) ===" << endl;
    NaiveTrie naive_mode1;
    naive_mode1.init(test_text, 1);
    
    positions = naive_mode1.search_positions("hello");
    cout << "Búsqueda 'hello': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en líneas: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = naive_mode1.search_positions("this is a test");
    cout << "Búsqueda 'this is a test': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en líneas: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = naive_mode1.search_positions("hello again");
    cout << "Búsqueda 'hello again': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en líneas: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    cout << endl;
    
    // Test RadixTrie - Modo 0
    cout << "=== RadixTrie - Modo 0 (palabras con posición) ===" << endl;
    RadixTrie radix_mode0;
    radix_mode0.init(test_text, 0);
    
    positions = radix_mode0.search_positions("hello");
    cout << "Búsqueda 'hello': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = radix_mode0.search_positions("test");
    cout << "Búsqueda 'test': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    cout << endl;
    
    // Test RadixTrie - Modo 1
    cout << "=== RadixTrie - Modo 1 (líneas con número de línea) ===" << endl;
    RadixTrie radix_mode1;
    radix_mode1.init(test_text, 1);
    
    positions = radix_mode1.search_positions("hello");
    cout << "Búsqueda 'hello': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en líneas: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = radix_mode1.search_positions("hello again");
    cout << "Búsqueda 'hello again': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en líneas: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    cout << endl;
    
    // Test NaiveTrie - Modo 2
    cout << "=== NaiveTrie - Modo 2 (todos los substrings 1-20) ===" << endl;
    string test_short = "Hello World";
    NaiveTrie naive_mode2;
    naive_mode2.init(test_short, 2);
    
    // Buscar algunos substrings
    positions = naive_mode2.search_positions("hello");
    cout << "Búsqueda 'hello': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = naive_mode2.search_positions("llo wo");
    cout << "Búsqueda 'llo wo': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = naive_mode2.search_positions("orld");
    cout << "Búsqueda 'orld': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    cout << endl;
    
    // Test RadixTrie - Modo 2
    cout << "=== RadixTrie - Modo 2 (todos los substrings 1-20) ===" << endl;
    RadixTrie radix_mode2;
    radix_mode2.init(test_short, 2);
    
    positions = radix_mode2.search_positions("hello");
    cout << "Búsqueda 'hello': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = radix_mode2.search_positions("wo");
    cout << "Búsqueda 'wo': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    positions = radix_mode2.search_positions("world");
    cout << "Búsqueda 'world': ";
    if (positions.empty()) {
        cout << "No encontrado" << endl;
    } else {
        cout << "Encontrado en posiciones: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    return 0;
}
