// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    return 0;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Clear the table
    for (int i = 0; i < (sizeof(table) / sizeof(table[0])); i++)
    {
        table[i] = NULL;
    }
    // Init some cars
    char c;   
    char aword[45] = "";
    char fscanf_rtn = 0;
    // open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Failed to open\n");
    }
    else
    {
        printf("opened\n");
    }
    // read strings from file one at a time
    // c will crawl accross the file, one char at a time
    while (fscanf_rtn != EOF) {
        fscanf_rtn = fscanf(file, "%s", aword);
        if (fscanf_rtn != EOF)
        {
            printf("word %s at hash %i\n", aword, hash(aword));
            node *new_node = calloc(sizeof(node), 1);
            printf("malloc\n");
            strcpy(new_node->word, aword);
            printf("copied\n");
            node *pointing = table[hash(aword)];
            while (pointing != 0)
            { 
                printf("next card\n");
                pointing = pointing->next;
            }
            new_node->next = pointing;
            
        }
    }
    printf("---\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", table[0]->word);
    }
    

    // for words
    // create a new node for each word
    
    // hash word to obtain hash bucket
    
    // insert node into hash table at bucket location
    
        // 
        // strcpy(n->word, word);
        // n->next = NULL;
    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
}
