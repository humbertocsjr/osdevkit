#include "osasm.h"

#define NAME_LEN 80
#define BUF_LEN 128
#define TOKEN_QTY 32

typedef struct
{
    uint8_t type;
    uint8_t len;
    char * buf;
} token_t;

char * _cpu;
bool _in_eof;
FILE * _in;
uint16_t _in_line;
uint16_t _in_col;
char _in_buf[BUF_LEN+1];
uint8_t _in_buf_pos;
char _in_name[NAME_LEN];
FILE * _out;
char _out_name[NAME_LEN];
cmd_t * _cmd;
arg_t _arg1;
arg_t _arg2;

char _tok_buf[BUF_LEN+1];
token_t _tok[TOKEN_QTY];
uint8_t _tok_col[TOKEN_QTY];
uint8_t _tok_curr;
char _proc_curr[NAME_LEN];

void exit_error()
{
    fclose(_out);
    remove(_out_name);
    exit(1);
}

void error(uint8_t col, char * msg)
{
    printf("[ FAIL ]\n%s:%d:%d: Error: %s\n", _in_name, _in_line, col, msg);
    exit_error();
}

void error2(uint8_t col, char * msg, char * msg2)
{
    printf("[ FAIL ]\n%s:%d:%d: Error: %s%s\n", _in_name, _in_line, col, msg, msg2);
    exit_error();
}

bool buf_read()
{
    bool read;
    memset(_in_buf, 0, BUF_LEN+1);
    read = fread(_in_buf, 1, BUF_LEN, _in) > 0 ? true : false;
    _in_buf_pos = 0;
    return read;
}

char buf_getc()
{
    char c;
    if(_in_buf_pos >= BUF_LEN)
    {
        if(!buf_read()) return 0;
    }
    c = _in_buf[_in_buf_pos++];
    _in_col++;
    if(_in_buf_pos >= BUF_LEN)
    {
        if(!buf_read()) return 0;
    }
    return c;
}

char buf_peekc()
{
    if((_in_buf_pos + 1) >= BUF_LEN)
    {
        return 0;
    }
    return _in_buf[_in_buf_pos + 1];
}

uint8_t tk_type()
{
    return _tok[_tok_curr].type;
}

bool tk_next()
{
    if(_tok[_tok_curr].type == TK_NONE) return false;
    _tok_curr++;
    return _tok[_tok_curr].type != TK_NONE ? true : false;
}

uint8_t tk_peek()
{
    if(_tok[_tok_curr].type == TK_NONE) return TK_NONE;
    return _tok[_tok_curr+1].type;
}

bool tk_peek_id_equal(char * value)
{
    if(_tok[_tok_curr].type == TK_NONE) return false;
    if(_tok[_tok_curr+1].type != TK_ID) return false;
    if(_tok[_tok_curr+1].buf == 0) return false;
    return (strcmp(value, _tok[_tok_curr+1].buf) == 0) ? true : false;
}

bool tk_read(char * buf, int len)
{
    memset(buf, 0, len);
    if(_tok[_tok_curr].buf == 0) return false;
    if(len <= _tok[_tok_curr].len) return false;
    strncpy(buf, _tok[_tok_curr].buf, len);
    return true;
}

bool tk_equal(uint8_t type)
{
    return _tok[_tok_curr].type == type ? true : false;
}

bool id_equal(char * value1, char * value2)
{
    uint16_t len = strlen(value1) + 1;
    for (size_t i = 0; i < len; i++)
    {
        if(toupper(value1[i]) != toupper(value2[i]))
        {
            return false;
        }
    }
    return true;
}

bool tk_id_equal(char * value)
{
    if(_tok[_tok_curr].type != TK_ID) return false;
    if(_tok[_tok_curr].buf == 0) return false;
    return id_equal(_tok[_tok_curr].buf, value);
}

uint16_t tk_num()
{
    return parsenum(_tok[_tok_curr].buf, _tok[_tok_curr].len);
}

bool line_read()
{
    int i;
    char c;
    char n;
    int tok;
    _tok_curr = 0;
    _in_line ++;
    _in_col = 0;
    memset(_tok_buf, 0, BUF_LEN);
    tok = 0;
    _tok[tok].type = TK_NONE;
    if(_in_eof) return false;
    for (i = 0; i < BUF_LEN; i++)
    {
        _tok_buf[i] = 0;
        _tok_buf[i+1] = 0;
        c = buf_getc();
        n = buf_peekc();
line_process:
        if((tok + 1) >= TOKEN_QTY)
        {
            printf("\nError: Token list overflow\n");
            exit_error();
        }
        if(c == 26 || c == 0)
        {
            _in_eof = true;
            if(_tok[tok].type != TK_NONE) tok++;
            _tok[tok].type = TK_NONE;
            return tok > 0 ? true : false;
        }
        switch(_tok[tok].type)
        {
            case TK_NONE:
                _tok[tok+1].type = TK_NONE;
                _tok[tok].buf = 0;
                _tok[tok].len = 0;
                _tok_col[tok] = _in_col;
                _tok_col[tok+1] = _in_col;
                if(isalpha(c) || c == '.' || c == '_')
                {
                    _tok[tok].type = TK_ID;
                }
                else if(isdigit(c))
                {
                    _tok[tok].type = TK_NUM;
                }
                else if(c == ',') _tok[tok++].type = TK_COMMA;
                else if(c == ':') _tok[tok++].type = TK_COLON;
                else if(c == '(') _tok[tok++].type = TK_PAR_OPEN;
                else if(c == ')') _tok[tok++].type = TK_PAR_CLOSE;
                else if(c == '+') _tok[tok++].type = TK_MATH_ADD;
                else if(c == '-') _tok[tok++].type = TK_MATH_SUB;
                else if(c == '"') 
                {
                    _tok[tok++].type = TK_STR;
                    _tok[tok].buf = &_tok_buf[i+1];
                    _tok[tok].len = 0;

                }
                else if(c == 10) return true;
                else if(c == 13 || c == 32) {}
                if(_tok[tok].type != TK_NONE && _tok[tok].type != TK_STR)
                {
                    _tok_buf[i] = c;
                    _tok[tok].buf = &_tok_buf[i];
                    _tok[tok].len = 1;
                }
                break;
            case TK_ID:
                if(isalnum(c) || c == '.' || c == '_' || c == '\'')
                {
                    _tok_buf[i] = c;
                    _tok[tok].len ++;
                }
                else
                {
                    tok ++;
                    goto line_process;
                }
                break;
            case TK_NUM:
                if
                (
                    isxdigit(c) || 
                    c == 'X' || 
                    c == 'x' || 
                    c == 'b' || 
                    c == 'B' || 
                    c == 'h' || 
                    c == 'H'
                )
                {
                    _tok_buf[i] = c;
                    _tok[tok].len ++;
                }
                else
                {
                    tok ++;
                    goto line_process;
                }
                break;
            case TK_STR:
                if
                (
                    c == '\\' &&
                    n == '"' 
                )
                {
                    _tok_buf[i] = n;
                    _tok[tok].len ++;
                    buf_getc();
                }
                else if(c != '"')
                {
                    _tok_buf[i] = c;
                    _tok[tok].len ++;
                }
                else
                {
                    tok ++;
                }
                break;
        }
        

    }
    error(i, "Line buffer overflow");
}

uint8_t reg_find(char * id)
{
    uint8_t pos;
    char tmp[10];
    strncpy(tmp, id, 10);
    for (size_t i = 0; i < 10; i++)
    {
        tmp[i] = toupper(tmp[i]);
    }
    tmp[9] = 0;
    pos = 0;
    while (_regs[pos].name != 0)
    {
        if(!strcmp(_regs[pos].name, tmp))
        {
            return _regs[pos].id;
        }
        pos++;
    }
    return REG_NONE;
}

void lnk_expr()
{
    if(tk_equal(TK_NUM))
    {
        obj_writeword(_out, LNK_EXPR_PUSH_VALUE, tk_num());
    }
    tk_next();
}

void arg_clear(arg_t * arg)
{
    arg->value = 0;
    arg->reg1 = REG_NONE;
    arg->reg2 = REG_NONE;
    arg->type = ARG_NONE;
    arg->lbl1_name = 0;
    arg->lbl1_op = TK_NONE;
    arg->lbl2_name = 0;
    arg->lbl2_op = TK_NONE;
}

void arg_expr(arg_t * arg)
{
    uint8_t op = TK_MATH_ADD;
    uint8_t reg_tmp;
    bool first = true;
    arg_clear(arg);
    if(tk_equal(TK_IDX_OPEN))
    {
        while(tk_next() && !tk_equal(TK_IDX_CLOSE))
        {
            if(tk_equal(TK_NUM))
            {
                if(arg->type == ARG_NONE)
                {
                    arg->type = ARG_PTR_VALUE;
                }
                else if(arg->type == ARG_PTR_REG)
                {
                    arg->type = ARG_PTR_REG_VALUE;
                }
                else if(arg->type == ARG_PTR_2REG)
                {
                    arg->type = ARG_PTR_2REG_VALUE;
                }
                if(op == TK_MATH_ADD)
                {
                    arg->value += tk_num();
                }
                else if(op == TK_MATH_SUB)
                {
                    arg->value -= tk_num();
                }
                else error(_tok_col[_tok_curr], "Math operator not supported");
            }
            else if(tk_equal(TK_ID))
            {
                reg_tmp = reg_find(_tok[_tok_curr].buf);
                if(reg_tmp != REG_NONE)
                {
                    if(op != TK_MATH_ADD) error(_tok_col[_tok_curr], "Math operator not supported");
                    if(arg->reg1 == REG_NONE)
                    {
                        arg->reg1 = reg_tmp;
                        if(arg->type == ARG_NONE)
                        {
                            arg->type = ARG_PTR_REG;
                        }
                        else if(arg->type == ARG_PTR_VALUE)
                        {
                            arg->type = ARG_PTR_REG_VALUE;
                        }
                    }
                    else if(arg->reg2 == REG_NONE)
                    {
                        arg->reg2 = reg_tmp;
                        if(arg->type == ARG_PTR_REG)
                        {
                            arg->type = ARG_PTR_2REG;
                        }
                        else if(arg->type == ARG_PTR_REG_VALUE)
                        {
                            arg->type = ARG_PTR_2REG_VALUE;
                        }
                    }
                    else error(_tok_col[_tok_curr], "Registers overflow. (In argument)");
                }
                else if(op == TK_MATH_ADD || op == TK_MATH_SUB)
                {
                    if(arg->type == ARG_NONE)
                    {
                        arg->type = ARG_PTR_VALUE;
                    }
                    else if(arg->type == ARG_PTR_REG)
                    {
                        arg->type = ARG_PTR_REG_VALUE;
                    }
                    else if(arg->type == ARG_PTR_2REG)
                    {
                        arg->type = ARG_PTR_2REG_VALUE;
                    }
                    if(arg->lbl1_op == TK_NONE)
                    {
                        arg->lbl1_op = op;
                        arg->lbl1_name = _tok[_tok_curr].buf;
                    }
                    else if(arg->lbl2_op == TK_NONE)
                    {
                        arg->lbl2_op = op;
                        arg->lbl2_name = _tok[_tok_curr].buf;
                    }
                    else error(_tok_col[_tok_curr], "Labels overflow. (In argument)");
                }
                else error(_tok_col[_tok_curr], "Math operator not supported");
            }
            op = TK_NONE;
            if(tk_peek() == TK_MATH_ADD)
            {
                op = TK_MATH_ADD;
                tk_next();
            }
            else if(tk_peek() == TK_MATH_SUB)
            {
                op = TK_MATH_SUB;
                tk_next();
            }
        }
    }
    else
    {
        while
        (
            first ||
            (
                (
                    tk_peek() == TK_ID ||
                    tk_peek() == TK_NUM
                ) && tk_next()
            )
        )
        {
            first = false;
            if(tk_equal(TK_NUM))
            {
                if(arg->type == ARG_NONE)
                {
                    arg->type = ARG_VALUE;
                }
                else if(arg->type == ARG_REG)
                {
                    arg->type = ARG_REG_VALUE;
                }
                else if(arg->type == ARG_2REG)
                {
                    arg->type = ARG_2REG_VALUE;
                }
                if(op == TK_MATH_ADD)
                {
                    arg->value += tk_num();
                }
                else if(op == TK_MATH_SUB)
                {
                    arg->value -= tk_num();
                }
                else error(_tok_col[_tok_curr], "Math operator not supported");
            }
            else if(tk_equal(TK_ID))
            {
                reg_tmp = reg_find(_tok[_tok_curr].buf);
                if(reg_tmp != REG_NONE)
                {
                    if(op != TK_MATH_ADD) error(_tok_col[_tok_curr], "Math operator not supported");
                    if(arg->reg1 == REG_NONE)
                    {
                        arg->reg1 = reg_tmp;
                        if(arg->type == ARG_NONE)
                        {
                            arg->type = ARG_REG;
                        }
                        else if(arg->type == ARG_VALUE)
                        {
                            arg->type = ARG_REG_VALUE;
                        }
                    }
                    else if(arg->reg2 == REG_NONE)
                    {
                        arg->reg2 = reg_tmp;
                        if(arg->type == ARG_REG)
                        {
                            arg->type = ARG_2REG;
                        }
                        else if(arg->type == ARG_REG_VALUE)
                        {
                            arg->type = ARG_2REG_VALUE;
                        }
                    }
                    else error(_tok_col[_tok_curr], "Registers overflow. (In argument)");
                }
                else if(op == TK_MATH_ADD || op == TK_MATH_SUB)
                {
                    if(arg->type == ARG_NONE)
                    {
                        arg->type = ARG_VALUE;
                    }
                    else if(arg->type == ARG_REG)
                    {
                        arg->type = ARG_REG_VALUE;
                    }
                    else if(arg->type == ARG_2REG)
                    {
                        arg->type = ARG_2REG_VALUE;
                    }
                    if(arg->lbl1_op == TK_NONE)
                    {
                        arg->lbl1_op = op;
                        arg->lbl1_name = _tok[_tok_curr].buf;
                    }
                    else if(arg->lbl2_op == TK_NONE)
                    {
                        arg->lbl2_op = op;
                        arg->lbl2_name = _tok[_tok_curr].buf;
                    }
                    else error(_tok_col[_tok_curr], "Labels overflow. (In argument)");
                }
                else error(_tok_col[_tok_curr], "Math operator not supported");
            }
            op = TK_NONE;
            if(tk_peek() == TK_MATH_ADD)
            {
                op = TK_MATH_ADD;
                tk_next();
            }
            else if(tk_peek() == TK_MATH_SUB)
            {
                op = TK_MATH_SUB;
                tk_next();
            }
        }
    }
}

bool arg_cmp(arg_t * arg, bool argc, uint8_t type, uint8_t reg1, uint8_t reg2)
{
    if(!argc && type == ARG_NONE) return true;
    if(argc && reg1 != REG_NONE && arg->reg1 != reg1) return false;
    if(argc && reg2 != REG_NONE && arg->reg2 != reg2) return false;
    if(argc && type == arg->type) return true;
    if(argc && type == ARG_REG_VALUE && arg->type == ARG_REG) return true;
    if(argc && type == ARG_2REG_VALUE && arg->type == ARG_2REG) return true;
    if(argc && type == ARG_PTR_REG_VALUE && arg->type == ARG_PTR_REG) return true;
    if(argc && type == ARG_PTR_2REG_VALUE && arg->type == ARG_PTR_2REG) return true;
    return false;
}

bool process_token()
{
    bool ok;
    uint16_t num;
    uint16_t cmd;
    uint16_t times_qty;
    uint16_t times_ptr;
    char * cmd_name;
    obj_writeword(_out, LNK_FILE_COL, (uint16_t)_tok_col[_tok_curr]);
    switch(_tok[_tok_curr].type)
    {
        case TK_ID:
            if(tk_id_equal("end"))
            {
                _in_eof = true;
                return false;
            }
            else if(tk_id_equal("section"))
            {
                if(!tk_next()) error(_tok_col[_tok_curr], "Segment type expected");
                if(tk_id_equal("code"))
                {
                    obj_writeraw(_out, LNK_SEG_CODE, 0, 0);
                }
                else if(tk_id_equal("data"))
                {
                    obj_writeraw(_out, LNK_SEG_DATA, 0, 0);
                }
                else if(tk_id_equal("bss"))
                {
                    obj_writeraw(_out, LNK_SEG_BSS, 0, 0);
                }
                else error(_tok_col[_tok_curr], "Segment type unknown");
            }
            else if(tk_id_equal("cpu"))
            {
                if(!(tk_next() && tk_id_equal(_cpu)))
                {
                    error(_tok_col[_tok_curr], "CPU Unsupported");
                }
            }
            else if(tk_id_equal("proc"))
            {
                if(_tok[_tok_curr].len >= NAME_LEN) error(_tok_col[_tok_curr], "Procedure name overflow");
                tk_next();
                strncpy(_proc_curr, _tok[_tok_curr].buf, NAME_LEN);
                obj_writestr(_out, LNK_FUNC_START, _proc_curr);
                obj_writestr(_out, LNK_PTR_PUBLIC, _proc_curr);
                obj_writestr(_out, LNK_PTR_GLOBAL, _proc_curr);
            }
            else if(tk_id_equal("endp"))
            {
                obj_writestr(_out, LNK_FUNC_END, _proc_curr);
                memset(_proc_curr, 0, NAME_LEN);
            }
            else if(tk_id_equal("use"))
            {
                tk_next();
                obj_writestr(_out, LNK_FUNC_USE, _tok[_tok_curr].buf);
            }
            else if(tk_id_equal("db"))
            {
                while(tk_next())
                {
                    if(tk_equal(TK_STR))
                    {
                        obj_writestr(_out, LNK_BINARY, _tok[_tok_curr].buf);
                    }
                    else
                    {
                        obj_writeraw(_out, LNK_EXPR_START, 0,0);
                        lnk_expr();
                        obj_writeraw(_out, LNK_EXPR_EMIT_POP_BYTE, 0,0);
                    }
                    if(tk_equal(TK_COMMA)) tk_next();
                }
            }
            else if(tk_id_equal("dw"))
            {
                while(tk_next())
                {
                    if(tk_equal(TK_STR))
                    {
                        obj_writestr(_out, LNK_BINARY, _tok[_tok_curr].buf);
                    }
                    else
                    {
                        obj_writeraw(_out, LNK_EXPR_START, 0,0);
                        lnk_expr();
                        obj_writeraw(_out, LNK_EXPR_EMIT_POP_WORD, 0,0);
                    }
                    if(tk_equal(TK_COMMA)) tk_next();
                }
            }
            else if(tk_peek() == TK_COLON)
            {
                obj_writestr(_out, LNK_PTR_GLOBAL, _tok[_tok_curr].buf);
                tk_next();
            }
            else if(tk_peek_id_equal("equ") || tk_peek_id_equal("EQU"))
            {
                obj_writestr(_out, LNK_CONST_GLOBAL, _tok[_tok_curr].buf);
                tk_next();
                tk_next();
                lnk_expr();
            }
            else 
            {
                cmd_name = _tok[_tok_curr].buf;
                arg_clear(&_arg1);
                arg_clear(&_arg2);
                cmd = 0;
                ok = false;
                while(_prefixes[cmd].cmd != 0)
                {
                    if(id_equal(_prefixes[cmd].cmd, cmd_name))
                    {
                        if
                        (
                            arg_cmp(&_arg1, _prefixes[cmd].args > 0 ? true : false, _prefixes[cmd].a1, _prefixes[cmd].a1reg1, _prefixes[cmd].a1reg2) &&
                            arg_cmp(&_arg2, _prefixes[cmd].args > 1 ? true : false, _prefixes[cmd].a2, _prefixes[cmd].a2reg1, _prefixes[cmd].a2reg2)
                        )
                        {
                            _prefixes[cmd].assemble(_in_name, _in_line, &_prefixes[cmd], &_arg1, &_arg2);
                            ok = true;
                            break;
                        }
                    }
                    cmd++;
                }
                if(!ok)
                {
                    if(tk_next())
                    {
                        arg_expr(&_arg1);
                        if(tk_next())
                        {
                            if(!tk_equal(TK_COMMA)) error(_tok_col[_tok_curr], "',' expected");
                            if(tk_next())
                            {
                                arg_expr(&_arg2);
                            }
                        }
                    }
                    cmd = 0;
                    ok = false;
                    if(id_equal("call", cmd_name))
                    {
                        if(_arg1.type == ARG_VALUE && _arg1.lbl1_name)obj_writestr(_out, LNK_FUNC_USE, _arg1.lbl1_name);
                        if(_arg2.type == ARG_VALUE && _arg2.lbl1_name)obj_writestr(_out, LNK_FUNC_USE, _arg2.lbl1_name);
                    }
                    while(_cmds[cmd].cmd != 0)
                    {
                        if(id_equal(_cmds[cmd].cmd, cmd_name))
                        {
                            if
                            (
                                arg_cmp(&_arg1, _cmds[cmd].args > 0 ? true : false, _cmds[cmd].a1, _cmds[cmd].a1reg1, _cmds[cmd].a1reg2) &&
                                arg_cmp(&_arg2, _cmds[cmd].args > 1 ? true : false, _cmds[cmd].a2, _cmds[cmd].a2reg1, _cmds[cmd].a2reg2)
                            )
                            {
                                _cmds[cmd].assemble(_in_name, _in_line, &_cmds[cmd], &_arg1, &_arg2);
                                ok = true;
                                break;
                            }
                        }
                        cmd++;
                    }
                    if(!ok)
                    {
                        printf("[%s?]", cmd_name);
                        error(_tok_col[_tok_curr], "Command not found");
                    }
                }
            }
            tk_next();
            break;
        case TK_NONE:
            return false;
        default:
            error(_tok_col[_tok_curr], "Mnemonic expected");
    }
    return _tok[_tok_curr].type == TK_NONE ? false : true;
}

void build_file(char * filename)
{
    strncpy(_in_name, filename, NAME_LEN);
    _in = fopen(_in_name, "r");
    if(_in == 0)
    {
        printf("\nError: Input file cant be opened\n");
        exit_error();
    }
    obj_writestr(_out, LNK_FILE, _in_name);
    printf("Assembling %s", _in_name);
    
    _in_eof = false;
    buf_read();
    _in_line = 0;
    

    while(line_read())
    {
        obj_writeword(_out, LNK_FILE_LINE, _in_line);
        while(process_token());
        printf(".");
    }
    printf("[ OK ]\n");

    fclose(_in);
}

void asm_custom(uint8_t type, void * buf, uint8_t len)
{
    obj_writeraw(_out, type, buf, len);
}

void asm_byte(uint8_t value)
{
    obj_writeraw(_out, LNK_BINARY, &value, 1);
}

void asm_word(uint16_t value)
{
    obj_writeraw(_out, LNK_BINARY, &value, 2);
}

void asm_hex(char * hex)
{
    uint8_t out[32];
    uint8_t pos = 0;
    uint8_t byte = 0;
    bool first = true;
    uint8_t len = strlen(hex);
    if(len >= 64) error(_tok_col[_tok_curr], "asm_hex overflow");
    for (uint8_t i = 0; i < len; i++)
    {
        char c = toupper(hex[i]);
        if(c >= '0' && c <= '9')
        {
            byte = (byte << 4) | (c - '0');
        }
        else if(c >= 'A' && c <= 'F')
        {
            byte = (byte << 4) | (c - 'A' + 10);
        }
        else break;
        if(first)
        {
            first = false;
        }
        else 
        {
            first = true;
            out[pos++] = byte;
            byte = 0;
        }
    }
    obj_writeraw(_out, LNK_BINARY, out, pos);
}

void build(int argc, char ** argv, char * cpu)
{
    _cpu = cpu;
    if(argc < 2 || argc > 3)
    {
        show_help();
        exit(1);
    }
    if(argc == 2)
    {
        strncpy(_in_name, argv[1], NAME_LEN - 5);
        strncpy(_out_name, argv[1], NAME_LEN - 5);
        strcat(_in_name, ".s");
        strcat(_out_name, ".obj");
    }
    else
    {
        strncpy(_in_name, argv[1], NAME_LEN);
        strncpy(_out_name, argv[2], NAME_LEN);
    }
    _out = fopen(_out_name, "wb+");
    obj_writeraw(_out, LNK_SEG_CODE, 0, 0);
    if(_out == 0)
    {
        printf("Error: Output file cant be opened\n");
        exit(2);
    }
    memset(_proc_curr, 0, NAME_LEN);
    build_file(_in_name);
    obj_writeraw(_out, LNK_END, 0, 0);
    fclose(_out);
    printf("\n");
}