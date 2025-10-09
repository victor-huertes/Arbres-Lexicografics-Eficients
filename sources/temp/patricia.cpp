// C++ Program to implement Patricia trie for ASCII strings

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

// Main function
int main()
{
    PatriciaTrie pt;

    cout << "Patricia Trie for ASCII Strings\n"
         << endl;

    char ch;

    do
    {
        cout << "\n Patricia Trie Operations\n"
             << endl;
        cout << "1. Insert" << endl;
        cout << "2. Search" << endl;
        cout << "3. Check Empty" << endl;
        cout << "4. Make Empty" << endl;
        cout << "Make your choice" << endl;

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << "Enter word to insert" << endl;
            string element;
            cin >> element;
            pt.insert(element);
            break;
        }

        case 2:
        {
            cout << "Enter word to search" << endl;
            string element;
            cin >> element;
            cout << "Search result: " << (pt.search(element) ? "true" : "false") << endl;
            break;
        }

        case 3:
        {
            cout << "Empty status: " << (pt.isEmpty() ? "true" : "false") << endl;
            break;
        }

        case 4:
        {
            cout << "Patricia Trie Cleared" << endl;
            pt.makeEmpty();
            break;
        }

        default:
        {
            cout << "Wrong entry\n"
                 << endl;
            break;
        }
        }

        cout << "\n Do you want to continue (Type y or n)\n"
             << endl;
        cin >> ch;
    } while (ch == 'Y' || ch == 'y');

    return 0;
}
