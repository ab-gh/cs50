
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

unsigned int hash(const char *word)
{
    // Get index of first letter
    char first_char = tolower(word[0]);
    // Find next char that is alpha
    int i = 1;
    while (true)
    {
        if (isalpha(word[i]))
        {
            char second_char = word[i];
            break;
        }
        i++;
    }
    unsigned int first_int = ((int) first_char) - 97;
    // Get index of second letter. If there isnt one, pretend its A
    unsigned int second_int = (word[1] == '\0') ? 0 : ((int) word[1]) - 97;
    unsigned int hash_val = (first_int * 26) + second_int;
    return hash_val;
}