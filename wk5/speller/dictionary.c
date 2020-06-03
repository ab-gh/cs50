// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

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
    unsigned int first_char = 0;
    printf("word: %s\n", word);
    printf("char: %c hash: %i\n", tolower(word[0]), tolower(word[0]) - 97);
    unsigned int hash_val = tolower(word[0]) - 97;
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
        return false;
    }
    // read strings from file one at a time
    // c will crawl accross the file, one char at a time
    while (fscanf_rtn != EOF) {
        // Scan in the next word with a return check
        fscanf_rtn = fscanf(file, "%s", aword);
        if (fscanf_rtn != EOF)
        {
            // Make a new empty node for this word
            node *new_node = calloc(sizeof(node), 1);
            // Copy the new word into the new node
            strcpy(new_node->word, aword);
            // Point this new node to the current first node in the bucket
            new_node->next = table[hash(aword)];
            // Push this node into the front of the bucket
            table[hash(aword)] = new_node;
        }
    }
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int total_cards_seen = 0;
    for (int i = 0; i < N; i++)
    {
        node *seeking_pointer = table[i];
        int cards_seen_in_bucket = 1;
        while (seeking_pointer->next != 0)
        {
            cards_seen_in_bucket++;
            seeking_pointer = seeking_pointer->next;
        }
        printf("%i words seen in bucket %i\n", cards_seen_in_bucket, i);
        total_cards_seen = total_cards_seen + cards_seen_in_bucket;
    }
    printf("total words: %i\n", total_cards_seen);
    return total_cards_seen;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
}
