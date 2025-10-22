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
# Columns: 1:Dataset  2:Modo  3:Estructura  4:Palabras_Buscadas  5:Palabras_Encontradas
#          6:Tiempo_Insercion_ms  7:Tiempo_Busqueda_ms  8:Memoria_KB  9:Exito
get_column(metric) = \
    (metric eq "Tiempo_Insercion_ms" ? 6 : \
    (metric eq "Tiempo_Busqueda_ms" ? 7 : \
    (metric eq "Memoria_KB" ? 8 : \
    (metric eq "Porcentage_Memoria" ? 8 : 0))))

metrics = "Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB Porcentage_Memoria"

do for [metric in metrics] {
    col = get_column(metric)
    if (col == 0) continue

    set output sprintf("plot_%s.png", metric)
    set title sprintf("Comparativa de %s entre estructuras, modos y datasets", metric)
    set ylabel metric
    set xlabel "Dataset / Modo"
    set xtics rotate by -30 font ",9"

    # Use logscale for all except Porcentage_Memoria
    if (metric eq "Porcentage_Memoria") {
        unset logscale y
    } else {
        set logscale y
    }

    plot \
        'results.csv' using (stringcolumn("Estructura") eq "NaiveTrie" ? column(col) : 1/0):xticlabels(sprintf("%s (Modo %s)", stringcolumn("Dataset"), stringcolumn("Modo"))) title "NaiveTrie", \
        'results.csv' using (stringcolumn("Estructura") eq "RadixTrie" ? column(col) : 1/0):xticlabels(sprintf("%s (Modo %s)", stringcolumn("Dataset"), stringcolumn("Modo"))) title "RadixTrie"

    unset output
}

unset logscale y
