# ==============================
#  GNUplot script: compare_metrics_aligned_fixed.gnuplot
# ==============================

set datafile separator ","
set key outside
set grid
set style data linespoints
set pointsize 1.5
set term pngcairo size 1200,800
set boxwidth 0.9
set xlabel "Dataset / Modo"
set xtics rotate by -30 font ",9"

# Function to map metric names to column numbers
get_column(metric) = \
    (metric eq "Tiempo_Insercion_ms" ? 6 : \
    (metric eq "Tiempo_Busqueda_ms" ? 7 : \
    (metric eq "Memoria_KB" ? 8 : \
    (metric eq "Porcentage_Memoria" ? 8 : 0))))

metrics = "Tiempo_Insercion_ms Tiempo_Busqueda_ms Memoria_KB Porcentage_Memoria"

set style increment user

do for [metric in metrics] {
    col = get_column(metric)
    if (col == 0) continue

    set output sprintf("plot_%s.png", metric)
    set title sprintf("Comparativa de %s entre estructuras, modos y datasets", metric)
    set ylabel metric

    if (metric eq "Porcentage_Memoria") {
        unset logscale y
    } else {
        set logscale y
    }

    # Numeric X index per unique Dataset/Modo pair
    plot \
        '< awk -F"," "NR>1 {print NR, $0}" results.csv' \
            using 1:(if (stringcolumn(3) eq "NaiveTrie") column(col) else 1/0):xticlabels(sprintf("%s (Modo %s)", stringcolumn(1), stringcolumn(2))) \
            title "NaiveTrie" lw 2 pt 7, \
        '< awk -F"," "NR>1 {print NR, $0}" results.csv' \
            using 1:(if (stringcolumn(3) eq "RadixTrie") column(col) else 1/0) \
            title "RadixTrie" lw 2 pt 5

    unset output
}

unset logscale y
