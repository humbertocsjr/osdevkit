#include "osout.h"

#define DICT_LEN 4096
#define CONST_LEN 2048
#define EXPR_STACK_SIZE 64

#define CONST_NONE 0
#define CONST_PUBLIC 1
#define CONST_GLOBAL 1
#define CONST_LOCAL 1

typedef struct const_s
{
    char * name;
    uint8_t type;
    struct const_s * parent;
    uint16_t value;
    
} const_t;

char _dict[DICT_LEN];
uint16_t _dict_len = 0;
const_t _const[CONST_LEN];
const_t * _const_current_global = 0;

FILE * _in;
char _in_objname[NAME_LEN];
char _in_filename[NAME_LEN];
uint16_t _in_line;
uint16_t _in_col;
FILE * _out;
char _out_name[NAME_LEN];
obj_t _obj;
uint16_t _file_count = 0;
uint16_t _org_code = 0;
uint16_t _org_data = 0;
uint16_t _org_bss = 0;
uint16_t _pos = 0;
uint8_t _sector = LNK_NONE;

uint16_t _expr_stack[EXPR_STACK_SIZE];
uint8_t _expr_stack_ptr = EXPR_STACK_SIZE - 1;

void error(char * msg)
{
    fprintf(stderr, "%s:%d:%d: Error: %s\n", _in_filename, _in_line, _in_col, msg);
    fclose(_out);
    remove(_out_name);
    exit(1);
}

void expr_push(uint16_t value)
{
    if(_expr_stack_ptr <= 1) error("Expression stack overflow");
    _expr_stack[_expr_stack_ptr--] = value;

}

uint16_t expr_pop()
{
    if(_expr_stack_ptr >= (EXPR_STACK_SIZE - 1)) error("Expression stack underflow");
    return _expr_stack[++_expr_stack_ptr];
}

char * search(char * dict, uint16_t dict_len, char * name)
{
    uint16_t i;
    char * ptr;
    for (i = 0; i < dict_len; i++)
    {
        ptr = &dict[i];
        if(!strcmp(ptr, name))
        {
            return ptr;
        }
        i += strlen(ptr);
    }
    return 0;
}

char * add(char * dict, uint16_t * dict_len, uint16_t dict_capacity, char * name, char * overflow_msg)
{
    uint16_t len = strlen(name);
    char * ptr = search(dict, *dict_len, name);
    if(ptr) return ptr;
    ptr = &dict[*dict_len];
    if((*dict_len + len + 1) >= dict_capacity) error(overflow_msg);
    memcpy(ptr, name, len + 1);
    *dict_len += len + 1;
    dict[*dict_len] = 0;
    return ptr;
}

bool equals(uint8_t type, char * txt)
{
    if(type != _obj.type) return false;
    if(txt == 0) return true;
    return !strcmp(txt, _obj.body) ? true : false;
}

bool next()
{
    bool ok;
    ok = obj_read(_in, &_obj);
    if(equals(LNK_FILE, 0) && !equals(LNK_FILE, _in_filename))
    {
        if(_file_count > 0) printf("[ OK ]\n");
        strncpy(_in_filename, _obj.body, NAME_LEN);
        printf("  - %s", _obj.body);
        _file_count ++;
    }
    if(equals(LNK_FILE_LINE, 0))
    {
        _in_line = *((uint16_t *)_obj.body);
        printf(".");
    }
    if(equals(LNK_FILE_COL, 0))
    {
        _in_col = *((uint16_t *)_obj.body);
    }
    if(equals(LNK_SEG_CODE, 0)) _sector = LNK_SEG_CODE;
    if(equals(LNK_SEG_DATA, 0)) _sector = LNK_SEG_DATA;
    if(equals(LNK_SEG_BSS, 0)) _sector = LNK_SEG_BSS;
    if(equals(LNK_END, 0)) ok = false;
    if(!ok) printf("[ OK ]\n");
    return ok;
}

void open_in()
{
    _file_count = 0;
    memcpy(_in_filename, _in_objname, NAME_LEN);
    _in = fopen(_in_objname, "rb");
    if(_in == 0) error("File not found");
}

void close_in()
{
    fclose(_in);
}

void reopen_in()
{
    close_in();
    open_in();
}

void out_byte(uint8_t value)
{
    fwrite(&value, 1, 1, _out);
}

void out_word(uint16_t value)
{
    fwrite(&value, 1, 2, _out);
}

void out_str(char * value)
{
    fwrite(value, 1, strlen(value), _out);
}

void out_data(uint8_t * value, uint16_t len)
{
    fwrite(value, 1, len, _out);
}

uint16_t read_const(char * name)
{
    for (size_t i = 0; i < CONST_LEN; i++)
    {
        if(_const[i].type == CONST_LOCAL && !strcmp(name, _const[i].name))
        {
            return _const[i].value;
        }
    }
    for (size_t i = 0; i < CONST_LEN; i++)
    {
        if(_const[i].type == CONST_GLOBAL && !strcmp(name, _const[i].name))
        {
            return _const[i].value;
        }
    }
    for (size_t i = 0; i < CONST_LEN; i++)
    {
        if(_const[i].type == CONST_PUBLIC && !strcmp(name, _const[i].name))
        {
            return _const[i].value;
        }
    }
    printf("[%s?]", name);
    error("Constant not found");
}

void clear_const(uint8_t type)
{
    for (size_t i = 0; i < CONST_LEN; i++)
    {
        if(_const[i].type == type)
        {
            _const[i].type = CONST_NONE;
        }
    }
}

void write_const(uint8_t type, char * name, uint16_t value)
{
    char * ptr = add(_dict, &_dict_len, DICT_LEN, name, "Constant dictionary overflow");
    
    for (size_t i = 0; i < CONST_LEN; i++)
    {
        if(_const[i].type == CONST_NONE)
        {
            _const[i].type = type;
            _const[i].name = ptr;
            _const[i].parent = type == CONST_LOCAL ? _const_current_global : NULL;
            _const[i].value = value;
            return;
        }
    }
    error("Constant list overflow");
}

uint16_t pos()
{
    switch (_sector)
    {
        case LNK_SEG_CODE:
            return _pos + _org_code;
        case LNK_SEG_DATA:
            return _pos + _org_data;
        case LNK_SEG_BSS:
            return _pos + _org_bss;
        
        default:
            error("Sector not found");
            break;
    }
}

void step_calc_pos(uint8_t sector)
{
    reopen_in();
    while(next())
    {
        switch (_obj.type)
        {
            case LNK_BINARY:
                if(_sector == sector)
                {
                    _pos += _obj.size;
                }
                break;
            case LNK_EXPR_EMIT_POP_BYTE:
                if(_sector == sector)
                {
                    _pos += 1;
                }
                break;
            case LNK_EXPR_EMIT_POP_WORD:
                if(_sector == sector)
                {
                    _pos += 2;
                }
                break;
            case LNK_PTR_PUBLIC:
                if(_sector == sector)
                {
                    write_const(CONST_PUBLIC, _obj.body, pos());
                }
                break;
            case LNK_PTR_GLOBAL:
                if(_sector == sector)
                {
                    write_const(CONST_GLOBAL, _obj.body, pos());
                }
                break;
            case LNK_PTR_LOCAL:
                if(_sector == sector)
                {
                    write_const(CONST_LOCAL, _obj.body, pos());
                }
                break;
                
        }
    }
}

void step_calc_pos_code()
{
    printf(" -= Calculating code position =-\n");
    step_calc_pos(LNK_SEG_CODE);
}

void step_calc_pos_data()
{
    printf(" -= Calculating data position =-\n");
    step_calc_pos(LNK_SEG_DATA);

}

void step_calc_pos_bss()
{
    printf(" -= Calculating bss position =-\n");
    step_calc_pos(LNK_SEG_BSS);

}

void step_calc_const()
{
    printf(" -= Calculating constants =-\n");
    reopen_in();
    while(next())
    {
        
    }

}

void step_write(uint8_t sector)
{
    uint16_t value = 0;
    int16_t tmp;
    reopen_in();
    while(next())
    {
        if(_sector == sector)
        {
            switch (_obj.type)
            {
                case LNK_BINARY:
                    out_data(_obj.body, _obj.size);
                    break;
                case LNK_EXPR_PUSH_VALUE:
                    expr_push(*((uint16_t *)_obj.body));
                    break;
                case LNK_EXPR_PUSH_CURR_POS:
                    expr_push(pos());
                    break;
                case LNK_EXPR_PUSH_CONST:
                    expr_push(read_const(_obj.body));
                    break;
                case LNK_EXPR_PUSH_CODE_OFFSET_POS:
                    expr_push(_org_code);
                    break;
                case LNK_EXPR_PUSH_DATA_OFFSET_POS:
                    expr_push(_org_data);
                    break;
                case LNK_EXPR_PUSH_BSS_OFFSET_POS:
                    expr_push(_org_bss);
                    break;
                case LNK_EXPR_PUSH_POP_ADD_PUSH:
                    value = expr_pop();
                    expr_push(expr_pop() + value);
                    break;
                case LNK_EXPR_PUSH_POP_DIV_PUSH:
                    value = expr_pop();
                    if(value == 0) error("Division by 0");
                    expr_push(expr_pop() / value);
                    break;
                case LNK_EXPR_PUSH_POP_MOD_PUSH:
                    value = expr_pop();
                    if(value == 0) error("Division by 0");
                    expr_push(expr_pop() % value);
                    break;
                case LNK_EXPR_PUSH_POP_MUL_PUSH:
                    value = expr_pop();
                    expr_push(expr_pop() * value);
                    break;
                case LNK_EXPR_PUSH_POP_SHL_PUSH:
                    value = expr_pop();
                    expr_push(expr_pop() << value);
                    break;
                case LNK_EXPR_PUSH_POP_SHR_PUSH:
                    value = expr_pop();
                    expr_push(expr_pop() >> value);
                    break;
                case LNK_EXPR_PUSH_POP_SUB_PUSH:
                    value = expr_pop();
                    expr_push(expr_pop() - value);
                    break;
                case LNK_EXPR_EMIT_POP_BYTE:
                    value = expr_pop();
                    tmp = *((int16_t *)&value);
                    if(tmp < INT8_MIN || tmp > INT8_MAX) error("Byte value overflow");
                    out_byte((uint8_t) value);
                    break;
                case LNK_EXPR_EMIT_POP_WORD:
                    out_word(expr_pop());
                    break;
            }
        }
        
    }
}

void step_write_code()
{
    printf(" -= Writing code =-\n");
    step_write(LNK_SEG_CODE);
}

void step_write_data()
{
    printf(" -= Writing data =-\n");
    step_write(LNK_SEG_DATA);
}


void init()
{
    for (size_t i = 0; i < CONST_LEN; i++)
    {
        _const[i].type = CONST_NONE;
    }
}