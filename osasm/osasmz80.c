#include "osasm.h"

#define REG_A 0x01
#define REG_B 0x02
#define REG_C 0x03
#define REG_D 0x04
#define REG_E 0x05
#define REG_F 0x06
#define REG_I 0x07
#define REG_R 0x08
#define REG_H 0x09
#define REG_L 0x0a
#define REG_AF 0x11
#define REG_HL 0x12
#define REG_BC 0x13
#define REG_DE 0x14
#define REG_IX 0x15
#define REG_IY 0x16
#define REG_SP 0x17
#define REG_JP_NZ 0x20
#define REG_JP_NC 0x21
#define REG_JP_Z 0x22
/*#define REG_JP_C 0x23 USE REG_C*/
#define REG_JP_PO 0x24
#define REG_JP_P 0x25
#define REG_JP_PE 0x26
#define REG_JP_M 0x27
#define REG_AF_ALT 0x41
#define REG_HL_ALT 0x42
#define REG_BC_ALT 0x43
#define REG_DE_ALT 0x44

void asm_error(char * file, uint16_t line, char * msg)
{
    printf("[ FAIL ]\n");
    printf("%s:%d: Error: %s\n", file, (int)line, msg);
    exit_error();
}

void asm_w(char * file, uint16_t line, uint8_t type, arg_t * arg)
{
    switch (type)
    {
    case ARG_VALUE:
    case ARG_PTR_VALUE:
    case ARG_REG_VALUE:
    case ARG_PTR_REG_VALUE:
    case ARG_2REG_VALUE:
    case ARG_PTR_2REG_VALUE:
        if(arg->lbl1_op == TK_NONE && arg->lbl2_op == TK_NONE)
        {
            asm_word(arg->value);
        }
        else
        {
            asm_custom(LNK_EXPR_START, 0,0);
            asm_custom(LNK_EXPR_PUSH_VALUE, &arg->value, 2);
            if(arg->lbl1_name != 0) 
            {
                asm_custom(LNK_EXPR_PUSH_CONST, arg->lbl1_name, strlen(arg->lbl1_name));
                switch (arg->lbl1_op)
                {
                case TK_MATH_ADD:
                    asm_custom(LNK_EXPR_PUSH_POP_ADD_PUSH, 0, 0);
                    break;
                case TK_MATH_SUB:
                    asm_custom(LNK_EXPR_PUSH_POP_SUB_PUSH, 0, 0);
                    break;
                default:
                    asm_error(file, line, "Math operation not supported");
                    break;
                }
            }
            if(arg->lbl2_name != 0) 
            {
                asm_custom(LNK_EXPR_PUSH_CONST, arg->lbl2_name, strlen(arg->lbl2_name));
                switch (arg->lbl2_op)
                {
                case TK_MATH_ADD:
                    asm_custom(LNK_EXPR_PUSH_POP_ADD_PUSH, 0, 0);
                    break;
                case TK_MATH_SUB:
                    asm_custom(LNK_EXPR_PUSH_POP_SUB_PUSH, 0, 0);
                    break;
                default:
                    asm_error(file, line, "Math operation not supported");
                    break;
                }
            }
            asm_custom(LNK_EXPR_EMIT_POP_WORD, 0,0);
        }
        break;
    
    default:
        asm_error(file, line, "Number type argument expected");
        break;
    }
}


void asm_b(char * file, uint16_t line, uint8_t type, arg_t * arg)
{
    switch (type)
    {
    case ARG_VALUE:
    case ARG_PTR_VALUE:
    case ARG_REG_VALUE:
    case ARG_PTR_REG_VALUE:
    case ARG_2REG_VALUE:
    case ARG_PTR_2REG_VALUE:
        if(arg->lbl1_op == TK_NONE && arg->lbl2_op == TK_NONE)
        {
            asm_byte(arg->value);
        }
        else
        {
            asm_custom(LNK_EXPR_START, 0,0);
            asm_custom(LNK_EXPR_PUSH_VALUE, &arg->value, 2);
            if(arg->lbl1_name != 0) 
            {
                asm_custom(LNK_EXPR_PUSH_CONST, arg->lbl1_name, strlen(arg->lbl1_name));
                switch (arg->lbl1_op)
                {
                case TK_MATH_ADD:
                    asm_custom(LNK_EXPR_PUSH_POP_ADD_PUSH, 0, 0);
                    break;
                case TK_MATH_SUB:
                    asm_custom(LNK_EXPR_PUSH_POP_SUB_PUSH, 0, 0);
                    break;
                default:
                    asm_error(file, line, "Math operation not supported");
                    break;
                }
            }
            if(arg->lbl2_name != 0) 
            {
                asm_custom(LNK_EXPR_PUSH_CONST, arg->lbl2_name, strlen(arg->lbl2_name));
                switch (arg->lbl2_op)
                {
                case TK_MATH_ADD:
                    asm_custom(LNK_EXPR_PUSH_POP_ADD_PUSH, 0, 0);
                    break;
                case TK_MATH_SUB:
                    asm_custom(LNK_EXPR_PUSH_POP_SUB_PUSH, 0, 0);
                    break;
                default:
                    asm_error(file, line, "Math operation not supported");
                    break;
                }
            }
            asm_custom(LNK_EXPR_EMIT_POP_BYTE, 0,0);
        }
        break;
    
    default:
        asm_error(file, line, "Number type argument expected");
        break;
    }
}


void asm_posb(char * file, uint16_t line, uint8_t type, arg_t * arg)
{
    switch (type)
    {
    case ARG_VALUE:
    case ARG_PTR_VALUE:
    case ARG_REG_VALUE:
    case ARG_PTR_REG_VALUE:
    case ARG_2REG_VALUE:
    case ARG_PTR_2REG_VALUE:
        asm_custom(LNK_EXPR_START, 0,0);
        asm_custom(LNK_EXPR_PUSH_VALUE, &arg->value, 2);
        if(arg->lbl1_name != 0) 
        {
            asm_custom(LNK_EXPR_PUSH_CONST, arg->lbl1_name, strlen(arg->lbl1_name));
            switch (arg->lbl1_op)
            {
            case TK_MATH_ADD:
                asm_custom(LNK_EXPR_PUSH_POP_ADD_PUSH, 0, 0);
                break;
            case TK_MATH_SUB:
                asm_custom(LNK_EXPR_PUSH_POP_SUB_PUSH, 0, 0);
                break;
            default:
                asm_error(file, line, "Math operation not supported");
                break;
            }
        }
        if(arg->lbl2_name != 0) 
        {
            asm_custom(LNK_EXPR_PUSH_CONST, arg->lbl2_name, strlen(arg->lbl2_name));
            switch (arg->lbl2_op)
            {
            case TK_MATH_ADD:
                asm_custom(LNK_EXPR_PUSH_POP_ADD_PUSH, 0, 0);
                break;
            case TK_MATH_SUB:
                asm_custom(LNK_EXPR_PUSH_POP_SUB_PUSH, 0, 0);
                break;
            default:
                asm_error(file, line, "Math operation not supported");
                break;
            }
        }
        asm_custom(LNK_EXPR_PUSH_CURR_POS, 0,0);
        asm_custom(LNK_EXPR_PUSH_POP_ADD_PUSH, 0,0);
        asm_custom(LNK_EXPR_EMIT_POP_BYTE, 0,0);
        break;
    
    default:
        asm_error(file, line, "Number type argument expected");
        break;
    }
}

void asm_simple(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
}

void asm_simple_w(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
    asm_w(file, line, cmd->a1, arg1);
}

void asm_simple_b(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
    asm_b(file, line, cmd->a1, arg1);
}

void asm_simple_posb(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
    asm_posb(file, line, cmd->a1, arg1);
}

void asm_simple_second_posb(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
    asm_posb(file, line, cmd->a2, arg2);
}

void asm_simple_second_b(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
    asm_b(file, line, cmd->a2, arg2);
}

void asm_simple_second_w(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
    asm_w(file, line, cmd->a2, arg2);
}

void asm_simple_b_w(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
    asm_b(file, line, cmd->a1, arg1);
    asm_w(file, line, cmd->a2, arg2);
}

void asm_simple_w_w(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    asm_hex(cmd->hex);
    asm_w(file, line, cmd->a1, arg1);
    asm_w(file, line, cmd->a2, arg2);
}

void asm_2byte_b_1byte(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    char prefix[5];
    char suffix[3];
    memset(prefix, 0, 5);
    memcpy(prefix, cmd->hex, 4);
    memset(suffix, 0, 3);
    memcpy(suffix, &cmd->hex[4], 2);
    asm_hex(prefix);
    asm_b(file, line, cmd->a1, arg1);
    asm_hex(suffix);
}

void asm_2byte_second_b_1byte(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    char prefix[5];
    char suffix[3];
    memset(prefix, 0, 5);
    memcpy(prefix, cmd->hex, 4);
    memset(suffix, 0, 3);
    memcpy(suffix, &cmd->hex[4], 2);
    asm_hex(prefix);
    asm_b(file, line, cmd->a2, arg2);
    asm_hex(suffix);
}

void asm_2byte_second_b_1byte_bit_instr(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    char prefix[5];
    memset(prefix, 0, 5);
    memcpy(prefix, cmd->hex, 4);
    asm_hex(prefix);
    asm_b(file, line, cmd->a2, arg2);
    uint8_t byte = parsehex(&cmd->hex[4], 2);
    if(arg1->type == ARG_VALUE && arg1->value >= 0 && arg1->value <= 7)
    {
        byte += arg1->value * 8;
    }
    else asm_error(file, line, "Argument not supported (asm_2byte_second_b_1byte_bit_instr)");
    asm_byte(byte);
}

void asm_byte_reg(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    uint8_t byte = parsehex(cmd->hex, strlen(cmd->hex));
    arg_t * arg = arg2->reg1 != REG_NONE ? arg2 : arg1;
    switch (arg->reg1)
    {
        case REG_A:
            byte += 7;
            break;
        case REG_B:
            byte += 0;
            break;
        case REG_C:
            byte += 1;
            break;
        case REG_D:
            byte += 2;
            break;
        case REG_E:
            byte += 3;
            break;
        case REG_H:
            byte += 4;
            break;
        case REG_L:
            byte += 5;
            break;
        case REG_HL:
            byte += 6;
            if(arg->type != ARG_PTR_REG) asm_error(file, line, "Argument not supported (asm_byte_reg 1)");
            break;
        default:
        printf("Token found: %d\n", arg->reg1);;
            asm_error(file, line, "Argument not supported (asm_byte_reg 2)");
            break;
    }
    asm_byte(byte);
}


void asm_2byte_reg(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    uint8_t byte = parsehex(&cmd->hex[2], 2);
    asm_byte(parsehex(cmd->hex, 2));
    arg_t * arg = arg2->reg1 != REG_NONE ? arg : arg1;
    switch (arg->reg1)
    {
        case REG_A:
            byte += 7;
            break;
        case REG_B:
            byte += 0;
            break;
        case REG_C:
            byte += 1;
            break;
        case REG_D:
            byte += 2;
            break;
        case REG_E:
            byte += 3;
            break;
        case REG_H:
            byte += 4;
            break;
        case REG_L:
            byte += 5;
            break;
        case REG_HL:
            byte += 6;
            if(arg->type != ARG_PTR_REG) asm_error(file, line, "Argument not supported (asm_2byte_reg 1)");
            break;
        default:
            asm_error(file, line, "Argument not supported  (asm_2byte_reg 2)");
            break;
    }
    asm_byte(byte);
}

void asm_rst(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    uint8_t byte = parsehex(cmd->hex, 2);
    switch (arg1->value)
    {
        case 0x0:
        case 0x8:
        case 0x10:
        case 0x18:
        case 0x20:
        case 0x28:
        case 0x30:
        case 0x38:
            byte += arg1->value;
            break;
        default:
            printf("[%d?]", arg1->value);
            asm_error(file, line, "Argument not supported (asm_rst)");
            break;
    }
    asm_byte(byte);
}

void asm_bit_instr(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    uint8_t byte = parsehex(&cmd->hex[2], 2);
    asm_byte(parsehex(cmd->hex, 2));
    switch (arg2->reg1)
    {
        case REG_A:
            byte += 7;
            break;
        case REG_B:
            byte += 0;
            break;
        case REG_C:
            byte += 1;
            break;
        case REG_D:
            byte += 2;
            break;
        case REG_E:
            byte += 3;
            break;
        case REG_H:
            byte += 4;
            break;
        case REG_L:
            byte += 5;
            break;
        case REG_HL:
            byte += 6;
            if(arg2->type != ARG_PTR_REG) asm_error(file, line, "Argument not supported (asm_bit_instr 1)");
            break;
        default:
            asm_error(file, line, "Argument not supported (asm_bit_instr 2)");
            break;
    }
    if(arg1->type == ARG_VALUE && arg1->value >= 0 && arg1->value <= 7)
    {
        byte += arg1->value * 8;
    }
    else asm_error(file, line, "Argument not supported (asm_bit_instr 3)");
    asm_byte(byte);
}

void asm_im(char * file, uint16_t line, cmd_t * cmd, arg_t * arg1, arg_t * arg2)
{
    uint8_t byte = parsehex(&cmd->hex[2], 2);
    asm_byte(parsehex(cmd->hex, 2));
    if(arg1->type == ARG_VALUE && arg1->value >= 0 && arg1->value <= 1)
    {
        byte += arg1->value * 16;
    }
    else if(arg1->type == ARG_VALUE && arg1->value == 2)
    {
        byte += 24;
    }
    else asm_error(file, line, "Argument not supported (asm_im)");
    asm_byte(byte);
}


cmd_t _cmds[] = {
    {"NOP", "00", &asm_simple, 0},
    {"HLT", "76", &asm_simple, 0},
    {"HALT", "76", &asm_simple, 0},
    {"ADD", "DD09", &asm_simple, 2, ARG_REG, REG_IX, REG_NONE, ARG_REG, REG_BC, REG_NONE},
    {"ADD", "DD19", &asm_simple, 2, ARG_REG, REG_IX, REG_NONE, ARG_REG, REG_DE, REG_NONE},
    {"ADD", "DD29", &asm_simple, 2, ARG_REG, REG_IX, REG_NONE, ARG_REG, REG_IX, REG_NONE},
    {"ADD", "DD39", &asm_simple, 2, ARG_REG, REG_IX, REG_NONE, ARG_REG, REG_SP, REG_NONE},
    {"LD", "DD21", &asm_simple_second_w, 2, ARG_REG, REG_IX, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "DD22", &asm_simple_second_w, 2, ARG_PTR_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_IX, REG_NONE},
    {"INC", "DD23", &asm_simple, 1, ARG_REG, REG_IX, REG_NONE},
    {"INC", "DD34", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"DEC", "DD35", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"LD", "DD36", &asm_simple_b_w, 2, ARG_PTR_REG_VALUE, REG_IX, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "DD2A", &asm_simple_second_w, 2, ARG_REG, REG_IX, REG_NONE, ARG_PTR_VALUE, REG_NONE, REG_NONE},
    {"LD", "DD46", &asm_simple_second_b, 2, ARG_REG, REG_B, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"LD", "DD56", &asm_simple_second_b, 2, ARG_REG, REG_D, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"LD", "DD66", &asm_simple_second_b, 2, ARG_REG, REG_H, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"LD", "DD70", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IX, REG_NONE, ARG_REG, REG_B, REG_NONE},
    {"LD", "DD71", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IX, REG_NONE, ARG_REG, REG_C, REG_NONE},
    {"LD", "DD72", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IX, REG_NONE, ARG_REG, REG_D, REG_NONE},
    {"LD", "DD73", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IX, REG_NONE, ARG_REG, REG_E, REG_NONE},
    {"LD", "DD74", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IX, REG_NONE, ARG_REG, REG_H, REG_NONE},
    {"LD", "DD75", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IX, REG_NONE, ARG_REG, REG_L, REG_NONE},
    {"LD", "DD77", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IX, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"ADD", "DD86", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"SUB", "DD96", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"SUB", "DD96", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"AND", "DDA6", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"AND", "DDA6", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"OR", "DDB6", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"OR", "DDB6", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"DEC", "DD2B", &asm_simple, 1, ARG_REG, REG_IX, REG_NONE},
    {"LD", "DD4E", &asm_simple_second_b, 2, ARG_REG, REG_C, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"LD", "DD5E", &asm_simple_second_b, 2, ARG_REG, REG_E, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"LD", "DD6E", &asm_simple_second_b, 2, ARG_REG, REG_D, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"LD", "DD7E", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"ADC", "DD8E", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"SBC", "DD9E", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"XOR", "DDAE", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"XOR", "DDAE", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"CP", "DDBE", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"CP", "DDBE", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"POP", "DDE1", &asm_simple, 1, ARG_REG, REG_IX, REG_NONE},
    {"PUSH", "DDE5", &asm_simple, 1, ARG_REG, REG_IX, REG_NONE},
    {"EX", "DDE3", &asm_simple, 2, ARG_PTR_REG, REG_SP, REG_NONE, ARG_REG, REG_IX, REG_NONE},
    {"JP", "DDE9", &asm_simple, 1, ARG_PTR_REG, REG_IX, REG_NONE},
    {"LD", "DDF9", &asm_simple, 2, ARG_REG, REG_SP, REG_NONE, ARG_REG, REG_IX, REG_NONE},
    {"RLC", "DDCB06", &asm_2byte_b_1byte, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"RL", "DDCB16", &asm_2byte_b_1byte, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"SLA", "DDCB26", &asm_2byte_b_1byte, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"SLL", "DDCB36", &asm_2byte_b_1byte, 1, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"BIT", "DDCB46", &asm_2byte_second_b_1byte_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"RES", "DDCB86", &asm_2byte_second_b_1byte_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},
    {"SET", "DDCBC6", &asm_2byte_second_b_1byte_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG_VALUE, REG_IX, REG_NONE},

    {"ADD", "FD09", &asm_simple, 2, ARG_REG, REG_IY, REG_NONE, ARG_REG, REG_BC, REG_NONE},
    {"ADD", "FD19", &asm_simple, 2, ARG_REG, REG_IY, REG_NONE, ARG_REG, REG_DE, REG_NONE},
    {"ADD", "FD29", &asm_simple, 2, ARG_REG, REG_IY, REG_NONE, ARG_REG, REG_IY, REG_NONE},
    {"ADD", "FD39", &asm_simple, 2, ARG_REG, REG_IY, REG_NONE, ARG_REG, REG_SP, REG_NONE},
    {"LD", "FD21", &asm_simple_second_w, 2, ARG_REG, REG_IY, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "FD22", &asm_simple_second_w, 2, ARG_PTR_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_IY, REG_NONE},
    {"INC", "FD23", &asm_simple, 1, ARG_REG, REG_IY, REG_NONE},
    {"INC", "FD34", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"DEC", "FD35", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"LD", "FD36", &asm_simple_b_w, 2, ARG_PTR_REG_VALUE, REG_IY, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "FD2A", &asm_simple_second_w, 2, ARG_REG, REG_IY, REG_NONE, ARG_PTR_VALUE, REG_NONE, REG_NONE},
    {"LD", "FD46", &asm_simple_second_b, 2, ARG_REG, REG_B, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"LD", "FD56", &asm_simple_second_b, 2, ARG_REG, REG_D, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"LD", "FD66", &asm_simple_second_b, 2, ARG_REG, REG_H, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"LD", "FD70", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IY, REG_NONE, ARG_REG, REG_B, REG_NONE},
    {"LD", "FD71", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IY, REG_NONE, ARG_REG, REG_C, REG_NONE},
    {"LD", "FD72", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IY, REG_NONE, ARG_REG, REG_D, REG_NONE},
    {"LD", "FD73", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IY, REG_NONE, ARG_REG, REG_E, REG_NONE},
    {"LD", "FD74", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IY, REG_NONE, ARG_REG, REG_H, REG_NONE},
    {"LD", "FD75", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IY, REG_NONE, ARG_REG, REG_L, REG_NONE},
    {"LD", "FD77", &asm_simple_b, 2, ARG_PTR_REG_VALUE, REG_IY, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"ADD", "FD86", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"SUB", "FD96", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"SUB", "FD96", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"AND", "FDA6", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"AND", "FDA6", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"OR", "FDB6", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"OR", "FDB6", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"DEC", "FD2B", &asm_simple, 1, ARG_REG, REG_IY, REG_NONE},
    {"LD", "FD4E", &asm_simple_second_b, 2, ARG_REG, REG_C, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"LD", "FD5E", &asm_simple_second_b, 2, ARG_REG, REG_E, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"LD", "FD6E", &asm_simple_second_b, 2, ARG_REG, REG_D, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"LD", "FD7E", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"ADC", "FD8E", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"SBC", "FD9E", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"XOR", "FDAE", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"XOR", "FDAE", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"CP", "FDBE", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"CP", "FDBE", &asm_simple_b, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"POP", "FDE1", &asm_simple, 1, ARG_REG, REG_IY, REG_NONE},
    {"PUSH", "FDE5", &asm_simple, 1, ARG_REG, REG_IY, REG_NONE},
    {"EX", "FDE3", &asm_simple, 2, ARG_PTR_REG, REG_SP, REG_NONE, ARG_REG, REG_IY, REG_NONE},
    {"JP", "FDE9", &asm_simple, 1, ARG_PTR_REG, REG_IY, REG_NONE},
    {"LD", "FDF9", &asm_simple, 2, ARG_REG, REG_SP, REG_NONE, ARG_REG, REG_IY, REG_NONE},
    {"RLC", "FDCB06", &asm_2byte_b_1byte, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"RL", "FDCB16", &asm_2byte_b_1byte, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"SLA", "FDCB26", &asm_2byte_b_1byte, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"SLL", "FDCB36", &asm_2byte_b_1byte, 1, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"BIT", "FDCB46", &asm_2byte_second_b_1byte_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"RES", "FDCB86", &asm_2byte_second_b_1byte_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},
    {"SET", "FDCBC6", &asm_2byte_second_b_1byte_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG_VALUE, REG_IY, REG_NONE},

    {"IN", "ED40", &asm_simple, 2, ARG_REG, REG_B, REG_NONE, ARG_PTR_REG, REG_C, REG_NONE},
    {"IN", "ED50", &asm_simple, 2, ARG_REG, REG_D, REG_NONE, ARG_PTR_REG, REG_C, REG_NONE},
    {"IN", "ED60", &asm_simple, 2, ARG_REG, REG_H, REG_NONE, ARG_PTR_REG, REG_C, REG_NONE},
    {"IN", "ED70", &asm_simple, 1, ARG_PTR_REG, REG_C, REG_NONE},
    {"OUT", "ED41", &asm_simple, 2, ARG_PTR_REG, REG_C, REG_NONE, ARG_REG, REG_B, REG_NONE},
    {"OUT", "ED51", &asm_simple, 2, ARG_PTR_REG, REG_C, REG_NONE, ARG_REG, REG_D, REG_NONE},
    {"OUT", "ED61", &asm_simple, 2, ARG_PTR_REG, REG_C, REG_NONE, ARG_REG, REG_H, REG_NONE},
    {"OUT", "ED71", &asm_simple, 1, ARG_PTR_REG, REG_C, REG_NONE},
    {"SBC", "ED42", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_BC, REG_NONE},
    {"SBC", "ED52", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_DE, REG_NONE},
    {"SBC", "ED62", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_HL, REG_NONE},
    {"SBC", "ED72", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_SP, REG_NONE},
    {"LD", "ED43", &asm_simple_w, 2, ARG_PTR_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_BC, REG_NONE},
    {"LD", "ED53", &asm_simple_w, 2, ARG_PTR_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_DE, REG_NONE},
    {"LD", "ED73", &asm_simple_w, 2, ARG_PTR_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_SP, REG_NONE},
    {"LDI", "EDA0", &asm_simple, 0},
    {"LDIR", "EDB0", &asm_simple, 0},
    {"CPI", "EDA1", &asm_simple, 0},
    {"CPIR", "EDB1", &asm_simple, 0},
    {"INI", "EDA2", &asm_simple, 0},
    {"INIR", "EDB2", &asm_simple, 0},
    {"OUTI", "EDA3", &asm_simple, 0},
    {"OTIR", "EDB3", &asm_simple, 0},
    {"LDD", "EDA8", &asm_simple, 0},
    {"LDDR", "EDB8", &asm_simple, 0},
    {"CPD", "EDA9", &asm_simple, 0},
    {"CPDR", "EDB9", &asm_simple, 0},
    {"IND", "EDAA", &asm_simple, 0},
    {"INDR", "EDBA", &asm_simple, 0},
    {"OUTD", "EDAB", &asm_simple, 0},
    {"OTDR", "EDBB", &asm_simple, 0},
    {"NEG", "ED44", &asm_simple, 0},
    {"RETN", "ED45", &asm_simple, 0},
    {"IM", "ED46", &asm_im, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "ED47", &asm_simple, 2, ARG_REG, REG_I, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"LD", "ED57", &asm_simple, 2, ARG_REG, REG_A, REG_NONE, ARG_REG, REG_I, REG_NONE},
    {"RRD", "ED67", &asm_simple, 0},
    {"IN", "ED48", &asm_simple, 2, ARG_REG, REG_C, REG_NONE, ARG_PTR_REG, REG_C, REG_NONE},
    {"IN", "ED58", &asm_simple, 2, ARG_REG, REG_E, REG_NONE, ARG_PTR_REG, REG_C, REG_NONE},
    {"IN", "ED68", &asm_simple, 2, ARG_REG, REG_L, REG_NONE, ARG_PTR_REG, REG_C, REG_NONE},
    {"IN", "ED78", &asm_simple, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG, REG_C, REG_NONE},
    {"OUT", "ED49", &asm_simple, 2, ARG_PTR_REG, REG_C, REG_NONE, ARG_REG, REG_C, REG_NONE},
    {"OUT", "ED59", &asm_simple, 2, ARG_PTR_REG, REG_C, REG_NONE, ARG_REG, REG_E, REG_NONE},
    {"OUT", "ED69", &asm_simple, 2, ARG_PTR_REG, REG_C, REG_NONE, ARG_REG, REG_L, REG_NONE},
    {"OUT", "ED79", &asm_simple, 2, ARG_PTR_REG, REG_C, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"ADC", "ED4A", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_BC, REG_NONE},
    {"ADC", "ED5A", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_DE, REG_NONE},
    {"ADC", "ED6A", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_HL, REG_NONE},
    {"ADC", "ED7A", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_SP, REG_NONE},
    {"LD", "ED4B", &asm_simple_second_w, 2, ARG_REG, REG_BC, REG_NONE, ARG_PTR_VALUE, REG_NONE, REG_NONE},
    {"LD", "ED4B", &asm_simple_second_w, 2, ARG_REG, REG_DE, REG_NONE, ARG_PTR_VALUE, REG_NONE, REG_NONE},
    {"LD", "ED4B", &asm_simple_second_w, 2, ARG_REG, REG_SP, REG_NONE, ARG_PTR_VALUE, REG_NONE, REG_NONE},
    {"RETI", "ED4D", &asm_simple, 0},
    {"LD", "ED4F", &asm_simple_second_w, 2, ARG_REG, REG_R, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"LD", "ED5F", &asm_simple_second_w, 2, ARG_REG, REG_A, REG_NONE, ARG_REG, REG_R, REG_NONE},
    {"RLD", "ED6F", &asm_simple, 0},

    {"INC", "03", &asm_simple, 1, ARG_REG, REG_BC, REG_NONE},
    {"INC", "04", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"INC", "13", &asm_simple, 1, ARG_REG, REG_DE, REG_NONE},
    {"INC", "14", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"INC", "23", &asm_simple, 1, ARG_REG, REG_HL, REG_NONE},
    {"INC", "24", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"INC", "33", &asm_simple, 1, ARG_REG, REG_SP, REG_NONE},
    {"INC", "34", &asm_simple, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"INC", "0C", &asm_simple, 1, ARG_REG, REG_C, REG_NONE},
    {"INC", "1C", &asm_simple, 1, ARG_REG, REG_E, REG_NONE},
    {"INC", "2C", &asm_simple, 1, ARG_REG, REG_L, REG_NONE},
    {"INC", "3C", &asm_simple, 1, ARG_REG, REG_C, REG_NONE},
    {"DEC", "05", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"DEC", "15", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"DEC", "25", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"DEC", "35", &asm_simple, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"DEC", "0B", &asm_simple, 1, ARG_REG, REG_BC, REG_NONE},
    {"DEC", "1B", &asm_simple, 1, ARG_REG, REG_DE, REG_NONE},
    {"DEC", "2B", &asm_simple, 1, ARG_REG, REG_HL, REG_NONE},
    {"DEC", "3B", &asm_simple, 1, ARG_REG, REG_SP, REG_NONE},
    {"DEC", "0D", &asm_simple, 1, ARG_REG, REG_C, REG_NONE},
    {"DEC", "1D", &asm_simple, 1, ARG_REG, REG_E, REG_NONE},
    {"DEC", "2D", &asm_simple, 1, ARG_REG, REG_L, REG_NONE},
    {"DEC", "3D", &asm_simple, 1, ARG_REG, REG_A, REG_NONE},
    {"DJNZ", "10", &asm_simple_second_posb, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JR", "20", &asm_simple_second_posb, 2, ARG_REG, REG_JP_NZ, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JR", "30", &asm_simple_second_posb, 2, ARG_REG, REG_JP_NC, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "01", &asm_simple_second_w, 2, ARG_REG, REG_BC, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "11", &asm_simple_second_w, 2, ARG_REG, REG_DE, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "21", &asm_simple_second_w, 2, ARG_REG, REG_HL, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "31", &asm_simple_second_w, 2, ARG_REG, REG_SP, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "02", &asm_simple_second_w, 2, ARG_PTR_REG, REG_BC, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"LD", "12", &asm_simple_second_w, 2, ARG_PTR_REG, REG_DE, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"LD", "22", &asm_simple_w, 2, ARG_PTR_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_HL, REG_NONE},
    {"LD", "32", &asm_simple_w, 2, ARG_PTR_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"LD", "06", &asm_simple_second_b, 2, ARG_REG, REG_B, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "16", &asm_simple_second_b, 2, ARG_REG, REG_D, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "26", &asm_simple_second_b, 2, ARG_REG, REG_H, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "36", &asm_simple_second_b, 2, ARG_PTR_REG, REG_HL, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"RLCA", "07", &asm_simple, 0},
    {"RLA", "17", &asm_simple, 0},
    {"DAA", "27", &asm_simple, 0},
    {"SCF", "37", &asm_simple, 0},
    {"EX", "08", &asm_simple, 2, ARG_REG, REG_AF, REG_NONE, ARG_REG, REG_AF_ALT, REG_NONE},
    {"JR", "18", &asm_simple_posb, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JR", "28", &asm_simple_second_posb, 2, ARG_REG, REG_JP_Z, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JR", "38", &asm_simple_second_posb, 2, ARG_REG, REG_C, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"ADD", "09", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_BC, REG_NONE},
    {"ADD", "19", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_DE, REG_NONE},
    {"ADD", "29", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_HL, REG_NONE},
    {"ADD", "39", &asm_simple, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_SP, REG_NONE},
    {"LD", "0A", &asm_simple, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG, REG_BC, REG_NONE},
    {"LD", "1A", &asm_simple, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG, REG_DE, REG_NONE},
    {"LD", "2A", &asm_simple_second_w, 2, ARG_REG, REG_HL, REG_NONE, ARG_PTR_VALUE, REG_NONE, REG_NONE},
    {"LD", "3A", &asm_simple_second_w, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_VALUE, REG_NONE, REG_NONE},
    {"LD", "0E", &asm_simple_second_b, 2, ARG_REG, REG_C, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "1E", &asm_simple_second_b, 2, ARG_REG, REG_E, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "2E", &asm_simple_second_b, 2, ARG_REG, REG_L, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LD", "3E", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"RRCA", "37", &asm_simple, 0},
    {"RRA", "37", &asm_simple, 0},
    {"CPL", "37", &asm_simple, 0},
    {"CCF", "37", &asm_simple, 0},
    {"RET", "C0", &asm_simple, 1, ARG_REG, REG_JP_NZ, REG_NONE},
    {"RET", "D0", &asm_simple, 1, ARG_REG, REG_JP_NC, REG_NONE},
    {"RET", "E0", &asm_simple, 1, ARG_REG, REG_JP_PO, REG_NONE},
    {"RET", "F0", &asm_simple, 1, ARG_REG, REG_JP_P, REG_NONE},
    {"POP", "C1", &asm_simple, 1, ARG_REG, REG_BC, REG_NONE},
    {"POP", "D1", &asm_simple, 1, ARG_REG, REG_DE, REG_NONE},
    {"POP", "E1", &asm_simple, 1, ARG_REG, REG_HL, REG_NONE},
    {"POP", "F1", &asm_simple, 1, ARG_REG, REG_AF, REG_NONE},
    {"JP", "C2", &asm_simple_second_w, 2, ARG_REG, REG_JP_NZ, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JP", "D2", &asm_simple_second_w, 2, ARG_REG, REG_JP_NC, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JP", "E2", &asm_simple_second_w, 2, ARG_REG, REG_JP_PO, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JP", "F2", &asm_simple_second_w, 2, ARG_REG, REG_JP_P, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JP", "C3", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"OUT", "D3", &asm_simple_b, 2, ARG_PTR_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"OUT", "D3", &asm_simple_b, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_A, REG_NONE},
    {"EX", "E3", &asm_simple, 2, ARG_PTR_REG, REG_SP, REG_NONE, ARG_REG, REG_HL, REG_NONE},
    {"DI", "F3", &asm_simple, 0},
    {"CALL", "C4", &asm_simple_second_w, 2, ARG_REG, REG_JP_NZ, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"CALL", "D4", &asm_simple_second_w, 2, ARG_REG, REG_JP_NC, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"CALL", "E4", &asm_simple_second_w, 2, ARG_REG, REG_JP_PO, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"CALL", "F4", &asm_simple_second_w, 2, ARG_REG, REG_JP_P, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"PUSH", "C5", &asm_simple, 1, ARG_REG, REG_BC, REG_NONE},
    {"PUSH", "D5", &asm_simple, 1, ARG_REG, REG_DE, REG_NONE},
    {"PUSH", "E5", &asm_simple, 1, ARG_REG, REG_HL, REG_NONE},
    {"PUSH", "F5", &asm_simple, 1, ARG_REG, REG_AF, REG_NONE},
    {"ADD", "C6", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"SUB", "D6", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"SUB", "D6", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"AND", "E6", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"AND", "E6", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"OR", "D6", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"OR", "D6", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"RST", "C7", &asm_rst, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"RST", "C8", &asm_simple, 1, ARG_REG, REG_JP_Z, REG_NONE},
    {"RST", "D8", &asm_simple, 1, ARG_REG, REG_C, REG_NONE},
    {"RST", "E8", &asm_simple, 1, ARG_REG, REG_JP_PE, REG_NONE},
    {"RST", "F8", &asm_simple, 1, ARG_REG, REG_JP_M, REG_NONE},
    {"RET", "C9", &asm_simple, 0},
    {"EXX", "D9", &asm_simple, 0},
    {"JP", "E9", &asm_simple, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "F9", &asm_simple, 1, ARG_REG, REG_SP, REG_NONE, ARG_REG, REG_HL, REG_NONE},
    {"JP", "CA", &asm_simple_second_posb, 2, ARG_REG, REG_JP_Z, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JP", "DA", &asm_simple_second_posb, 2, ARG_REG, REG_C, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JP", "EA", &asm_simple_second_posb, 2, ARG_REG, REG_JP_PE, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"JP", "FA", &asm_simple_second_posb, 2, ARG_REG, REG_JP_M, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"IN", "DB", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_VALUE, REG_NONE, REG_NONE},
    {"IN", "DB", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"EX", "EB", &asm_simple_second_b, 2, ARG_REG, REG_DE, REG_NONE, ARG_REG, REG_HL, REG_NONE},
    {"EX", "EB", &asm_simple_second_b, 2, ARG_REG, REG_HL, REG_NONE, ARG_REG, REG_DE, REG_NONE},
    {"EI", "FB", &asm_simple, 0},
    {"CALL", "CC", &asm_simple_second_w, 2, ARG_REG, REG_JP_Z, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"CALL", "DC", &asm_simple_second_w, 2, ARG_REG, REG_C, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"CALL", "EC", &asm_simple_second_w, 2, ARG_REG, REG_JP_PE, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"CALL", "FC", &asm_simple_second_w, 2, ARG_REG, REG_JP_M, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"CALL", "CD", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"ADC", "CE", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"SBC", "DE", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"XOR", "EE", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"XOR", "EE", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"CP", "FE", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CP", "FE", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"RLC", "CB00", &asm_2byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"RLC", "CB00", &asm_2byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"RRC", "CB08", &asm_2byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"RRC", "CB08", &asm_2byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"RL", "CB10", &asm_2byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"RL", "CB10", &asm_2byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"RR", "CB18", &asm_2byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"RR", "CB18", &asm_2byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"SLA", "CB20", &asm_2byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"SLA", "CB20", &asm_2byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"SRA", "CB28", &asm_2byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"SRA", "CB28", &asm_2byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"SLL", "CB30", &asm_2byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"SLL", "CB30", &asm_2byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"SRL", "CB38", &asm_2byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"SRL", "CB38", &asm_2byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"BIT", "CB40", &asm_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"BIT", "CB40", &asm_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"RES", "CB80", &asm_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"RES", "CB80", &asm_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"SET", "CBC0", &asm_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"SET", "CBC0", &asm_bit_instr, 2, ARG_VALUE, REG_NONE, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"ADD", "80", &asm_byte_reg, 2, ARG_REG, REG_A, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"ADD", "80", &asm_byte_reg, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"ADD", "80", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"ADD", "80", &asm_byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"ADC", "88", &asm_byte_reg, 2, ARG_REG, REG_A, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"ADC", "88", &asm_byte_reg, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"ADC", "88", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"ADC", "88", &asm_byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"SUB", "90", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"SUB", "90", &asm_byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"SBC", "98", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"SBC", "98", &asm_byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"AND", "A0", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"AND", "A0", &asm_byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"XOR", "A8", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"XOR", "A8", &asm_byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"OR", "B0", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"OR", "B0", &asm_byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"CP", "B8", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"CP", "B8", &asm_byte_reg, 1, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "40", &asm_byte_reg, 2, ARG_REG, REG_B, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"LD", "40", &asm_byte_reg, 2, ARG_REG, REG_B, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "48", &asm_byte_reg, 2, ARG_REG, REG_C, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"LD", "48", &asm_byte_reg, 2, ARG_REG, REG_C, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "50", &asm_byte_reg, 2, ARG_REG, REG_D, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"LD", "50", &asm_byte_reg, 2, ARG_REG, REG_D, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "58", &asm_byte_reg, 2, ARG_REG, REG_E, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"LD", "58", &asm_byte_reg, 2, ARG_REG, REG_E, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "60", &asm_byte_reg, 2, ARG_REG, REG_H, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"LD", "60", &asm_byte_reg, 2, ARG_REG, REG_H, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "68", &asm_byte_reg, 2, ARG_REG, REG_L, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"LD", "68", &asm_byte_reg, 2, ARG_REG, REG_L, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "70", &asm_byte_reg, 2, ARG_PTR_REG, REG_HL, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"LD", "70", &asm_byte_reg, 2, ARG_PTR_REG, REG_HL, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {"LD", "78", &asm_byte_reg, 2, ARG_REG, REG_A, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"LD", "78", &asm_byte_reg, 2, ARG_REG, REG_A, REG_NONE, ARG_PTR_REG, REG_HL, REG_NONE},
    {0}
};

cmd_t _prefixes[] = {
    {0}
};

reg_t _regs[] = {
    {"A", REG_A},
    {"B", REG_B},
    {"C", REG_C},
    {"D", REG_D},
    {"E", REG_E},
    {"F", REG_F},
    {"I", REG_I},
    {"R", REG_R},
    {"AF", REG_AF},
    {"HL", REG_HL},
    {"BC", REG_BC},
    {"DE", REG_DE},
    {"IX", REG_IX},
    {"IY", REG_IY},
    {"SP", REG_SP},
    {"AF'", REG_AF_ALT},
    {"BC'", REG_BC_ALT},
    {"DE'", REG_DE_ALT},
    {"HL'", REG_HL_ALT},
    {"M", REG_JP_M},
    {"NC", REG_JP_NC},
    {"NZ", REG_JP_NZ},
    {"P", REG_JP_P},
    {"PE", REG_JP_PE},
    {"PO", REG_JP_PO},
    {"Z", REG_JP_Z},
    {0}
};

void show_help()
{
    printf("Usage: osasmz80 [file withtout extension]\n");
    printf("Usage: osasmz80 [input.s] [output.obj]\n");
}

int main(int argc, char ** argv)
{
    printf("Old-School Assembler for Z80\n");
    printf("Copyright (c) 2023, Humberto Costa\n\n");
    build(argc, argv, "z80");
}