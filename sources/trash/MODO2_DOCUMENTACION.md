# Modo 2: Búsqueda de Substrings Completa

## Descripción

El **Modo 2** implementa una funcionalidad de búsqueda de texto completo insertando todos los substrings posibles del texto de longitud 1 a 20 caracteres.

## Funcionamiento

Para un texto dado, el Modo 2 genera todos los substrings posibles:

### Ejemplo con "Hello World" (mostrando límite de 4 para simplicidad):

```
Posición 0: "H", "He", "Hel", "Hell"
Posición 1: "e", "el", "ell", "ello"  
Posición 2: "l", "ll", "llo", "llo "
Posición 3: "l", "lo", "lo ", "lo W"
Posición 4: "o", "o ", "o W", "o Wo"
Posición 5: " ", " W", " Wo", " Wor"
Posición 6: "W", "Wo", "Wor", "Worl"
Posición 7: "o", "or", "orl", "orld"
Posición 8: "r", "rl", "rld"
Posición 9: "l", "ld"
Posición 10: "d"
```

Con el límite real de 20 caracteres, se generan hasta 20 substrings por posición.

## Características

- **Longitud de substrings**: 1 a 20 caracteres
- **Conversión**: Todo se convierte a minúsculas
- **Posición almacenada**: Posición inicial del substring en el texto original
- **Uso**: Búsqueda de cualquier subcadena arbitraria

## Ejemplos de Uso

```cpp
string texto = "Hello World";
NaiveTrie trie;
trie.init(texto, 2);

// Búsquedas exitosas:
trie.search_positions("hello");  // {0}
trie.search_positions("llo");    // {2}
trie.search_positions("lo wo");  // {3}
trie.search_positions("world");  // {6}
trie.search_positions("or");     // {7}
```

## Ventajas

1. **Búsqueda completa**: Encuentra cualquier subcadena del texto
2. **Posiciones exactas**: Devuelve dónde comienza cada aparición
3. **Flexible**: No depende de delimitadores de palabras
4. **Útil para**: 
   - Búsqueda de patrones
   - Coincidencias parciales
   - Análisis de secuencias
   - Indexación completa de texto

## Desventajas

1. **Memoria**: Consume mucho más espacio que los modos 0 y 1
   - Para un texto de N caracteres: aproximadamente N × 20 inserciones
   - Para "Hello World" (11 chars): 155 substrings
   
2. **Tiempo de inicialización**: Más lento que los otros modos
   - Complejidad: O(N × L) donde N es la longitud del texto y L es el límite (20)

3. **Límite de 20 caracteres**: No encontrará substrings más largos

## Comparación de Modos

| Modo | Tipo de inserción | Posición almacenada | Uso principal |
|------|-------------------|---------------------|---------------|
| 0 | Palabras separadas | Carácter inicial | Búsqueda de palabras |
| 1 | Palabras por línea | Número de línea | Indexación por línea |
| 2 | Todos los substrings (1-20) | Carácter inicial | Búsqueda de texto completo |

## Rendimiento Esperado

Para el archivo "Alice in Wonderland" (~150KB):
- **Modo 0**: ~30K palabras insertadas
- **Modo 1**: ~30K palabras insertadas
- **Modo 2**: ~3M substrings insertados (100x más) - **OMITIDO AUTOMÁTICAMENTE**

**Recomendación**: Usar Modo 2 solo cuando sea necesario buscar subcadenas arbitrarias. Para búsquedas de palabras completas, los modos 0 o 1 son mucho más eficientes.

## Protección contra Desbordamiento de Memoria

El sistema implementa protecciones automáticas para evitar crashes:

### Límite de Seguridad: 50,000 caracteres

- **Verificación automática**: Antes de iniciar el Modo 2, se verifica el tamaño del texto
- **Omisión inteligente**: Si el texto excede 50KB, el experimento se omite automáticamente
- **Mensaje informativo**: Se muestra cuántos substrings se generarían
- **Continuación sin interrupciones**: Los demás experimentos continúan normalmente

### Ejemplo de Salida

Cuando un texto es demasiado grande:
```
=====================================
 Experimento: Naive Trie (Modo 2)
=====================================
⚠️  OMITIDO: Texto demasiado grande para Modo 2 (170597 chars > 50000 límite)
    El Modo 2 genera ~3411940 substrings, lo cual excedería la memoria disponible.
```

### Manejo de Errores

Además del límite preventivo, el sistema incluye:
- **try-catch**: Captura excepciones de memoria (`std::bad_alloc`)
- **Recuperación**: Si ocurre un error, se muestra un mensaje y continúa con el siguiente experimento
- **Sin crashes**: El programa nunca se interrumpe completamente

### Ajuste del Límite

El límite puede modificarse editando `experiments.cpp`:
```cpp
const size_t MAX_TEXT_SIZE_MODE2 = 50000;  // Modificar según la memoria disponible
```

**Recomendaciones**:
- Sistemas con 8GB RAM: 50,000 caracteres (por defecto)
- Sistemas con 16GB RAM: 100,000 caracteres
- Sistemas con 32GB+ RAM: 200,000 caracteres
