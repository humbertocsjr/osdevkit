
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#pragma pack(1)

#ifndef OSDEVKIT_H

#define OBJ_SIZE 256

typedef struct 
{
    uint8_t type;
    uint8_t size;
    uint8_t body[OBJ_SIZE];
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
#define LNK_EXPR_PUSH_NEXT_POS 0x44
#define LNK_EXPR_PUSH_START_POS 0x45
#define LNK_EXPR_PUSH_POP_ADD_PUSH 0x46
#define LNK_EXPR_PUSH_POP_SUB_PUSH 0x47
#define LNK_EXPR_PUSH_POP_MUL_PUSH 0x48
#define LNK_EXPR_PUSH_POP_DIV_PUSH 0x49
#define LNK_EXPR_PUSH_POP_MOD_PUSH 0x4a
#define LNK_EXPR_PUSH_POP_SHL_PUSH 0x4b
#define LNK_EXPR_PUSH_POP_SHR_PUSH 0x4c
#define LNK_EXPR_EMIT_POP_BYTE 0x4d
#define LNK_EXPR_EMIT_POP_WORD 0x4e
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

#endif
