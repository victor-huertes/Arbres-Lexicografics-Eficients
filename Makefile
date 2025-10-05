# Makefile per al projecte Trie
# Autor: Projecte Professional
# Data: $(shell date +%Y-%m-%d)

# Variables de configuració
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
DEBUG_FLAGS = -std=c++17 -Wall -Wextra -pedantic -g -DDEBUG
INCLUDE_DIR = sources/include
SRC_DIR = sources/src
BIN_DIR = bin
TEST_DIR = tests

# Noms dels fitxers
TARGET = $(BIN_DIR)/trie_demo
DEBUG_TARGET = $(BIN_DIR)/trie_demo_debug
TEST_TARGET = $(BIN_DIR)/test_trie
TEST_DEBUG_TARGET = $(BIN_DIR)/test_trie_debug
TEST_ADVANCED_TARGET = $(BIN_DIR)/test_trie_advanced
TEST_ADVANCED_DEBUG_TARGET = $(BIN_DIR)/test_trie_advanced_debug
SOURCES = $(SRC_DIR)/trie.cpp $(SRC_DIR)/main.cpp
TEST_SOURCES = $(SRC_DIR)/trie.cpp $(TEST_DIR)/test_trie.cpp
TEST_ADVANCED_SOURCES = $(SRC_DIR)/trie.cpp $(TEST_DIR)/test_trie_advanced.cpp
OBJECTS = $(BIN_DIR)/trie.o $(BIN_DIR)/main.o
TEST_OBJECTS = $(BIN_DIR)/trie_test.o $(BIN_DIR)/test_trie.o
TEST_ADVANCED_OBJECTS = $(BIN_DIR)/trie_test_adv.o $(BIN_DIR)/test_trie_advanced.o
DEBUG_OBJECTS = $(BIN_DIR)/trie_debug.o $(BIN_DIR)/main_debug.o
TEST_DEBUG_OBJECTS = $(BIN_DIR)/trie_test_debug.o $(BIN_DIR)/test_trie_debug.o
TEST_ADVANCED_DEBUG_OBJECTS = $(BIN_DIR)/trie_test_adv_debug.o $(BIN_DIR)/test_trie_advanced_debug.o
HEADERS = $(INCLUDE_DIR)/trie.h

# Colors per als missatges
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
NC = \033[0m # No Color

# Target per defecte
.PHONY: all
all: $(TARGET)
	@echo "$(GREEN)✓ Compilació completada amb èxit!$(NC)"

# Compilar el programa principal
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "$(BLUE)Enllaçant $(TARGET)...$(NC)"
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar objectes
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BIN_DIR)
	@echo "$(YELLOW)Compilant $<...$(NC)"
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Crear directori bin si no existeix
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Target per debug
.PHONY: debug
debug: $(DEBUG_TARGET)
	@echo "$(GREEN)✓ Compilació de debug completada!$(NC)"

$(DEBUG_TARGET): $(DEBUG_OBJECTS) | $(BIN_DIR)
	@echo "$(BLUE)Enllaçant $(DEBUG_TARGET)...$(NC)"
	$(CXX) $(DEBUG_FLAGS) -o $@ $^

$(BIN_DIR)/%_debug.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BIN_DIR)
	@echo "$(YELLOW)Compilant $< (debug)...$(NC)"
	$(CXX) $(DEBUG_FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Executar el programa
.PHONY: run
run: $(TARGET)
	@echo "$(BLUE)Executant el programa...$(NC)"
	@./$(TARGET)

# Executar en mode debug
.PHONY: run-debug
run-debug: $(DEBUG_TARGET)
	@echo "$(BLUE)Executant el programa en mode debug...$(NC)"
	@./$(DEBUG_TARGET)

# Compilar tests
.PHONY: test
test: $(TEST_TARGET)
	@echo "$(GREEN)✓ Tests compilats amb èxit!$(NC)"

$(TEST_TARGET): $(TEST_OBJECTS) | $(BIN_DIR)
	@echo "$(BLUE)Enllaçant $(TEST_TARGET)...$(NC)"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/trie_test.o: $(SRC_DIR)/trie.cpp $(HEADERS) | $(BIN_DIR)
	@echo "$(YELLOW)Compilant trie.cpp per tests...$(NC)"
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DIR)/test_trie.o: $(TEST_DIR)/test_trie.cpp $(HEADERS) | $(BIN_DIR)
	@echo "$(YELLOW)Compilant test_trie.cpp...$(NC)"
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(TEST_DIR) -c $< -o $@

# Executar tests
.PHONY: run-tests
run-tests: $(TEST_TARGET)
	@echo "$(BLUE)Executant els tests...$(NC)"
	@./$(TEST_TARGET)

# Compilar tests en mode debug
.PHONY: test-debug
test-debug: $(TEST_DEBUG_TARGET)
	@echo "$(GREEN)✓ Tests de debug compilats amb èxit!$(NC)"

$(TEST_DEBUG_TARGET): $(TEST_DEBUG_OBJECTS) | $(BIN_DIR)
	@echo "$(BLUE)Enllaçant $(TEST_DEBUG_TARGET)...$(NC)"
	$(CXX) $(DEBUG_FLAGS) -o $@ $^

$(BIN_DIR)/trie_test_debug.o: $(SRC_DIR)/trie.cpp $(HEADERS) | $(BIN_DIR)
	@echo "$(YELLOW)Compilant trie.cpp per tests (debug)...$(NC)"
	$(CXX) $(DEBUG_FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DIR)/test_trie_debug.o: $(TEST_DIR)/test_trie.cpp $(HEADERS) | $(BIN_DIR)
	@echo "$(YELLOW)Compilant test_trie.cpp (debug)...$(NC)"
	$(CXX) $(DEBUG_FLAGS) -I$(INCLUDE_DIR) -I$(TEST_DIR) -c $< -o $@

# Executar tests en mode debug
.PHONY: run-tests-debug
run-tests-debug: $(TEST_DEBUG_TARGET)
	@echo "$(BLUE)Executant els tests en mode debug...$(NC)"
	@./$(TEST_DEBUG_TARGET)

# Compilar tests avançats
.PHONY: test-advanced
test-advanced: $(TEST_ADVANCED_TARGET)
	@echo "$(GREEN)✓ Tests avançats compilats amb èxit!$(NC)"

$(TEST_ADVANCED_TARGET): $(TEST_ADVANCED_OBJECTS) | $(BIN_DIR)
	@echo "$(BLUE)Enllaçant $(TEST_ADVANCED_TARGET)...$(NC)"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/trie_test_adv.o: $(SRC_DIR)/trie.cpp $(HEADERS) | $(BIN_DIR)
	@echo "$(YELLOW)Compilant trie.cpp per tests avançats...$(NC)"
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DIR)/test_trie_advanced.o: $(TEST_DIR)/test_trie_advanced.cpp $(HEADERS) | $(BIN_DIR)
	@echo "$(YELLOW)Compilant test_trie_advanced.cpp...$(NC)"
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(TEST_DIR) -c $< -o $@

# Executar tests avançats
.PHONY: run-tests-advanced
run-tests-advanced: $(TEST_ADVANCED_TARGET)
	@echo "$(BLUE)Executant els tests avançats...$(NC)"
	@./$(TEST_ADVANCED_TARGET)

# Executar tots els tests (bàsics + avançats)
.PHONY: run-all-tests
run-all-tests: run-tests run-tests-advanced
	@echo "$(GREEN)✅ Tots els tests executats!$(NC)"

# Executar amb valgrind per detectar memory leaks
.PHONY: valgrind
valgrind: $(DEBUG_TARGET)
	@echo "$(BLUE)Executant amb Valgrind...$(NC)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(DEBUG_TARGET)

# Executar tests amb valgrind
.PHONY: valgrind-tests
valgrind-tests: $(TEST_DEBUG_TARGET)
	@echo "$(BLUE)Executant tests amb Valgrind...$(NC)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_DEBUG_TARGET)

# Netejar fitxers generats
.PHONY: clean
clean:
	@echo "$(RED)Netejant fitxers generats...$(NC)"
	@rm -rf $(BIN_DIR)/*.o $(BIN_DIR)/trie_demo* $(BIN_DIR)/test_trie*
	@if [ -d $(BIN_DIR) ] && [ -z "$$(ls -A $(BIN_DIR))" ]; then rmdir $(BIN_DIR); fi
	@echo "$(GREEN)✓ Neteja completada!$(NC)"

# Netejar completament (inclou documentació generada)
.PHONY: distclean
distclean: clean
	@echo "$(RED)Neteja completa...$(NC)"
	@rm -rf docs/html docs/latex/*.aux docs/latex/*.log docs/latex/*.fls docs/latex/*.fdb_latexmk docs/latex/*.synctex.gz
	@echo "$(GREEN)✓ Neteja completa finalitzada!$(NC)"

# Generar documentació amb Doxygen (opcional)
.PHONY: docs
docs:
	@echo "$(BLUE)Generant documentació...$(NC)"
	@if command -v doxygen >/dev/null 2>&1; then \
		doxygen Doxyfile 2>/dev/null || echo "$(YELLOW)⚠ Fitxer Doxyfile no trobat$(NC)"; \
	else \
		echo "$(YELLOW)⚠ Doxygen no està instal·lat$(NC)"; \
	fi

# Instal·lar dependències (només informatiu)
.PHONY: install-deps
install-deps:
	@echo "$(BLUE)Per a aquest projecte necessites:$(NC)"
	@echo "  - g++ amb suport per C++17"
	@echo "  - make"
	@echo "  - valgrind (opcional, per debug)"
	@echo "  - doxygen (opcional, per documentació)"

# Mostrar ajuda
.PHONY: help
help:
	@echo "$(GREEN)Makefile per al projecte Trie$(NC)"
	@echo ""
	@echo "$(YELLOW)Targets disponibles:$(NC)"
	@echo "  $(BLUE)all$(NC)            - Compila el programa principal"
	@echo "  $(BLUE)debug$(NC)          - Compila amb informació de debug"
	@echo "  $(BLUE)run$(NC)            - Compila i executa el programa"
	@echo "  $(BLUE)run-debug$(NC)      - Compila i executa en mode debug"
	@echo "  $(BLUE)test$(NC)           - Compila els tests bàsics"
	@echo "  $(BLUE)run-tests$(NC)      - Compila i executa els tests bàsics"
	@echo "  $(BLUE)test-advanced$(NC)  - Compila els tests avançats"
	@echo "  $(BLUE)run-tests-advanced$(NC) - Compila i executa els tests avançats"
	@echo "  $(BLUE)run-all-tests$(NC)  - Executa tots els tests (bàsics + avançats)"
	@echo "  $(BLUE)test-debug$(NC)     - Compila els tests amb informació de debug"
	@echo "  $(BLUE)run-tests-debug$(NC) - Compila i executa els tests en mode debug"
	@echo "  $(BLUE)valgrind$(NC)       - Executa amb Valgrind per detectar memory leaks"
	@echo "  $(BLUE)valgrind-tests$(NC) - Executa tests amb Valgrind"
	@echo "  $(BLUE)clean$(NC)          - Neteja fitxers objecte i executables"
	@echo "  $(BLUE)distclean$(NC)      - Neteja completa (inclou documentació)"
	@echo "  $(BLUE)docs$(NC)           - Genera documentació amb Doxygen"
	@echo "  $(BLUE)install-deps$(NC)   - Mostra les dependències necessàries"
	@echo "  $(BLUE)help$(NC)           - Mostra aquesta ajuda"
	@echo ""
	@echo "$(YELLOW)Exemples d'ús:$(NC)"
	@echo "  make             # Compila el programa"
	@echo "  make run         # Compila i executa"
	@echo "  make run-tests       # Compila i executa els tests bàsics"
	@echo "  make run-all-tests   # Executa tots els tests"
	@echo "  make test-debug      # Compila tests per debug"
	@echo "  make clean           # Neteja"

# Evitar conflictes amb fitxers del mateix nom
.PHONY: all debug run run-debug clean distclean docs install-deps help valgrind test run-tests test-debug run-tests-debug valgrind-tests test-advanced run-tests-advanced run-all-tests