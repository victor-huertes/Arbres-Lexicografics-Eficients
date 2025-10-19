#include "../include/radix.h"

// Constructor
RadixTrie::RadixTrie() {
    // TODO: Inicializar el nodo raíz
    root = nullptr;
}

// Destructor
RadixTrie::~RadixTrie() {
    // TODO: Liberar memoria de todos los nodos
}

// Inicializar el trie con un texto (insertar todos los sufijos)
void RadixTrie::init(const string& text) {
    // TODO: Implementar
    // Para cada posición i en el texto:
    //   - Extraer el sufijo desde i hasta el final
    //   - Insertar el sufijo con su posición i
}

// Insertar una palabra y su posición
void RadixTrie::insert(const string& word, int position) {
    // TODO: Implementar
    // Algoritmo:
    // 1. Empezar desde la raíz
    // 2. Para cada nodo hijo:
    //    a. Comparar el prefijo común con la etiqueta del hijo
    //    b. Si hay coincidencia total: continuar al hijo
    //    c. Si hay coincidencia parcial: dividir el nodo
    //    d. Si no hay coincidencia: crear nuevo hijo
    // 3. Marcar el nodo final y guardar la posición
}

// Buscar palabra y devolver posiciones
vector<int> RadixTrie::search(const string& word) {
    vector<int> positions;
    
    // TODO: Implementar
    // Algoritmo:
    // 1. Navegar por el árbol siguiendo los prefijos
    // 2. Si se encuentra la palabra exacta, devolver sus posiciones
    // 3. Si no se encuentra, devolver vector vacío
    
    return positions;
}

// Encontrar palabras que comienzan con un prefijo (devuelve posiciones)
vector<int> RadixTrie::starts_with(const string& prefix) {
    vector<int> positions;
    
    // TODO: Implementar
    // Algoritmo:
    // 1. Navegar hasta el nodo que contiene el prefijo
    // 2. Recolectar todas las posiciones del subárbol
    // 3. Devolver el vector de posiciones
    
    return positions;
}

// Autocomplete: devuelve todas las palabras que empiezan por un prefijo
vector<pair<string, int>> RadixTrie::autocomplete(const string& prefix) {
    vector<pair<string, int>> results;
    
    // TODO: Implementar
    // Algoritmo:
    // 1. Navegar hasta el nodo que contiene el prefijo
    // 2. Recolectar todas las palabras (con sus posiciones) del subárbol
    // 3. Devolver el vector de pares (palabra, posición)
    
    return results;
}

// Obtener todas las palabras del trie
vector<pair<string, int>> RadixTrie::get_words() {
    // TODO: Implementar
    // Sugerencia: usar autocomplete con prefijo vacío
    return autocomplete("");
}