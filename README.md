# Projecte Trie - Estructura de Dades en C++

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)](Makefile)

## 📋 Descripció

Aquest projecte implementa una estructura de dades **Trie** (també conegut com a arbre de prefixos) en C++17. Un Trie és una estructura de dades especialitzada per emmagatzemar i cercar strings de manera eficient, especialment útil per:

- Autocompletat de text
- Correctors ortogràfics
- Diccionaris i cercadors de paraules
- Sistemes de cerca per prefixos

## 🏗️ Estructura del Projecte

```
Proyecto/
├── sources/
│   ├── include/
│   │   └── trie.h          # Definició de la classe Trie
│   └── src/
│       ├── trie.cpp        # Implementació del Trie
│       └── main.cpp        # Programa de demostració
├── bin/                    # Fitxers executables (generat automàticament)
├── tests/                  # Tests unitaris
│   ├── test_framework.h    # Framework de testing personalitzat
│   ├── test_trie.cpp       # Tests bàsics del Trie
│   └── test_trie_advanced.cpp # Tests avançats
├── docs/                   # Documentació del projecte
│   ├── README.md           # Guia de la documentació
│   ├── latex/              # Fitxers LaTeX i auxiliars
│   └── pdf/                # Documents PDF compilats
├── Makefile               # Sistema de construcció
└── README.md              # Aquest fitxer
```

## ✨ Característiques

### Operacions Principals
- **Inserció**: Afegir paraules al Trie - `O(m)` on `m` és la longitud de la paraula
- **Cerca**: Verificar si una paraula existeix - `O(m)`
- **Cerca de prefixos**: Comprovar si existeixen paraules amb un prefix - `O(m)`
- **Eliminació**: Eliminar paraules del Trie - `O(m)`
- **Autocompletat**: Obtenir totes les paraules amb un prefix específic

### Funcionalitats Addicionals
- Gestió automàtica de memòria amb smart pointers
- Mètodes per obtenir totes les paraules emmagatzemades
- Operacions per netejar i comprovar si el Trie està buit
- Implementació robusta amb maneig d'errors

### Sistema de Tests Complet
- **Framework de testing personalitzat** amb output colorejat
- **16 tests bàsics** que cobreixen totes les operacions
- **8 tests avançats** per casos complexos i stress testing
- **Timing precís** i estadístiques detallades
- **Integració completa** amb el Makefile

## 🚀 Compilació i Execució

### Requisits del Sistema
- **Compilador**: g++ amb suport per C++17 o superior
- **Sistema**: Linux/Unix (testejat en Ubuntu/Debian)
- **Eines**: make

### Instal·lació de Dependències
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential g++ make

# (Opcional) Per debugging i documentació
sudo apt install valgrind doxygen
```

### Compilació
```bash
# Compilació estàndard
make

# O simplement
make all

# Compilació amb informació de debug
make debug
```

### Execució
```bash
# Executar el programa de demostració
make run

# Executar en mode debug
make run-debug

# Executar amb Valgrind (detectar memory leaks)
make valgrind
```

### Execució de Tests
```bash
# Executar tots els tests (recomanat)
make run-all-tests

# Executar només tests bàsics
make run-tests

# Executar només tests avançats
make run-tests-advanced

# Compilar tests en mode debug
make test-debug
```

### Neteja
```bash
# Netejar fitxers objecte i executables
make clean

# Neteja completa (inclou documentació)
make distclean
```

## 📖 Ús de l'API

### Exemple Bàsic

```cpp
#include "include/trie.h"
#include <iostream>
#include <vector>

int main() {
    // Crear un nou Trie
    Trie trie;
    
    // Inserir paraules
    trie.insert("gat");
    trie.insert("gats");
    trie.insert("casa");
    trie.insert("cases");
    
    // Cercar paraules
    bool found = trie.search("gat");        // true
    bool notFound = trie.search("gato");    // false
    
    // Comprovar prefixos
    bool hasPrefix = trie.starts_with("ga"); // true
    
    // Obtenir paraules amb prefix
    std::vector<std::string> words = trie.get_words_with_prefix("ga");
    // Retorna: ["gat", "gats"]
    
    // Eliminar paraules
    trie.remove("gats");
    
    return 0;
}
```

### Mètodes Disponibles

| Mètode | Descripció | Complexitat |
|--------|------------|-------------|
| `insert(word)` | Insereix una paraula | O(m) |
| `search(word)` | Cerca una paraula | O(m) |
| `starts_with(prefix)` | Comprova si existeix un prefix | O(m) |
| `remove(word)` | Elimina una paraula | O(m) |
| `get_words_with_prefix(prefix)` | Obté paraules amb prefix | O(ANSLEN) |
| `get_all_words()` | Obté totes les paraules | O(ANSLEN) |
| `empty()` | Comprova si està buit | O(1) |
| `clear()` | Neteja tot el contingut | O(1) |

*On `m` és la longitud de la paraula/prefix i `ANSLEN` és la longitud total de la resposta*

## 🧪 Execució de la Demostració

Quan executis `make run`, veuràs una demostració completa que inclou:

```
=== Demostració del Trie ===
Inserint paraules al Trie...
  Inserit: gat
  Inserit: gats
  Inserit: gatet
  ...

Totes les paraules al Trie:
  - casa
  - casat
  - cases
  - col
  - color
  - colors
  - cotxe
  - cotxes
  - gat
  - gatet
  - gats

Cercant paraules:
  'gat': TROBADA
  'gato': NO TROBADA
  ...
```

## 🔧 Personalització i Extensió

### Afegir Nous Tests
Crea fitxers de test a la carpeta `tests/` seguint el patró:
```cpp
// tests/test_trie_basic.cpp
#include "../sources/include/trie.h"
// Implementar tests
```

### Generar Documentació
Si tens Doxygen instal·lat:
```bash
make docs
```

## 🐛 Debugging

### Compilar amb Debug
```bash
make debug
```

### Utilitzar Valgrind
```bash
make valgrind
```

### Opcions de Debug Addicionals
El programa compilat amb `make debug` inclou:
- Símbols de debug per GDB
- Comprovacions addicionals en temps d'execució
- Informació detallada per al profiling

## 📊 Rendiment

### Complexitats Temporals
- **Inserció**: O(m) on m és la longitud de la paraula
- **Cerca**: O(m)
- **Cerca de prefix**: O(m)
- **Eliminació**: O(m)
- **Autocompletat**: O(ANSLEN) on ANSLEN és la longitud total de la resposta

### Complexitat Espacial
- **Espai**: O(ALPHABET_SIZE × N × M) en el pitjor cas
  - ALPHABET_SIZE: Mida de l'alfabet (256 per char)
  - N: Nombre de paraules
  - M: Longitud mitjana de les paraules

## 🤝 Contribucions

Per contribuir al projecte:

1. Fork del repositori
2. Crea una branca per la nova funcionalitat (`git checkout -b feature/nova-funcionalitat`)
3. Commit dels canvis (`git commit -am 'Afegir nova funcionalitat'`)
4. Push a la branca (`git push origin feature/nova-funcionalitat`)
5. Crear un Pull Request

## 📄 Llicència

Aquest projecte està sota la llicència MIT. Veure el fitxer `LICENSE` per més detalls.

## 👥 Autors

- **Projecte Professional** - Implementació inicial

## 🎯 Futures Millores

- [ ] Implementar tests unitaris complets
- [ ] Afegir suport per Unicode
- [ ] Optimitzar l'ús de memòria amb compressed tries
- [ ] Implementar serialització/deserialització
- [ ] Afegir benchmarks de rendiment
- [ ] Suport per wildcards en les cerques

## 📞 Suport

Si tens preguntes o problemes:
1. Revisa la documentació en aquest README
2. Comprova els issues existents
3. Crea un nou issue amb detalls del problema

---

**Desenvolupat amb ❤️ per al projecte professional**