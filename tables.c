#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"


/*********************************************************************freeSymbolTable************************************************************************************/ 

void freeSymbolTable(symbol_table *symbolTable)
{
    symbol_table *temp;

    while (symbolTable != NULL)
    {
        temp = symbolTable->next;

        free(symbolTable->symbol);

        free(symbolTable);
        
        symbolTable = temp;
    }
}

/*********************************************************************freeCodeImage**************************************************************************************/ 
                                      
void freeCodeImage(code_image *codeImage)
{
    code_line *temp;

     while (codeImage->lastDataLine != NULL)
    {
        temp = codeImage->lastDataLine->next;

        free(codeImage->lastDataLine->code);

        free(codeImage->lastDataLine);
        
        codeImage->lastDataLine = temp;
    }
}

void freeCommand(command *new_command)
{
    free(new_command);
    free(new_command->destinationOperand);
    free(new_command->originOperand);
}