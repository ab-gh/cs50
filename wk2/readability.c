#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    // Ask for the text to analyse
    string text = get_string("Input the text to analyse.\n> ");

    int letters_count = 0;
    // The last word doesn't have a space after it
    int words_count = 1;
    int sentences_count = 0;

    // Step charachter by charachter
    for (int i = 0; i < strlen(text); i++)
    {
        // If its a letter
        if (isalpha(text[i]))
        {
            // Increase letter count
            letters_count++;
        }
        // If its a space followed by a letter OR a colon
        else if ((isspace(text[i]) && isalpha(text[i + 1])) || (int) text[i] == 58)
        {
            // Increase word count
            words_count++;
        }
        // If it's a stop, bang, or interr
        else if ((int) text[i] == 46 || (int) text[i] == 63 || (int) text[i] == 33)
        {
            // Increase sentence count
            sentences_count++;
        }
    }

    // Constant calculation
    const float L = ((float) letters_count / (float) words_count) * 100;
    const float S = ((float) sentences_count / (float) words_count) * 100;
    const float INDEX = (0.0588 * L) - (0.296 * S) - 15.8;
    // Zero or below exception
    if (INDEX < 0)
    {
        printf("Before Grade 1\n");
    }
    // GE 16 exception
    else if (INDEX >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(INDEX));
    }

}
