
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

int main(void)
{
    char *str1 = "hello";
    char *str2 = "Hellooo";
    printf("%i\n", strcasecmp(str1, str2));
}