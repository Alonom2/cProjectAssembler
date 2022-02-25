#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "firstPass.h"



int main()
{
    code_image *codeImage; /*code image*/
    symbol_table *symbolTable; /*symbol table*/
    symbol_table *pointer;
    int *IC, *DC;
    char *label;

    char currentLine[] = "LIST:  .data   +6, -9";
    char currentLine2[] = "LOOP:  .data   +12, -5";
    char currentLine3[] = "OMER:  .data   +2, -9";
    int currentDataLine = 0;
    /*symbol_table mySymbolTable;*/
    IC = (int *)malloc(sizeof(int)); 
    DC = (int *)malloc(sizeof(int));
    IC[0] = 100;
    DC[currentDataLine] = 0;
    /*insertMacros("bacon.txt", "eggs.am");*/

    /*pointer = (symbol_table *)malloc(sizeof(symbol_table));*/
    symbolTable = (symbol_table *)malloc(sizeof(symbol_table));
    codeImage = (code_image *)malloc(sizeof(code_image));


    symbolTable->next = symbolTable;
    codeImage->currCodeLine = NULL;
    codeImage->firstCodeLine = NULL;
    codeImage->firstDataLine = NULL;
    codeImage->lastCodeLine = NULL;
    codeImage->lastDataLine = NULL;

    label = extractLabel(currentLine); 
    symbolTable = processToSymbolTable(label, IC, DATA_ATT, symbolTable);
    label = extractLabel(currentLine2);
    symbolTable = processToSymbolTable(label, IC, DATA_ATT, symbolTable);
    label = extractLabel(currentLine3);
    symbolTable = processToSymbolTable(label, IC, DATA_ATT, symbolTable);
    
    /*printf("\n%d\n", symbolTable->next->offset);*/
    /*printf("%d\n", DC[currentDataLine]);
    printf("\n\n%d\n", amountOfData(currentLine));
    extractDataToCodeImage(currentLine, DC);*/


    free(IC);
    free(DC);
    free(symbolTable);
    free(codeImage);
    return 0; /*fgfdhg*/ 
}