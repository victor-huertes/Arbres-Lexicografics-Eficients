# ================================================================================
#  GNUplot script: Generación completa de gráficos para análisis de Tries
# ================================================================================
# Este script unificado genera múltiples tipos de visualizaciones:
#   1. Histogramas (barras) con escala logarítmica
#   2. Gráficos de líneas y puntos con escala logarítmica
#   3. Diagramas de caja y bigotes (boxplots) para análisis estadístico
# ================================================================================

set datafile separator ","
set key outside
set grid
set term pngcairo size 1200,800

# Function to convert dataset names from snake_case to Title Case
format_dataset(name) = \
    (name eq "lorem_ipsum" ? "Lorem Ipsum" : \
    (name eq "bee_movie" ? "Bee Movie" : \
    (name eq "alice_wonderland" ? "Alice Wonderland" : \
    (name eq "moby_dick" ? "Moby Dick" : \
    (name eq "words_alpha" ? "Words Alpha" : \
    (name eq "dna_genome" ? "DNA Genome" : \
    (name eq "wikipedia_titles" ? "Wikipedia Titles" : name)))))))

# Function to map metric names to column numbers
# Columns: 1:Dataset  2:Modo  3:Estructura  4:Nodos_Totales  5:Profundidad_Maxima  6:Profundidad_Mediana
#          7:Palabras_Buscadas  8:Palabras_Encontradas  9:Tiempo_Insercion_ms  10:Tiempo_Busqueda_ms
#          11:Memoria_KB  12:Porcentaje_Memoria  13:Nodos_Visitados  14:Exito
get_column(metric) = \
    (metric eq "Nodos_Totales" ? 4 : \
    (metric eq "Profundidad_Maxima" ? 5 : \
    (metric eq "Profundidad_Mediana" ? 6 : \
    (metric eq "Palabras_Buscadas" ? 7 : \
    (metric eq "Palabras_Encontradas" ? 8 : \
    (metric eq "Tiempo_Insercion_ms" ? 9 : \
    (metric eq "Tiempo_Busqueda_ms" ? 10 : \
    (metric eq "Memoria_KB" ? 11 : \
    (metric eq "Porcentaje_Memoria" ? 12 : \
    (metric eq "Nodos_Visitados" ? 13 : 0))))))))))

metrics = "Nodos_Totales Profundidad_Maxima Profundidad_Mediana Palabras_Buscadas Palabras_Encontradas Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB Porcentaje_Memoria Nodos_Visitados"

# ================================================================================
# SECCIÓN 1: GRÁFICOS DE HISTOGRAMAS (BARRAS)
# ================================================================================
print "Generando gráficos de histogramas..."

set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9

do for [metric in metrics] {
    col = get_column(metric)
    
    # Convert metric name to Title Case for display
    metric_display = metric
    metric_display = (metric eq "Nodos_Totales" ? "Nodos Totales" : metric_display)
    metric_display = (metric eq "Profundidad_Maxima" ? "Profundidad Máxima" : metric_display)
    metric_display = (metric eq "Profundidad_Mediana" ? "Profundidad Mediana" : metric_display)
    metric_display = (metric eq "Palabras_Buscadas" ? "Palabras Buscadas" : metric_display)
    metric_display = (metric eq "Palabras_Encontradas" ? "Palabras Encontradas" : metric_display)
    metric_display = (metric eq "Tiempo_Insercion_ms" ? "Tiempo Inserción (ms)" : metric_display)
    metric_display = (metric eq "Tiempo_Busqueda_ms" ? "Tiempo Búsqueda (ms)" : metric_display)
    metric_display = (metric eq "Memoria_KB" ? "Memoria (KB)" : metric_display)
    metric_display = (metric eq "Porcentaje_Memoria" ? "Porcentaje Memoria" : metric_display)
    metric_display = (metric eq "Nodos_Visitados" ? "Nodos Visitados" : metric_display)
    
    set output sprintf("plot_histogram_%s.png", metric)
    set title sprintf("Comparativa de %s entre estructuras y datasets (Histograma)", metric_display)
    set ylabel metric_display
    set xlabel "Dataset"
    set xtics rotate by -30 font ",9"

    # Use logscale for most metrics except percentage and profundidad_mediana
    if (metric eq "Porcentaje_Memoria") {
        unset logscale y
    } else {
        if (metric eq "Profundidad_Mediana") {
            unset logscale y
        } else {
            set logscale y
        }
    }

    # Use every 2 rows starting from different offsets for each structure
    # Row 1,3,5,7,9,11,13 are NaiveTrie (odd rows after header)
    # Row 2,4,6,8,10,12,14 are RadixTrie (even rows after header)
    plot \
        'results.csv' every 2::1 using col:xtic(format_dataset(strcol(1))) title "NaiveTrie", \
        'results.csv' every 2::2 using col title "RadixTrie"

    unset output
}

unset logscale y

# ================================================================================
# SECCIÓN 2: GRÁFICOS DE LÍNEAS Y PUNTOS
# ================================================================================
print "Generando gráficos de líneas y puntos..."

set style data linespoints
set pointsize 1.5
set xlabel "Dataset / Modo"
set xtics rotate by -30 font ",9"

do for [metric in metrics] {
    col = get_column(metric)
    
    # Convert metric name to Title Case for display
    metric_display = metric
    metric_display = (metric eq "Nodos_Totales" ? "Nodos Totales" : metric_display)
    metric_display = (metric eq "Profundidad_Maxima" ? "Profundidad Máxima" : metric_display)
    metric_display = (metric eq "Profundidad_Mediana" ? "Profundidad Mediana" : metric_display)
    metric_display = (metric eq "Palabras_Buscadas" ? "Palabras Buscadas" : metric_display)
    metric_display = (metric eq "Palabras_Encontradas" ? "Palabras Encontradas" : metric_display)
    metric_display = (metric eq "Tiempo_Insercion_ms" ? "Tiempo Inserción (ms)" : metric_display)
    metric_display = (metric eq "Tiempo_Busqueda_ms" ? "Tiempo Búsqueda (ms)" : metric_display)
    metric_display = (metric eq "Memoria_KB" ? "Memoria (KB)" : metric_display)
    metric_display = (metric eq "Porcentaje_Memoria" ? "Porcentaje Memoria" : metric_display)
    metric_display = (metric eq "Nodos_Visitados" ? "Nodos Visitados" : metric_display)
    
    set output sprintf("plot_lines_%s.png", metric)
    set title sprintf("Comparativa de %s entre estructuras y datasets (Líneas)", metric_display)
    set ylabel metric_display

    if (metric eq "Porcentaje_Memoria") {
        unset logscale y
    } else {
        if (metric eq "Profundidad_Mediana") {
            unset logscale y
        } else {
            set logscale y
        }
    }

    # Simple numeric X axis
    plot \
        'results.csv' every ::1 using 0:(stringcolumn(3) eq "NaiveTrie" ? column(col) : 1/0):xtic(format_dataset(strcol(1))) \
            title "NaiveTrie" lw 2 pt 7, \
        'results.csv' every ::1 using 0:(stringcolumn(3) eq "RadixTrie" ? column(col) : 1/0) \
            title "RadixTrie" lw 2 pt 5

    unset output
}

unset logscale y

# ================================================================================
# SECCIÓN 3: DIAGRAMAS DE CAJA Y BIGOTES (BOXPLOTS)
# ================================================================================
# Estos diagramas utilizan datos estadísticos: min, Q1, median, Q3, max
# del archivo results_stats.csv generado con el modo --stats
# ================================================================================
print "Verificando disponibilidad de datos estadísticos para boxplots..."

# Verificar si existe el archivo de estadísticas
stats_file = "results_stats.csv"
file_exists = system(sprintf("test -f %s && echo 1 || echo 0", stats_file))

if (file_exists + 0 == 1) {
    print "Generando diagramas de caja y bigotes..."
    
    # Configuración para boxplots
    set style fill solid 0.25 border -1
    set key outside
    unset logscale y
    
    # Lista de datasets únicos (deberías ajustar esto según tus datos)
    datasets = system("awk -F',' 'NR>1 {print $1}' results_stats.csv | sort -u | tr '\n' ' '")
    
    # Métricas para boxplots
    boxplot_metrics = "Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB Nodos_Visitados Nodos_Totales"
    
    do for [metric in boxplot_metrics] {
        # Convert metric name to Title Case for display
        metric_display = metric
        metric_display = (metric eq "Tiempo_Insercion_ms" ? "Tiempo Inserción (ms)" : metric_display)
        metric_display = (metric eq "Tiempo_Busqueda_ms" ? "Tiempo Búsqueda (ms)" : metric_display)
        metric_display = (metric eq "Memoria_KB" ? "Memoria (KB)" : metric_display)
        metric_display = (metric eq "Nodos_Visitados" ? "Nodos Visitados" : metric_display)
        metric_display = (metric eq "Nodos_Totales" ? "Nodos Totales" : metric_display)
        
        set output sprintf("plot_boxplot_%s.png", metric)
        set title sprintf("Distribución de %s por estructura y dataset (Boxplot)", metric_display)
        set ylabel metric_display
        set xlabel "Dataset"
        set xtics rotate by -30 font ",9"
        set yrange [*:*]
        
        # Usar escala logarítmica para métricas grandes
        if (metric ne "Profundidad_Mediana") {
            set logscale y
        }
        
        # Preparar datos para boxplot manual usando candlesticks
        set style data candlesticks
        set style candlesticks whiskerbars
        
        # Plot con candlesticks: x min q1 median q3 max
        # Separar NaiveTrie y RadixTrie
        plot stats_file using 0:6:9:5:10:xtic(sprintf("%s\n%s", format_dataset(strcol(1)), stringcolumn(3))) \
                every ::0 \
                title "" \
                whiskerbars lw 2 lc rgb "blue" notitle, \
             stats_file using ($0-0.3):(stringcolumn(3) eq "NaiveTrie" && stringcolumn(4) eq metric ? $6 : 1/0):9:5:10 \
                title "NaiveTrie" \
                whiskerbars lw 1.5 lc rgb "blue", \
             stats_file using ($0+0.3):(stringcolumn(3) eq "RadixTrie" && stringcolumn(4) eq metric ? $6 : 1/0):9:5:10 \
                title "RadixTrie" \
                whiskerbars lw 1.5 lc rgb "red"
        
        unset logscale y
        unset output
    }
    
    # Gráficos adicionales: comparar media con barras de error
    print "Generando gráficos de medias con barras de error..."
    
    set style data histogram
    set style histogram errorbars gap 1 lw 1
    set style fill solid border -1
    set boxwidth 0.9
    
    do for [metric in boxplot_metrics] {
        # Convert metric name to Title Case for display
        metric_display = metric
        metric_display = (metric eq "Tiempo_Insercion_ms" ? "Tiempo Inserción (ms)" : metric_display)
        metric_display = (metric eq "Tiempo_Busqueda_ms" ? "Tiempo Búsqueda (ms)" : metric_display)
        metric_display = (metric eq "Memoria_KB" ? "Memoria (KB)" : metric_display)
        metric_display = (metric eq "Nodos_Visitados" ? "Nodos Visitados" : metric_display)
        metric_display = (metric eq "Nodos_Totales" ? "Nodos Totales" : metric_display)
        
        set output sprintf("plot_means_%s.png", metric)
        set title sprintf("Media de %s con desviación estándar", metric_display)
        set ylabel metric_display
        set xlabel "Dataset"
        set xtics rotate by -30 font ",9"
        
        if (metric ne "Profundidad_Mediana") {
            set logscale y
        }
        
        # Histograma con barras de error: dataset estructura mean std_dev
        plot stats_file using \
                (stringcolumn(3) eq "NaiveTrie" && stringcolumn(4) eq metric ? $5 : 1/0):7:xtic(format_dataset(strcol(1))) \
                title "NaiveTrie" lc rgb "blue", \
             stats_file using \
                (stringcolumn(3) eq "RadixTrie" && stringcolumn(4) eq metric ? $5 : 1/0):7 \
                title "RadixTrie" lc rgb "red"
        
        unset logscale y
        unset output
    }
    
    print "Boxplots y gráficos de medias generados exitosamente."
} else {
    print "NOTA: No se encontró results_stats.csv. Los boxplots no se generarán."
    print "      Para generar boxplots, ejecute: ./bin/experiments --stats 5"
    print "      (donde 5 es el número de corridas por experimento)"
}

# ================================================================================
# SECCIÓN 3B: BOXPLOTS SIMPLES CON DATOS ACTUALES
# ================================================================================
print "Generando boxplots simples con datos de results.csv..."

set style fill solid 0.25 border -1
set style data boxplot
set boxwidth 0.2
set pointsize 0.5

# Métricas para boxplots
boxplot_metrics = "Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB Nodos_Totales Nodos_Visitados"

do for [metric in boxplot_metrics] {
    col = get_column(metric)
    
    # Convert metric name to Title Case for display
    metric_display = metric
    metric_display = (metric eq "Tiempo_Insercion_ms" ? "Tiempo Inserción (ms)" : metric_display)
    metric_display = (metric eq "Tiempo_Busqueda_ms" ? "Tiempo Búsqueda (ms)" : metric_display)
    metric_display = (metric eq "Memoria_KB" ? "Memoria (KB)" : metric_display)
    metric_display = (metric eq "Nodos_Visitados" ? "Nodos Visitados" : metric_display)
    metric_display = (metric eq "Nodos_Totales" ? "Nodos Totales" : metric_display)
    
    set output sprintf("plot_boxplot_%s.png", metric)
    set title sprintf("Distribución de %s por dataset y estructura (Boxplot)", metric_display)
    set ylabel metric_display
    set xlabel "Dataset"
    set xtics rotate by -30 font ",9"
    set yrange [*:*]
    
    # Usar escala logarítmica para métricas grandes
    if (metric ne "Profundidad_Mediana") {
        set logscale y
    }
    
    # Crear boxplots para cada dataset - posiciones pares para NaiveTrie, impares para RadixTrie
    plot 'results.csv' every 2::1 using (0.8):(column(col)):xtic(format_dataset(strcol(1))) title "NaiveTrie" lc rgb "blue", \
         'results.csv' every 2::2 using (1.2):(column(col)) title "RadixTrie" lc rgb "red", \
         'results.csv' every 2::3 using (2.8):(column(col)):xtic(format_dataset(strcol(1))) notitle lc rgb "blue", \
         'results.csv' every 2::4 using (3.2):(column(col)) notitle lc rgb "red", \
         'results.csv' every 2::5 using (4.8):(column(col)):xtic(format_dataset(strcol(1))) notitle lc rgb "blue", \
         'results.csv' every 2::6 using (5.2):(column(col)) notitle lc rgb "red", \
         'results.csv' every 2::7 using (6.8):(column(col)):xtic(format_dataset(strcol(1))) notitle lc rgb "blue", \
         'results.csv' every 2::8 using (7.2):(column(col)) notitle lc rgb "red", \
         'results.csv' every 2::9 using (8.8):(column(col)):xtic(format_dataset(strcol(1))) notitle lc rgb "blue", \
         'results.csv' every 2::10 using (9.2):(column(col)) notitle lc rgb "red", \
         'results.csv' every 2::11 using (10.8):(column(col)):xtic(format_dataset(strcol(1))) notitle lc rgb "blue", \
         'results.csv' every 2::12 using (11.2):(column(col)) notitle lc rgb "red", \
         'results.csv' every 2::13 using (12.8):(column(col)):xtic(format_dataset(strcol(1))) notitle lc rgb "blue", \
         'results.csv' every 2::14 using (13.2):(column(col)) notitle lc rgb "red"
    
    unset logscale y
    unset output
}

print "Boxplots simples generados exitosamente."

# ================================================================================
# SECCIÓN 4: GRÁFICOS COMPARATIVOS ADICIONALES
# ================================================================================
print "Generando gráficos comparativos adicionales..."

# Gráfico de eficiencia: Tiempo de búsqueda vs Memoria
set output "plot_efficiency_search_vs_memory.png"
set title "Eficiencia: Tiempo de Búsqueda vs Memoria Utilizada"
set xlabel "Memoria (KB)"
set ylabel "Tiempo de Búsqueda (ms)"
set logscale x
set logscale y
set key top left
set style data points
set pointsize 2

plot 'results.csv' every ::1 using 11:10:(stringcolumn(3) eq "NaiveTrie" ? 1 : 1/0) \
        title "NaiveTrie" pt 7 lc rgb "blue", \
     'results.csv' every ::1 using 11:10:(stringcolumn(3) eq "RadixTrie" ? 1 : 1/0) \
        title "RadixTrie" pt 5 lc rgb "red"

unset logscale x
unset logscale y
unset output

# Gráfico de eficiencia: Tiempo de inserción vs Memoria
set output "plot_efficiency_insert_vs_memory.png"
set title "Eficiencia: Tiempo de Inserción vs Memoria Utilizada"
set xlabel "Memoria (KB)"
set ylabel "Tiempo de Inserción (ms)"
set logscale x
set logscale y

plot 'results.csv' every ::1 using 11:9:(stringcolumn(3) eq "NaiveTrie" ? 1 : 1/0) \
        title "NaiveTrie" pt 7 lc rgb "blue", \
     'results.csv' every ::1 using 11:9:(stringcolumn(3) eq "RadixTrie" ? 1 : 1/0) \
        title "RadixTrie" pt 5 lc rgb "red"

unset logscale x
unset logscale y
unset output

# Gráfico de nodos visitados por palabra encontrada
set output "plot_nodes_per_found_word.png"
set title "Eficiencia de Búsqueda: Nodos Visitados por Palabra Encontrada"
set xlabel "Dataset"
set ylabel "Nodos Visitados / Palabras Encontradas"
set xtics rotate by -30 font ",9"
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1

plot 'results.csv' every 2::1 using (column(13)/column(8)):xtic(format_dataset(strcol(1))) \
        title "NaiveTrie", \
     'results.csv' every 2::2 using (column(13)/column(8)) \
        title "RadixTrie"

unset output

print ""
print "==============================================="
print "  Generación de gráficos completada"
print "==============================================="
print "Gráficos generados:"
print "  - Histogramas: plot_histogram_*.png"
print "  - Líneas: plot_lines_*.png"
print "  - Boxplots: plot_boxplot_*.png (si hay datos estadísticos)"
print "  - Comparativos: plot_efficiency_*.png, plot_nodes_per_found_word.png"
print "==============================================="
