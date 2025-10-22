# Changelog - Integración de Funcionalidad CSV

## Fecha: 22 de octubre de 2025

### Cambios Realizados

Se ha integrado la funcionalidad de salida CSV desde `experiments_csv.cpp` dentro de `experiments.cpp`, permitiendo que un solo ejecutable maneje ambos modos de salida.

### Nuevas Funcionalidades en `experiments.cpp`

#### 1. Estructura `ExperimentResult`
```cpp
struct ExperimentResult {
    string dataset;
    int mode;
    string structure;
    size_t palabras_buscadas;
    size_t palabras_encontradas;
    double tiempo_insercion_ms;
    double tiempo_busqueda_ms;
    double memoria_kb;
    double percentage_mem;
    bool success;
};
```

#### 2. Función `runExperimentCSV`
Ejecuta experimentos de manera simplificada y retorna resultados estructurados para formato CSV. Incluye manejo de excepciones para errores de memoria y otros fallos.

#### 3. Main con Argumento de Línea de Comandos
El programa ahora acepta el flag `--csv` para cambiar entre dos modos:

- **Modo Normal** (sin argumentos): Salida detallada con métricas estructurales
- **Modo CSV** (con `--csv`): Salida en formato CSV para análisis de datos

### Uso

#### Modo Normal (Detallado)
```bash
./bin/experiments
```

Salida incluye:
- Métricas estructurales (nodos, profundidad)
- Información detallada de cada experimento
- Estadísticas de búsqueda y memoria

#### Modo CSV
```bash
./bin/experiments --csv
```

Salida en formato CSV con columnas:
- Dataset
- Modo (0 o 1)
- Estructura (NaiveTrie o RadixTrie)
- Palabras_Buscadas
- Palabras_Encontradas
- Tiempo_Insercion_ms
- Tiempo_Busqueda_ms
- Memoria_KB
- Porcentaje_Memoria
- Exito (1 o 0)

#### Redirigir a Archivo CSV
```bash
./bin/experiments --csv > resultados.csv
```

### Compilación

El Makefile existente compila automáticamente el nuevo `experiments.cpp`:

```bash
make clean
make all
```

### Ventajas de la Integración

1. **Un solo ejecutable**: No es necesario mantener dos versiones separadas
2. **Flexibilidad**: El usuario elige el formato de salida según sus necesidades
3. **Mantenimiento simplificado**: Los cambios se aplican una sola vez
4. **Compatibilidad**: El modo normal sigue funcionando exactamente igual
5. **Análisis de datos**: El modo CSV facilita la importación en Excel, R, Python, etc.

### Compatibilidad con Código Anterior

- El comportamiento por defecto (sin argumentos) es idéntico a la versión anterior
- `experiments_csv.cpp` puede mantenerse como referencia o eliminarse si ya no es necesario
- La función `saveToCSV` original se mantiene para compatibilidad

### Ejemplos de Uso

#### Análisis en Python
```python
import pandas as pd

# Leer resultados CSV
df = pd.read_csv('resultados.csv')

# Análisis por estructura
print(df.groupby('Estructura')[['Tiempo_Insercion_ms', 'Tiempo_Busqueda_ms', 'Memoria_KB']].mean())

# Comparar modos
print(df.groupby(['Estructura', 'Modo'])['Tiempo_Busqueda_ms'].mean())
```

#### Análisis en R
```r
# Leer datos
datos <- read.csv('resultados.csv')

# Crear gráfico
library(ggplot2)
ggplot(datos, aes(x=Dataset, y=Tiempo_Busqueda_ms, fill=Estructura)) +
  geom_bar(stat='identity', position='dodge') +
  theme_minimal()
```

### Testing

Se ha verificado que:
- ✅ La compilación es exitosa sin errores
- ✅ El modo normal funciona correctamente
- ✅ El modo CSV genera salida válida
- ✅ Los datos en CSV son consistentes con la salida normal
- ✅ El manejo de excepciones funciona correctamente

### Archivos Modificados

- `sources/src/experiments.cpp`: Añadidas estructuras y funciones CSV, modificado main
- `Makefile`: Actualizado target `csv` para usar `experiments --csv`, eliminadas referencias a `experiments_csv`

### Archivos Eliminados

- `sources/src/experiments_csv.cpp`: Funcionalidad integrada en `experiments.cpp`

### Comandos Makefile Actualizados

- `make test`: Ejecuta experimentos en modo normal (detallado)
- `make csv`: Ejecuta experimentos en modo CSV (compila y ejecuta con flag `--csv`)
- `make clean`: Limpia archivos temporales (eliminada referencia a `experiments_csv`)
