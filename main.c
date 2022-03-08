#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "firstPass.h"
#include "shortFuncs.h"



int main()
{
    char currentLine[SIZE_LINE];
    FILE* fPointer1;

    char command_table[16][5] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    int *IC, *DC;
    char *label;

    command *commandPointer;
    code_image *codeImage; /*code image*/
    symbol_table **symbolTable; /*symbol table*/


/*************************Strings for tests*******************************/

    /*char currentLine2[] = "LOOP:  .data   +12, -5";
    char currentLine3[] = "OMER:  .data   +2, -9";
    char currentLine4[] = "STR: .string \"ab c  d\"";
    char currentLine5[] = "add r3 ,LIST";
    char currentLine6[] = "sub  r1, r4";
    char currentLine7[] = "bne END[r15]";
    char currentLine8[] = "stop";*/

/*************************************************************************/

    fPointer1 = fopen("eggs.am", "r");
    IC = (int *)malloc(sizeof(int)); 
    DC = (int *)malloc(sizeof(int));

    IC[0] = 100;
    DC[0] = 0;
    /*insertMacros("bacon.txt", "eggs.am");*/

    label = (char *)malloc(SIZE_LINE*sizeof(char));
    symbolTable = (symbol_table **)malloc(sizeof(symbol_table));
    codeImage = (code_image *)malloc(sizeof(code_image));
    commandPointer = (command *)malloc(sizeof(command));

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

    while(!feof(fPointer1))
    {
        fgets(currentLine, SIZE_LINE, fPointer1);

        if (is_label(currentLine))
        {
            if (is_data(currentLine))
            {
                label = extractLabel(currentLine);

                codeImage = extractDataToCodeImage(currentLine, IC, DC, codeImage);

                symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, 0, symbolTable[0]);

                continue;
            }

            else if (is_string(currentLine))
            {
                label = extractLabel(currentLine);

                symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, 0, symbolTable[0]);

                codeImage = extractStringToCodeImage(currentLine, IC, DC, codeImage);

                continue;
            }

            else if (is_extern(currentLine))
            {
                label = extractLabel(currentLine);

                symbolTable[0] = processToSymbolTable(label, IC, EXTERN_ATT, 1, symbolTable[0]);

                continue;
            }

            else if (is_entry(currentLine))
            {

                continue;
            }
            
            else
            {
                remove_spaces(currentLine);

                if (*(currentLine) == '\0' || *(currentLine) == '\n')
                {
                    continue;
                }

                else
                {
                    label = extractLabel(currentLine);

                    symbolTable[0] = processToSymbolTable(label, IC, CODE_ATT, 0, symbolTable[0]);

                    updateCommandAndFunc(currentLine, command_table, commandPointer);

                    updateOperands(currentLine, commandPointer);

                    updateAddressingMethods(currentLine, commandPointer);

                    codeImage = extractCommandToCodeImage(IC, commandPointer, codeImage);
                }
            }
        }

        else
        {
            remove_spaces(currentLine);

                if (*(currentLine) == '\0' || *(currentLine) == '\n')
                {
                    continue;
                }

                else
                {
                    updateCommandAndFunc(currentLine, command_table, commandPointer);

                    updateOperands(currentLine, commandPointer);

                    updateAddressingMethods(currentLine, commandPointer);

                    codeImage = extractCommandToCodeImage(IC, commandPointer, codeImage);
                }
        }
    }

    freeSymbolTable(symbolTable[0]);
    freeCodeImage(codeImage);
    free(IC);
    free(DC);
    freeCommand(commandPointer);

    return 0; 
}