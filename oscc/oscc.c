#include "oscc.h"

FILE * _in = 0;
char _in_name[NAME_LEN];
uint16_t _in_line = 0;
uint16_t _in_col = 0;
FILE * _out = 0;
char _out_name[NAME_LEN];

obj_t _prev;
obj_t _curr;
obj_t _next;


void error_header()
{
    fail();
    fprintf(stderr, "%s:%u:%u: Error: ", _curr.internal_name, (unsigned)_curr.internal_line, (unsigned)_curr.internal_col);
}

void error(char * txt)
{
    error_header();
    fprintf(stderr, "%s", txt);
    fputs("",_in);
    exit(1);
}

void error_fmt1(char * fmt, size_t arg1)
{
    error_header();
    fprintf(stderr, fmt, arg1);
    fputs("",_in);
    exit(1);
}

void error_fmt2(char * fmt, size_t arg1, size_t arg2)
{
    error_header();
    fprintf(stderr, fmt, arg1, arg2);
    fputs("",_in);
    exit(1);
}

void tok_init()
{
    _next.type = TK_NONE;
    _next.internal_name = "";
    _curr.type = TK_NONE;
    _curr.internal_name = "";
}

bool tok_next()
{
    memcpy(&_prev, &_curr, sizeof(obj_t));
    memcpy(&_curr, &_next, sizeof(obj_t));
    if(_next.type == LNK_END) return false;
    if(_curr.type == LNK_END) return false;
    obj_read(_in, &_next);
    return true;
}
