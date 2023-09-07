
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#pragma pack(1)

#ifndef OSDEVKIT_H
#define OSDEVKIT_H

#define OBJ_SIZE 256

typedef struct 
{
    uint8_t type;
    uint8_t size;
    uint8_t body[OBJ_SIZE];
    uint16_t internal_line;
    uint16_t internal_col;
    char * internal_name;
} obj_t;

extern bool obj_read(FILE * in, obj_t * obj);
extern bool obj_write(FILE * out, obj_t * obj);
extern bool obj_writeraw(FILE * out, uint8_t type, void * body, uint8_t size);
extern bool obj_writebyte(FILE * out, uint8_t type, uint8_t value);
extern bool obj_writeword(FILE * out, uint8_t type, uint16_t value);
extern bool obj_writestr(FILE * out, uint8_t type, char * string);

extern uint16_t parsenum(char * txt, int len);
extern uint16_t parsehex(char * txt, int len);

#define TK_NONE 0
#define LNK_NONE 0
#define LNK_END 1
#define LNK_FILE 2
#define LNK_FILE_LINE 3
#define LNK_FILE_COL 4
#define LNK_FUNC_START 5
#define LNK_FUNC_END 6
#define LNK_FUNC_USE 7
#define LNK_SEG_CODE 0x10
#define LNK_SEG_DATA 0x11
#define LNK_SEG_BSS 0x12
#define LNK_BINARY 0x20
#define LNK_CONST_PUBLIC 0x30
#define LNK_CONST_GLOBAL 0x31
#define LNK_CONST_LOCAL 0x32
#define LNK_CONST_VALUE 0x33
#define LNK_EXPR_START 0x40
#define LNK_EXPR_PUSH_VALUE 0x41
#define LNK_EXPR_PUSH_CONST 0x42
#define LNK_EXPR_PUSH_CURR_POS 0x43
#define LNK_EXPR_PUSH_CODE_OFFSET_POS 0x44
#define LNK_EXPR_PUSH_DATA_OFFSET_POS 0x45
#define LNK_EXPR_PUSH_BSS_OFFSET_POS 0x46
#define LNK_EXPR_PUSH_POP_ADD_PUSH 0x47
#define LNK_EXPR_PUSH_POP_SUB_PUSH 0x48
#define LNK_EXPR_PUSH_POP_MUL_PUSH 0x49
#define LNK_EXPR_PUSH_POP_DIV_PUSH 0x4a
#define LNK_EXPR_PUSH_POP_MOD_PUSH 0x4b
#define LNK_EXPR_PUSH_POP_SHL_PUSH 0x4c
#define LNK_EXPR_PUSH_POP_SHR_PUSH 0x4d
#define LNK_EXPR_EMIT_POP_BYTE 0x4e
#define LNK_EXPR_EMIT_POP_WORD 0x4f
#define LNK_PTR_PUBLIC 0x50
#define LNK_PTR_GLOBAL 0x51
#define LNK_PTR_LOCAL 0x52

#define TK_ID 0x80
#define TK_NUM 0x81
#define TK_STR 0x82
#define TK_COMMA 0x83
#define TK_PAR_OPEN 0x84
#define TK_PAR_CLOSE 0x85
#define TK_MATH_ADD 0x86
#define TK_MATH_SUB 0x87
#define TK_COLON 0x88
#define TK_IDX_OPEN 0x89
#define TK_IDX_CLOSE 0x8a
#define TK_SEMICOLON 0x8b
#define TK_BLOCK_OPEN 0x8c
#define TK_BLOCK_CLOSE 0x8d
#define TK_NEW_LINE 0x8e
#define TK_MATH_INC 0x8f
#define TK_MATH_DEC 0x90
#define TK_MATH_MUL 0x91
#define TK_MATH_DIV 0x92
#define TK_MATH_MOD 0x93
#define TK_CMP_AND 0x94
#define TK_CMP_OR 0x95
#define TK_CMP_EQUAL 0x96
#define TK_CMP_NOT_EQUAL 0x97
#define TK_CMP_LESSER 0x98
#define TK_CMP_LESSER_EQUAL 0x99
#define TK_CMP_GREATER 0x9a
#define TK_CMP_GREATER_EQUAL 0x9b
#define TK_CMP_NOT 0x9c
#define TK_BIT_AND 0x9d
#define TK_BIT_OR 0x9e
#define TK_BIT_XOR 0x9f
#define TK_BIT_NOT 0xa0
#define TK_BIT_SHL 0xa1
#define TK_BIT_SHR 0xa2
#define TK_ASSIGN 0xa3
#define TK_ASSIGN_ADD 0xa4
#define TK_ASSIGN_SUB 0xa5
#define TK_ASSIGN_MUL 0xa6
#define TK_ASSIGN_DIV 0xa7
#define TK_ASSIGN_MOD 0xa8
#define TK_ASSIGN_SHL 0xa9
#define TK_ASSIGN_SHR 0xaa
#define TK_ASSIGN_AND 0xab
#define TK_ASSIGN_OR 0xac
#define TK_ASSIGN_XOR 0xad
#define TK_POINTER_MARKER 0xae
#define TK_ADDRESS_MARKER 0xaf

#define TK_CUSTOM 0xc0


typedef struct
{
    uint16_t low;
    uint16_t high;
} ui32_t;

typedef struct
{
    uint16_t llow;
    uint16_t lhigh;
    uint16_t hlow;
    uint16_t hhigh;
} ui64_t;

#define CMP_EQUAL 0
#define CMP_NOT_EQUAL 1
#define CMP_LESSER 2
#define CMP_LESSER_EQUAL 3
#define CMP_GREATER 4
#define CMP_GREATER_EQUAL 5

void ui32_clear(ui32_t * value);
void ui32_parse(ui32_t * dest, char * orig);
void ui32_tostring(char * dest, ui32_t * orig, uint16_t len);
void ui32_add(ui32_t * dest, ui32_t * orig);
void ui32_sub(ui32_t * dest, ui32_t * orig);
void ui32_mul(ui32_t * dest, ui32_t * orig);
bool ui32_div(ui32_t * dest, ui32_t * out_mod, ui32_t * orig);
void ui32_shl(ui32_t * dest, uint8_t bits);
void ui32_shr(ui32_t * dest, uint8_t bits);
void ui32_rol(ui32_t * dest, uint8_t bits);
void ui32_ror(ui32_t * dest, uint8_t bits);

void ui64_clear(ui64_t * value);
void ui64_parse(ui64_t * dest, char * orig);
void ui64_tostring(char * dest, ui64_t * orig, uint16_t len);
void ui64_add(ui64_t * dest, ui64_t * orig);
void ui64_sub(ui64_t * dest, ui64_t * orig);
void ui64_mul(ui64_t * dest, ui64_t * orig);
bool ui64_div(ui64_t * dest, ui64_t * out_mod, ui64_t * orig);
void ui64_shl(ui64_t * dest, uint8_t bits);
void ui64_shr(ui64_t * dest, uint8_t bits);
void ui64_rol(ui64_t * dest, uint8_t bits);
void ui64_ror(ui64_t * dest, uint8_t bits);


#endif
