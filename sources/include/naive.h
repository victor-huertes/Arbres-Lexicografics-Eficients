#ifndef NAIVE_H
#define NAIVE_H

#include "trie.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

/**
 * @brief Implementació d'un Trie Naive (arbre de prefixos bàsic) per emmagatzemar strings
 * 
 * Un Trie Naive és la implementació més bàsica d'una estructura de dades en forma 
 * d'arbre que permet emmagatzemar un conjunt dinàmic de strings. Ofereix operacions 
 * eficients d'inserció, cerca i eliminació de paraules.
 */
class NaiveTrie : public Trie {
private:
    /**
     * @brief Node intern del Trie
     */
    struct TrieNode {
        //mapa que guarda los posibles hijos del trie 
        //unordered_map<char, unique_ptr<TrieNode>> children;
        vector<TrieNode*> children;
        vector<int> index;
        bool end_of_word;
        //TrieNode() : index(-1){}
    };
    
    unique_ptr<TrieNode> root;
    
    /**
     * @brief Funció auxiliar per recollir totes les paraules amb un prefix
     * @param node Node actual
     * @param prefix Prefix construït fins ara
     * @param results Vector per emmagatzemar els resultats
     */
    void collect_words_with_prefix(TrieNode* node, const string& prefix, vector<string>& results) const;
    
    /**
     * @brief Funció auxiliar per recollir paraules amb les seves posicions
     * @param node Node actual
     * @param prefix Prefix construït fins ara
     * @param results Vector per emmagatzemar els resultats (paraula, posició)
     */
    void collect_words_with_positions(TrieNode* node, const string& prefix, vector<pair<string, int>>& results) const;
    
    /**
     * @brief Funció auxiliar per recollir posicions d'un subarbre
     * @param node Node actual
     * @param positions Vector per emmagatzemar les posicions
     */
    void collect_positions_from_subtree(TrieNode* node, vector<int>& positions) const;

public:
    /**
     * @brief Constructor del NaiveTrie
     */
    NaiveTrie();
    
    /**
     * @brief Destructor del NaiveTrie
     */
    ~NaiveTrie() = default;
    
    /**
     * @brief Inicialitza el trie amb un text (insereix tots els sufixos del text)
     * @param text Text a processar
     */
    void init(const string& text);
    
    /**
     * @brief Insereix una paraula i la seva posició al NaiveTrie
     * @param word Paraula a inserir
     * @param position Posició de la paraula al text original
     */
    void insert(const string& word, int position);
    
    /**
     * @brief Cerca una paraula i retorna les posicions on apareix
     * @param word Paraula a cercar
     * @return Vector amb les posicions on apareix la paraula
     */
    vector<int> search_positions(const string& word) const;
    
    /**
     * @brief Troba paraules que comencen amb un prefix
     * @param prefix Prefix a buscar
     * @return Vector amb les posicions de paraules que comencen amb el prefix
     */
    vector<int> starts_with_positions(const string& prefix) const;
    
    /**
     * @brief Retorna totes les paraules que empiecen per un prefix
     * @param prefix Prefix a buscar
     * @return Vector de parells (paraula, posició) que comencen amb el prefix
     */
    vector<pair<string, int>> autocomplete(const string& prefix) const;
    
    /**
     * @brief Obté totes les paraules del trie
     * @return Vector de parells (paraula, posició)
     */
    vector<pair<string, int>> get_words() const;
    
    // Métodos legacy para compatibilidad con Trie base
    void insert(const string& word) override;
    bool search(const string& word) const override;
    bool starts_with(const string& prefix) const override;
    
    /**
     * @brief Retorna totes les paraules que comencin amb el prefix donat
     * @param prefix Prefix a buscar
     * @return Vector amb totes les paraules que comencin amb el prefix
     */
    vector<string> get_words_with_prefix(const string& prefix) const override;
    
    /**
     * @brief Retorna totes les paraules emmagatzemades al NaiveTrie
     * @return Vector amb totes les paraules
     */
    vector<string> get_all_words() const override;
    
    /**
     * @brief Comprova si el NaiveTrie està buit
     * @return true si no hi ha paraules emmagatzemades
     */
    bool empty() const override;
    
    /**
     * @brief Neteja tot el contingut del NaiveTrie
     */
    void clear() override;
};

#endif // NAIVE_H