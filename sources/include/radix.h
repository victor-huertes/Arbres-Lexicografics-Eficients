#ifndef RADIX_H
#define RADIX_H

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>
using namespace std;

/**
 * @brief Implementació d'un Radix Trie (Trie compacte)
 */
class RadixTrie {
private:
    /**
     * @brief Node intern del Radix Trie.
     * label: El substring comprimit.
     * children: Map de char (primer caràcter de l'etiqueta del fill) a punter al fill.
     * positions: Llista de posicions on comença la clau (paraula/sufix).
     * is_end_of_key: Marca si aquest node representa el final d'una clau vàlida.
     */
    struct RadixNode {
        string label;
        // Utilitzem unique_ptr per a la gestió automàtica de la memòria
        unordered_map<char, unique_ptr<RadixNode>> children; 
        vector<int> positions;
        bool is_end_of_key;

        // Constructor
        RadixNode(const string& l = "") : label(l), is_end_of_key(false) {}
    };
    
    unique_ptr<RadixNode> root;
    
    // Funcions auxiliars privades per a recorreguts:
    void collect_words_recursive(RadixNode* node, string current_prefix, vector<pair<string, int>>& results) const;
    void collect_positions_recursive(RadixNode* node, vector<int>& positions) const;

public:
    // Constructor i Destructor
    RadixTrie();
    ~RadixTrie() = default; // unique_ptr s'encarrega de l'alliberament

    // Funcions d'indexació
    void init(const string& text);
    void insert(const string& word, int position);
    
    // Funcions de cerca (Declarades com a 'const' per al Wrapper i bones pràctiques)
    vector<int> search(const string& word) const; 
    vector<int> starts_with(const string& prefix) const;
    vector<pair<string, int>> autocomplete(const string& prefix) const;
    vector<pair<string, int>> get_words() const;

    // Funcions de compatibilitat (si són necessàries a 'main.cpp')
    void insert(const string& word) { insert(word, -1); } // Posició fictícia
    bool search_bool(const string& word) const { return !search(word).empty(); }
    bool starts_with_bool(const string& prefix) const { return !starts_with(prefix).empty(); }

    /**
     * @brief Neteja el trie, eliminant tots els nodes.
     */
    void clear(); 

    /**
     * @brief Comprova si el trie conté alguna paraula.
     * @return true si el trie està buit.
     */
    bool empty() const;
    
    // Si RadixTrie ha d'implementar la interfície 'Trie', haureu d'afegir:
    /*
    bool search(const string& word) const override;
    bool starts_with(const string& prefix) const override;
    vector<string> get_words_with_prefix(const string& prefix) const override;
    vector<string> get_all_words() const override;
    // ...etc
    */
};

// **Nota:** No incloc el 'RadixTrieWrapper' aquí, ja que normalment es defineix a 'main.cpp'.

#endif // RADIX_H
