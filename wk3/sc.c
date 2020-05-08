#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct
{
    string name;
    string number;
}
person;

int main(void)
{
    int numbers[6] = {4, 8, 15, 16, 23, 42}; // static initialized
    string names[1] = {"NAME"};
    if (strcmp(names[0], "NAME") == 0) // strcmp
    {
        // printf("Not here\n");
    }
    
    //structs
    
    person people[1];
    people[0].name = "EMMA";
    people[0].number = "617-555-0101";
    //printf("%lu\n", sizeof people / sizeof *people);
    
    string name = "NAME";
    if (strcmp(name, "NAME") == 0)
    {
        printf("yes\n");
    }
}