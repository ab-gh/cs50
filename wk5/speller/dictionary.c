// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int checking_word_hash = hash(word);
    // Get the head
    node *cursor = table[checking_word_hash];
    // If the head is empty
    if (cursor == NULL)
    {
        // Empty bucket
        return false;
    }
    while (true) {
        // If the cursor word is our word
        if (strcasecmp(word, cursor->word) == 0)
        {
            // Found
            return true;
        }
        // If there is no next node
        if (cursor->next == NULL)
        {
            // word not found
            return false;
        }        
        // Move the cursor
        cursor = cursor->next;
    } 
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Get index of first letter
    unsigned int first_letter = tolower(word[0]) - 97;
    // Get index of second letter. If there isnt one, pretend its A
    unsigned int second_letter = (word[1] == '\0') ? 0 : tolower(word[1] - 97);
    unsigned int hash_val = (first_letter * 26) + second_letter;
    return hash_val;
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
    char current_dict_word[45] = "";
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
        fscanf_rtn = fscanf(file, "%s", current_dict_word);
        if (fscanf_rtn != EOF)
        {
            int word_hash = hash(current_dict_word);
            // Make a new empty node for this word
            node *new_node = calloc(sizeof(node), 1);
            // Copy the new word into the new node
            strcpy(new_node->word, current_dict_word);
            // Point this new node to the current first node in the bucket
            new_node->next = table[word_hash];
            // Push this node into the front of the bucket
            table[word_hash] = new_node;
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
        if (seeking_pointer == 0)
        {
            // printf("fuck empty bucket %i\n", i);
            continue;
        }
        else
        {
            int cards_seen_in_bucket = 1;
            while (seeking_pointer->next != 0)
            {
                cards_seen_in_bucket++;
                seeking_pointer = seeking_pointer->next;
            }
            // printf("%i words seen in bucket %i\n", cards_seen_in_bucket, i);
            total_cards_seen = total_cards_seen + cards_seen_in_bucket;
        }
    }
    return total_cards_seen;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // for each bucket
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];
        if (cursor == NULL)
        {
            // empty bucket
            free(table[i]);
            continue;
        }
        while (cursor->next != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
        free(cursor);
    }
    return true;
}
