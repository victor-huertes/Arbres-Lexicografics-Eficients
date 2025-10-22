# ==============================
#  GNUplot script: compare_metrics_log.gnuplot
# ==============================

set datafile separator ","
set key outside
set grid
set term pngcairo size 1200,800
set logscale y
set style data linespoints
set pointsize 1.5
set xlabel "Dataset / Modo"
set xtics rotate by -30 font ",9"

metrics = "TiempoInsercionms TiempoBusqueda_ms Memoria_KB"

do for [metric in metrics] {
    set output sprintf("plot_%s_log.png", metric)
    set title sprintf("Comparativa (escala log) de %s entre estructuras, modos y datasets", metric)
    set ylabel sprintf("%s (escala logarítmica)", metric)

    plot \
        'results.csv' using (stringcolumn("Estructura") eq "NaiveTrie" ? \
            column(metric) : 1/0):xticlabels(sprintf("%s (Modo %s)", stringcolumn("Dataset"), stringcolumn("Modo"))) \
            title "NaiveTrie" lw 2 pt 7, \
        'results.csv' using (stringcolumn("Estructura") eq "RadixTrie" ? \
            column(metric) : 1/0):xticlabels(sprintf("%s (Modo %s)", stringcolumn("Dataset"), stringcolumn("Modo"))) \
            title "RadixTrie" lw 2 pt 5
}

unset output
unset logscale y
