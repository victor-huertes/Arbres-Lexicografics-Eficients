#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <random>
#include <regex>
#include <algorithm>

// ===========================================================
// FUNCIONES AUXILIARES
// ===========================================================

// Lee todas las palabras alfabéticas de un archivo
std::vector<std::string> extractWords(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> words;
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    std::regex wordRegex(R"([A-Za-z]+)");
    auto words_begin = std::sregex_iterator(content.begin(), content.end(), wordRegex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
        words.push_back((*i).str());

    return words;
}

// Genera una palabra aleatoria de longitud entre 4 y 10 letras
std::string randomWord(std::mt19937& rng) {
    std::uniform_int_distribution<int> lenDist(4, 10);
    std::uniform_int_distribution<int> charDist(0, 25);
    int length = lenDist(rng);
    std::string w;
    for (int i = 0; i < length; ++i)
        w.push_back('a' + charDist(rng));
    return w;
}

// ===========================================================
// GENERADOR DE DATASET
// ===========================================================

void generateSearchDataset(const std::string& inputFile,
                           const std::string& outputFile,
                           double realRatio = 0.5,
                           double fakeRatio = 0.1,
                           size_t maxFake = 50000) {
    std::cout << "Procesando archivo: " << inputFile << std::endl;

    auto words = extractWords(inputFile);
    if (words.empty()) {
        std::cerr << "⚠️ No se encontraron palabras en " << inputFile << std::endl;
        return;
    }

    // Convertir a minúsculas y eliminar duplicados
    std::unordered_set<std::string> uniqueSet;
    for (auto& w : words) {
        std::transform(w.begin(), w.end(), w.begin(), ::tolower);
        uniqueSet.insert(w);
    }

    std::vector<std::string> uniqueWords(uniqueSet.begin(), uniqueSet.end());

    // 30% reales
    size_t realCount = std::min<size_t>(uniqueWords.size() * realRatio, uniqueWords.size());
    std::shuffle(uniqueWords.begin(), uniqueWords.end(), std::mt19937{std::random_device{}()});
    std::vector<std::string> realSample(uniqueWords.begin(), uniqueWords.begin() + realCount);

    // 10% falsas
    size_t fakeCount = std::min<size_t>(uniqueWords.size() * fakeRatio, maxFake);
    std::unordered_set<std::string> fakeWords;
    std::mt19937 rng(std::random_device{}());

    while (fakeWords.size() < fakeCount) {
        std::string w = randomWord(rng);
        if (uniqueSet.find(w) == uniqueSet.end())
            fakeWords.insert(w);
    }

    // Combinar y mezclar
    std::vector<std::string> finalWords = realSample;
    finalWords.insert(finalWords.end(), fakeWords.begin(), fakeWords.end());
    std::shuffle(finalWords.begin(), finalWords.end(), rng);

    // Guardar
    std::ofstream out(outputFile);
    for (const auto& w : finalWords)
        out << w << "\n";

    std::cout << "✅ Dataset generado: " << outputFile
              << " (" << finalWords.size() << " palabras)\n\n";
}

// ===========================================================
// MAIN
// ===========================================================

int main() {
    // Archivos base y destinos
    std::vector<std::pair<std::string, std::string>> datasets = {
        {"lorem_ipsum.txt", "dataset_busqueda_loremipsum.txt"},
    };

    for (auto& [input, output] : datasets) {
        generateSearchDataset(input, output);
    }

    std::cout << "🎯 Todos los datasets generados correctamente.\n";
    return 0;
}
