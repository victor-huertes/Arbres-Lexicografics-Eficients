#ifndef TRIE_H
#define TRIE_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief Implementació d'un Trie (arbre de prefixos) per emmagatzemar strings
 * 
 * Un Trie és una estructura de dades en forma d'arbre que permet emmagatzemar
 * un conjunt dinàmic de strings. Ofereix operacions eficients d'inserció,
 * cerca i eliminació de paraules.
 */
class Trie {
private:
    /**
     * @brief Node intern del Trie
     */
    struct TrieNode {
        std::unordered_map<char, std::unique_ptr<TrieNode>> children;
        bool is_end_of_word;
        
        TrieNode() : is_end_of_word(false) {}
    };
    
    std::unique_ptr<TrieNode> root;
    
    /**
     * @brief Funció auxiliar per eliminar paraules recursivament
     * @param node Node actual
     * @param word Paraula a eliminar
     * @param index Índex actual de la paraula
     * @return true si el node pot ser eliminat
     */
    bool remove_helper(TrieNode* node, const std::string& word, size_t index);
    
    /**
     * @brief Funció auxiliar per recollir totes les paraules amb un prefix
     * @param node Node actual
     * @param prefix Prefix construït fins ara
     * @param results Vector per emmagatzemar els resultats
     */
    void collect_words_with_prefix(TrieNode* node, const std::string& prefix, std::vector<std::string>& results) const;

public:
    /**
     * @brief Constructor del Trie
     */
    Trie();
    
    /**
     * @brief Destructor del Trie
     */
    ~Trie() = default;
    
    /**
     * @brief Insereix una paraula al Trie
     * @param word Paraula a inserir
     */
    void insert(const std::string& word);
    
    /**
     * @brief Cerca si una paraula existeix al Trie
     * @param word Paraula a cercar
     * @return true si la paraula existeix, false altrament
     */
    bool search(const std::string& word) const;
    
    /**
     * @brief Comprova si hi ha paraules que comencin amb el prefix donat
     * @param prefix Prefix a comprovar
     * @return true si existeix almenys una paraula amb aquest prefix
     */
    bool starts_with(const std::string& prefix) const;
    
    /**
     * @brief Elimina una paraula del Trie
     * @param word Paraula a eliminar
     * @return true si la paraula es va eliminar correctament, false si no existia
     */
    bool remove(const std::string& word);
    
    /**
     * @brief Retorna totes les paraules que comencin amb el prefix donat
     * @param prefix Prefix a buscar
     * @return Vector amb totes les paraules que comencin amb el prefix
     */
    std::vector<std::string> get_words_with_prefix(const std::string& prefix) const;
    
    /**
     * @brief Retorna totes les paraules emmagatzemades al Trie
     * @return Vector amb totes les paraules
     */
    std::vector<std::string> get_all_words() const;
    
    /**
     * @brief Comprova si el Trie està buit
     * @return true si no hi ha paraules emmagatzemades
     */
    bool empty() const;
    
    /**
     * @brief Neteja tot el contingut del Trie
     */
    void clear();
};

#endif // TRIE_H