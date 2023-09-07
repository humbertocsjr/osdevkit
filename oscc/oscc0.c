// C Parser and Pre-processor
#include "oscc.h"

#define PP_START TK_CUSTOM
#define PP_END TK_CUSTOM + 1
#define PP_ARG1 TK_CUSTOM + 2
#define PP_ARG2 TK_CUSTOM + 3
#define PP_ARG3 TK_CUSTOM + 4
#define PP_ARG4 TK_CUSTOM + 5

#define DEFS_LEN 128
#define BUFFER_LEN 8192

typedef struct
{
    char * name;
    uint8_t * contents;
} def_t;

char _src_c = 1;
bool _src_c_skip = false;
def_t _defs[DEFS_LEN];
uint8_t _buffer[BUFFER_LEN];
uint16_t _buffer_pos = 0;


FILE * _in2 = 0;
char _in2_name[NAME_LEN];
uint16_t _in2_line;
uint16_t _in2_col;
obj_t _in2_curr;
obj_t _in2_next;

FILE * _in3 = 0;
char _in3_name[NAME_LEN];
uint16_t _in3_line;
uint16_t _in3_col;
obj_t _in3_curr;
obj_t _in3_next;

obj_t _def_obj;

void def_init()
{
    for (uint16_t i = 0; i < DEFS_LEN; i++)
    {
        _defs[i].name = 0;
    }
    
}

void def_add(char * name)
{
    uint16_t len = strlen(name);
    for (uint16_t i = 0; i < DEFS_LEN; i++)
    {
        if(_defs[i].name == 0)
        {
            if((len + _buffer_pos) >= BUFFER_LEN) error("Definition buffer overflow");
            _defs[i].name = (char *)&_buffer[_buffer_pos];
            strcpy(_defs[i].name, name);
            _buffer_pos += len + 1;
            _defs[i].contents = &_buffer[_buffer_pos];
            return;
        }
    }
    error("Definition list overflow");
}

void def_remove(char * name)
{
    for (uint16_t i = 0; i < DEFS_LEN; i++)
    {
        if(_defs[i].name != 0 && !strcmp(_defs[i].name, name))
        {
            _defs[i].name[0] = 0;
            return;
        }
    }
    error_fmt1("Definition not found: %s", (size_t)name);   
}

char * def_get(char * name)
{
    for (uint16_t i = 0; i < DEFS_LEN; i++)
    {
        if(_defs[i].name != 0 && !strcmp(_defs[i].name, name))
        {
            return _defs[i].contents;
        }
    }
    error_fmt1("Definition not found: %s", (size_t)name);  
    return 0;
}

bool def_exists(char * name)
{
    for (uint16_t i = 0; i < DEFS_LEN; i++)
    {
        if(_defs[i].name != 0 && !strcmp(_defs[i].name, name))
        {
            return true;
        }
    }
    return false;
}

char src_next_c()
{
    if(_src_c_skip)
    {
        _src_c_skip = false;
        return _src_c;
    }
    int c = getc(_in);
    _in_col++;
    if(c == '\n')
    {
        _in_col = 0;
        _in_line ++;
    }
    if(c == '\r') return src_next_c();
    if(c == EOF) _src_c = 0;
    else _src_c = (char)c;
    return _src_c;
}

void buf_writeraw(uint8_t type, char * contents, uint8_t len)
{
    _buffer[_buffer_pos++] = *((char*)&type);
    _buffer[_buffer_pos++] = *((char*)&len);
    if(len > 0)
    {
        memcpy(&_buffer[_buffer_pos], contents, len);
        _buffer_pos += len;
    }
}

void buf_write(obj_t * obj)
{
    buf_writeraw(obj->type, obj->body, obj->size);
}

uint8_t * buf_read(obj_t * obj, uint8_t * buf)
{
    obj->type = *buf++;
    obj->size = *buf++;
    memset(obj->body, 0, OBJ_SIZE);
    if(obj->size > 0)
    {
        memcpy(obj->body, buf, obj->size);
        buf += obj->size;
    }
    return buf;
}

bool tok_src_next()
{
    bool add_c = false;
    bool done = false;
    memcpy(&_prev, &_curr, sizeof(obj_t));
    memcpy(&_curr, &_next, sizeof(obj_t));
    if(_curr.type == LNK_END) return false;
    if(_next.type == LNK_END) return true;
    memset(_next.body, 0, OBJ_SIZE);
    _next.type = TK_NONE;
    while(src_next_c() != EOF)
    {
        switch (_next.type)
        {
            case TK_NONE:
                _next.size = 0;
                _next.body[0] = 0;
                _next.internal_name = _in_name;
                _next.internal_line = _in_line;
                _next.internal_col = _in_col;
                if(isalpha(_src_c) || _src_c == '_' || _src_c == '#')
                {
                    _next.type = TK_ID;
                    add_c = true;
                }
                else if(isdigit(_src_c))
                {
                    _next.type = TK_NUM;
                    add_c = true;
                }
                else if(_src_c == ',')
                {
                    _next.type = TK_COMMA;
                    return true;
                }
                else if(_src_c == ':')
                {
                    _next.type = TK_COLON;
                    return true;
                }
                else if(_src_c == ';')
                {
                    _next.type = TK_SEMICOLON;
                    return true;
                }
                else if(_src_c == '{')
                {
                    _next.type = TK_BLOCK_OPEN;
                    return true;
                }
                else if(_src_c == '}')
                {
                    _next.type = TK_BLOCK_CLOSE;
                    return true;
                }
                else if(_src_c == '(')
                {
                    _next.type = TK_PAR_OPEN;
                    return true;
                }
                else if(_src_c == ')')
                {
                    _next.type = TK_PAR_OPEN;
                    return true;
                }
                else if(_src_c == '[')
                {
                    _next.type = TK_IDX_OPEN;
                    return true;
                }
                else if(_src_c == ']')
                {
                    _next.type = TK_IDX_CLOSE;
                    return true;
                }
                else if(_src_c == '\n')
                {
                    _next.type = TK_NEW_LINE;
                    return true;
                }
                else if(_src_c == ' ' || _src_c == '\t')
                {
                }
                else if(_src_c == 0)
                {
                    _next.type = LNK_END;
                    return true;
                }
                else error_fmt1("Character unknown", (int)_src_c);
                break;
            case TK_ID:
                if(isalnum(_src_c) || _src_c == '_')
                {
                    add_c = true;
                }
                else
                {
                    _src_c_skip = true;
                    return true;
                }
                break;
            case TK_NUM:
                _src_c = tolower(_src_c);
                if(_next.body[0] == '0' &&_next.body[1] == 0 && (_src_c == 'x' || _src_c == 'b'))
                {
                    add_c = true;
                }
                else if(isxdigit(_src_c))
                {
                    add_c = true;
                }
                else if(_src_c == 'h' || _src_c == 'b')
                {
                    add_c = true;
                    done = true;
                }
                else
                {
                    _src_c_skip = true;
                    return true;
                }
                break;
            default:
                break;
        }
        if(add_c)
        {
            add_c = false;
            _next.body[_next.size ++] = _src_c;
            _next.body[_next.size] = 0;
        }
        if(done) return true;
    }
    return true;
}

void show_help()
{
    printf("Usage: oscc0 [filename without extension] [DEFINEs....]\n");
}

void fail()
{
    fclose(_in);
    fclose(_out);
    remove(_out_name);
}

void out_pos(obj_t * obj)
{
    obj_writeword(_out, LNK_FILE_LINE, obj->internal_line);
    obj_writeword(_out, LNK_FILE_COL, obj->internal_col);
}
void out_fullpos(obj_t * obj)
{
    obj_writestr(_out, LNK_FILE, obj->internal_name);
    out_pos(obj);
}

void process_def(char * name)
{
    uint8_t * buf = def_get(name);
    buf = buf_read(&_def_obj, buf);
    while(_def_obj.type != PP_END)
    {
        if(_def_obj.type == TK_ID && def_exists(_def_obj.body))
        {
            process_def(_def_obj.body);
        }
        else
        {
            obj_write(_out, &_def_obj);
        }
        buf = buf_read(&_def_obj, buf);
    }

}

void process(char * filename)
{
    if(_in3 != 0) error("Input files overflow");
    _in3 = _in2;
    _in3_line = _in2_line;
    _in3_col = _in2_col;
    strncpy(_in3_name, _in2_name, NAME_LEN);
    memcpy(&_in3_curr, &_in2_curr, sizeof(obj_t));
    memcpy(&_in3_next, &_in2_next, sizeof(obj_t));
    _in2 = _in;
    _in2_line = _in_line;
    _in2_col = _in_col;
    strncpy(_in2_name, _in_name, NAME_LEN);
    memcpy(&_in2_curr, &_curr, sizeof(obj_t));
    memcpy(&_in2_next, &_next, sizeof(obj_t));
    _in_line = 1;
    _in_col = 0;
    strncpy(_in_name, filename, NAME_LEN);
    if(_in != 0)
    {
        printf("[SUSPEND]\n");
    }
    _in = fopen(_in_name, "r");
    if(!_in) error_fmt1("File can't be opened: %s", (size_t)_in_name);
    printf(" - %s", _in_name);
    obj_writestr(_out, LNK_FILE, _in_name);
    _src_c_skip = false;
    _src_c = ' ';
    tok_src_next();
    while(tok_src_next())
    {
        out_pos(&_curr);
        if(_curr.type == TK_NEW_LINE) printf(".");
        if(_curr.type == TK_ID && _curr.body[0] == '#')
        {
            printf(".");
            if(!strcmp(_curr.body, "#define"))
            {
                if(!tok_src_next() || _curr.type != TK_ID) error("Definition name expected");
                def_add(_curr.body);
                buf_writeraw(PP_START, 0, 0);
                out_fullpos(&_curr);
                while(tok_src_next() && _curr.type != TK_NEW_LINE)
                {
                    buf_write(&_curr);
                }
                buf_writeraw(PP_END, 0, 0);
            }
            else if(!strcmp(_curr.body, "#include"))
            {
                if(!tok_src_next() || _curr.type != TK_STR) error("Include filename expected");
                process(_curr.body);
            }
            else error_fmt1("Pre-processor command not implemented: %s", (size_t)_curr.body);
        }
        else if(_curr.type == TK_ID)
        {
            if(def_exists(_curr.body))
            {
                process_def(_curr.body);
            }
            else
            {
                obj_write(_out, &_curr);
            }
        }
        else
        {
            obj_write(_out, &_curr);
        }
    }

    fclose(_in);
    _in = _in2;
    _in_line = _in2_line;
    _in_col = _in2_col;
    _src_c_skip = false;
    strncpy(_in_name, _in2_name, NAME_LEN);
    memcpy(&_curr, &_in2_curr, sizeof(obj_t));
    memcpy(&_next, &_in2_next, sizeof(obj_t));
    _in2 = _in3;
    _in2_line = _in3_line;
    _in2_col = _in3_col;
    strncpy(_in2_name, _in3_name, NAME_LEN);
    memcpy(&_in2_curr, &_in3_curr, sizeof(obj_t));
    memcpy(&_in2_next, &_in3_next, sizeof(obj_t));
    _in3 = 0;
    printf("[ OK ]\n");
    if(_in != 0)
    {
        printf(" - %s", _in_name);
        obj_writestr(_out, LNK_FILE, _in_name);
        obj_writeword(_out, LNK_FILE_LINE, _in_line);
        obj_writeword(_out, LNK_FILE_COL, _in_col);
    }
}

void main(int argc, char ** argv)
{
    printf("Old-School C Compiler - Parser and Pre-processor\n");
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
    strcat(_in_name, ".c");
    strcat(_out_name, ".cc1");
    tok_init();
    def_init();
    _in = 0;
    _in2 = 0;
    _in3 = 0;
    if(!(_out = fopen(_out_name, "wb+"))) error_fmt1("File can't be opened: %s", (size_t)_out_name);
    process(_in_name);
    fclose(_out);
}