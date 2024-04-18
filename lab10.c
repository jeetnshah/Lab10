#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LENGTH 256

// Trie structure
struct Trie
{
    struct Trie *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *pNode = (struct Trie *)malloc(sizeof(struct Trie));
    pNode->isEndOfWord = 0;
    pNode->count = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;

    return pNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *pCrawl = pTrie;

    for (int level = 0; level < strlen(word); level++)
    {
        int index = word[level] - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = createTrie();

        pCrawl = pCrawl->children[index];
    }

    pCrawl->isEndOfWord = 1;
    pCrawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *pCrawl = pTrie;

    for (int level = 0; level < strlen(word); level++)
    {
        int index = word[level] - 'a';

        if (!pCrawl->children[index])
            return 0;

        pCrawl = pCrawl->children[index];
    }

    if (pCrawl != NULL && pCrawl->isEndOfWord)
        return pCrawl->count;

    return 0;
}

// Deallocate the trie structure
void deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        deallocateTrie(pTrie->children[i]);

    free(pTrie);
}

// This function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", filename);
        return 0;
    }

    char word[MAX_WORD_LENGTH];
    int i = 0;

    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[i] = malloc(strlen(word) + 1);
        strcpy(pInWords[i], word);
        i++;
    }

    fclose(file);
    return i;
}

int main(void)
{
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }
    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    // Free the memory allocated for the words
    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }

    deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
