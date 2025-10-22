# ==============================
#  GNUplot script: compare_metrics.gnuplot
# ==============================

set datafile separator ","
set key outside
set grid
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set term pngcairo size 1200,800

# Output filenames will include metric name dynamically
metrics = "Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB"

# Loop over each metric
do for [metric in metrics] {
    set output sprintf("plot_%s.png", metric)
    set title sprintf("Comparativa de %s entre estructuras, modos y datasets", metric)
    set ylabel metric
    set xlabel "Dataset / Modo"
    
    # Create grouped x-axis labels combining Dataset and Modo
    set xtics rotate by -30
    set xtics font ",9"

    plot for [struct in "NaiveTrie RadixTrie"] \
        'results.csv' using (stringcolumn("Estructura") eq struct ? \
        column(metric) : 1/0):xticlabels(sprintf("%s (Modo %s)", stringcolumn("Dataset"), stringcolumn("Modo"))) \
        title struct
}

unset output
