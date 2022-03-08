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
        n += 65536;
    }

    hexa[3] = changeIntToHexaChar(n % 16);

    n = n / 16;

    hexa[2] = changeIntToHexaChar(n % 16);

    n = n / 16;

    hexa[1] = changeIntToHexaChar(n % 16);

    n = n / 16;

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
    {
        value = TRUE;
    }

    else if (*currentLine == '.')
    {
        if (*(currentLine + 1) == 'e')
        {
            value = TRUE;
        }

        else
        {
            value = FALSE;
        }
    }

    else
    {
        value = FALSE;
    }
    
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

/****************************************************************************updateCommandAndFunc*****************************************************************************/    

void updateCommandAndFunc(char* currentLine, char command_table[16][5], command *new_command)
{
    int counter;

    remove_spaces(currentLine);

    for (counter = 0; counter < 16; counter++)
    {
        if (is_label(currentLine))
        {
            currentLine = strstr(currentLine, ":") + 1;
        }

        if (strstr(currentLine, command_table[counter]) == currentLine)
        {
            if (counter == 0 || counter == 1 || counter == 4 || counter == 12 || counter == 13 || counter == 14 || counter == 15)
            {
                new_command->opcode = counter;
                new_command->funct = NONE_FUNCT;
            }

            else if (counter == 2 || counter == 3)
            {
                new_command->opcode = 2;
                
                switch(counter)
                {
                    case 2:
                       new_command->funct = ADD_FUNCT; 
                       break;

                    case 3:
                        new_command->funct = SUB_FUNCT; 
                        break;
                }
            }

            else if (counter == 5 || counter == 6 || counter == 7 || counter == 8)
            {
                new_command->opcode = 5;

                switch(counter)
                {
                    case 5:
                       new_command->funct = CLR_FUNCT; 
                       break;

                    case 6:
                        new_command->funct = NOT_FUNCT; 
                        break;

                    case 7:
                       new_command->funct = INC_FUNCT; 
                       break;

                    case 8:
                        new_command->funct = DEC_FUNCT; 
                        break;
                }
            }

            else if (counter == 9 || counter == 10 || counter == 11 )
            {
                new_command->opcode = 9;

                switch(counter)
                {
                    case 9:
                       new_command->funct = JMP_FUNCT; 
                       break;

                    case 10:
                        new_command->funct = BNE_FUNCT; 
                        break;

                    case 11:
                       new_command->funct = JSR_FUNCT; 
                       break;
                }
            }
        }
    }
}

/******************************************************************************updateOperands********************************************************************************/    

void updateOperands(char *currentLine, command *new_command)
{
    int num_of_operands;
    int counter = 0;
    char *tempOperand1 = (char *)malloc(SIZE_LINE*sizeof(char));
    char *tempOperand2 = (char *)malloc(SIZE_LINE*sizeof(char));

    if (is_label(currentLine))
    {
        currentLine = strstr(currentLine, ":") + 1;
    }

    switch(new_command->opcode)
    {
        case 0:
            num_of_operands = 2;
            break;
        case 1:
            num_of_operands = 2;
            break;
        case 2:
            num_of_operands = 2;
            break;
        case 4:
            num_of_operands = 2;
            break;
        case 5:
            num_of_operands = 1;
            break;
        case 9:
            num_of_operands = 1;
            break;
        case 12:
            num_of_operands = 1;
            break;
        case 13:
            num_of_operands = 1;
            break;
        case 14:
            num_of_operands = 0;
            break;
        case 15:
            num_of_operands = 0;
            break;
    }

    switch (num_of_operands)
    {
        case 2:
            while (*(currentLine + 3 + counter) != ',' && *(currentLine + 3 + counter) != '\0')
            {
                counter++;
            }
            strncpy(tempOperand1, currentLine + 3, counter);

            *(tempOperand1 + counter) = '\0';

            new_command->originOperand = tempOperand1;

            currentLine = currentLine + counter + 4;

            strcpy(tempOperand2, currentLine);

            new_command->destinationOperand = tempOperand2;

            break;

        case 1:
            strcpy(tempOperand2, currentLine + 3);

            new_command->destinationOperand = tempOperand2;

            tempOperand1[0] = '0';

            new_command->originOperand = tempOperand1;

            break;

        case 0:

            tempOperand1[0] = '0';

            tempOperand2[0] = '0';

            new_command->originOperand = tempOperand1;

            new_command->destinationOperand = tempOperand2;

            break;
    }
}

/***********************************************************************isIMMEDIATE_ADDR*********************************************************************************/    

bool isIMMEDIATE_ADDR(char *currentLine, int operand)
{
    bool value = FALSE;

    switch(operand)
    {
        case 1:
            if ((*(currentLine + 3) == '#'))
                value = TRUE;
            break;

        case 2:
            if (strstr(currentLine, ","))
            {
                if (*(strstr(currentLine, ",") + 1) == '#')
                {
                    value = TRUE;
                }
            }
            break;

        case 3:
            if (*(currentLine + 3) == '#')
                value = TRUE;
            break;
    }
    
    return value;
}

/*************************************************************************isINDEX_ADDR***********************************************************************************/    

bool isINDEX_ADDR(char *currentLine, int operand)
{
    char *pt1, *pt2;

    if (operand == 1)
    {
        if (strstr(currentLine, "[r10]"))  
        {
            if (strstr(currentLine, "[r10]"))  
            {
                pt1 = strstr(currentLine, "[r10]");
                pt2 = strstr(currentLine, ",");

                if (pt1 < pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r11]"))  
        {
            if (strstr(currentLine, "[r11]"))  
            {
                pt1 = strstr(currentLine, "[r11]");
                pt2 = strstr(currentLine, ",");

                if (pt1 < pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r12]"))  
        {
            if (strstr(currentLine, "[r12]"))  
            {
                pt1 = strstr(currentLine, "[r12]");
                pt2 = strstr(currentLine, ",");

                if (pt1 < pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r13]"))  
        {
            if (strstr(currentLine, "[r13]"))  
            {
                pt1 = strstr(currentLine, "[r13]");
                pt2 = strstr(currentLine, ",");

                if (pt1 < pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r14]"))  
        {
            if (strstr(currentLine, "[r14]"))  
            {
                pt1 = strstr(currentLine, "[r14]");
                pt2 = strstr(currentLine, ",");

                if (pt1 < pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r15]"))  
        {
            if (strstr(currentLine, "[r15]"))  
            {
                pt1 = strstr(currentLine, "[r15]");
                pt2 = strstr(currentLine, ",");

                if (pt1 < pt2)
                {
                    return TRUE;
                }
            }
        }

        else
        {
            return FALSE;
        }
    }

    else if (operand == 2)
    {
        if (strstr(currentLine, "[r10]"))  
        {
            if (strstr(currentLine, "[r10]"))  
            {
                pt1 = strstr(currentLine, "[r10]");
                pt2 = strstr(currentLine, ",");

                if (pt1 > pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r11]"))  
        {
            if (strstr(currentLine, "[r11]"))  
            {
                pt1 = strstr(currentLine, "[r11]");
                pt2 = strstr(currentLine, ",");

                if (pt1 > pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r12]"))  
        {
            if (strstr(currentLine, "[r12]"))  
            {
                pt1 = strstr(currentLine, "[r12]");
                pt2 = strstr(currentLine, ",");

                if (pt1 > pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r13]"))  
        {
            if (strstr(currentLine, "[r13]"))  
            {
                pt1 = strstr(currentLine, "[r13]");
                pt2 = strstr(currentLine, ",");

                if (pt1 > pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r14]"))  
        {
            if (strstr(currentLine, "[r14]"))  
            {
                pt1 = strstr(currentLine, "[r14]");
                pt2 = strstr(currentLine, ",");

                if (pt1 > pt2)
                {
                    return TRUE;
                }
            }
        }

        else if (strstr(currentLine, "[r15]"))  
        {
            if (strstr(currentLine, "[r15]"))  
            {
                pt1 = strstr(currentLine, "[r15]");
                pt2 = strstr(currentLine, ",");

                if (pt1 > pt2)
                {
                    return TRUE;
                }
            }
        }

        else
        {
            return FALSE;
        }
    }

    else if (operand == 3)
    {
        if (strstr(currentLine, "[r10]"))
        {
            return TRUE;
        }

        if (strstr(currentLine, "[r11]"))
        {
            return TRUE;
        }

        if (strstr(currentLine, "[r12]"))
        {
            return TRUE;
        }

        if (strstr(currentLine, "[r13]"))
        {
            return TRUE;
        }

        if (strstr(currentLine, "[r14]"))
        {
            return TRUE;
        }

        if (strstr(currentLine, "[r15]"))
        {
            return TRUE;
        }

        else
        {
            return FALSE;
        }
    }

    return FALSE;
}

/*************************************************************************isREGISTER_ADDR********************************************************************************/    

bool isREGISTER_ADDR(char *currentLine, int operand)
{
    if (operand == 1 || operand == 3)
    {
        if (strstr(currentLine, "r1") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r2") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r3") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r4") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r5") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r6") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r7") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r8") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r9") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r10") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r11") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r12") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r13") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r14") == (currentLine + 3))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r15") == (currentLine + 3))  
        {
            return TRUE;
        }

        else
        {
            return FALSE;
        }
    }

    else if (operand == 2)
    {
        if (strstr(currentLine, "r1") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r2") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r3") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r4") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r5") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r6") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r7") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r8") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r9") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r10") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r11") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r12") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r13") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r14") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else if (strstr(currentLine, "r15") == (strstr(currentLine, ",") + 1))  
        {
            return TRUE;
        }

        else
        {
            return FALSE;
        }
    }

   else
   {
        return FALSE;
   }
}


/***********************************************************************updateAddressingMethods**************************************************************************/    


void updateAddressingMethods(char *currentLine, command *new_command)
{
    int done = 0;

    remove_spaces(currentLine);

    if (is_label(currentLine))
    {
        currentLine = strstr(currentLine, ":") + 1;
    }

    if (*(new_command->originOperand) == '0')
    {
        new_command->originOperandAddressingMethod = NO_OPERAND;
    }
    
    else if (isIMMEDIATE_ADDR(currentLine, 1))
    {
        if (new_command->opcode == 0 || new_command->opcode == 1 || new_command->opcode == 2)
        {
            new_command->originOperandAddressingMethod = IMMEDIATE_ADDR;
        }
    }

    else if (isINDEX_ADDR(currentLine, 1))
    {
        if (new_command->opcode == 0 || new_command->opcode == 1 || new_command->opcode == 2 || new_command->opcode == 4)
        {
            new_command->originOperandAddressingMethod = INDEX_ADDR;
        }
    }

    else if (isREGISTER_ADDR(currentLine, 1))
    {
        if (new_command->opcode == 0 || new_command->opcode == 1 || new_command->opcode == 2)
        {
            new_command->originOperandAddressingMethod = REGISTER_ADDR;
        }
    }

    else if (new_command->opcode == 0 || new_command->opcode == 1 || new_command->opcode == 2 || new_command->opcode == 4)
    {
        new_command->originOperandAddressingMethod = DIRECT_ADDR;
    }

    if (isIMMEDIATE_ADDR(currentLine, 2))
    {
        if (new_command->opcode == 1)
        {
            new_command->destinationOperandAddressingMethod = IMMEDIATE_ADDR;

            done = 1;
        }
    }

    else if (isINDEX_ADDR(currentLine, 2))
    {
        if (new_command->opcode == 0 || new_command->opcode == 1 || new_command->opcode == 2 || new_command->opcode == 4)
        {
            new_command->destinationOperandAddressingMethod = INDEX_ADDR;

            done = 1;
        }
    }

    else if (isREGISTER_ADDR(currentLine, 2))
    {
        if (new_command->opcode == 0 || new_command->opcode == 1 || new_command->opcode == 2 || new_command->opcode == 4)
        {
            new_command->destinationOperandAddressingMethod = REGISTER_ADDR;

            done = 1;
        }
    }

    else if (new_command->opcode == 0 || new_command->opcode == 1 || new_command->opcode == 2 || new_command->opcode == 4)
    {
        new_command->destinationOperandAddressingMethod = DIRECT_ADDR;

        done = 1;
    }
    
    if (done != 1)
    {
        if (isIMMEDIATE_ADDR(currentLine, 3))
        {
            if (new_command->opcode == 13)
            {
                new_command->destinationOperandAddressingMethod = IMMEDIATE_ADDR;
            }
        }

        else if (isINDEX_ADDR(currentLine, 3))
        {
            if (new_command->opcode == 5 || new_command->opcode == 9 || new_command->opcode == 12 || new_command->opcode == 13)
            {
                new_command->destinationOperandAddressingMethod = INDEX_ADDR;
            }
        }

        else if (isREGISTER_ADDR(currentLine, 3))
        {
            if (new_command->opcode == 5 || new_command->opcode == 12 || new_command->opcode == 13)
            {
                new_command->destinationOperandAddressingMethod = REGISTER_ADDR;
            }
        }

        else if (new_command->opcode == 5 || new_command->opcode == 9 || new_command->opcode == 12 || new_command->opcode == 13)
        {
            new_command->destinationOperandAddressingMethod = DIRECT_ADDR;
        }

        else
        {
            new_command->destinationOperandAddressingMethod = NO_OPERAND;
        }
    }
    
}