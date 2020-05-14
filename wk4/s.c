#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Lecture 4: Memory

int main(void)
{
    int n = 1;
    int *p = &n; // *pointer to &addressofn
    //printf("%i\n", *p);

    // string s = "EMMA"; // E M M A \0

    // there is no string, its just a pointer to a char array


    char *s = "FOO";
    printf("%p\n", s);
    printf("%p\n", &*(s+1));
    printf("%p\n", &*(s));

    printf("%p\n", &s);
    //printf("%p\n", &(s+1));

    printf("%c\n", *s);
    printf("%c\n", *(s+1));

    printf("%p", *q);

}