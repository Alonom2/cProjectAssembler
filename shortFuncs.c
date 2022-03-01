#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "firstPass.h"
#include "tables.h"
#include "macro.h"
#include "shortFuncs.h"


/****************************************************************************changeIntToHexa*****************************************************************************/    


char *changeIntToHexa(int n)
{
    char *hexa = (char *)malloc(sizeof(char) * 5);

    strcpy(hexa, "0000");

    if (n < 0)
    {
        n += 4096;
    }

    hexa[3] = changeIntToHexaChar(n % 16);

    n = n / 16;

    hexa[2] = changeIntToHexaChar(n % 16);

    n = n / 16;

    hexa[1] = changeIntToHexaChar(n % 16);

    if (n > 15)
    {
         n = n / 16;
    }

    hexa[0] = changeIntToHexaChar(n);
    
    return hexa;
}

/****************************************************************************changeIntToHexaChar*************************************************************************/    


char changeIntToHexaChar(int n)
{
    char result;

    if (n >= 0 && n < 10)
    {
        result = n + '0';
        return result;
    }

    else if (n >= 10 && n <=15)
    {
        result = ('A' + n - 10);
        return result;
    }

    else
    {
        return 0;
    }
}

/****************************************************************************is_entry************************************************************************************/    

int is_entry(char* currentLine)
{
    int value;

    remove_spaces(currentLine);

    if (strstr(currentLine, ".entry"))
        value = TRUE;

    else
        value = FALSE;
    
    return value;
}

/****************************************************************************is_extern***********************************************************************************/    

int is_extern(char* currentLine)
{
    int value;

    remove_spaces(currentLine);

    if (strstr(currentLine, ".extern"))
        value = TRUE;

    else
        value = FALSE;
    
    return value;
}

/****************************************************************************is_label************************************************************************************/    

int is_label(char* currentLine)
{
    int value;

    remove_spaces(currentLine);

    if (strstr(currentLine, ":"))
        value = TRUE;

    else
        value = FALSE;
    
    return value;
}

/****************************************************************************is_string***********************************************************************************/    

int is_string(char* currentLine)
{
    int value;

    remove_spaces(currentLine);

    if (strstr(currentLine, ".string"))
        value = TRUE;

    else
        value = FALSE;
    
    return value;
}

/****************************************************************************is_data*************************************************************************************/    

int is_data(char* currentLine)
{
    int value;

    remove_spaces(currentLine);

    if (strstr(currentLine, ".data"))
        value = TRUE;

    else
        value = FALSE;
    
    return value;
}
