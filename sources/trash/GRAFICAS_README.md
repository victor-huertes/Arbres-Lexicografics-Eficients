# Guía de Generación de Gráficos

Este documento describe cómo generar y utilizar los diversos tipos de gráficos disponibles para analizar el rendimiento de las estructuras de datos Trie.

## Estructura de Archivos

- `sources/utils/script.gnuplot` - Script unificado de GNUplot para generar todos los gráficos
- `output/results.csv` - Resultados de experimentos individuales
- `output/results_stats.csv` - Estadísticas de múltiples ejecuciones (opcional)

## Modos de Ejecución de Experimentos

### Modo Normal
Ejecuta experimentos una vez con salida detallada:
```bash
./bin/experiments
```

### Modo CSV
Ejecuta experimentos una vez y genera `output/results.csv`:
```bash
./bin/experiments --csv > output/results.csv
```

### Modo Estadísticas (NUEVO)
Ejecuta múltiples corridas de cada experimento y genera estadísticas:
```bash
./bin/experiments --stats 5
```
- El número después de `--stats` indica cuántas veces ejecutar cada experimento (por defecto: 5)
- Genera `output/results_stats.csv` con medias, medianas, desviaciones estándar, mínimos, máximos, Q1 y Q3

## Tipos de Gráficos Generados

### 1. Histogramas (Barras)
**Archivos:** `plot_histogram_*.png`

Gráficos de barras que comparan métricas entre estructuras y datasets. Útiles para comparar valores absolutos.

**Características:**
- Barras agrupadas por dataset
- Escala logarítmica (excepto para porcentajes y profundidad mediana)
- Compara NaiveTrie vs RadixTrie

### 2. Gráficos de Líneas y Puntos
**Archivos:** `plot_lines_*.png`

Gráficos con líneas y puntos que muestran tendencias a lo largo de los datasets.

**Características:**
- Visualiza tendencias
- Escala logarítmica automática
- Puntos diferenciados por estructura

### 3. Diagramas de Caja y Bigotes (Boxplots) - NUEVO
**Archivos:** `plot_boxplot_*.png`

Muestran la distribución estadística de las métricas a través de múltiples ejecuciones.

**Características:**
- Muestra mínimo, Q1, mediana, Q3, máximo
- Útil para identificar variabilidad y outliers
- Requiere ejecutar con `--stats`

**Métricas incluidas:**
- Tiempo_Insercion_ms
- Tiempo_Busqueda_ms
- Memoria_KB
- Nodos_Visitados
- Nodos_Totales

### 4. Gráficos de Medias con Barras de Error - NUEVO
**Archivos:** `plot_means_*.png`

Histogramas que muestran la media con barras de error (desviación estándar).

**Características:**
- Media ± desviación estándar
- Compara estructuras visualmente con incertidumbre
- Requiere ejecutar con `--stats`

### 5. Gráficos de Eficiencia - NUEVO
**Archivos:** 
- `plot_efficiency_search_vs_memory.png` - Tiempo de búsqueda vs Memoria
- `plot_efficiency_insert_vs_memory.png` - Tiempo de inserción vs Memoria
- `plot_nodes_per_found_word.png` - Nodos visitados por palabra encontrada

Muestran relaciones entre diferentes métricas para analizar trade-offs.

## Workflow Completo

### Paso 1: Compilar el proyecto
```bash
make
```

### Paso 2: Ejecutar experimentos

**Opción A: Experimentos estándar (una ejecución)**
```bash
./bin/experiments --csv > output/results.csv
```

**Opción B: Experimentos con estadísticas (múltiples ejecuciones)**
```bash
./bin/experiments --stats 10
```
Nota: Más ejecuciones = resultados más confiables pero más tiempo de cómputo

### Paso 3: Cambiar al directorio de salida
```bash
cd output/
```

### Paso 4: Generar gráficos
```bash
gnuplot ../sources/utils/script.gnuplot
```

### Paso 5: Ver los gráficos generados
```bash
ls plot_*.png
```

Los gráficos estarán disponibles en el directorio `output/`.

## Interpretación de Gráficos

### Histogramas y Líneas
- **Eje Y logarítmico**: Facilita la comparación de valores con rangos muy diferentes
- **Comparación directa**: Barras o puntos más bajos = mejor rendimiento (menor tiempo/memoria)

### Boxplots
- **Caja**: Contiene el 50% central de los datos (Q1 a Q3)
- **Línea central**: Mediana
- **Bigotes**: Mínimo y máximo (excluyendo outliers)
- **Cajas más estrechas**: Resultados más consistentes
- **Cajas más anchas**: Mayor variabilidad

### Gráficos de Medias con Barras de Error
- **Barra central**: Media de todas las ejecuciones
- **Barras de error**: ±1 desviación estándar
- **Barras más cortas**: Mayor consistencia en los resultados

### Gráficos de Eficiencia
- **Puntos en la esquina inferior izquierda**: Mejor eficiencia (menos tiempo y memoria)
- **Compara trade-offs**: Una estructura puede ser más rápida pero usar más memoria

## Métricas Disponibles

1. **Nodos_Totales**: Número total de nodos en la estructura
2. **Profundidad_Maxima**: Altura máxima del árbol
3. **Profundidad_Mediana**: Profundidad media de los nodos
4. **Palabras_Buscadas**: Número de palabras buscadas en el test
5. **Palabras_Encontradas**: Número de palabras encontradas exitosamente
6. **Tiempo_Insercion_ms**: Tiempo de construcción de la estructura (milisegundos)
7. **Tiempo_Busqueda_ms**: Tiempo total de búsqueda (milisegundos)
8. **Memoria_KB**: Memoria utilizada por la estructura (kilobytes)
9. **Porcentaje_Memoria**: Porcentaje de memoria respecto al texto original
10. **Nodos_Visitados**: Número total de nodos visitados durante las búsquedas

## Formato del CSV de Estadísticas

El archivo `results_stats.csv` tiene el siguiente formato:

```csv
Dataset,Modo,Estructura,Metric,Mean,Median,StdDev,Min,Max,Q1,Q3,NumRuns
lorem_ipsum,0,NaiveTrie,Tiempo_Insercion_ms,0.245,0.242,0.012,0.230,0.260,0.235,0.250,5
...
```

**Columnas:**
- **Dataset**: Nombre del dataset
- **Modo**: Modo de inicialización (0 o 1)
- **Estructura**: NaiveTrie o RadixTrie
- **Metric**: Nombre de la métrica
- **Mean**: Media aritmética
- **Median**: Mediana (valor central)
- **StdDev**: Desviación estándar
- **Min**: Valor mínimo
- **Max**: Valor máximo
- **Q1**: Primer cuartil (25%)
- **Q3**: Tercer cuartil (75%)
- **NumRuns**: Número de ejecuciones exitosas

## Personalización

Para personalizar los gráficos, edita `sources/utils/script.gnuplot`:

### Cambiar tamaño de imagen
```gnuplot
set term pngcairo size 1920,1080  # Cambiar de 1200,800
```

### Agregar más métricas para boxplots
```gnuplot
boxplot_metrics = "Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB Profundidad_Maxima"
```

### Cambiar colores
```gnuplot
# En las secciones de plot, cambiar:
lc rgb "blue"   -> lc rgb "#1f77b4"
lc rgb "red"    -> lc rgb "#ff7f0e"
```

### Desactivar escala logarítmica
```gnuplot
# Comentar la línea:
# set logscale y
```

## Solución de Problemas

### Los boxplots no se generan
**Causa**: No existe `results_stats.csv`
**Solución**: Ejecutar con `./bin/experiments --stats 5`

### Gráficos vacíos o con errores
**Causa**: Formato incorrecto del CSV
**Solución**: Verificar que los archivos CSV tienen encabezados correctos y datos válidos

### Escala logarítmica causa problemas
**Causa**: Valores cero o negativos en los datos
**Solución**: Verificar que todas las métricas tienen valores positivos

### GNUplot no encuentra archivos
**Causa**: Ejecutando desde directorio incorrecto
**Solución**: Ejecutar `gnuplot` desde el directorio `output/` o usar rutas absolutas

## Recomendaciones

1. **Para análisis rápido**: Usar modo `--csv` con una ejecución
2. **Para resultados publicables**: Usar modo `--stats` con al menos 5-10 ejecuciones
3. **Para datasets grandes**: Reducir número de ejecuciones en modo `--stats`
4. **Comparar variabilidad**: Los boxplots son esenciales para entender la consistencia
5. **Análisis de trade-offs**: Los gráficos de eficiencia ayudan a tomar decisiones

## Referencias

- [Documentación de GNUplot](http://www.gnuplot.info/documentation.html)
- [Boxplots en GNUplot](http://gnuplot.sourceforge.net/demo/boxplot.html)
- [Candlesticks (para boxplots manuales)](http://gnuplot.sourceforge.net/demo/candlesticks.html)
