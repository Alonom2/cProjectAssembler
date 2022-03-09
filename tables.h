#ifndef _TABLES__HEADER_
#define _TABLES__HEADER_

#include "macro.h"

/***************************symbol_attributes********************************/

typedef enum symbol_attribute
{
    DATA_ATT,
    
    EXTERN_ATT,
   
    CODE_ATT,

    ENTRY_ATT,

    UNDEFINED_ATT

} symbol_attribute;

/*****************************symbol_table***********************************/

typedef struct symbol_table_t symbol_table;

typedef struct symbol_table_t
{
    /* symbol name */
    char *symbol;
    /* value */
    int value;
    /* base */
    int base;
    /* offset */
    int offset;
    /* attribute (data,external,code) */
    symbol_attribute attribute;

    symbol_attribute attribute_2;
    /* next */
    symbol_table *next;

} symbol_table_t;

/*****************************command_opcode***********************************/ 

typedef enum command_opcode
{
    MOV_OP = 0,
    CMP_OP = 1,
    ADD_OP = 2,
    SUB_OP = 2,
    LEA_OP = 4,
    CLR_OP = 5,
    NOT_OP = 5,
    INC_OP = 5,
    DEC_OP = 5,
    JMP_OP = 9,
    BNE_OP = 9,
    JSR_OP = 9,
    RED_OP = 12,
    PRN_OP = 13,
    RTS_OP = 14,
    STOP_OP = 15

} command_opcode;

/*****************************command_funct**********************************/ 

typedef enum command_funct
{
    /* OPCODE 2 */
    ADD_FUNCT = 10,
    SUB_FUNCT = 11,

    /* OPCODE 5 */
    CLR_FUNCT = 10,
    NOT_FUNCT = 11,
    INC_FUNCT = 12,
    DEC_FUNCT = 13,

    /* OPCODE 9 */
    JMP_FUNCT = 10,
    BNE_FUNCT = 11,
    JSR_FUNCT = 12,

    /** Default (No need/Error) */
    NONE_FUNCT = 0

} command_funct;

/**************************A,R,E properties*********************************/

typedef enum ARE_property
{
    /* absolute */
    ABSOLUTE,
    /* relocatatable */
    RELOCATEABLE,
    /* external */
    EXTERNAL,
    /* unknown yet(first pass) */
    UNDEFINED

} ARE_property;

/***********************operand_addressing_method****************************/

typedef enum operand_addressing_method
{
    /* immediate */
    IMMEDIATE_ADDR = 0,
    /* direct */
    DIRECT_ADDR = 1,
    /* index*/
    INDEX_ADDR = 2,
    /*register*/
    REGISTER_ADDR = 3,
    /* no operand */
    NO_OPERAND = -1,
    /* illegal symbol */
    ILLEGAL_SYMBOL = -2

} operand_addressing_method;

/***************************command_struct*********************************/

typedef struct command
{
    command_opcode opcode;

    command_funct funct;

    operand_addressing_method originOperandAddressingMethod;

    char *originOperand;

    operand_addressing_method destinationOperandAddressingMethod;

    char *destinationOperand;
     
} command;
/****************************machine_code**********************************/

typedef struct machine_code
{
    char *hexaCode;

    ARE_property ARE;

} machine_code;

/******************************code_line***********************************/

typedef struct code_line_t code_line;

typedef struct code_line_t
{
    int address;
    
    machine_code *code;

    char *label_1;

    char *label_2;

    code_line *next;

} code_line_t;


/******************************code_image**********************************/

typedef struct code_image
{
    /* code */
    code_line *firstCodeLine;

    code_line *lastCodeLine;

    /* data */
    code_line *firstDataLine;

    code_line *lastDataLine;

    /* pointer for second pass traversal; TODO: set to firstCodeLine in assembler func between first and second pass */

    code_line *currCodeLine;

} code_image;

/*************************function: freeSymbolTable************************/

/* The function frees up all the space allocated to symbolTable           */
                                      
void freeSymbolTable(symbol_table *symbolTable);

/**************************function: freeCodeImage*************************/

/* The function frees up all the space allocated to codeImage             */
                                      
void freeCodeImage(code_image *codeImage);

void freeCommand(command *new_command);

#endif