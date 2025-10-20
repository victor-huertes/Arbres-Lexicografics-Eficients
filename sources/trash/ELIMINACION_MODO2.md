# Eliminación del Modo 2

## Fecha
20 de octubre de 2025

## Resumen
El Modo 2 (inserción de todos los substrings de longitud 1-20) ha sido completamente eliminado del proyecto debido a:
- Alto consumo de memoria para textos medianos y grandes
- Complejidad adicional innecesaria para los casos de uso principales
- Tiempos de ejecución significativamente mayores que los modos 0 y 1

## Archivos Modificados

### 1. Implementaciones
- **sources/src/naive.cpp**: Eliminado bloque `else if (mode == 2)` de la función `init()`
- **sources/src/radix.cpp**: Eliminado bloque `else if (mode == 2)` de la función `init()`

### 2. Headers
- **sources/include/naive.h**: Eliminada referencia al Modo 2 en la documentación de `init()`
- **sources/include/radix.h**: Eliminada referencia al Modo 2 en la documentación de `init()`

### 3. Experimentos
- **sources/src/experiments.cpp**: 
  - Eliminada verificación de límite de tamaño para Modo 2
  - Eliminada sección de experimentos del Modo 2 en el `main()`
  - Mantenidas las protecciones de try-catch para robustez

### 4. Documentación
Los siguientes archivos fueron movidos a `sources/trash/` previamente:
- `MODO2_DOCUMENTACION.md`
- `OPTIMIZACIONES_MODO2.md`
- `PROTECCION_MEMORIA.md`
- `CAMBIOS_INIT_FUNCTION.md`

## Estado Actual

### Modos Disponibles
Ahora solo hay 2 modos de inicialización:

**Modo 0**: Insertar palabras con posición en texto
- Separa palabras por caracteres no alfanuméricos
- Almacena la posición del carácter inicial
- Ideal para búsqueda de palabras completas

**Modo 1**: Insertar palabras con número de línea
- Procesa el texto línea por línea
- Almacena el número de línea donde aparece cada palabra
- Ideal para indexación por líneas

## Verificación

### Compilación
✅ `make clean && make all` - Exitoso sin errores

### Tests
✅ `make test` - Todos los experimentos funcionan correctamente
- lorem_ipsum.txt: Modos 0 y 1 ejecutados
- bee_movie_script.txt: Modos 0 y 1 ejecutados
- Alice_in_Wonderland.txt: Modos 0 y 1 ejecutados
- words_alpha.txt: Modos 0 y 1 ejecutados

### Errores de Compilación
✅ Sin errores de compilación
✅ Sin warnings

## Impacto

### Rendimiento
- **Tiempo de ejecución**: Reducido significativamente (sin ejecutar Modo 2)
- **Memoria**: Uso mucho más eficiente
- **Ejemplos anteriores con Modo 2**:
  - lorem_ipsum.txt: ~86ms (Modo 2) vs 0.2ms (Modo 0/1)
  - bee_movie_script.txt: ~295ms (Modo 2) vs 1.8ms (Modo 0/1)

### Simplicidad
- Código más limpio y fácil de mantener
- Menos complejidad en la función `init()`
- Documentación más concisa

### Funcionalidad
- Sin pérdida de funcionalidad para los casos de uso principales
- Los modos 0 y 1 cubren las necesidades de:
  - Búsqueda de palabras completas
  - Indexación por posición
  - Indexación por línea

## Recomendaciones

Si en el futuro se necesita búsqueda de subcadenas arbitrarias, considerar:

1. **Suffix Array**: Estructura más eficiente para búsqueda de subcadenas
2. **FM-Index**: Índice comprimido para textos grandes
3. **Búsqueda aproximada**: Algoritmos como Boyer-Moore o KMP
4. **Implementación especializada**: Un trie de sufijos optimizado específicamente para el caso de uso

## Conclusión

El proyecto ahora es más eficiente, más simple y más fácil de mantener, manteniendo toda la funcionalidad necesaria para los casos de uso principales del análisis de texto.
