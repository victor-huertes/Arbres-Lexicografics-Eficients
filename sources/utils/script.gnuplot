# ================================================================================
#  GNUplot script: Análisis de Tries - Versión Estandarizada
# ================================================================================
# Genera múltiples tipos de visualizaciones:
#   1. Histogramas (con y sin escala logarítmica)
#   2. Gráficos de líneas (con y sin escala logarítmica)
#   3. Gráficos comparativos de eficiencia
# ================================================================================

set datafile separator ","
set key outside
set grid
set term pngcairo size 1200,800

# Formatear nombres de datasets
format_dataset(name) = \
    (name eq "lorem_ipsum" ? "Lorem Ipsum" : \
    (name eq "bee_movie" ? "Bee Movie" : \
    (name eq "alice_wonderland" ? "Alice Wonderland" : \
    (name eq "moby_dick" ? "Moby Dick" : \
    (name eq "words_alpha" ? "Words Alpha" : \
    (name eq "dna_genome" ? "DNA Genome" : \
    (name eq "wikipedia_titles" ? "Wikipedia Titles" : name)))))))

# Mapear métricas a columnas
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

# Formatear nombres de métricas
format_metric(metric) = \
    (metric eq "Nodos_Totales" ? "Nodos Totales" : \
    (metric eq "Profundidad_Maxima" ? "Profundidad Máxima" : \
    (metric eq "Profundidad_Mediana" ? "Profundidad Mediana" : \
    (metric eq "Palabras_Buscadas" ? "Palabras Buscadas" : \
    (metric eq "Palabras_Encontradas" ? "Palabras Encontradas" : \
    (metric eq "Tiempo_Insercion_ms" ? "Tiempo Inserción (ms)" : \
    (metric eq "Tiempo_Busqueda_ms" ? "Tiempo Búsqueda (ms)" : \
    (metric eq "Memoria_KB" ? "Memoria (KB)" : \
    (metric eq "Porcentaje_Memoria" ? "Porcentaje Memoria" : \
    (metric eq "Nodos_Visitados" ? "Nodos Visitados" : metric))))))))))

metrics = "Nodos_Totales Profundidad_Maxima Profundidad_Mediana Palabras_Buscadas Palabras_Encontradas Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB Porcentaje_Memoria Nodos_Visitados"
logscale_metrics = "Nodos_Totales Palabras_Buscadas Palabras_Encontradas Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB Nodos_Visitados"

# Histogramas
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set xlabel "Dataset"
set xtics rotate by -30 font ",9"

do for [metric in metrics] {
    col = get_column(metric)
    metric_display = format_metric(metric)
    
    set output sprintf("plot_histogram_%s.png", metric)
    set title sprintf("Comparativa de %s (Histograma)", metric_display)
    set ylabel metric_display
    unset logscale y
    
    plot 'results.csv' every 2::1 using col:xtic(format_dataset(strcol(1))) title "NaiveTrie", \
         'results.csv' every 2::2 using col title "RadixTrie"
    
    unset output
}

do for [metric in logscale_metrics] {
    col = get_column(metric)
    metric_display = format_metric(metric)
    
    set output sprintf("plot_histogram_log_%s.png", metric)
    set title sprintf("Comparativa de %s - Escala Log (Histograma)", metric_display)
    set ylabel sprintf("%s (escala log)", metric_display)
    set logscale y
    
    plot 'results.csv' every 2::1 using col:xtic(format_dataset(strcol(1))) title "NaiveTrie", \
         'results.csv' every 2::2 using col title "RadixTrie"
    
    unset logscale y
    unset output
}

# Líneas
set style data linespoints
set pointsize 1.5
set xlabel "Dataset"
set xtics rotate by -30 font ",9"

do for [metric in metrics] {
    col = get_column(metric)
    metric_display = format_metric(metric)
    
    set output sprintf("plot_lines_%s.png", metric)
    set title sprintf("Comparativa de %s (Líneas)", metric_display)
    set ylabel metric_display
    unset logscale y
    
    plot 'results.csv' every ::1 using 0:(stringcolumn(3) eq "NaiveTrie" ? column(col) : 1/0):xtic(format_dataset(strcol(1))) \
            title "NaiveTrie" lw 2 pt 7, \
         'results.csv' every ::1 using 0:(stringcolumn(3) eq "RadixTrie" ? column(col) : 1/0) \
            title "RadixTrie" lw 2 pt 5
    
    unset output
}

do for [metric in logscale_metrics] {
    col = get_column(metric)
    metric_display = format_metric(metric)
    
    set output sprintf("plot_lines_log_%s.png", metric)
    set title sprintf("Comparativa de %s - Escala Log (Líneas)", metric_display)
    set ylabel sprintf("%s (escala log)", metric_display)
    set logscale y
    
    plot 'results.csv' every ::1 using 0:(stringcolumn(3) eq "NaiveTrie" ? column(col) : 1/0):xtic(format_dataset(strcol(1))) \
            title "NaiveTrie" lw 2 pt 7, \
         'results.csv' every ::1 using 0:(stringcolumn(3) eq "RadixTrie" ? column(col) : 1/0) \
            title "RadixTrie" lw 2 pt 5
    
    unset logscale y
    unset output
}

# Gráficos comparativos
set style data points
set pointsize 2
set key top left

set output "plot_efficiency_search_vs_memory.png"
set title "Eficiencia: Tiempo de Búsqueda vs Memoria Utilizada"
set xlabel "Memoria (KB)"
set ylabel "Tiempo de Búsqueda (ms)"
set logscale xy

plot 'results.csv' every ::1 using 11:10:(stringcolumn(3) eq "NaiveTrie" ? 1 : 1/0) \
        title "NaiveTrie" pt 7 lc rgb "blue", \
     'results.csv' every ::1 using 11:10:(stringcolumn(3) eq "RadixTrie" ? 1 : 1/0) \
        title "RadixTrie" pt 5 lc rgb "red"

unset output

set output "plot_efficiency_insert_vs_memory.png"
set title "Eficiencia: Tiempo de Inserción vs Memoria Utilizada"
set xlabel "Memoria (KB)"
set ylabel "Tiempo de Inserción (ms)"

plot 'results.csv' every ::1 using 11:9:(stringcolumn(3) eq "NaiveTrie" ? 1 : 1/0) \
        title "NaiveTrie" pt 7 lc rgb "blue", \
     'results.csv' every ::1 using 11:9:(stringcolumn(3) eq "RadixTrie" ? 1 : 1/0) \
        title "RadixTrie" pt 5 lc rgb "red"

unset logscale xy
unset output

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
