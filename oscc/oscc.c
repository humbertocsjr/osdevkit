#include "oscc.h"

FILE * _in = 0;
uint16_t _in_count = 0;
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
    fflush(stdout);
    if(_in_count > 0) printf("[ ERROR ]\n");
    fail();
    fprintf(stderr, "%s:%u:%u: Error: ", _curr.internal_name, (unsigned)_curr.internal_line, (unsigned)_curr.internal_col);
}

void error(char * txt)
{
    error_header();
    fprintf(stderr, "%s\n", txt);
    exit(1);
}

void error_fmt1(char * fmt, size_t arg1)
{
    error_header();
    fprintf(stderr, fmt, arg1);
    fprintf(stderr, "\n");
    exit(1);
}

void error_fmt2(char * fmt, size_t arg1, size_t arg2)
{
    error_header();
    fprintf(stderr, fmt, arg1, arg2);
    fprintf(stderr, "\n");
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
    if(_curr.type == LNK_END) return false;
    if(_next.type == LNK_END) return true;
    while(1)
    {
        if(!obj_read(_in, &_next)) _next.type = LNK_END;
        if(_next.type == LNK_END) return true;
        switch (_next.type)
        {
            case LNK_FILE:
                obj_write(_out, &_next);
                if(strcmp(_in_name, _next.body)) 
                {
                    if(_in_count > 0) printf("[ OK ]\n");
                    printf(" - %s", _next.body);
                    _in_count++;
                }
                strncpy(_in_name, _next.body, NAME_LEN);
                _next.internal_name = _in_name;
                _next.internal_line = 0;
                _next.internal_col = 0;
                break;
            case LNK_FILE_LINE:
                printf(".");
                obj_write(_out, &_next);
                _next.internal_line = *((uint16_t *)_next.body);
                break;
            case TK_NEW_LINE:
                break;
            case LNK_FILE_COL:
                obj_write(_out, &_next);
                _next.internal_col = *((uint16_t *)_next.body);
                break;
            default:
                return true;
        }
    }
}
