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
    // Point a cursor at the bucket head
    node *cursor = table[checking_word_hash];
    // If the bucket is empty
    if (cursor == NULL)
    {
        // Empty bucket
        return false;
    }
    while (true) 
    {
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
    // Hash function is just the alphabetical index of the first charachter
    // Get index of first letter
    char first_char = tolower(word[0]);
    unsigned int first_int = ((int) first_char) - 97;
    unsigned int second_int = 0;
    // Find next char that is alpha
    for (int i = 1; i < strlen(word); i++)
    {
        if (word[1] == '\0')
        {
            second_int = 0;
            break;
        }
        else if (isalpha(word[i]))
        {
            second_int = ((int) tolower(word[i])) - 97;
            break;
        }
    }
    // Get index of second letter. If there isnt one, pretend its A
    unsigned int hash_val = (first_int * 26) + second_int;
    return hash_val;
    
    
    //return (unsigned int) tolower(word[0]) - 97;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Clear the table
    for (int i = 0; i < (sizeof(table) / sizeof(table[0])); i++)
    {
        table[i] = NULL;
    }
    // Init some chars
    char current_dict_word[45] = "";
    char fscanf_rtn = 0;
    // open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    // read strings from file one at a time
    // c will crawl accross the file, one char at a time
    while (fscanf_rtn != EOF) 
    {
        // Scan in the next word with a return check
        fscanf_rtn = fscanf(file, "%s", current_dict_word);
        // If its not the end of the file
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
    // Close the file (free memory)
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int total_cards_seen = 0;
    for (int i = 0; i < N; i++)
    {
        // Point a cursor at the start of the bucket
        node *cursor = table[i];
        // If the bucket is empty
        if (cursor == 0)
        {
            // Nothing to count
            continue;
        }
        else
        {
            // Start with one in the bucket
            int cards_seen_in_bucket = 1;
            // While there is a next node
            while (cursor->next != 0)
            {
                // Count it and move to the next
                cards_seen_in_bucket++;
                cursor = cursor->next;
            }
            // Total
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
        // Point a cursor and a temp at the start of each bucket
        node *cursor = table[i];
        node *tmp = table[i];
        // If no cursor
        if (cursor == NULL)
        {
            // empty bucket
            free(table[i]);
            continue;
        }
        while (cursor->next != NULL)
        {
            // Advance the cursor
            cursor = cursor->next;
            // Empty the current temp pointer
            free(tmp);
            // Advance the temp
            tmp = cursor;
        }
        // Finally frees the cursor
        free(cursor);
    }
    return true;
}
