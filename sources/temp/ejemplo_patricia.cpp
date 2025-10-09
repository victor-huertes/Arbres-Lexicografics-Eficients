// Ejemplo simple de uso del Patricia Trie
// Este programa muestra cómo usar el header patricia.h

#include "../include/patricia.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    cout << "=== Ejemplo de Uso del Patricia Trie ===" << endl
         << endl;

    // Crear una instancia del Patricia Trie
    PatriciaTrie diccionario;

    // Lista de palabras para insertar
    vector<string> palabras = {
        "arbol",
        "algoritmo",
        "patricia",
        "trie",
        "estructura",
        "datos"};

    // Insertar palabras
    cout << "Insertando palabras..." << endl;
    for (const auto &palabra : palabras)
    {
        diccionario.insert(palabra);
        cout << "  ✓ Insertada: " << palabra << endl;
    }
    cout << endl;

    // Verificar si el trie está vacío
    cout << "¿El trie está vacío? "
         << (diccionario.isEmpty() ? "Sí" : "No") << endl
         << endl;

    // Buscar palabras que existen
    cout << "Buscando palabras que EXISTEN:" << endl;
    vector<string> buscar_existen = {"arbol", "patricia", "datos"};
    for (const auto &palabra : buscar_existen)
    {
        bool encontrada = diccionario.search(palabra);
        cout << "  " << palabra << ": "
             << (encontrada ? "✓ Encontrada" : "✗ No encontrada") << endl;
    }
    cout << endl;

    // Buscar palabras que NO existen
    cout << "Buscando palabras que NO EXISTEN:" << endl;
    vector<string> buscar_no_existen = {"arb", "computer", "test"};
    for (const auto &palabra : buscar_no_existen)
    {
        bool encontrada = diccionario.search(palabra);
        cout << "  " << palabra << ": "
             << (encontrada ? "✓ Encontrada" : "✗ No encontrada") << endl;
    }
    cout << endl;

    // Intentar insertar una palabra que ya existe
    cout << "Intentando insertar palabra duplicada..." << endl;
    diccionario.insert("arbol");
    cout << endl;

    // Mostrar límites del trie
    cout << "Límites del Patricia Trie:" << endl;
    cout << "  - Máximo de bits: " << PatriciaTrie::getMaxBits() << endl;
    cout << "  - Longitud máxima: " << PatriciaTrie::getMaxLength()
         << " caracteres" << endl
         << endl;

    // Limpiar el trie
    cout << "Limpiando el trie..." << endl;
    diccionario.makeEmpty();
    cout << "¿El trie está vacío ahora? "
         << (diccionario.isEmpty() ? "Sí" : "No") << endl;

    cout << endl
         << "=== Fin del ejemplo ===" << endl;

    return 0;
}
