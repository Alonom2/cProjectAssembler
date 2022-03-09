#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "firstPass.h"
#include "shortFuncs.h"
#include "secondPass.h"



int main()
{
    char currentLine[SIZE_LINE];
    char previousLine[SIZE_LINE];
    FILE* fPointer1;

    char command_table[16][5] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    int *IC, *DC, counter, wasLabel = 0, notSameSymbol = 1, close_to_null_code = 0;
    char *label;
    symbol_table *symbolPointer;

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


/****************************************************FirstPass************************************************************/

    while(!feof(fPointer1))
    {
        for (counter = 0; counter < SIZE_LINE; counter++)
        {
            previousLine[counter] = currentLine[counter];
        }

        fgets(currentLine, SIZE_LINE, fPointer1);

        if (is_label(currentLine))
        {
            if (is_data(currentLine))
            {
                label = extractLabel(currentLine);

                symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, 0, symbolTable[0]);

                codeImage = extractDataToCodeImage(currentLine, IC, DC, codeImage);

                wasLabel = 1;

                continue;
            }

            else if (is_string(currentLine))
            {
                label = extractLabel(currentLine);

                symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, 0, symbolTable[0]);

                codeImage = extractStringToCodeImage(currentLine, IC, DC, codeImage);

                wasLabel = 1;

                continue;
            }

            else if (is_extern(currentLine))
            {
                label = extractLabel(currentLine);

                symbolTable[0] = processToSymbolTable(label, IC, EXTERN_ATT, 1, symbolTable[0]);

                if (wasLabel == 1)
                {
                    wasLabel = 0;
                }

                continue;
            }

            else if (is_entry(currentLine))
            {
                if (wasLabel == 1)
                {
                    wasLabel = 0;
                }

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

                    if (wasLabel == 1)
                    {
                        wasLabel = 0;
                    }
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

                else if (is_data(currentLine))
                {
                    if (is_data(previousLine) && wasLabel == 1)
                    {
                        codeImage = extractDataToCodeImage(currentLine, IC, DC, codeImage);
                    }
                }

                else if (is_string(currentLine))
                {
                    if (is_string(previousLine) && wasLabel == 1)
                    {
                        codeImage = extractStringToCodeImage(currentLine, IC, DC, codeImage);
                    }
                }

                else
                {
                    updateCommandAndFunc(currentLine, command_table, commandPointer);

                    updateOperands(currentLine, commandPointer);

                    updateAddressingMethods(currentLine, commandPointer);

                    codeImage = extractCommandToCodeImage(IC, commandPointer, codeImage);

                    if (wasLabel == 1)
                    {
                        wasLabel = 0;
                    }
                }
        }
    }

    fclose(fPointer1);

/****************************************************SecondPass***********************************************************/
    fPointer1 = fopen("eggs.am", "r");

    while(!feof(fPointer1))
    {
        fgets(currentLine, SIZE_LINE, fPointer1);

        if (is_entry(currentLine))
        {
            label = extractLabel(currentLine);

            symbolPointer = symbolTable[0];

            while (symbolPointer != NULL)
            {
                if (*(symbolPointer->symbol) == *(label) && strlen(symbolPointer->symbol) == strlen(label))
                {   
                    for (counter = 0; counter < strlen(label); counter++)
                    {
                        if (*(symbolPointer->symbol + counter) == *(label + counter))
                        {
                            continue;
                        }

                        else
                        {
                            notSameSymbol = 1;
                        }
                    }

                    if (notSameSymbol == 0)
                    {
                        symbolPointer->attribute_2 = ENTRY_ATT;
                    }
                }

                symbolPointer = symbolPointer->next;

                notSameSymbol = 0;
            }
        }
    }

    symbolPointer = symbolTable[0];

    while (symbolPointer != NULL)
    {
        codeImage->currCodeLine = codeImage->lastCodeLine;

        while (codeImage->currCodeLine != NULL)
        {
            if (codeImage->currCodeLine->next == NULL)
            {
                close_to_null_code = 1;
            }

            if (labelcmp(codeImage, symbolPointer) == 0 && close_to_null_code == 0)
            {
                codeImage->currCodeLine = codeImage->currCodeLine->next;

                continue;
            }

            else if (labelcmp(codeImage, symbolPointer) != 0)
            {
                if (codeImage->currCodeLine->code->hexaCode == NULL)
                {
                    if (labelcmp(codeImage, symbolPointer) > 0)
                    {
                        codeImage->currCodeLine->code->hexaCode = changeIntToHexa(symbolPointer->offset);

                        codeImage->currCodeLine->code->ARE = EXTERNAL;

                        codeImage->currCodeLine->next->code->hexaCode = changeIntToHexa(symbolPointer->base);

                        codeImage->currCodeLine->next->code->ARE = EXTERNAL;
                    }

                    if (labelcmp(codeImage, symbolPointer) == 3)
                    {
                        codeImage->currCodeLine->next->next->code->hexaCode = changeIntToHexa(symbolPointer->next->offset);

                        codeImage->currCodeLine->next->next->code->ARE = EXTERNAL;

                        codeImage->currCodeLine->next->next->next->code->hexaCode = changeIntToHexa(symbolPointer->next->base);

                        codeImage->currCodeLine->next->next->next->code->ARE = EXTERNAL;
                    }
                }

                else if (codeImage->currCodeLine->code->hexaCode != NULL)
                {
                    if (labelcmp(codeImage, symbolPointer) > 0)
                    {
                        codeImage->currCodeLine->next->code->hexaCode = changeIntToHexa(symbolPointer->offset);

                        codeImage->currCodeLine->next->code->ARE = EXTERNAL;

                        codeImage->currCodeLine->next->next->code->hexaCode = changeIntToHexa(symbolPointer->base);

                        codeImage->currCodeLine->next->next->code->ARE = EXTERNAL;
                    }

                    if (labelcmp(codeImage, symbolPointer) == 3)
                    {
                        codeImage->currCodeLine->next->next->next->code->hexaCode = changeIntToHexa(symbolPointer->next->offset);

                        codeImage->currCodeLine->next->next->next->code->ARE = EXTERNAL;

                        codeImage->currCodeLine->next->next->next->next->code->hexaCode = changeIntToHexa(symbolPointer->next->base);

                        codeImage->currCodeLine->next->next->next->next->code->ARE = EXTERNAL;
                    }
                }
            }
            

            if (close_to_null_code == 1)
            {
                close_to_null_code = 0;

                break;
            }

            codeImage->currCodeLine = codeImage->currCodeLine->next;
        }
        
        if (symbolPointer->next == NULL)
        {
            break;
        }

        else
        {
            symbolPointer = symbolPointer->next;
        }
    }

    fclose(fPointer1);
    freeSymbolTable(symbolTable[0]);
    freeCodeImage(codeImage);
    free(IC);
    free(DC);
    freeCommand(commandPointer);
    free(symbolPointer);
    free(commandPointer);

    return 0; 
}