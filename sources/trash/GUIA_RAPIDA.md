# Guía Rápida - Sistema de Gráficos

## 🚀 Inicio Rápido

### Generar gráficos básicos (1 ejecución)
```bash
make
./bin/experiments --csv > output/results.csv
cd output/
gnuplot ../sources/utils/script.gnuplot
```

### Generar gráficos con estadísticas (múltiples ejecuciones)
```bash
make
./bin/experiments --stats 5
cd output/
gnuplot ../sources/utils/script.gnuplot
```

## 📊 Tipos de Gráficos

| Tipo | Archivos | Requiere --stats | Uso |
|------|----------|------------------|-----|
| **Histogramas** | `plot_histogram_*.png` | ❌ | Comparar valores absolutos |
| **Líneas** | `plot_lines_*.png` | ❌ | Ver tendencias |
| **Boxplots** | `plot_boxplot_*.png` | ✅ | Analizar variabilidad |
| **Medias** | `plot_means_*.png` | ✅ | Comparar con incertidumbre |
| **Eficiencia** | `plot_efficiency_*.png` | ❌ | Ver trade-offs |

## 🎯 Comandos Principales

### Compilar
```bash
make              # Compilar todo
make clean        # Limpiar binarios
```

### Ejecutar Experimentos
```bash
./bin/experiments                    # Modo normal (verbose)
./bin/experiments --csv              # Modo CSV (1 corrida)
./bin/experiments --stats 5          # Modo estadísticas (5 corridas)
./bin/experiments --stats 10         # 10 corridas (más preciso)
```

### Generar Gráficos
```bash
cd output/
gnuplot ../sources/utils/script.gnuplot
```

## 📁 Archivos Importantes

### Entrada
- `data/*_search.txt` - Palabras a buscar
- `data/*.txt` - Datasets para construir tries

### Salida
- `output/results.csv` - Resultados individuales
- `output/results_stats.csv` - Estadísticas agregadas
- `output/plot_*.png` - Gráficos generados

### Scripts
- `sources/utils/script.gnuplot` - Script principal de gráficos
- `sources/src/experiments.cpp` - Programa de experimentos

## 💡 Consejos

### ¿Cuántas corridas usar?
- **3 corridas**: Prueba rápida
- **5 corridas**: Análisis normal (recomendado)
- **10 corridas**: Análisis detallado
- **20+ corridas**: Para publicaciones

### Optimizar tiempo
```bash
# Solo datasets pequeños (para pruebas)
# Editar experiments.cpp y comentar datasets grandes

# Ver solo tiempo de ejecución
time ./bin/experiments --stats 5
```

### Verificar resultados
```bash
# Ver estadísticas generadas
head -20 output/results_stats.csv

# Contar gráficos generados
ls output/plot_*.png | wc -l

# Ver lista de gráficos
ls output/plot_*.png
```

## 🔍 Interpretación Rápida

### Boxplots
- **Caja estrecha** → Resultados consistentes ✅
- **Caja ancha** → Alta variabilidad ⚠️
- **Puntos fuera** → Outliers (valores atípicos)

### Barras de Error
- **Barras cortas** → Mediciones precisas ✅
- **Barras largas** → Resultados variables ⚠️

### Gráficos de Eficiencia
- **Esquina inferior izquierda** → Mejor (menos tiempo/memoria) ✅
- **Esquina superior derecha** → Peor (más tiempo/memoria) ❌

## 🐛 Solución de Problemas

### No se generan boxplots
```bash
# Causa: Falta results_stats.csv
# Solución:
./bin/experiments --stats 5
```

### Error de compilación
```bash
make clean
make
```

### GNUplot no encuentra archivos
```bash
# Ejecutar desde output/
cd output/
gnuplot ../sources/utils/script.gnuplot
```

### Gráficos vacíos
```bash
# Verificar CSV tiene datos
head output/results.csv
```

## 📖 Documentación Completa

Ver `docs/GRAFICAS_README.md` para:
- Explicación detallada de cada gráfico
- Formato de archivos CSV
- Personalización avanzada
- Más ejemplos

## 🎨 Personalización Rápida

### Cambiar tamaño de gráficos
Editar `sources/utils/script.gnuplot`:
```gnuplot
set term pngcairo size 1920,1080  # Default: 1200,800
```

### Cambiar número de corridas predeterminado
Editar `sources/src/experiments.cpp`:
```cpp
int numRuns = 10;  // Default: 5
```

### Agregar dataset
Editar `sources/src/experiments.cpp`:
```cpp
vector<string> insertDatasetPath = {
    // ... datasets existentes ...
    "data/mi_dataset.txt"  // Añadir aquí
};
vector<string> searchDatasetPath = {
    // ... datasets existentes ...
    "data/mi_dataset_search.txt"  // Añadir aquí
};
```

## ⚡ Workflow Recomendado

1. **Desarrollo/Pruebas**
   ```bash
   ./bin/experiments --csv > output/results.csv
   cd output && gnuplot ../sources/utils/script.gnuplot
   ```

2. **Análisis Serio**
   ```bash
   ./bin/experiments --stats 10
   cd output && gnuplot ../sources/utils/script.gnuplot
   ```

3. **Paper/Presentación**
   ```bash
   ./bin/experiments --stats 20
   cd output && gnuplot ../sources/utils/script.gnuplot
   # Personalizar colores/tamaños en script.gnuplot
   ```

## 📞 Ayuda Adicional

- Ver ejemplos: `docs/GRAFICAS_README.md`
- Ver cambios: `docs/RESUMEN_CAMBIOS.md`
- Comentarios en código: `sources/utils/script.gnuplot`
