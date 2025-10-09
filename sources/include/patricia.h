#ifndef PATRICIA_H
#define PATRICIA_H

#include <string>
#include <vector>
#include <utility>
using namespace std;

/**
 * @brief Nodo del Patricia Trie
 * 
 * Almacena:
 * - number: Posición del bit donde se produce la ramificación
 * - data: La cadena almacenada en el nodo
 * - leftChild/rightChild: Hijos izquierdo y derecho
 * - positions: Posiciones donde aparece la palabra en el texto
 */
class PatriciaTrieNode {
public:
    int number;
    string data;
    PatriciaTrieNode* leftChild;
    PatriciaTrieNode* rightChild;
    vector<int> positions;

    PatriciaTrieNode();
};

/**
 * @brief Implementación de un Patricia Trie (PATRICIA - Practical Algorithm 
 * to Retrieve Information Coded in Alphanumeric)
 * 
 * Un Patricia Trie es una estructura de datos eficiente para almacenar strings
 * que comprime nodos con un solo hijo. A diferencia de un Trie estándar,
 * el Patricia Trie crea nodos solo en los puntos de ramificación.
 * 
 * Esta implementación trabaja con strings ASCII mediante extracción directa
 * de bits de los caracteres, sin necesidad de conversión a binario.
 * 
 * Características:
 * - Inserción eficiente: O(k) donde k es la longitud de la cadena
 * - Búsqueda eficiente: O(k) donde k es la longitud de la cadena
 * - Extracción de bits: O(1) por bit
 * - Capacidad máxima: 32 caracteres por cadena (256 bits)
 */
class PatriciaTrie {
private:
    PatriciaTrieNode* root;
    static const int MaxBits = 256;  // 32 caracteres * 8 bits

    /**
     * @brief Extrae el i-ésimo bit de una cadena
     * 
     * Cada carácter ASCII se representa con 8 bits. Los bits se numeran
     * secuencialmente a través de todos los caracteres de la cadena.
     * 
     * Ejemplo para "AB":
     * - 'A' (ASCII 65 = 01000001): bits 1-8
     * - 'B' (ASCII 66 = 01000010): bits 9-16
     * 
     * @param str La cadena de la cual extraer el bit
     * @param i Posición del bit (1-indexed)
     * @return true si el bit es 1, false si es 0 o está fuera de rango
     */
    bool bit(const string& str, int i);

    /**
     * @brief Función auxiliar de búsqueda recursiva
     * 
     * Recorre el árbol siguiendo los bits de la cadena buscada hasta
     * encontrar un nodo candidato.
     * 
     * @param t Nodo actual
     * @param k Cadena a buscar
     * @return Puntero al nodo encontrado o nullptr
     */
    PatriciaTrieNode* search(PatriciaTrieNode* t, const string& k);

    /**
     * @brief Función auxiliar de inserción recursiva
     * 
     * Inserta una nueva cadena en el árbol, creando nodos de ramificación
     * solo donde las cadenas difieren.
     * 
     * @param t Nodo actual
     * @param element Cadena a insertar
     * @return Puntero a la raíz del subárbol
     */
    PatriciaTrieNode* insert(PatriciaTrieNode* t, const string& element);
    
    /**
     * @brief Función auxiliar de inserción con posición
     * @param t Nodo actual
     * @param element Cadena a insertar
     * @param position Posición en el texto
     * @return Puntero a la raíz del subárbol
     */
    PatriciaTrieNode* insert(PatriciaTrieNode* t, const string& element, int position);
    
    /**
     * @brief Función auxiliar para recolectar posiciones del subárbol
     * @param node Nodo actual
     * @param positions Vector para almacenar posiciones
     */
    void collect_positions(PatriciaTrieNode* node, vector<int>& positions);
    
    /**
     * @brief Función auxiliar para recolectar palabras con posiciones
     * @param node Nodo actual
     * @param results Vector para almacenar pares (palabra, posición)
     */
    void collect_words(PatriciaTrieNode* node, vector<pair<string, int>>& results);

public:
    /**
     * @brief Constructor por defecto
     * 
     * Inicializa un Patricia Trie vacío.
     */
    PatriciaTrie();

    /**
     * @brief Destructor
     * 
     * Libera toda la memoria utilizada por el árbol.
     */
    ~PatriciaTrie();

    /**
     * @brief Verifica si el árbol está vacío
     * 
     * @return true si el árbol no contiene elementos, false en caso contrario
     */
    bool isEmpty() const;

    /**
     * @brief Vacía el árbol eliminando todos los elementos
     * 
     * Libera la memoria de todos los nodos y deja el árbol vacío.
     */
    void makeEmpty();

    /**
     * @brief Busca una cadena en el árbol
     * 
     * Realiza una búsqueda exacta de la cadena proporcionada.
     * 
     * @param k Cadena a buscar
     * @return true si la cadena existe en el árbol, false en caso contrario
     */
    bool search(const string& k);

    /**
     * @brief Inserta una cadena en el árbol
     * 
     * Si la cadena ya existe, no se realiza ninguna acción.
     * La cadena no puede estar vacía ni exceder los 32 caracteres.
     * 
     * @param element Cadena a insertar
     */
    void insert(const string& element);
    
    /**
     * @brief Inicializa el trie con un texto (inserta todos los sufijos del texto)
     * @param text Texto a procesar
     */
    void init(const string& text);
    
    /**
     * @brief Inserta una palabra y su posición en el trie
     * @param word Palabra a insertar
     * @param position Posición de la palabra en el texto original
     */
    void insert(const string& word, int position);
    
    /**
     * @brief Busca una palabra y devuelve las posiciones donde aparece
     * @param word Palabra a buscar
     * @return Vector con las posiciones donde aparece la palabra
     */
    vector<int> search_positions(const string& word);
    
    /**
     * @brief Encuentra palabras que comienzan con un prefijo
     * @param prefix Prefijo a buscar
     * @return Vector con las posiciones de palabras que comienzan con el prefijo
     */
    vector<int> starts_with_positions(const string& prefix);
    
    /**
     * @brief Devuelve todas las palabras que empiezan por un prefijo
     * @param prefix Prefijo a buscar
     * @return Vector de pares (palabra, posición) que comienzan con el prefijo
     */
    vector<pair<string, int>> autocomplete(const string& prefix);
    
    /**
     * @brief Obtiene todas las palabras del trie
     * @return Vector de pares (palabra, posición)
     */
    vector<pair<string, int>> get_words();

    /**
     * @brief Obtiene el número máximo de bits soportados
     * 
     * @return Número máximo de bits (256 = 32 caracteres * 8 bits)
     */
    static int getMaxBits() { return MaxBits; }

    /**
     * @brief Obtiene la longitud máxima de cadena soportada
     * 
     * @return Número máximo de caracteres (32)
     */
    static int getMaxLength() { return MaxBits / 8; }
};

#endif // PATRICIA_H
