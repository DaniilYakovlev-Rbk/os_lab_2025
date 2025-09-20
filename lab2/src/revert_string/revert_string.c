#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "revert_string.h"

void RevertString(char *str)
{
    int length = strlen(str);
    char *tmp = malloc(length + 1);
    strcpy(tmp, str);
    
    for (int i = 0; i < length; i++)
    {
        str[i] = tmp[length - 1 - i];
    }
    
    free(tmp);
}

