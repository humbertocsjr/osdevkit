#include "osasm.h"

#define REG_A 0x01
#define REG_B 0x02
#define REG_C 0x03
#define REG_D 0x04
#define REG_E 0x05
#define REG_F 0x06
#define REG_H 0x07
#define REG_L 0x08
#define REG_M 0x11
#define REG_SP 0x12
#define REG_PSW 0x13

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
        case REG_M:
            byte += 6;
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
        case REG_M:
            byte += 6;
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
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            byte += arg1->value * 8;
            break;
        default:
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
        case REG_M:
            byte += 6;
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
    {"LXI", "01", &asm_simple_second_w, 2, ARG_REG, REG_B, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LXI", "11", &asm_simple_second_w, 2, ARG_REG, REG_D, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LXI", "21", &asm_simple_second_w, 2, ARG_REG, REG_H, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"LXI", "31", &asm_simple_second_w, 2, ARG_REG, REG_SP, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"STAX", "02", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"STAX", "12", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"SHLD", "32", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"STA", "32", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"INX", "03", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"INX", "13", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"INX", "23", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"INX", "33", &asm_simple, 1, ARG_REG, REG_SP, REG_NONE},
    {"INR", "04", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"INR", "14", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"INR", "24", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"INR", "34", &asm_simple, 1, ARG_REG, REG_M, REG_NONE},
    {"DCR", "05", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"DCR", "15", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"DCR", "25", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"DCR", "35", &asm_simple, 1, ARG_REG, REG_M, REG_NONE},
    {"MVI", "06", &asm_simple_second_b, 2, ARG_REG, REG_B, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"MVI", "16", &asm_simple_second_b, 2, ARG_REG, REG_D, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"MVI", "26", &asm_simple_second_b, 2, ARG_REG, REG_H, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"MVI", "36", &asm_simple_second_b, 2, ARG_REG, REG_M, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"RLC", "07", &asm_simple, 0},
    {"RAL", "17", &asm_simple, 0},
    {"DAA", "27", &asm_simple, 0},
    {"STC", "37", &asm_simple, 0},
    {"DAD", "09", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"DAD", "19", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"DAD", "29", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"DAD", "39", &asm_simple, 1, ARG_REG, REG_SP, REG_NONE},
    {"LDAX", "0A", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"LDAX", "1A", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"LHLD", "2A", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"LDA", "3A", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"DCX", "0B", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"DCX", "1B", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"DCX", "2B", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"DCX", "3B", &asm_simple, 1, ARG_REG, REG_SP, REG_NONE},
    {"INR", "0C", &asm_simple, 1, ARG_REG, REG_C, REG_NONE},
    {"INR", "1C", &asm_simple, 1, ARG_REG, REG_E, REG_NONE},
    {"INR", "2C", &asm_simple, 1, ARG_REG, REG_L, REG_NONE},
    {"INR", "3C", &asm_simple, 1, ARG_REG, REG_A, REG_NONE},
    {"DCR", "0D", &asm_simple, 1, ARG_REG, REG_C, REG_NONE},
    {"DCR", "1D", &asm_simple, 1, ARG_REG, REG_E, REG_NONE},
    {"DCR", "2D", &asm_simple, 1, ARG_REG, REG_L, REG_NONE},
    {"DCR", "3D", &asm_simple, 1, ARG_REG, REG_A, REG_NONE},
    {"MVI", "0E", &asm_simple_second_b, 2, ARG_REG, REG_C, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"MVI", "1E", &asm_simple_second_b, 2, ARG_REG, REG_E, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"MVI", "2E", &asm_simple_second_b, 2, ARG_REG, REG_L, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"MVI", "3E", &asm_simple_second_b, 2, ARG_REG, REG_A, REG_NONE, ARG_VALUE, REG_NONE, REG_NONE},
    {"RRC", "0F", &asm_simple, 0},
    {"RAR", "1F", &asm_simple, 0},
    {"CMA", "2F", &asm_simple, 0},
    {"CMC", "3F", &asm_simple, 0},
    {"MOV", "40", &asm_byte_reg, 2, ARG_REG, REG_B, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"MOV", "48", &asm_byte_reg, 2, ARG_REG, REG_C, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"MOV", "50", &asm_byte_reg, 2, ARG_REG, REG_D, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"MOV", "58", &asm_byte_reg, 2, ARG_REG, REG_E, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"MOV", "60", &asm_byte_reg, 2, ARG_REG, REG_H, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"MOV", "68", &asm_byte_reg, 2, ARG_REG, REG_L, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"MOV", "70", &asm_byte_reg, 2, ARG_REG, REG_M, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"MOV", "78", &asm_byte_reg, 2, ARG_REG, REG_A, REG_NONE, ARG_REG, REG_NONE, REG_NONE},
    {"ADD", "80", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"ADC", "88", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"SUB", "90", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"SBB", "98", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"ANA", "A0", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"XRA", "A8", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"ORA", "B0", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"CMP", "B8", &asm_byte_reg, 1, ARG_REG, REG_NONE, REG_NONE},
    {"RNZ", "C0", &asm_simple, 0},
    {"RNC", "D0", &asm_simple, 0},
    {"RPO", "E0", &asm_simple, 0},
    {"RP", "F0", &asm_simple, 0},
    {"POP", "C1", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"POP", "D1", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"POP", "E1", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"POP", "F1", &asm_simple, 1, ARG_REG, REG_PSW, REG_NONE},
    {"JNZ", "C2", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JNC", "D2", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JPO", "E2", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JP", "F2", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JMP", "C3", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"OUT", "D3", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"XTHL", "E3", &asm_simple, 0},
    {"DI", "F3", &asm_simple, 0},
    {"CNZ", "C4", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CNC", "D4", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CPO", "E4", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CP", "R4", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"PUSH", "C5", &asm_simple, 1, ARG_REG, REG_B, REG_NONE},
    {"PUSH", "D5", &asm_simple, 1, ARG_REG, REG_D, REG_NONE},
    {"PUSH", "E5", &asm_simple, 1, ARG_REG, REG_H, REG_NONE},
    {"PUSH", "F5", &asm_simple, 1, ARG_REG, REG_PSW, REG_NONE},
    {"ADI", "C6", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"SUI", "D6", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"ANI", "E6", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"ORI", "R6", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"RST", "C7", &asm_rst, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"RZ", "C8", &asm_simple, 0},
    {"RC", "D8", &asm_simple, 0},
    {"RPE", "E8", &asm_simple, 0},
    {"RM", "F8", &asm_simple, 0},
    {"RET", "C9", &asm_simple, 0},
    {"PCHL", "E9", &asm_simple, 0},
    {"SPHL", "F9", &asm_simple, 0},
    {"JZ", "CA", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JC", "DA", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JPE", "EA", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"JM", "FA", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"IN", "DB", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"XCHG", "EB", &asm_simple, 0},
    {"EI", "FB", &asm_simple, 0},
    {"CZ", "CC", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CC", "DC", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CPE", "EC", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CM", "FC", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CALL", "CD", &asm_simple_w, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"ACI", "CE", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"SBI", "DE", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"XRI", "EE", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
    {"CPI", "RE", &asm_simple_b, 1, ARG_VALUE, REG_NONE, REG_NONE},
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
    {"M", REG_M},
    {"PSW", REG_PSW},
    {"SP", REG_SP},
    {0}
};

void show_help()
{
    printf("Usage: osasm80 [file withtout extension]\n");
    printf("Usage: osasm80 [input.s] [output.obj]\n");
}


int main(int argc, char ** argv)
{
    printf("Old-School Assembler for i80\n");
    printf("Copyright (c) 2023, Humberto Costa\n\n");
    build(argc, argv, "i80");
}