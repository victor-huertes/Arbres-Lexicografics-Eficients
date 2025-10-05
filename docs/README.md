# Documentació del Projecte Trie

Aquesta carpeta conté tota la documentació del projecte organitzada de manera professional.

## 📁 Estructura de la Documentació

```
docs/
├── README.md           # Aquest fitxer
├── latex/             # Fitxers LaTeX del projecte
│   ├── main.tex       # Document principal LaTeX
│   ├── informe_tecnic.tex
│   ├── informe_autoaprenentatge.tex
│   ├── informe_apendixs.tex
│   └── *.aux, *.log   # Fitxers auxiliars de compilació LaTeX
├── pdf/               # Documents PDF generats
│   ├── main.pdf       # Document principal compilat
│   └── Proyecto.pdf   # Document del projecte
└── html/              # Documentació HTML (generada per Doxygen)
```

## 📋 Tipus de Documentació

### 📄 Documentació LaTeX
- **Ubicació**: `latex/`
- **Contingut**: Informes tècnics, documentació acadèmica
- **Format**: Fitxers `.tex` i compilats auxiliars

### 📑 Documents PDF
- **Ubicació**: `pdf/`
- **Contingut**: Versions finals compilades dels documents
- **Format**: Fitxers `.pdf`

### 🌐 Documentació HTML (Futura)
- **Ubicació**: `html/` (es generarà amb Doxygen)
- **Contingut**: Documentació de l'API del codi C++
- **Generació**: `make docs`

## 🔧 Com Generar Documentació

### Compilar LaTeX
```bash
cd docs/latex
pdflatex main.tex
```

### Generar Documentació del Codi
```bash
# Des del directori arrel del projecte
make docs
```

## 📝 Fitxers de Documentació

| Fitxer | Descripció |
|--------|------------|
| `main.tex` | Document principal LaTeX |
| `informe_tecnic.tex` | Informe tècnic del projecte |
| `informe_autoaprenentatge.tex` | Documentació d'autoaprenentatge |
| `informe_apendixs.tex` | Apèndixs del projecte |
| `main.pdf` | Document compilat principal |
| `Proyecto.pdf` | Document del projecte |

## 🎯 Manteniment

- Els fitxers auxiliars de LaTeX (`.aux`, `.log`, etc.) es mantenen aquí per facilitar la recompilació
- Els PDFs es regeneren automàticament quan es modifiquen els fitxers LaTeX
- La documentació HTML del codi s'actualitza amb `make docs`

---

**Nota**: Aquesta organització segueix les millors pràctiques per projectes professionals, separant clarament la documentació del codi font.