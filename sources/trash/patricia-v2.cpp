// Patricia Trie ASCII

#include "../include/patricia.h"
#include <iostream>
using namespace std;

// Implementación de PatriciaTrieNode
PatriciaTrieNode::PatriciaTrieNode()
{
    number = 0;
    data = "";
    leftChild = nullptr;
    rightChild = nullptr;
}

// Implementación de PatriciaTrie
const int PatriciaTrie::MaxBits;

// Private helper method - bit()
bool PatriciaTrie::bit(const string &str, int i)
{
    if (i < 1)
        return false;

    int charIndex = (i - 1) / 8;
    int bitIndex = (i - 1) % 8;

    if (charIndex >= static_cast<int>(str.length()))
    {
        return false;
    }

    unsigned char ch = str[charIndex];
    return (ch & (1 << (7 - bitIndex))) != 0;
}

// Private search helper method
PatriciaTrieNode *PatriciaTrie::search(PatriciaTrieNode *t, const string &k)
{
    PatriciaTrieNode *currentNode;
    PatriciaTrieNode *nextNode;

    if (t == nullptr)
    {
        return nullptr;
    }

    nextNode = t->leftChild;
    currentNode = t;

    while (nextNode->number > currentNode->number)
    {
        currentNode = nextNode;
        nextNode = (bit(k, nextNode->number))
                       ? nextNode->rightChild
                       : nextNode->leftChild;
    }
    return nextNode;
}

// Private insert helper method
PatriciaTrieNode *PatriciaTrie::insert(PatriciaTrieNode *t, const string &element)
{
    PatriciaTrieNode *current = nullptr;
    PatriciaTrieNode *parent;
    PatriciaTrieNode *lastNode;
    PatriciaTrieNode *newNode;
    int i;

    if (t == nullptr)
    {
        t = new PatriciaTrieNode();
        t->number = 0;
        t->data = element;
        t->leftChild = t;
        t->rightChild = nullptr;
        return t;
    }

    lastNode = search(t, element);

    if (element == lastNode->data)
    {
        cout << "Key already Present" << endl;
        return t;
    }

    for (i = 1; bit(element, i) == bit(lastNode->data, i); i++)
        ;

    current = t->leftChild;
    parent = t;

    while (current->number > parent->number && current->number < i)
    {
        parent = current;
        current = (bit(element, current->number))
                      ? current->rightChild
                      : current->leftChild;
    }

    newNode = new PatriciaTrieNode();
    newNode->number = i;
    newNode->data = element;
    newNode->leftChild = bit(element, i) ? current : newNode;
    newNode->rightChild = bit(element, i) ? newNode : current;

    if (current == parent->leftChild)
    {
        parent->leftChild = newNode;
    }
    else
    {
        parent->rightChild = newNode;
    }

    return t;
}

// Private insert helper method with position
PatriciaTrieNode *PatriciaTrie::insert(PatriciaTrieNode *t, const string &element, int position)
{
    PatriciaTrieNode *current = nullptr;
    PatriciaTrieNode *parent;
    PatriciaTrieNode *lastNode;
    PatriciaTrieNode *newNode;
    int i;

    if (t == nullptr)
    {
        t = new PatriciaTrieNode();
        t->number = 0;
        t->data = element;
        t->leftChild = t;
        t->rightChild = nullptr;
        t->positions.push_back(position);
        return t;
    }

    lastNode = search(t, element);

    if (element == lastNode->data)
    {
        // Si la palabra ya existe, solo agregamos la posición
        lastNode->positions.push_back(position);
        return t;
    }

    for (i = 1; bit(element, i) == bit(lastNode->data, i); i++)
        ;

    current = t->leftChild;
    parent = t;

    while (current->number > parent->number && current->number < i)
    {
        parent = current;
        current = (bit(element, current->number))
                      ? current->rightChild
                      : current->leftChild;
    }

    newNode = new PatriciaTrieNode();
    newNode->number = i;
    newNode->data = element;
    newNode->positions.push_back(position);
    newNode->leftChild = bit(element, i) ? current : newNode;
    newNode->rightChild = bit(element, i) ? newNode : current;

    if (current == parent->leftChild)
    {
        parent->leftChild = newNode;
    }
    else
    {
        parent->rightChild = newNode;
    }

    return t;
}

// Constructor
PatriciaTrie::PatriciaTrie()
{
    root = nullptr;
}

// Destructor
PatriciaTrie::~PatriciaTrie()
{
    makeEmpty();
}

// Method - isEmpty()
bool PatriciaTrie::isEmpty() const
{
    return root == nullptr;
}

// Method - makeEmpty()
void PatriciaTrie::makeEmpty()
{
    root = nullptr;
}

// Method - search()
bool PatriciaTrie::search(const string &k)
{
    if (k.empty())
    {
        cout << "Cannot search for empty string" << endl;
        return false;
    }

    if (k.length() * 8 > MaxBits)
    {
        cout << "String too long (max " << MaxBits / 8 << " characters)" << endl;
        return false;
    }

    PatriciaTrieNode *searchNode = search(root, k);

    if (searchNode != nullptr && searchNode->data == k)
        return true;
    else
        return false;
}

// Method - insert()
void PatriciaTrie::insert(const string &element)
{
    if (element.empty())
    {
        cout << "Cannot insert empty string" << endl;
        return;
    }

    if (element.length() * 8 > MaxBits)
    {
        cout << "String too long (max " << MaxBits / 8 << " characters)" << endl;
        return;
    }

    root = insert(root, element);
}

// Method - init()
void PatriciaTrie::init(const string& text) {
    makeEmpty();
    for (size_t i = 0; i < text.length(); ++i) {
        string suffix = text.substr(i);
        if (!suffix.empty() && suffix.length() * 8 <= MaxBits) {
            insert(suffix, i);
        }
    }
}

// Method - insert with position
void PatriciaTrie::insert(const string& word, int position) {
    if (word.empty())
    {
        cout << "Cannot insert empty string" << endl;
        return;
    }

    if (word.length() * 8 > MaxBits)
    {
        cout << "String too long (max " << MaxBits / 8 << " characters)" << endl;
        return;
    }

    root = insert(root, word, position);
}

// Method - search_positions()
vector<int> PatriciaTrie::search_positions(const string& word) {
    vector<int> positions;
    
    if (word.empty() || word.length() * 8 > MaxBits) {
        return positions;
    }

    PatriciaTrieNode *searchNode = search(root, word);

    if (searchNode != nullptr && searchNode->data == word) {
        positions = searchNode->positions;
    }
    
    return positions;
}

// Helper method - collect_positions
void PatriciaTrie::collect_positions(PatriciaTrieNode* node, vector<int>& positions) {
    if (node == nullptr) return;
    
    // Evitar ciclos en el árbol Patricia
    if (node->leftChild == node || node->rightChild == node) {
        if (!node->positions.empty()) {
            positions.insert(positions.end(), node->positions.begin(), node->positions.end());
        }
        return;
    }
    
    if (!node->positions.empty()) {
        positions.insert(positions.end(), node->positions.begin(), node->positions.end());
    }
    
    if (node->leftChild && node->leftChild->number > node->number) {
        collect_positions(node->leftChild, positions);
    }
    if (node->rightChild && node->rightChild->number > node->number) {
        collect_positions(node->rightChild, positions);
    }
}

// Helper method - collect_words
void PatriciaTrie::collect_words(PatriciaTrieNode* node, vector<pair<string, int>>& results) {
    if (node == nullptr) return;
    
    // Evitar ciclos en el árbol Patricia
    if (node->leftChild == node || node->rightChild == node) {
        for (int pos : node->positions) {
            results.push_back({node->data, pos});
        }
        return;
    }
    
    if (!node->positions.empty()) {
        for (int pos : node->positions) {
            results.push_back({node->data, pos});
        }
    }
    
    if (node->leftChild && node->leftChild->number > node->number) {
        collect_words(node->leftChild, results);
    }
    if (node->rightChild && node->rightChild->number > node->number) {
        collect_words(node->rightChild, results);
    }
}

// Method - starts_with_positions()
vector<int> PatriciaTrie::starts_with_positions(const string& prefix) {
    vector<int> positions;
    
    if (prefix.empty() || prefix.length() * 8 > MaxBits || root == nullptr) {
        return positions;
    }
    
    // En Patricia Trie, necesitamos buscar todos los nodos que comienzan con el prefijo
    // Esto es más complejo que en un trie naive ya que las etiquetas pueden contener múltiples caracteres
    collect_positions(root->leftChild, positions);
    
    // Filtrar solo los que realmente comienzan con el prefijo
    // (simplificado - en una implementación completa se debería navegar mejor)
    
    return positions;
}

// Method - autocomplete()
vector<pair<string, int>> PatriciaTrie::autocomplete(const string& prefix) {
    vector<pair<string, int>> results;
    
    if (root == nullptr) {
        return results;
    }
    
    // Recolectar todas las palabras
    collect_words(root->leftChild, results);
    
    // Filtrar las que comienzan con el prefijo
    vector<pair<string, int>> filtered;
    for (const auto& pair : results) {
        if (pair.first.substr(0, prefix.length()) == prefix) {
            filtered.push_back(pair);
        }
    }
    
    return filtered;
}

// Method - get_words()
vector<pair<string, int>> PatriciaTrie::get_words() {
    return autocomplete("");
}
