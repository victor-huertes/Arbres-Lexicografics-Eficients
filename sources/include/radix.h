#ifndef RADIX_H
#define RADIX_H

#include <string>
#include <vector>
#include <utility>
using namespace std;

/**
 * @brief Implementación de un Radix Trie
 * 
 * Un Radix Trie (también conocido como radix tree o compact prefix tree) es una 
 * estructura de datos optimizada para almacenar strings que comprime ramas 
 * lineales en nodos individuales.
 */
class RadixTrie {
private:
    /**
     * @brief Nodo interno del Radix Trie
     */
    struct RadixNode {
        // TODO: Definir estructura del nodo
        // - Etiqueta/substring almacenado
        // - Hijos del nodo
        // - Posiciones donde aparece la palabra
        // - Marcador de fin de palabra
    };
    
    RadixNode* root;
    
    // TODO: Añadir funciones auxiliares privadas aquí
    
public:
    /**
     * @brief Constructor del RadixTrie
     */
    RadixTrie();
    
    /**
     * @brief Destructor del RadixTrie
     */
    ~RadixTrie();
    
    /**
     * @brief Inicializa el trie con un texto (inserta todos los sufijos del texto)
     * @param text Texto a procesar
     * 
     * TODO: Implementar
     * Ejemplo:
     *   texto = "Hola"
     *   (palabra, posicion)[] = [("Hola", 0), ("ola", 1), ("la", 2), ("a", 3)]
     */
    void init(const string& text);
    
    /**
     * @brief Inserta una palabra y su posición en el trie
     * @param word Palabra a insertar
     * @param position Posición de la palabra en el texto original
     * 
     * TODO: Implementar
     */
    void insert(const string& word, int position);
    
    /**
     * @brief Busca una palabra y devuelve las posiciones donde aparece
     * @param word Palabra a buscar
     * @return Vector con las posiciones donde aparece la palabra
     * 
     * TODO: Implementar
     */
    vector<int> search(const string& word);
    
    /**
     * @brief Encuentra palabras que comienzan con un prefijo
     * @param prefix Prefijo a buscar
     * @return Vector con las posiciones de palabras que comienzan con el prefijo
     * 
     * TODO: Implementar
     */
    vector<int> starts_with(const string& prefix);
    
    /**
     * @brief Devuelve todas las palabras que empiezan por un prefijo
     * @param prefix Prefijo a buscar
     * @return Vector de pares (palabra, posición) que comienzan con el prefijo
     * 
     * TODO: Implementar
     * Es como un starts_with(...) pero busca la palabra en cada texto
     */
    vector<pair<string, int>> autocomplete(const string& prefix);
    
    /**
     * @brief Obtiene todas las palabras del trie
     * @return Vector de pares (palabra, posición)
     * 
     * TODO: Implementar
     */
    vector<pair<string, int>> get_words();
};

#endif // RADIX_H