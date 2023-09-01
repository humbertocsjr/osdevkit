#include "osout.h"


FILE * _in;
char _in_objname[NAME_LEN];
char _in_filename[NAME_LEN];
uint16_t _in_line;
uint16_t _in_col;
FILE * _out;
char _out_name[NAME_LEN];
obj_t _obj;
uint16_t _file_count = 0;
uint16_t _org = 0;
uint16_t _pos = 0;
uint8_t _sector = LNK_NONE;

void error(char * msg)
{
    fprintf(stderr, "%s:%d:%d: Error: %s\n", _in_filename, _in_line, _in_col, msg);
    fclose(_out);
    remove(_out_name);
    exit(1);
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

void step_calc_pos_code()
{
    printf(" -= Calculating code position =-\n");
    reopen_in();
    while(next())
    {
        switch (_obj.type)
        {
            case LNK_PTR_PUBLIC:
                break;
        }
    }
}

void step_calc_pos_data()
{
    printf(" -= Calculating data position =-\n");
    reopen_in();
    while(next())
    {
        
    }

}

void step_calc_pos_bss()
{
    printf(" -= Calculating bss position =-\n");
    reopen_in();
    while(next())
    {
        
    }

}

void step_calc_const()
{
    printf(" -= Calculating constants =-\n");
    reopen_in();
    while(next())
    {
        
    }

}

void step_write_code()
{
    printf(" -= Writing code =-\n");
    uint16_t value = 0;
    int16_t tmp;
    reopen_in();
    while(next())
    {
        switch (_obj.type)
        {
            case LNK_BINARY:
                if(_sector == LNK_SEG_CODE)
                {
                    out_data(_obj.body, _obj.size);
                }
                break;
            case LNK_EXPR_EMIT_POP_BYTE:
                if(_sector == LNK_SEG_CODE)
                {
                    tmp = *((int16_t *)&value);
                    if(tmp < INT8_MIN || tmp > INT8_MAX) error("Byte value overflow");
                    out_byte((uint8_t) value);
                }
                break;
            case LNK_EXPR_EMIT_POP_WORD:
                if(_sector == LNK_SEG_CODE)
                {
                    out_word(value);
                }
                break;
        }
        
    }
}

void step_write_data()
{
    printf(" -= Writing data =-\n");
    reopen_in();
    while(next())
    {
        
    }
}


