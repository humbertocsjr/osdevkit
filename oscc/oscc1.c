// C Syntax Analyzer
#include "oscc.h"

char _cc1_name[NAME_LEN];
bool _global_level = true;

extern void process();

bool equals(char * str1, char * str2)
{
    return !strcmp(str1, str2);
}

bool tok_equals(char * str)
{
    if(_curr.type != TK_ID) return false;
    return !strcmp(_curr.body, str);
}

bool tok_preview_equals(char * str)
{
    if(_next.type != TK_ID) return false;
    return !strcmp(_next.body, str);
}

bool tok_is_type()
{
    return
        tok_equals("void") ||
        tok_equals("int") ||
        tok_equals("char") ||
        tok_equals("short") ||
        tok_equals("long") ||
        tok_equals("unsigned") ||
        tok_equals("signed") ||
        tok_equals("struct");
}

bool tok_preview_is_type()
{
    return
        tok_preview_equals("void") ||
        tok_preview_equals("int") ||
        tok_preview_equals("char") ||
        tok_preview_equals("short") ||
        tok_preview_equals("long") ||
        tok_preview_equals("unsigned") ||
        tok_preview_equals("signed") ||
        tok_preview_equals("struct");
}

void tok_next_expected(uint8_t type, char * msg)
{
    if(!tok_next() || _curr.type != type) error_fmt1("%s expected", (size_t)msg);
}

void tok_expected(uint8_t type, char * msg)
{
    if(_curr.type != type) error_fmt1("%s expected", (size_t)msg);
}

void tok_valid_id_size()
{
    if((_curr.size + 1) >= ID_LEN) error_fmt1("'%s' is too big", (size_t)_curr.body);
}

uint8_t get_type(char * id)
{
    uint8_t type = TK_NONE;
    memset(id, 0, ID_LEN);
    if(tok_equals("void")) type = TYPE_VOID;
    else if(tok_equals("unsigned")) type = TYPE_UNSIGNED;
    else if(tok_equals("signed")) type = TYPE_SIGNED;
    else if(tok_equals("char")) type = TYPE_CHAR;
    else if(tok_equals("short")) type = TYPE_SHORT;
    else if(tok_equals("int")) type = TYPE_INT;
    else if(tok_equals("long")) type = TYPE_LONG;
    else if(tok_equals("struct"))
    {
        tok_next_expected(TK_ID, "Structure name");
        tok_valid_id_size();
        strcpy(id, _curr.body);
        tok_next();
        return TYPE_STRUCT;
    }
    else error("Data type expected");
    if(type == TYPE_SIGNED)
    {
        if(tok_preview_equals("char")){ type = TYPE_CHAR; tok_next(); }
        else if(tok_preview_equals("short")){ type = TYPE_SHORT; tok_next(); }
        else if(tok_preview_equals("int")){ type = TYPE_INT; tok_next(); }
        else if(tok_preview_equals("long")){ type = TYPE_LONG; tok_next(); } 
        else type = TYPE_INT;
    }
    else if(type == TYPE_UNSIGNED)
    {
        if(tok_preview_equals("char")){ type = TYPE_UCHAR; tok_next(); }
        else if(tok_preview_equals("short")){ type = TYPE_USHORT; tok_next(); }
        else if(tok_preview_equals("int")){ type = TYPE_UINT; tok_next(); }
        else if(tok_preview_equals("long")){ type = TYPE_ULONG; tok_next(); } 
        else type = TYPE_INT;
    }
    tok_next();
    return type;
}

void expr()
{

}

void process_type()
{
    char type_id[ID_LEN];
    char name_id[ID_LEN];
    uint8_t pointers = 0;
    uint8_t type = get_type(type_id);
    while(_curr.type == TK_MATH_MUL && tok_next())
    {
        pointers++;
    }
    tok_expected(TK_ID, "Name");
    tok_valid_id_size();
    strcpy(name_id, _curr.body);
    tok_next();
    if(_curr.type == TK_SEMICOLON)
    {
        obj_writeraw(_out, _global_level ? TK_PUBLIC_VAR_MARKER : TK_LOCAL_VAR_MARKER, name_id, strlen(name_id));
        obj_writeraw(_out, type, type_id, strlen(type_id));
        if(pointers > 0) obj_writeword(_out, TK_POINTER_MARKER, pointers);
        obj_writeraw(_out, TK_END_VAR_MARKER, 0, 0);
        return;
    }
    else if(_curr.type == TK_PAR_CLOSE)
    {
        obj_writeraw(_out, _global_level ? TK_PUBLIC_VAR_MARKER : TK_LOCAL_VAR_MARKER, name_id, strlen(name_id));
        obj_writeraw(_out, type, type_id, strlen(type_id));
        if(pointers > 0) obj_writeword(_out, TK_POINTER_MARKER, pointers);
        obj_writeraw(_out, TK_END_VAR_MARKER, 0, 0);
        return;
    }
    else if(_curr.type == TK_ASSIGN)
    {
        obj_writeraw(_out, _global_level ? TK_PUBLIC_VAR_MARKER : TK_LOCAL_VAR_MARKER, 0, 0);
        obj_writeraw(_out, type, type_id, strlen(type_id));
        if(pointers > 0) obj_writeword(_out, TK_POINTER_MARKER, pointers);
        obj_writeraw(_out, TK_INITIAL_VALUE_VAR_MARKER, 0, 0);
        tok_next();
        expr();
        obj_writeraw(_out, TK_END_VAR_MARKER, 0, 0);
    }
    else if(_curr.type == TK_PAR_OPEN)
    {
        obj_writeraw(_out, LNK_FUNC_START, name_id, strlen(name_id));
        obj_writeraw(_out, type, type_id, strlen(type_id));
        if(pointers > 0) obj_writeword(_out, TK_POINTER_MARKER, pointers);
        _global_level = false;
        while (tok_next() && _curr.type != TK_PAR_CLOSE )
        {
            process_type();
            if(_curr.type == TK_PAR_CLOSE) break;
            else tok_expected(TK_COMMA, "','");
        }
        tok_expected(TK_PAR_CLOSE, "')'");
        process();
        _global_level = true;
        obj_writeraw(_out, LNK_FUNC_END, name_id, strlen(name_id));
        return;
    }

    while(_curr.type == TK_COMMA)
    {
        if(tok_preview_is_type()) return;
        tok_next();
        if(_curr.type == TK_ID)
        {
            tok_valid_id_size();
            strcpy(name_id, _curr.body);
            tok_next();
            obj_writeraw(_out, _global_level ? TK_PUBLIC_VAR_MARKER : TK_LOCAL_VAR_MARKER, name_id, strlen(name_id));
            obj_writeraw(_out, type, type_id, strlen(type_id));
            if(pointers > 0) obj_writeword(_out, TK_POINTER_MARKER, pointers);
            obj_writeraw(_out, TK_END_VAR_MARKER, 0, 0);
        }
    }
    tok_expected(TK_SEMICOLON, "';'");
}

void process_block()
{
    while (tok_next() && _curr.type != TK_BLOCK_CLOSE)
    {
        process();
    }
    tok_expected(TK_BLOCK_CLOSE, "'}'");
}

void process()
{
    fflush(stdout);
    if(_curr.type == TK_ID)
    {
        if(tok_is_type())
        {
            process_type();
        }
    }
    else if(_curr.type == TK_BLOCK_OPEN)
    {
        process_block();
    }
    else if(_curr.type == TK_SEMICOLON)
    {

    }
}

void show_help()
{
    printf("Usage: oscc1 [filename without extension]\n");
}

void fail()
{
    fclose(_in);
    fclose(_out);
    remove(_out_name);
}

void main(int argc, char ** argv)
{
    printf("Old-School C Compiler - Syntax Analyzer\n");
    printf("Copyright (c) 2023, Humberto Costa\n\n");
    if(argc < 2)
    {
        show_help();
        exit(0);
    }
    if(!strcmp(argv[1], "-h"))
    {
        show_help();
        exit(0);
    }
    strncpy(_in_name, argv[1], NAME_LEN - 5);
    strncpy(_out_name, argv[1], NAME_LEN - 5);
    strcat(_in_name, ".cc1");
    strcpy(_cc1_name, _in_name);
    strcat(_out_name, ".cc");
    tok_init();
    if(!(_out = fopen(_out_name, "wb+"))) error_fmt1("File can't be opened: %s", (size_t)_out_name);
    if(!(_in = fopen(_in_name, "rb"))) error_fmt1("File can't be opened: %s", (size_t)_in_name);
    while(tok_next())
    {
        process();
    }
    obj_writeraw(_out, LNK_END, 0, 0);
    fclose(_in);
    fclose(_out);
    remove(_cc1_name);
    printf("[ OK ]\n");
    exit(0);
}