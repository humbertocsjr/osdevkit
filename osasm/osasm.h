
#include <osdevkit.h>

#ifndef OSASM_H

typedef struct
{
    uint8_t type;
    uint8_t reg1;
    uint8_t reg2;
    int16_t value;
    char * lbl1_name;
    uint8_t lbl1_op;
    char * lbl2_name;
    uint8_t lbl2_op;
} arg_t;

#define ARG_NONE 0
#define ARG_VALUE 1
#define ARG_REG 2
#define ARG_REG_VALUE 3
#define ARG_2REG 4
#define ARG_2REG_VALUE 5
#define ARG_PTR_VALUE 6
#define ARG_PTR_REG 7
#define ARG_PTR_REG_VALUE 8
#define ARG_PTR_2REG 9
#define ARG_PTR_2REG_VALUE 10

#define REG_NONE 0

typedef struct cmd_s
{
    char * cmd;
    char * hex;
    void (*assemble)(char * file, uint16_t line, struct cmd_s * cmd, arg_t * arg1, arg_t * arg2);
    uint8_t args;
    uint8_t a1;
    uint8_t a1reg1;
    uint8_t a1reg2;
    uint8_t a2;
    uint8_t a2reg1;
    uint8_t a2reg2;
} cmd_t;

typedef struct reg_s
{
    char * name;
    uint8_t id;
} reg_t;

void build(int argc, char ** argv, char * cpu);
void show_help();
void exit_error();
extern cmd_t _cmds[];
extern cmd_t _prefixes[];
extern reg_t _regs[];
void asm_hex(char * hex);
void asm_byte(uint8_t value);
void asm_word(uint16_t value);
void asm_custom(uint8_t type, void * buf, uint8_t len);

#endif
