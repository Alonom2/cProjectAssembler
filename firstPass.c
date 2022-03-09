#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firstPass.h"
#include "shortFuncs.h"

/***********************************************************************findSizeSymbol***********************************************************************************/ 

int findSizeSymbol(char* currentLine)
{
    int sizeSymbol;

    char *ptLabel;

    remove_spaces(currentLine);

    if (is_extern(currentLine))
    {
        ptLabel = strstr(currentLine, "extern") + 6;

        for (sizeSymbol = 0; (*(ptLabel + sizeSymbol) != '\0') && sizeSymbol < SIZE_LINE; sizeSymbol++)
        {
            continue;
        }
    } 

    else
    {
        for (sizeSymbol = 0; (*(currentLine + sizeSymbol) != ':') && sizeSymbol < SIZE_LINE; sizeSymbol++)
        {
            continue;
        }
    }

    return sizeSymbol;
}

/*************************************************************************extractLabel***********************************************************************************/ 

char* extractLabel(char* currentLine)
{
    int sizeSymbol = findSizeSymbol(currentLine);

    char *symbol = (char *)malloc((sizeSymbol+1)*sizeof(char));

    if (is_extern(currentLine))
    {
        strncpy(symbol, currentLine + 7, sizeSymbol);
    }

    else if (is_entry(currentLine))
    {
        strncpy(symbol, currentLine + 6, sizeSymbol);
    }

    else
    {
        strncpy(symbol, currentLine, sizeSymbol);
    }

    return symbol;
}

/*********************************************************************processToSymbolTable*******************************************************************************/ 

symbol_table *processToSymbolTable(char* label, int* IC, symbol_attribute attribute, int isExtern, symbol_table *symbolTable)
{
    symbol_table *newSymbol;

    newSymbol = (symbol_table *)malloc(sizeof(symbol_table));

    newSymbol->symbol = label;

    newSymbol->attribute = attribute;

    newSymbol->attribute_2 = UNDEFINED_ATT;

    if (isExtern == 1)
    {
        newSymbol->value = 0;

        newSymbol->offset = 0;

        newSymbol->base = 0;
    }

    else
    {
        newSymbol->value = *IC;

        newSymbol->offset = *(IC)%16;

        newSymbol->base = *(IC) - newSymbol->offset;
    }

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
        if (strstr(currentLine, "\""))
        {
            pointerToChar = (strstr(currentLine, "\"") + 1);

            for (charCounter = 0; (*pointerToChar != '\"') && (charCounter < SIZE_LINE); charCounter++)
            {
                pointerToChar++;
            }
        }

        else if (strstr(currentLine, "“"))
        {
            printf("Warning! This code only accepts parentheses that looks like this \"\n");
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

    int* array = (int *)malloc((amount+1)*sizeof(int));

    remove_spaces(currentLine);

    if (strstr(currentLine, ":"))
    {
        currentLine = strstr(currentLine, ":") + 6;
    }

    else
    {
        currentLine = strstr(currentLine, ".data") + 5;
    }

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

    if (amount > 1)
    {
        *(array + counter - 1) = tempInt;
    }

    else
    {
        *(array) = tempInt;
    }

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

code_image *extractDataToCodeImage(char* currentLine, int *IC, int* DC, code_image *codeImage)
{
    int counter;

    int amount_of_data = amountOfDatas(currentLine);

    int *dataArray = (int *)malloc(amount_of_data*sizeof(int));

    code_line *tempLine = (code_line *)malloc(sizeof(code_line));

    dataArray = turnDataLineToArray(currentLine);

    tempLine = codeImage->lastDataLine;

    for (counter = 0; counter < amount_of_data; counter++)
    {
        tempLine = extractDataLineToCodeImage(*(dataArray + counter), IC, DC, tempLine);

        (IC[0])++;
    }

    codeImage->lastDataLine = tempLine;


    return codeImage;
}

/*******************************************************************extractStringToCodeImage*****************************************************************************/ 

code_image *extractStringToCodeImage(char* currentLine, int *IC, int* DC, code_image *codeImage)
{
    int counter;

    int amount_of_chars = amountOfChars(currentLine);

    int *charsArray = (int *)malloc(amount_of_chars*sizeof(int));

    code_line *tempLine = (code_line *)malloc(sizeof(code_line));

    charsArray = turnStringLineToArray(currentLine);

    tempLine = codeImage->lastDataLine;

    for (counter = 0; counter < amount_of_chars; counter++)
    {
        tempLine = extractDataLineToCodeImage(*(charsArray + counter), IC, DC, tempLine);

        (IC[0])++;
    }

    (IC[0])++;
    
    codeImage->lastDataLine = tempLine;

    return codeImage;
}
/******************************************************************extractDataLineToCodeImage****************************************************************************/ 

code_line *extractDataLineToCodeImage(int n, int *IC, int* DC, code_line *codeLine)
{
    code_line *newCodeLine = (code_line *)malloc(sizeof(code_line));

    newCodeLine->address = (DC[0])++;

    newCodeLine->code = (machine_code *)malloc(sizeof(machine_code));

    newCodeLine->code->hexaCode = changeIntToHexa(n);

    newCodeLine->code->ARE = ABSOLUTE;

    newCodeLine->next = codeLine;

    return newCodeLine;
}


/**********************************************************************extractCommandToCodeImage*************************************************************************/ 

code_image *extractCommandToCodeImage(int* IC, command* new_command, code_image *codeImage)
{
    code_line *tempLine = (code_line *)malloc(sizeof(code_line));

    int counter = 0;

    char *hexaChar;
    
    char char3 = CodeLineSecondWordToMachineCode(new_command, 3);

    char char2 = CodeLineSecondWordToMachineCode(new_command, 2);

    char char1 = CodeLineSecondWordToMachineCode(new_command, 1);

    char char0 = CodeLineSecondWordToMachineCode(new_command, 0);

    hexaChar = (char *)malloc(5*sizeof(char));

    *(hexaChar) = char3;

    *(hexaChar + 1) = char2;

    *(hexaChar + 2) = char1;

    *(hexaChar + 3) = char0;

    tempLine = codeImage->lastCodeLine;

    tempLine = extractCodeLineToCodeImage(1, IC, hexaChar, tempLine, new_command);

    if (new_command->opcode == RTS_OP || new_command->opcode == STOP_OP)
    {
        codeImage->lastCodeLine = tempLine;

        tempLine->label_1 = "NO_LABEL";

        tempLine->label_2 = "NO_LABEL";

        return codeImage;
    }

    else
    {
        tempLine = extractCodeLineToCodeImage(2, IC, hexaChar, tempLine, new_command);
    }

    if (new_command->originOperandAddressingMethod == IMMEDIATE_ADDR)
    {
        tempLine = extractCodeLineToCodeImage(3, IC, hexaChar, tempLine, new_command);
    }

    else if (new_command->originOperandAddressingMethod == INDEX_ADDR || new_command->originOperandAddressingMethod == DIRECT_ADDR)
    {
        tempLine = extractCodeLineToCodeImage(4, IC, hexaChar, tempLine, new_command);

        tempLine = extractCodeLineToCodeImage(4, IC, hexaChar, tempLine, new_command);
    }

    if (new_command->destinationOperandAddressingMethod == IMMEDIATE_ADDR)
    {
        tempLine = extractCodeLineToCodeImage(3, IC, hexaChar, tempLine, new_command);
    }

    else if (new_command->destinationOperandAddressingMethod == INDEX_ADDR || new_command->destinationOperandAddressingMethod == DIRECT_ADDR)
    {
        tempLine = extractCodeLineToCodeImage(4, IC, hexaChar, tempLine, new_command);

        tempLine = extractCodeLineToCodeImage(4, IC, hexaChar, tempLine, new_command);
    }

    if (new_command->originOperandAddressingMethod == DIRECT_ADDR)
    {
        tempLine->label_1 = new_command->originOperand;
    }

    else if (new_command->originOperandAddressingMethod == INDEX_ADDR)
    {
        tempLine->label_1 = new_command->originOperand;

        while (*(new_command->originOperand + counter) != '[')
        {
            counter++;
        }

        *(tempLine->label_1 + counter) = '\0';

        counter = 0;
    }

    if (new_command->destinationOperandAddressingMethod == DIRECT_ADDR)
    {
        tempLine->label_2 = new_command->destinationOperand;
    }

    else if (new_command->destinationOperandAddressingMethod == INDEX_ADDR)
    {
        tempLine->label_2 = new_command->destinationOperand;

        while (*(new_command->destinationOperand + counter) != '[')
        {
            counter++;
        }

        *(tempLine->label_2 + counter) = '\0';

        counter = 0;
    }

    codeImage->lastCodeLine = tempLine;

    return codeImage;
}

/******************************************************************extractCodeLineToCodeImage****************************************************************************/ 

code_line *extractCodeLineToCodeImage(int word, int* IC, char *hexaChar, code_line *codeLine, command* new_command)
{
    code_line *newCodeLine = (code_line *)malloc(sizeof(code_line));

    if (word == 1)
    {
        newCodeLine->address = (IC[0])++;

        newCodeLine->code = (machine_code *)malloc(sizeof(machine_code));

        newCodeLine->code->hexaCode = changeIntToHexa(1<<(new_command->opcode));

        newCodeLine->code->ARE = ABSOLUTE;

        newCodeLine->label_1 = "NO_LABEL";

        newCodeLine->label_2 = "NO_LABEL";

        newCodeLine->next = codeLine;

        return newCodeLine;
    }

    else if (word == 2)
    {
        newCodeLine->address = (IC[0])++;

        newCodeLine->code = (machine_code *)malloc(sizeof(machine_code));

        newCodeLine->code->hexaCode = hexaChar;

        newCodeLine->code->ARE = ABSOLUTE;

        newCodeLine->label_1 = "NO_LABEL";

        newCodeLine->label_2 = "NO_LABEL";

        newCodeLine->next = codeLine;

        return newCodeLine;
    }

    else if (word == 3)
    {
        newCodeLine->address = (IC[0])++;

        newCodeLine->code = (machine_code *)malloc(sizeof(machine_code));

        if (new_command->originOperandAddressingMethod == IMMEDIATE_ADDR)
        {
            newCodeLine->code->hexaCode = changeIntToHexa(atol(new_command->originOperand));
        }

        else
        {
            newCodeLine->code->hexaCode = changeIntToHexa(atol((new_command->destinationOperand) + 1));
        }

        newCodeLine->code->ARE = ABSOLUTE;

        newCodeLine->label_1 = "NO_LABEL";

        newCodeLine->label_2 = "NO_LABEL";

        newCodeLine->next = codeLine;

        return newCodeLine;
    }

    else if (word == 4)
    {
        newCodeLine->address = (IC[0])++;

        newCodeLine->code = (machine_code *)malloc(sizeof(machine_code));

        newCodeLine->label_1 = "NO_LABEL";

        newCodeLine->label_2 = "NO_LABEL";

        newCodeLine->next = codeLine;

        return newCodeLine;
    }

    return newCodeLine;
}

/**********************************************************************turnLastCharsToMachineCode************************************************************************/ 

char CodeLineSecondWordToMachineCode(command *new_command, int which)
{
    char char3, char2, char1, char0;
    char binary_table_long[16][5] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
    char *binaryNumOfRegister = binary_table_long[atol((new_command->destinationOperand) + 1)];
    char *binaryNumOfRegisterIndex = binary_table_long[numOfRegisterIndex_ADDR(new_command)];
    int num1 = 0;
    int num0 = 0;

    if (which == 3)
    {
        char3 = changeIntToHexaChar(new_command->funct);
        return char3;
    }

    else if (which == 2)
    {
        if (new_command->originOperandAddressingMethod == REGISTER_ADDR)
        {
            char2 = changeIntToHexaChar(atol((new_command->originOperand) + 1));
        }

        else if (new_command->originOperandAddressingMethod == INDEX_ADDR)
        {
            char2 = changeIntToHexaChar(atol(strstr(new_command->originOperand, "[r") + 2));
        }

        else
        {
            char2 = '0';
        }

        return char2;
    }

    if (new_command->originOperandAddressingMethod == NO_OPERAND)
    {
        num1 += 0;
    }

    else
    {
        num1 += 4*(new_command->originOperandAddressingMethod);
    }

    if (new_command->destinationOperandAddressingMethod == REGISTER_ADDR)
    {
        num1 += 2*((*binaryNumOfRegister) - 48);

        num1 += 1*((*(binaryNumOfRegister + 1)) - 48);

        num0 += 8*((*(binaryNumOfRegister + 2)) - 48);

        num0 += 4*((*(binaryNumOfRegister + 3)) - 48);
    }

    else if (new_command->destinationOperandAddressingMethod == INDEX_ADDR)
    {
        num1 += 2*(*(binaryNumOfRegisterIndex) - 48);

        num1 += 1*(*(binaryNumOfRegisterIndex + 1) - 48);

        num0 += 8*(*(binaryNumOfRegisterIndex + 2) - 48);

        num0 += 4*(*(binaryNumOfRegisterIndex + 3) - 48);
    }

    num0 += (new_command->destinationOperandAddressingMethod);

    if (which == 1)
    {
        char1 = changeIntToHexaChar(num1);

        return char1;
    }

    char0 = changeIntToHexaChar(num0);

    return char0;

}

/**********************************************************************numOfRegisterIndex_ADDR***************************************************************************/ 

int numOfRegisterIndex_ADDR(command *new_command)
{
    char tempChar[6], tempChar2[3];

    int result = 0;

    if (new_command->destinationOperandAddressingMethod == INDEX_ADDR)
    {
        strcpy(tempChar, strstr(new_command->destinationOperand, "[r"));

        if (tempChar[3] == ']')
        {
            strncpy(tempChar2, tempChar + 2, 1);
        }

        else
        {
            strncpy(tempChar2, tempChar + 2, 2);
        }

        result = atol(tempChar2);
    }

    return result;
}