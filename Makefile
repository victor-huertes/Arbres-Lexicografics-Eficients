# Makefile para Arbres Lexicografics Eficients

all:
	@mkdir -p bin
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/naive.cpp -o bin/naive.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/radix.cpp -o bin/radix.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/main.cpp -o bin/main.o
	@g++ -std=c++17 -Wall -O2 -o bin/trie_demo bin/naive.o bin/radix.o bin/main.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/experiments.cpp -o bin/experiments.o
	@g++ -std=c++17 -Wall -O2 -o bin/experiments bin/naive.o bin/radix.o bin/experiments.o
	@echo "Compilacion completada"

run:
	@mkdir -p bin
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/naive.cpp -o bin/naive.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/radix.cpp -o bin/radix.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/main.cpp -o bin/main.o
	@g++ -std=c++17 -Wall -O2 -o bin/trie_demo bin/naive.o bin/radix.o bin/main.o
	@./bin/trie_demo

test:
	@mkdir -p bin
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/naive.cpp -o bin/naive.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/radix.cpp -o bin/radix.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/experiments.cpp -o bin/experiments.o
	@g++ -std=c++17 -Wall -O2 -o bin/experiments bin/naive.o bin/radix.o bin/experiments.o
	@./bin/experiments > ./output/results.txt
	@cat ./output/results.txt

csv:
	@mkdir -p bin
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/naive.cpp -o bin/naive.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/radix.cpp -o bin/radix.o
	@g++ -std=c++17 -Wall -O2 -Isources/include -c sources/src/experiments.cpp -o bin/experiments.o
	@g++ -std=c++17 -Wall -O2 -o bin/experiments bin/naive.o bin/radix.o bin/experiments.o
	@./bin/experiments --csv > ./output/results.csv
	@./bin/experiments --stats 5
	@cat ./output/results.csv

plot: 
	@cd output && gnuplot -p ../sources/utils/script.gnuplot
	@echo "Generados graficos con gnuplot en /output"

pdf: 
	@cd docs/latex && pdflatex -interaction=nonstopmode main.tex > /dev/null
	@echo "PDF generado en docs/latex/main.pdf"

clean:
	@rm -rf bin/*.o bin/trie_demo bin/experiments
	@rm -f docs/latex/*.aux docs/latex/*.log docs/latex/*.out docs/latex/*.toc docs/latex/*.fls docs/latex/*.fdb_latexmk docs/latex/*.synctex.gz
	@rm -f output/*
	@echo "Archivos temporales eliminados"

help:
	@echo "Makefile para Arbres Lexicografics Eficients"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make       - Compila todo el proyecto"
	@echo "  make run   - Compila y ejecuta la interfaz de consola"
	@echo "  make test  - Compila y ejecuta los experimentos"
	@echo "  make csv   - Compila y ejecuta los experimentos en formato CSV"
	@echo "  make plot  - Genera graficas"
	@echo "  make pdf   - Compila el documento LaTeX"
	@echo "  make clean - Limpia archivos"
	@echo "  make help  - Muestra esta ayuda"
