#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    int n = 1;
    printf("addressof n : %p\n", &n);
    printf("goto addressof n : %i\n", *&n);
    int *p = &n;
    printf("p: %p\n", p);
    printf("addressof p %p\n", &p);
    printf("goto p : %i\n", *p);

    char *s = "Foobar";
    char *t = malloc(strlen(s) + 1);

    for (int i = 0; i < strlen(s); i++)
    {
        printf("%c\n", *(s + i));

        *(t + (strlen(s) - i)) = *(s+i);
    }
    *(t + strlen(s) + 1) = '\0';
    for (int j = 1; j <= strlen(s); j++)
    {
        printf("%c\n", *(t + j));
    }
    printf("%lu\n", strlen(t));



    return 0;
}