# Árboles Lexicográficos Eficientes

Implementación y comparación de estructuras de tries eficientes: **Naive Trie** y **Radix Trie**.

## Requisitos

- Compilador C++ con soporte para C++17 o superior
- GNU Make

## Compilación y Ejecución

### Ejecutar demo interactivo
```bash
make run
```

### Ejecutar experimentos y generar resultados
```bash
make test
```

### Generar archivo CSV con resultados
```bash
make csv
```

### Limpiar archivos compilados
```bash
make clean
```

## Estructura del Proyecto

```
sources/
├── include/            # Archivos de cabecera
│   ├── naive.h         # Naive Trie
│   └── radix.h         # Radix Trie
├── src/                # Implementaciones
│   ├── naive.cpp
│   ├── radix.cpp
│   ├── main.cpp        # Demo interactiva
│   └── experiments.cpp # Experimentos
data/                   # Datasets de prueba
output/                 # Resultados CSV
```

## Autores

Equipo 13

Laura Moreno Valencia - laura.christel.moreno@estudiantat.upc.edu

Víctor Huertes Montes - victor.huertes@estudiantat.upc.edu

Abraham Ruiz Vasquez - abraham.ruiz.vasquez@estudiantat.upc.edu

Pol Riveiro Clarà - pol.riveiro@estudiantat.upc.edu
