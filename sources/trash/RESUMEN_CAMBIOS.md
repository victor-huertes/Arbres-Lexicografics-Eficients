# Resumen de Cambios - Sistema de Gráficos Mejorado

## Fecha: 22 de octubre de 2025

## Cambios Implementados

### 1. Fusión de Scripts de GNUplot ✅
- **Archivo modificado:** `sources/utils/script.gnuplot`
- **Archivo eliminado:** `sources/utils/scriptlog.gnuplot`

**Descripción:**
Los dos scripts separados (`script.gnuplot` y `scriptlog.gnuplot`) han sido fusionados en un único archivo `script.gnuplot` que ahora:
- Genera histogramas (barras) con escala logarítmica
- Genera gráficos de líneas y puntos con escala logarítmica
- Incluye nuevos tipos de visualizaciones

### 2. Nuevas Funcionalidades en experiments.cpp ✅
- **Archivo modificado:** `sources/src/experiments.cpp`

**Nuevas estructuras y funciones:**
- `struct Statistics`: Almacena media, mediana, desviación estándar, min, max, Q1, Q3
- `calculateStatistics()`: Calcula todas las estadísticas de un conjunto de valores
- `struct ExperimentResultWithStats`: Estructura para resultados con estadísticas
- `runMultipleExperiments()`: Ejecuta N corridas de un experimento y calcula estadísticas
- `saveStatsToCSV()`: Guarda estadísticas en formato CSV

**Nuevo modo de ejecución:**
```bash
./bin/experiments --stats [NUM_RUNS]
```
- Ejecuta múltiples corridas de cada experimento
- Calcula estadísticas (media, mediana, std dev, min, max, Q1, Q3)
- Genera archivo `output/results_stats.csv`

### 3. Nuevos Tipos de Gráficos ✅

#### 3.1. Boxplots (Diagramas de Caja y Bigotes)
**Archivos generados:** `plot_boxplot_*.png`

Muestran la distribución estadística de las métricas:
- Visualización de min, Q1, mediana, Q3, max
- Útil para detectar variabilidad y outliers
- Generados con candlesticks en GNUplot

**Métricas con boxplot:**
- Tiempo_Insercion_ms
- Tiempo_Busqueda_ms
- Memoria_KB
- Nodos_Visitados
- Nodos_Totales

#### 3.2. Gráficos de Medias con Barras de Error
**Archivos generados:** `plot_means_*.png`

Histogramas que muestran:
- Media como altura de la barra
- Desviación estándar como barras de error
- Comparación visual de consistencia entre estructuras

#### 3.3. Gráficos de Eficiencia (Trade-offs)
**Archivos generados:**
- `plot_efficiency_search_vs_memory.png`: Tiempo búsqueda vs Memoria
- `plot_efficiency_insert_vs_memory.png`: Tiempo inserción vs Memoria
- `plot_nodes_per_found_word.png`: Eficiencia de búsqueda

Permiten analizar:
- Relaciones entre métricas
- Trade-offs de rendimiento
- Optimización de estructuras

### 4. Mejoras en el Script GNUplot ✅

**Características agregadas:**
- Verificación automática de existencia de `results_stats.csv`
- Mensajes informativos durante la generación
- Escala logarítmica inteligente (desactivada para porcentajes)
- Colores diferenciados para cada estructura
- Formato consistente en todos los gráficos

**Secciones del script:**
1. Histogramas (`plot_histogram_*.png`)
2. Líneas y puntos (`plot_lines_*.png`)
3. Boxplots (`plot_boxplot_*.png`)
4. Medias con error (`plot_means_*.png`)
5. Gráficos de eficiencia (varios)

### 5. Documentación Completa ✅
- **Archivo creado:** `docs/GRAFICAS_README.md`

Incluye:
- Guía de uso completa
- Descripción de cada tipo de gráfico
- Interpretación de resultados
- Workflow completo paso a paso
- Solución de problemas comunes
- Ejemplos de personalización

## Formato de Archivos CSV

### results.csv (Existente)
```
Dataset,Modo,Estructura,Nodos_Totales,Profundidad_Maxima,Profundidad_Mediana,...
lorem_ipsum,0,NaiveTrie,272,13,5.556,...
```

### results_stats.csv (Nuevo)
```
Dataset,Modo,Estructura,Metric,Mean,Median,StdDev,Min,Max,Q1,Q3,NumRuns
lorem_ipsum,0,NaiveTrie,Tiempo_Insercion_ms,0.245,0.242,0.012,0.230,0.260,0.235,0.250,5
```

## Uso del Sistema

### Workflow Completo

1. **Compilar:**
   ```bash
   make
   ```

2. **Opción A - Experimentos estándar:**
   ```bash
   ./bin/experiments --csv > output/results.csv
   cd output/
   gnuplot ../sources/utils/script.gnuplot
   ```
   Genera: histogramas y gráficos de líneas

3. **Opción B - Experimentos con estadísticas:**
   ```bash
   ./bin/experiments --stats 10
   cd output/
   gnuplot ../sources/utils/script.gnuplot
   ```
   Genera: todos los gráficos incluyendo boxplots y medias

### Parámetros del Modo Estadísticas

```bash
./bin/experiments --stats [NUM_RUNS]
```
- `NUM_RUNS`: Número de corridas por experimento (default: 5)
- Más corridas = resultados más confiables pero más tiempo
- Recomendado: 5-10 para análisis normales, 20+ para publicaciones

## Ventajas del Nuevo Sistema

### 1. Análisis Estadístico Robusto
- Múltiples corridas eliminan variabilidad del sistema
- Estadísticas completas (no solo promedios)
- Detección de outliers y comportamiento anómalo

### 2. Visualizaciones Completas
- Boxplots revelan distribuciones y consistencia
- Barras de error muestran confiabilidad
- Gráficos de eficiencia facilitan decisiones

### 3. Facilidad de Uso
- Un solo script para todo
- Verificación automática de archivos
- Mensajes informativos claros

### 4. Flexibilidad
- Modo rápido (--csv) para pruebas
- Modo completo (--stats) para análisis detallado
- Fácil personalización en el script

## Comparación con Sistema Anterior

| Aspecto | Anterior | Nuevo |
|---------|----------|-------|
| Scripts GNUplot | 2 archivos | 1 archivo unificado |
| Tipos de gráficos | 2 (histograms, lines) | 5 (+ boxplots, means, efficiency) |
| Análisis estadístico | No | Sí (completo) |
| Múltiples corridas | No | Sí (configurable) |
| Detección de variabilidad | No | Sí (std dev, IQR) |
| Documentación | Básica | Completa con ejemplos |

## Archivos Modificados/Creados

### Modificados:
1. `sources/utils/script.gnuplot` - Script unificado mejorado
2. `sources/src/experiments.cpp` - Soporte para estadísticas

### Eliminados:
1. `sources/utils/scriptlog.gnuplot` - Fusionado en script.gnuplot

### Creados:
1. `docs/GRAFICAS_README.md` - Documentación completa
2. `docs/RESUMEN_CAMBIOS.md` - Este archivo

### Generados por el sistema:
1. `output/results_stats.csv` - Estadísticas (con --stats)
2. `output/plot_boxplot_*.png` - Boxplots (con --stats)
3. `output/plot_means_*.png` - Gráficos de medias (con --stats)
4. `output/plot_efficiency_*.png` - Gráficos de trade-offs
5. `output/plot_histogram_*.png` - Histogramas (renombrados)
6. `output/plot_lines_*.png` - Líneas (renombrados)

## Notas Técnicas

### Cálculo de Estadísticas
- **Media**: Promedio aritmético de todas las corridas
- **Mediana**: Valor central (más robusto que la media)
- **Desviación estándar**: Medida de dispersión
- **Q1/Q3**: Primer y tercer cuartil (rango intercuartílico)
- **Min/Max**: Valores extremos observados

### Boxplots en GNUplot
Implementados con `candlesticks`:
```gnuplot
plot data using x:min:q1:median:q3:max with candlesticks
```
Cada "vela" representa:
- Bigotes: min y max
- Caja: Q1 a Q3
- Línea: mediana

### Escalas Logarítmicas
Aplicadas automáticamente excepto para:
- Porcentaje_Memoria (ya es porcentaje)
- Profundidad_Mediana (valores pequeños)

## Pruebas Realizadas

✅ Compilación exitosa de `experiments.cpp`
✅ Fusión correcta de scripts GNUplot
✅ Verificación de sintaxis en todos los archivos modificados
✅ Documentación completa generada

## Próximos Pasos Recomendados

1. **Ejecutar prueba con estadísticas:**
   ```bash
   ./bin/experiments --stats 3  # Prueba rápida con 3 corridas
   ```

2. **Generar todos los gráficos:**
   ```bash
   cd output/
   gnuplot ../sources/utils/script.gnuplot
   ```

3. **Revisar gráficos generados:**
   ```bash
   ls output/plot_*.png
   ```

4. **Para análisis completo:**
   ```bash
   ./bin/experiments --stats 10  # 10 corridas para resultados robustos
   ```

## Soporte y Ayuda

Consultar la documentación completa en:
- `docs/GRAFICAS_README.md` - Guía detallada de uso
- Comentarios en `sources/utils/script.gnuplot` - Detalles técnicos del script
- Comentarios en `sources/src/experiments.cpp` - Implementación de estadísticas

## Autor

Implementado con asistencia de GitHub Copilot
Fecha: 22 de octubre de 2025
