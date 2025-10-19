# Resumen de Modificaciones - Función init() para Arbres Lexicogràfics

## Cambios Realizados

### 1. Modificación de NaiveTrie (naive.h y naive.cpp)

#### Header (naive.h)
- Se actualizó la firma de la función `init()` para incluir un parámetro `mode` con valor por defecto 0:
  ```cpp
  void init(const string &text, int mode = 0);
  ```

#### Implementación (naive.cpp)
- Se añadieron los includes necesarios: `<sstream>` y `<cctype>`
- Se reimplementó la función `init()` con dos modos:

**Modo 0 (por defecto)**: Inserta palabras individuales del texto
- Separa palabras usando caracteres no alfanuméricos como delimitadores
- Convierte cada palabra a minúsculas
- Almacena la posición en el texto donde comienza cada palabra
- Similar al procesamiento que hacía `loadDataset()` en experiments.cpp

**Modo 1**: Inserta palabras de cada línea con número de línea
- Procesa el texto línea por línea
- Extrae las palabras individuales de cada línea
- Convierte a minúsculas automáticamente
- Almacena el número de línea (0, 1, 2...) como posición para cada palabra
- Útil para saber en qué líneas aparece una palabra específica

**Modo 2**: Inserta todos los substrings de longitud 1 a 20
- Genera todos los posibles substrings del texto
- Para cada posición inicial, crea substrings de longitud 1 hasta 20 caracteres
- Convierte a minúsculas automáticamente
- Almacena la posición inicial del substring en el texto
- Útil para búsquedas de subcadenas arbitrarias (búsqueda de texto completo)

### 2. Modificación de RadixTrie (radix.h y radix.cpp)

#### Header (radix.h)
- Se actualizó la firma de la función `init()` de forma idéntica:
  ```cpp
  void init(const string &text, int mode = 0);
  ```

#### Implementación (radix.cpp)
- Se añadieron los mismos includes: `<sstream>` y `<cctype>`
- Se implementó la misma lógica para ambos modos que en NaiveTrie

### 3. Modificación de experiments.cpp

#### Nuevas funciones
- Se añadió `loadTextFile()`: Carga el contenido completo de un archivo como string
- Se mantuvo `loadDataset()`: Sigue cargando palabras para las búsquedas

#### Cambios en runExperiment()
- Se modificó la firma para aceptar el texto completo en lugar de un vector de palabras:
  ```cpp
  template <typename Structure>
  void runExperiment(
      const string &name,
      Structure &structure,
      const string &textToInsert,
      const vector<string> &searchWords,
      int initMode = 0)
  ```
- Ahora usa `structure.init(textToInsert, initMode)` en lugar del bucle de inserciones

#### Cambios en main()
- Se carga el texto completo con `loadTextFile()` en lugar de `loadDataset()`
- Se ejecutan experimentos para ambos modos (0 y 1) en cada dataset
- Se muestran resultados separados para cada modo

## Ejemplo de Uso

```cpp
string texto = "Hello world!\nThis is a test.\nHello again!";

// Modo 0: Insertar palabras con posición en texto
NaiveTrie trie_mode0;
trie_mode0.init(texto, 0);
auto pos = trie_mode0.search_positions("hello"); // Retorna {0, 29} (posiciones en el texto)

// Modo 1: Insertar palabras con número de línea
NaiveTrie trie_mode1;
trie_mode1.init(texto, 1);
auto lines = trie_mode1.search_positions("hello"); // Retorna {0, 2} (números de línea)
auto lines2 = trie_mode1.search_positions("test"); // Retorna {1}

// Modo 2: Insertar todos los substrings (1 a 20 caracteres)
string texto2 = "Hello";
NaiveTrie trie_mode2;
trie_mode2.init(texto2, 2);
auto substr1 = trie_mode2.search_positions("hell"); // Retorna {0}
auto substr2 = trie_mode2.search_positions("llo"); // Retorna {2}
auto substr3 = trie_mode2.search_positions("lo"); // Retorna {3}
```

## Ventajas de los Cambios

1. **Modo 0**: Permite búsquedas de palabras individuales manteniendo su posición exacta en el texto (carácter donde comienza)
2. **Modo 1**: Permite saber en qué líneas aparece una palabra específica (útil para indexación de documentos)
3. **Modo 2**: Permite búsquedas de subcadenas arbitrarias, encontrando cualquier secuencia de caracteres en el texto (búsqueda de texto completo)
4. **Simplicidad**: La función init() encapsula toda la lógica de procesamiento
5. **Flexibilidad**: El parámetro mode permite elegir el comportamiento sin cambiar código
6. **Compatibilidad**: El valor por defecto (mode=0) mantiene un comportamiento razonable

## Verificación

Se crearon programas de prueba (`test_init_modes.cpp` y `test_mode2_detailed.cpp`) que verifican:
- Los tres modos funcionan correctamente
- Las posiciones se almacenan correctamente
- Las búsquedas funcionan en todos los modos
- Tanto NaiveTrie como RadixTrie implementan correctamente los modos

Ejemplo de resultados:
```
Texto: "Hello world!\nThis is a test.\nHello again!"

Modo 0 - Búsqueda "hello": Encontrado en posiciones 0, 29
Modo 1 - Búsqueda "hello": Encontrado en líneas 0, 2

Modo 0 - Búsqueda "test": Encontrado en posición 23
Modo 1 - Búsqueda "test": Encontrado en línea 1

Texto: "Hello"
Modo 2 - Búsqueda "hell": Encontrado en posición 0
Modo 2 - Búsqueda "llo": Encontrado en posición 2
Modo 2 - Búsqueda "l": Encontrado en posiciones 2, 3
```

El programa compila sin errores y los tests pasan exitosamente.

## Consideraciones de Rendimiento

**Modo 2** tiene consideraciones especiales:
- Genera muchos más datos que los modos 0 y 1
- Para un texto de longitud N, genera aproximadamente N × 20 substrings (o N × min(20, N-i) por cada posición i)
- Consume significativamente más memoria
- Es ideal para búsquedas de texto completo donde se necesita encontrar cualquier subcadena
- El límite de 20 caracteres es un compromiso entre funcionalidad y eficiencia
