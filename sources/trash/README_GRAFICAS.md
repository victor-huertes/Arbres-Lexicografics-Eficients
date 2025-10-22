# Generación de Gráficas con GNUplot

## Descripción

Los scripts de GNUplot (`script.gnuplot` y `scriptlog.gnuplot`) generan gráficas comparativas entre las estructuras NaiveTrie y RadixTrie para todos los datasets y modos.

## Métricas Incluidas

Los scripts generan gráficas para las siguientes métricas:

1. **Nodos_Totales** - Número total de nodos creados en la estructura
2. **Profundidad_Maxima** - Profundidad máxima del árbol
3. **Profundidad_Mediana** - Profundidad mediana del árbol
4. **Palabras_Buscadas** - Número de palabras que se buscaron
5. **Palabras_Encontradas** - Número de palabras encontradas exitosamente
6. **Tiempo_Insercion_ms** - Tiempo de inserción en milisegundos
7. **Tiempo_Busqueda_ms** - Tiempo de búsqueda en milisegundos
8. **Memoria_KB** - Memoria usada en kilobytes
9. **Porcentaje_Memoria** - Porcentaje de memoria respecto al texto original
10. **Nodos_Visitados** - Número total de nodos visitados durante las búsquedas

## Diferencias entre Scripts

### script.gnuplot
- Genera gráficas de tipo **histograma** con barras agrupadas
- Mejor para comparar valores absolutos entre estructuras

### scriptlog.gnuplot  
- Genera gráficas de tipo **líneas con puntos**
- Usa índices numéricos en el eje X
- Mejor para visualizar tendencias a lo largo de los datasets

## Uso

### Generar el CSV con datos actualizados:
```bash
make csv > results/results.csv
```

### Generar todas las gráficas con script.gnuplot:
```bash
cd results
gnuplot -p script.gnuplot
```

### Generar todas las gráficas con scriptlog.gnuplot:
```bash
cd results
gnuplot -p scriptlog.gnuplot
```

### O usar el Makefile:
```bash
make plot
```

## Salida

Cada script genera archivos PNG con el formato:
- `plot_Nodos_Totales.png`
- `plot_Profundidad_Maxima.png`
- `plot_Profundidad_Mediana.png`
- `plot_Palabras_Buscadas.png`
- `plot_Palabras_Encontradas.png`
- `plot_Tiempo_Insercion_ms.png`
- `plot_Tiempo_Busqueda_ms.png`
- `plot_Memoria_KB.png`
- `plot_Porcentaje_Memoria.png`
- `plot_Nodos_Visitados.png`

## Escalas

La mayoría de métricas usan **escala logarítmica** en el eje Y para mejor visualización de rangos amplios, excepto:
- **Profundidad_Mediana** - usa escala lineal
- **Porcentaje_Memoria** - usa escala lineal

## Requisitos

- GNUplot instalado en el sistema
- Archivo `results.csv` con el formato correcto generado por `experiments --csv`

## Formato CSV Esperado

```csv
Dataset,Modo,Estructura,Nodos_Totales,Profundidad_Maxima,Profundidad_Mediana,Palabras_Buscadas,Palabras_Encontradas,Tiempo_Insercion_ms,Tiempo_Busqueda_ms,Memoria_KB,Porcentaje_Memoria,Nodos_Visitados,Exito
```
