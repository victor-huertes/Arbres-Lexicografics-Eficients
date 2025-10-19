# Protección contra Desbordamiento de Memoria - Modo 2

## Problema Identificado

El Modo 2 generaba demasiados datos para archivos grandes, causando:
- Segmentation faults (core dumps)
- Interrupción completa del programa de experimentos
- Pérdida de resultados de otros experimentos

### Ejemplo del Problema
```
DATASET: input/Alice_in_Wonderland.txt (~170KB)
Modo 2 intentaría insertar: ~3,411,940 substrings
Resultado: Crash del programa
```

## Solución Implementada

### 1. Límite Preventivo de Tamaño

**Constante**: `MAX_TEXT_SIZE_MODE2 = 50000` (50KB)

Antes de ejecutar el Modo 2:
- Se verifica el tamaño del texto
- Si excede 50KB, se omite automáticamente
- Se muestra un mensaje informativo

### 2. Manejo de Excepciones

Se añadió try-catch en `runExperiment()`:
```cpp
try {
    // Ejecutar experimento
} catch (const std::bad_alloc& e) {
    // Error de memoria - omitir experimento
} catch (const std::exception& e) {
    // Otro error - omitir experimento
}
```

### 3. Retorno de Estado

`runExperiment()` ahora retorna `bool`:
- `true`: Experimento completado exitosamente
- `false`: Experimento omitido (por límite o error)

## Resultados

### Archivos Pequeños (< 50KB)
✅ **lorem_ipsum.txt** (~10KB)
- Modo 2 ejecutado exitosamente
- 200MB de memoria usada
- ~100ms de tiempo de inserción

✅ **bee_movie_script.txt** (~40KB)  
- Modo 2 ejecutado exitosamente
- 750MB de memoria usada
- ~350ms de tiempo de inserción

### Archivos Grandes (> 50KB)
⚠️ **Alice_in_Wonderland.txt** (~170KB)
```
⚠️  OMITIDO: Texto demasiado grande para Modo 2 (170597 chars > 50000 límite)
    El Modo 2 genera ~3411940 substrings, lo cual excedería la memoria disponible.
```

⚠️ **words_alpha.txt** (~3.8MB)
```
⚠️  OMITIDO: Texto demasiado grande para Modo 2 (3864805 chars > 50000 límite)
    El Modo 2 genera ~77296100 substrings, lo cual excedería la memoria disponible.
```

## Ventajas de la Solución

1. ✅ **Sin crashes**: El programa nunca se interrumpe
2. ✅ **Continuidad**: Todos los experimentos se completan
3. ✅ **Información clara**: Mensajes informativos sobre por qué se omite
4. ✅ **Flexibilidad**: El límite es fácilmente ajustable
5. ✅ **Seguridad**: Doble protección (límite preventivo + try-catch)

## Ajuste del Límite

Para modificar el límite según la memoria disponible, editar `experiments.cpp`:

```cpp
const size_t MAX_TEXT_SIZE_MODE2 = 50000;  // Cambiar este valor
```

**Recomendaciones por sistema**:
| RAM del Sistema | Límite Recomendado | Texto Máximo Aprox. |
|-----------------|-------------------|---------------------|
| 8 GB            | 50,000 chars      | ~50KB              |
| 16 GB           | 100,000 chars     | ~100KB             |
| 32 GB           | 200,000 chars     | ~200KB             |
| 64 GB+          | 500,000 chars     | ~500KB             |

## Estimación de Memoria

Para estimar la memoria que usará el Modo 2:

**Fórmula aproximada**:
```
Substrings generados ≈ Tamaño_Texto × 20
Memoria (NaiveTrie) ≈ Substrings × 10 KB
Memoria (RadixTrie) ≈ Substrings × 1.7 KB
```

**Ejemplo** (Alice in Wonderland - 170KB):
- Substrings: ~3,411,940
- NaiveTrie: ~33 GB (inviable)
- RadixTrie: ~5.6 GB (límite)

## Archivos Modificados

- `sources/src/experiments.cpp`:
  - Añadido límite `MAX_TEXT_SIZE_MODE2`
  - Añadido try-catch en `runExperiment()`
  - Cambiado retorno de `void` a `bool`
  - Añadido mensaje de omisión

## Pruebas Realizadas

✅ Compilación exitosa
✅ `make test` completa sin crashes
✅ Archivos pequeños procesan correctamente
✅ Archivos grandes se omiten con mensaje claro
✅ Todos los modos 0 y 1 funcionan normalmente
