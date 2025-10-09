#ifndef TRIE_H
#define TRIE_H

#include <memory>
#include <string>
#include <vector>
using namespace std;

/**
 * @brief Classe base abstracta per als diferents tipus de Trie
 * 
 * Aquesta classe defineix la interfície comuna per a totes les implementacions
 * de Trie (NaiveTrie, PatriciaTrie, RadixTrie). Utilitza polimorfisme per
 * permetre l'ús intercambiable de diferents implementacions.
 */
class Trie {
public:
    /**
     * @brief Destructor virtual
     */
    virtual ~Trie() = default;
    
    /**
     * @brief Insereix una paraula al Trie
     * @param word Paraula a inserir
     */
    virtual void insert(const string& word) = 0;
    
    /**
     * @brief Cerca si una paraula existeix al Trie
     * @param word Paraula a cercar
     * @return true si la paraula existeix, false altrament
     */
    virtual bool search(const string& word) const = 0;
    
    /**
     * @brief Comprova si hi ha paraules que comencin amb el prefix donat
     * @param prefix Prefix a comprovar
     * @return true si existeix almenys una paraula amb aquest prefix
     */
    virtual bool starts_with(const string& prefix) const = 0;
    
    /**
     * @brief Retorna totes les paraules que comencin amb el prefix donat
     * @param prefix Prefix a buscar
     * @return Vector amb totes les paraules que comencin amb el prefix
     */
    virtual vector<string> get_words_with_prefix(const string& prefix) const = 0;
    
    /**
     * @brief Retorna totes les paraules emmagatzemades al Trie
     * @return Vector amb totes les paraules
     */
    virtual vector<string> get_all_words() const = 0;
    
    /**
     * @brief Comprova si el Trie està buit
     * @return true si no hi ha paraules emmagatzemades
     */
    virtual bool empty() const = 0;
    
    /**
     * @brief Neteja tot el contingut del Trie
     */
    virtual void clear() = 0;
};

#endif // TRIE_H
