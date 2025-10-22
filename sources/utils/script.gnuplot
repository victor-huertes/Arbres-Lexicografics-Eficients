# ==============================
#  GNUplot script: compare_metrics_mixedscale.gnuplot
# ==============================

set datafile separator ","
set key outside
set grid
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set term pngcairo size 1200,800

# Function to map metric names to column numbers (adjust if needed)
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

do for [metric in metrics] {
    col = get_column(metric)
    
    set output sprintf("plot_%s.png", metric)
    set title sprintf("Comparativa de %s entre estructuras y datasets", metric)
    set ylabel metric
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

    plot \
        'results.csv' using (stringcolumn(3) eq "NaiveTrie" ? column(col) : 1/0):xticlabels(1) title "NaiveTrie", \
        'results.csv' using (stringcolumn(3) eq "RadixTrie" ? column(col) : 1/0):xticlabels(1) title "RadixTrie"

    unset output
}

unset logscale y
