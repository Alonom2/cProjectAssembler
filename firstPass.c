#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firstPass.h"
#include "shortFuncs.h"

/***********************************************************************findSizeSymbol***********************************************************************************/ 

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

/*************************************************************************extractLabel***********************************************************************************/ 

char* extractLabel(char* currentLine)
{
    int sizeSymbol = findSizeSymbol(currentLine);
    
    char *symbol = (char *)malloc((sizeSymbol+1)*sizeof(char));

    strncpy(symbol, currentLine, sizeSymbol);

    return symbol;
}

/*********************************************************************processToSymbolTable*******************************************************************************/ 

symbol_table *processToSymbolTable(char* label, int* IC, symbol_attribute attribute, symbol_table *symbolTable)
{
    symbol_table *newSymbol;

    newSymbol = (symbol_table *)malloc(sizeof(symbol_table));

    newSymbol->symbol = label;

    newSymbol->attribute = attribute;

    newSymbol->value = *IC;

    newSymbol->offset = *(IC)%16;

    newSymbol->base = *(IC) - newSymbol->offset;

    newSymbol->next = symbolTable;

    return newSymbol;
}

/*******************************************************************amountOfCharsInData**********************************************************************************/ 

int amountOfCharsInData(char* currentLine)
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

/***********************************************************************amountOfDatas************************************************************************************/ 

int amountOfDatas(char* currentLine)
{
    char *pointerToData;

    int dataCounter = 0;

    int charCounter = 0; /* extra safety measure against infinite loop */

    if (strstr(currentLine, ".data"))
    {
        remove_spaces(currentLine);

        pointerToData = strstr(currentLine, ".data");

        for (charCounter = 0; (*pointerToData != '\0') && (charCounter < SIZE_LINE); charCounter++)
        {
            pointerToData++;

            if (*pointerToData == ',')
            {
                dataCounter++;
            }
        }

    }

    return ++dataCounter;
}

/***********************************************************************amountOfChars************************************************************************************/ 

int amountOfChars(char* currentLine)
{
    char *pointerToChar;

    int charCounter = 0; 

    if (strstr(currentLine, ".string"))
    {
        pointerToChar = (strstr(currentLine, "\"") + 1);

        for (charCounter = 0; (*pointerToChar != '\"') && (charCounter < SIZE_LINE); charCounter++)
        {
            pointerToChar++;
        }

    }

    return charCounter;
}

/***********************************************************************turnDataLineToArray******************************************************************************/ 

int *turnDataLineToArray(char* currentLine)
{
    int counter, amount_of_data_chars, tempInt;

    int amount = amountOfDatas(currentLine);

    char char_tempInt[SIZE_LINE];

    int* array = (int *)malloc(amount*sizeof(int));

    remove_spaces(currentLine);

    currentLine = strstr(currentLine, ":") + 6;

    for (counter = 0; counter < amount - 1; counter++)
    {
        amount_of_data_chars = amountOfCharsInData(currentLine);

        char_tempInt[amount_of_data_chars] = '\0';

        strncpy(char_tempInt, currentLine, amount_of_data_chars);

        tempInt = atof(char_tempInt);

        *(array + counter) = tempInt;

        currentLine = (strstr(currentLine, ",") + 1);
    }

    counter = 0;

    while (*(currentLine + counter) != '\0')
    {
        counter++;
    }

    char_tempInt[counter] = '\0';

    strcpy(char_tempInt, currentLine);

    tempInt = atof(char_tempInt);

    *(array + counter) = tempInt;

    return array;
}

/***********************************************************************turnStringLineToArray****************************************************************************/ 

int *turnStringLineToArray(char* currentLine)
{
    int counter;

    int amount_of_chars = amountOfChars(currentLine);

    int* array = (int *)malloc(amount_of_chars*sizeof(int));

    currentLine = (strstr(currentLine, "\"") + 1);

    for (counter = 0; counter < amount_of_chars; counter++)
    {
        *(array + counter) = *(currentLine + counter);
    }

    return array;
}

/********************************************************************extractDataToCodeImage******************************************************************************/ 

code_image *extractDataToCodeImage(char* currentLine, int* DC, code_image *codeImage)
{
    int counter;

    int amount_of_data = amountOfDatas(currentLine);

    int *dataArray = (int *)malloc(amount_of_data*sizeof(int));

    code_line **tempLine = (code_line **)malloc(sizeof(code_line));

    dataArray = turnDataLineToArray(currentLine);

    tempLine[0] = codeImage->lastDataLine;

    for (counter = 0; counter < amount_of_data; counter++)
    {
        tempLine[0] = extractDataLineToCodeImage(*(dataArray + counter), DC, tempLine[0]);
    }
    
    codeImage->lastDataLine = tempLine[0];


    return codeImage;
}

/*******************************************************************extractStringToCodeImage*****************************************************************************/ 

code_image *extractStringToCodeImage(char* currentLine, int* DC, code_image *codeImage)
{
    int counter;

    int amount_of_chars = amountOfChars(currentLine);

    int *charsArray = (int *)malloc(amount_of_chars*sizeof(int));

    code_line **tempLine = (code_line **)malloc(sizeof(code_line));

    charsArray = turnStringLineToArray(currentLine);

    tempLine[0] = codeImage->lastDataLine;

    for (counter = 0; counter < amount_of_chars; counter++)
    {
        tempLine[0] = extractDataLineToCodeImage(*(charsArray + counter), DC, tempLine[0]);
    }
    
    codeImage->lastDataLine = tempLine[0];

    return codeImage;
}
/******************************************************************extractDataLineToCodeImage****************************************************************************/ 

code_line *extractDataLineToCodeImage(int n, int* DC, code_line *codeLine)
{
    code_line *newCodeLine = (code_line *)malloc(sizeof(code_line));

    newCodeLine->address = (DC[0])++;

    newCodeLine->code = (machine_code *)malloc(sizeof(machine_code));

    newCodeLine->code->hexaCode = changeIntToHexa(n);

    newCodeLine->code->ARE = ABSOLUTE;

    newCodeLine->next = codeLine;

    return newCodeLine;
}

