#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firstPass.h"

/****************************func_is_entry**********************************/    

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

/****************************func_is_extern*********************************/    

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

/****************************func_is_label*********************************/    

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

/******************************func_is_string*******************************/    

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

/*******************************func_is_data********************************/    

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

/******************************findSizeSymbol*******************************/ 

int findSizeSymbol(char* currentLine)
{
    int sizeSymbol;

    remove_spaces(currentLine);

    for (sizeSymbol = 0; (*(currentLine + sizeSymbol) != ':') && sizeSymbol < SIZE_LINE; sizeSymbol++)
    {
        continue;
    }

    return sizeSymbol;
}

/********************************extractLabel*******************************/    

char* extractLabel(char* currentLine)
{
    int sizeSymbol = findSizeSymbol(currentLine);
    
    char *symbol = (char *)malloc((sizeSymbol+1)*sizeof(char));

    strncpy(symbol, currentLine, sizeSymbol);

    return symbol;
}
/*******************************processSymbol*******************************/    

symbol_table *processToSymbolTable(char* label, int* IC, symbol_attribute attribute, symbol_table *symbolTable)
{
    symbol_table *newSymbol;

    char *tempString = malloc(sizeof(char) * (strlen(label)+1));

    strcpy(tempString, label);

    newSymbol = (symbol_table *)malloc(sizeof(symbol_table));

    newSymbol->symbol = label;

    newSymbol->attribute = attribute;

    newSymbol->value = *IC;

    newSymbol->offset = *(IC)%16;

    newSymbol->base = *(IC) - newSymbol->offset;

    newSymbol->next = symbolTable;

    printf("%s\n%d\n%d\n%d\n", newSymbol->symbol, newSymbol->attribute, newSymbol->offset, newSymbol->base);

    return newSymbol;

}

/*******************************amountOfData*******************************/    

int amountOfData(char* currentLine)
{
    int charCounter = 0;

    remove_spaces(currentLine);

    if (strstr(currentLine, ".data"))
    {
        currentLine = (strstr(currentLine, ".data") + 5);
    }

    else if (strstr(currentLine, ","))
    {
        for (charCounter = 0; (*(currentLine + charCounter) != ',') && (charCounter < SIZE_LINE) && (*(currentLine + charCounter) != '\0'); charCounter++)
        {
            continue;
        }
    }

    return charCounter;
}
/*******************************extractDataToCodeImage*******************************/    

void extractDataToCodeImage(char* currentLine, int* DC)
{
    code_line *tempLine = (code_line *)malloc(sizeof(code_line));

    char char_tempInt[SIZE_LINE];

    int tempInt;

    int amount_of_data; 

    tempLine->address = (DC[0])++;

    tempLine->code = (machine_code *)malloc(sizeof(machine_code));

    remove_spaces(currentLine);


    currentLine = (strstr(currentLine, ":") + 6);

    while (strstr(currentLine, ","))
    {
        amount_of_data = amountOfData(currentLine);

        strncpy(char_tempInt, currentLine, amount_of_data);

        tempInt = atof(char_tempInt);

        currentLine = (strstr(currentLine, ",") + 1);

        printf("\n\n%d\n", tempInt);
    }

    strncpy(char_tempInt, currentLine, sizeof(currentLine));

    tempInt = atof(char_tempInt);


    printf("\n\n%d\n", tempInt);


    /*tempLine->address = (DC[0])++;

    tempLine->code = (machine_code *)malloc(sizeof(machine_code));

    tempLine->code->hexaCode = intToHexaCode(tempInt);

    tempLine->code->ARE = ABSOLUTE;

    tempLine->next=NULL;

    int dataCounter, dataLen;


    remove_spaces(currentLine);

    currentLine = (strstr(currentLine, ":") + 6);

    for (dataCounter = 0; dataCounter < amount_of_data; dataCounter++)
    {
        dataLen = amountOfData(currentLine);

    }

    printf("\n\n%d\n", amountOfData(currentLine));*/
}