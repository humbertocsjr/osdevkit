#include "../common/osdevkit.h"

#ifndef OSCC_H
#define OSCC_H

#define NAME_LEN 80



extern FILE * _in;
extern char _in_name[NAME_LEN];
extern uint16_t _in_line;
extern uint16_t _in_col;
extern FILE * _out;
extern char _out_name[NAME_LEN];

extern obj_t _prev;
extern obj_t _curr;
extern obj_t _next;

void fail();
void error(char * txt);
void error_fmt1(char * fmt, size_t arg1);
void error_fmt2(char * fmt, size_t arg1, size_t arg2);

void tok_init();

#endif
