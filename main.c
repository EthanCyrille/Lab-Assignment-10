#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    struct TrieNode* children[26]; // 26 letters of the alphabet
    int count; // To keep track of word occurrences
};

// Trie structure
struct Trie
{
    struct TrieNode* root;
};

// Creates a new Trie node
struct TrieNode* createNode()
{
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    node->count = 0;
    for (int i = 0; i < 26; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

// Initializes a trie structure
struct Trie* createTrie()
{
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    pTrie->root = createNode();
    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word)
{
    struct TrieNode* currentNode = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL)
        {
            currentNode->children[index] = createNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie* pTrie, char* word)
{
    struct TrieNode* currentNode = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL)
        {
            return 0; // Word does not exist in the trie
        }
        currentNode = currentNode->children[index];
    }
    return currentNode->count;
}

// Deallocates the trie structure
void deallocateTrieHelper(struct TrieNode* node)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < 26; i++)
    {
        deallocateTrieHelper(node->children[i]);
    }
    free(node);
}

struct Trie* deallocateTrie(struct Trie* pTrie)
{
    deallocateTrieHelper(pTrie->root);
    free(pTrie);
    return NULL;
}

// This function will return the number of words in the dictionary
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening the dictionary file");
        exit(EXIT_FAILURE);
    }

    int numWords = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline characters
        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Test the occurrences of some words
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the trie
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
    {
        printf("There is an error in this program\n");
    }

    // Deallocate the words
    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }

    return 0;
}
