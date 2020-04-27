#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int check_key(string key);
void encrypt(string key);

int main(int argc, string argv[])
{
    // If only one argument given
    if (argc == 2)
    {
        // If the key is not the right length
        if (strlen(argv[1]) != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else
        {
            // Run key checker, if returned with 1
            if (check_key(argv[1]))
            {
                return 1;
            }
            else
            {
                // Run encryption
                encrypt(argv[1]);
                // Return OK
                return 0;
            }
        }

    }
    else
    {
        // More than one arg provided
        printf("Usage: ./substitution key");
        return 1;
    }
}

int check_key(string key)
{
    // Empty array for duplicate letter checking
    bool letters_seen[26];
    // Set array to false
    for (int k = 0; k < 26; k++)
    {
        letters_seen[k] = false;
    }
    // Secondary checking
    int check = 0;
    // Loop over key by char
    for (int i = 0; i < strlen(key); i++)
    {
        // If the chars corresponding element is already true
        if (letters_seen[(int) toupper(key[i]) - 65] == true)
        {
            printf("Key must contain 26 unique characters.\n");
            return 1;
        }
        else
        {
            // Else set it as true
            letters_seen[(int) toupper(key[i]) - 65] = true;
        }
        // If any char is non-alpha
        if (!isalpha(key[i]))
        {
            printf("Key must only be letters.\n");
            return 1;
        }
        // Add the ascii value to the checker
        check += (int) toupper(key[i]);
    }
    // Total for an A+B+C+D+E.. in ascii
    if (check != 2015)
    {
        printf("Key must contain 26 unique characters.\n");
        return 1;
    }
    else
    {
        // Correct key
        return 0;
    }
}

void encrypt(const string KEY)
{
    // Ask for the plaintext - use const!
    const string PLAINTEXT = get_string("plaintext: ");
    // Open an array for the ciphertext, plus one element for a null terminator
    char ciphertext[strlen(PLAINTEXT) + 1];
    for (int j = 0; j < strlen(PLAINTEXT); j++)
    {
        // Uppercase plaintext
        if (isupper(PLAINTEXT[j]))
        {
            ciphertext[j] = toupper(KEY[(int) PLAINTEXT[j] - 65]);
        }
        // Lowercase plaintext
        else if (islower(PLAINTEXT[j]))
        {
            ciphertext[j] = tolower(KEY[(int) PLAINTEXT[j] - 97]);
        }
        // Plaintext is non-alpha
        else
        {
            ciphertext[j] = PLAINTEXT[j];
        }
    }
    // Append the null terminator
    ciphertext[strlen(PLAINTEXT)] = (char) 0;
    // Print and end
    printf("ciphertext: %s\n", ciphertext);
}