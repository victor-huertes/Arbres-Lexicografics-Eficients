# Árboles Lexicográficos Eficientes

Implementación de diferentes estructuras de tries para almacenar y buscar strings eficientemente.

## Estructura del Proyecto

```
sources/
├── include/
│   ├── trie.h          # Trie Polimórfico
│   ├── naive.h         # Naive Trie
│   ├── patricia.h      # Patricia Trie
│   └── radix.h         # Radix Trie
└── src/
    ├── trie.cpp
    ├── naive.cpp
    ├── patricia.cpp
    ├── radix.cpp
    └── main.cpp
```

## API de las Clases

Todas las implementaciones (Trie, Patricia, Radix) siguen la misma interfaz:

```cpp
// Inicializar el trie con un texto (inserta todos los sufijos del texo)
// Ejemplo:
//   texto = "Hola"
//   (palabra, posicion)[] = [("Hola", 0), ("ola", 1), ("la", 2), ("a", 3)]
void init(string text);

// Insertar una palabra y su posición en el texto
void insert(string word, int position);

// Buscar palabra y devolver posiciones donde aparece
vector<int> search(string word);

// Encontrar palabras que comienzan con un prefijo
vector<int> starts_with(string prefix);

// Devuelve todas las palabras que empiecen por un prefijo
// Es como un starts_with(...) pero busca la palabra en cada texto
vector<(string, int)> autocomplete(string prefix);

// Obtener todas las palabras del trie
vector<(string, int)> get_words();
```

## Compilación

```bash
make run
```