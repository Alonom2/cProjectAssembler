#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"

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
