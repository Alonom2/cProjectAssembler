#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firstPass.h"
#include "tables.h"
#include "macro.h"


char *changeIntToHexa(int n)
{
    char *hexa = (char *)malloc(sizeof(char) * 4);

    strcpy(hexa, "000");

    if (n < 0)
    {
        n += 4096;
    }

    hexa[2] = changeIntToHexaChar(n % 16);

    n = n / 16;

    hexa[1] = changeIntToHexaChar(n % 16);

    n = n / 16;

    hexa[0] = changeIntToHexaChar(n);
    
    return hexa;
}

char changeIntToHexaChar(int n)
{
    if (n >= 0 && n < 10)
    {
        return n;
    }

    else if (n >= 10 && n <=15)
    {
        return ('A' + n - 10);
    }

    else
    {
        return 0;
    }
}