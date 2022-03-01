#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "firstPass.h"



int main()
{
    code_image *codeImage; /*code image*/
    symbol_table **symbolTable; /*symbol table*/
    symbol_table *pointer;

    int *IC, *DC, *array;
    char *label;

/*************************Strings for tests*******************************/

    char currentLine[] = "LIST:  .data   +6, -9, 8, 378";
    char currentLine2[] = "LOOP:  .data   +12, -5";
    char currentLine3[] = "OMER:  .data   +2, -9";
    char currentLine4[] = "STR: .string \"ab c  d\"";

/*************************************************************************/

    int currentDataLine = 0;
    IC = (int *)malloc(sizeof(int)); 
    DC = (int *)malloc(sizeof(int));
    array = (int *)malloc(4*sizeof(int));
    IC[0] = 100;
    DC[currentDataLine] = 0;
    /*insertMacros("bacon.txt", "eggs.am");*/

    pointer = (symbol_table *)malloc(sizeof(symbol_table));
    symbolTable = (symbol_table **)malloc(sizeof(symbol_table));
    codeImage = (code_image *)malloc(sizeof(code_image));

    symbolTable[0] = NULL;
    codeImage->currCodeLine = NULL;
    codeImage->firstCodeLine = NULL;
    codeImage->firstDataLine = NULL;
    codeImage->lastCodeLine = NULL;
    codeImage->lastDataLine = NULL;


    /*label = extractLabel(currentLine); 
    symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, symbolTable[0]);*/

    /*pointer = symbolTable[0];*/ /*now the pointer gets the value of the first struct*/

    /*label = extractLabel(currentLine2);
    symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, symbolTable[0]);
    label = extractLabel(currentLine3);
    symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, symbolTable[0]);*/

    codeImage = extractDataToCodeImage(currentLine, DC, codeImage);
    codeImage = extractDataToCodeImage(currentLine2, DC, codeImage);
    codeImage = extractStringToCodeImage(currentLine4, DC, codeImage);
    codeImage = extractDataToCodeImage(currentLine3, DC, codeImage);

    freeSymbolTable(symbolTable[0]);
    freeCodeImage(codeImage);
    free(IC);
    free(DC);
    return 0; 
}