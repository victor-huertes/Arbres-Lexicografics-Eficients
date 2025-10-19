#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include "sources/include/naive.h"
#include "sources/include/radix.h"

using namespace std;
using namespace std::chrono;

int main() {
    // Textos de prueba de diferentes tamaños
    vector<pair<string, string>> tests = {
        {"Pequeño (100 chars)", string(100, 'a')},
        {"Mediano (1000 chars)", string(1000, 'a')},
        {"Grande (5000 chars)", string(5000, 'a')},
        {"Muy Grande (20000 chars)", string(20000, 'a')}
    };
    
    cout << "=== Benchmark Modo 2 Optimizado ===" << endl;
    cout << fixed << setprecision(2);
    
    for (const auto& [name, text] : tests) {
        cout << "\n" << name << ":" << endl;
        
        // NaiveTrie
        {
            NaiveTrie trie;
            auto start = high_resolution_clock::now();
            trie.init(text, 2);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            
            size_t memory = trie.get_memory_usage();
            cout << "  NaiveTrie: " << duration.count() << " ms, "
                 << (memory / 1024.0) << " KB" << endl;
        }
        
        // RadixTrie
        {
            RadixTrie trie;
            auto start = high_resolution_clock::now();
            trie.init(text, 2);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            
            size_t memory = trie.get_memory_usage();
            cout << "  RadixTrie: " << duration.count() << " ms, "
                 << (memory / 1024.0) << " KB" << endl;
        }
    }
    
    // Prueba con texto real (lorem ipsum)
    cout << "\n=== Texto Real (lorem_ipsum.txt) ===" << endl;
    
    ifstream file("input/lorem_ipsum.txt");
    if (file.is_open()) {
        stringstream buffer;
        buffer << file.rdbuf();
        string text = buffer.str();
        
        cout << "Tamaño: " << text.length() << " caracteres" << endl;
        
        // NaiveTrie
        {
            NaiveTrie trie;
            auto start = high_resolution_clock::now();
            trie.init(text, 2);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            
            size_t memory = trie.get_memory_usage();
            cout << "  NaiveTrie: " << duration.count() << " ms, "
                 << (memory / 1024.0) << " KB" << endl;
        }
        
        // RadixTrie
        {
            RadixTrie trie;
            auto start = high_resolution_clock::now();
            trie.init(text, 2);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            
            size_t memory = trie.get_memory_usage();
            cout << "  RadixTrie: " << duration.count() << " ms, "
                 << (memory / 1024.0) << " KB" << endl;
        }
    }
    
    return 0;
}
