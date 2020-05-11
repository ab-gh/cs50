#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int check_key(string key);
void encrypt(string key);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        if (strlen(argv[1]) != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else
        {
            if (check_key(argv[1]))
            {
                return 1;
            }
            else
            {
                encrypt(argv[1]);
                return 0;
            }
        }

    }
}

int check_key(string key)
{
    int check = 0;
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Key must only be letters.\n");
            return 1;
        }
        check += (int) toupper(key[i]);
    }
    if (check != 2015)
    {
        printf("Key must contain 26 unique characters.\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

void encrypt(const string KEY)
{
    const string PLAINTEXT = get_string("plaintext: ");
    char ciphertext[strlen(PLAINTEXT)];
    for (int j = 0; j < strlen(PLAINTEXT); j++)
    {
        printf("%c\n", PLAINTEXT[j]);
        // if ( ((int) PLAINTEXT[j] >= 65 &&  (int) PLAINTEXT[j] <= 90) || ((int) PLAINTEXT[j] >= 97 &&  (int) PLAINTEXT[j] <= 122))
        if (isupper(PLAINTEXT[j]))
        {
            ciphertext[j] = toupper(KEY[(int) PLAINTEXT[j] - 65]);
        }
        else if (islower(PLAINTEXT[j]))
        {
            ciphertext[j] = tolower(KEY[(int) PLAINTEXT[j] - 97]);
        }
        else
        {
            ciphertext[j] = PLAINTEXT[j];
        }
    }
    printf("ciphertext: %s\n", ciphertext);
}
