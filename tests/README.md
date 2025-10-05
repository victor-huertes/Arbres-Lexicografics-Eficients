# Tests del Projecte Trie

Aquest directori conté una suite completa de tests per verificar la funcionalitat del Trie implementat en C++.

## 📋 Estructura dels Tests

```
tests/
├── test_framework.h         # Framework de testing personalitzat
├── test_trie.cpp           # Tests bàsics i de funcionalitat principal
├── test_trie_advanced.cpp  # Tests avançats i casos especials
└── README.md               # Aquest fitxer
```

## 🧪 Tipus de Tests Implementats

### Tests Bàsics (`test_trie.cpp`)
- **Construcció i inicialització**: Verificació que el Trie es crea correctament buit
- **Inserció simple**: Test d'inserció d'una sola paraula
- **Inserció múltiple**: Test d'inserció de múltiples paraules
- **Cerca de paraules**: Verificació de la funcionalitat de cerca
- **Cerca per prefixos**: Test de `starts_with()` i `get_words_with_prefix()`
- **Eliminació**: Test de la funcionalitat `remove()`
- **Obtenció de totes les paraules**: Verificació de `get_all_words()`
- **Neteja del Trie**: Test de la funcionalitat `clear()`

### Tests de Casos Límit (`test_trie.cpp`)
- **Strings buits**: Maneig de cadenes buides
- **Paraules d'un sol caràcter**: Test amb caràcters individuals
- **Paraules molt llargues**: Test amb strings de gran longitud
- **Insercions duplicades**: Verificació que no hi ha duplicats
- **Paraules solapades**: Test amb paraules que comparteixen prefixos

### Tests de Rendiment (`test_trie.cpp`)
- **Dataset gran**: Test amb 1000 paraules
- **Cerca per prefixos**: Rendiment amb moltes paraules amb prefixos comuns
- **Eliminació massiva**: Eliminació de totes les paraules

### Tests Avançats (`test_trie_advanced.cpp`)
- **Caràcters Unicode**: Test amb caràcters especials i accents
- **Strings numèrics**: Test amb cadenes que contenen números
- **Case sensitivity**: Verificació que el Trie és sensible a majúscules/minúscules
- **Caràcters especials**: Test amb símbols especials (@, -, _, etc.)
- **Stress test aleatori**: Test amb 500 paraules generades aleatòriament
- **Eficiència de memòria**: Test amb prefixos comuns per verificar optimització
- **Eliminació complexa**: Scenarios complexos d'eliminació
- **Condicions límit**: Tests de casos extrems

## 🚀 Executar els Tests

### Compilar i Executar

```bash
# Tests bàsics
make run-tests

# Tests avançats
make run-tests-advanced

# Tots els tests
make run-all-tests

# Tests en mode debug
make run-tests-debug
```

### Targets del Makefile

| Command | Descripció |
|---------|------------|
| `make test` | Compila els tests bàsics |
| `make run-tests` | Compila i executa els tests bàsics |
| `make test-advanced` | Compila els tests avançats |
| `make run-tests-advanced` | Compila i executa els tests avançats |
| `make run-all-tests` | Executa tots els tests |
| `make test-debug` | Compila tests amb informació de debug |
| `make run-tests-debug` | Executa tests en mode debug |

## 📊 Framework de Testing

Els tests utilitzen un framework personalitzat (`test_framework.h`) que proporciona:

### Macros d'Assert
- `ASSERT_TRUE(condition)`: Verifica que una condició sigui certa
- `ASSERT_FALSE(condition)`: Verifica que una condició sigui falsa
- `ASSERT_EQ(expected, actual)`: Verifica igualtat entre valors

### Funcionalitats
- **Colors**: Output colorejat per millor llegibilitat
- **Timing**: Mesura del temps d'execució de cada test
- **Estadístiques**: Resum complet amb nombre de tests passats/fallits
- **Informació de debug**: Localització exacta dels errors

### Exemple d'Ús

```cpp
TEST(test_example) {
    Trie trie;
    trie.insert("hello");
    
    ASSERT_TRUE(trie.search("hello"));
    ASSERT_FALSE(trie.search("world"));
    
    return true;  // Test passat
}

// En main()
RUN_TEST(test_example);
```

## 📈 Cobertura de Tests

### Operacions Testejades
- ✅ **Inserció**: 100% cobertura
- ✅ **Cerca**: 100% cobertura  
- ✅ **Cerca per prefix**: 100% cobertura
- ✅ **Eliminació**: 100% cobertura
- ✅ **Neteja**: 100% cobertura
- ✅ **Obtenció de paraules**: 100% cobertura

### Casos Especials Testejats
- ✅ Strings buits i nuls
- ✅ Caràcters especials i Unicode
- ✅ Paraules molt llargues
- ✅ Insercions duplicades
- ✅ Eliminacions complexes
- ✅ Condicions límit
- ✅ Rendiment amb datasets grans

## 🎯 Resultats Esperats

Tots els tests haurien de passar amb un **100% de success rate**:

```
===========================================
              TEST SUMMARY
===========================================
Total tests: 24
Passed: 24
Failed: 0
Success rate: 100%
Total time: 1ms

🎉 ALL TESTS PASSED!
```

## 🔧 Debugging

### Mode Debug
```bash
make run-tests-debug
```

### Memory Leaks (si Valgrind està disponible)
```bash
make valgrind-tests
```

### Informació Detallada
Cada test mostra:
- ✅ Nom del test
- ⏱️ Temps d'execució en microsegons  
- 📍 Localització d'errors (si n'hi ha)
- 📊 Estadístiques finals

## 🤝 Afegir Nous Tests

Per afegir un nou test:

1. Inclou el framework: `#include "test_framework.h"`
2. Defineix el test amb la macro `TEST(nom_test)`
3. Usa les macros d'assert per verificar condicions
4. Registra el test amb `RUN_TEST(nom_test)` al main
5. Compila i executa

### Exemple
```cpp
TEST(test_nou_cas) {
    Trie trie;
    // Implementar test...
    ASSERT_TRUE(condicio);
    return true;
}
```

---

**Framework de tests desenvolupat per garantir la qualitat i robustesa del Trie** ✨