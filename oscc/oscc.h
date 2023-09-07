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
bool tok_next();

#define ID_LEN 20

#define PP_START TK_CUSTOM
#define PP_END TK_CUSTOM + 1
#define PP_ARG1 TK_CUSTOM + 2
#define PP_ARG2 TK_CUSTOM + 3
#define PP_ARG3 TK_CUSTOM + 4
#define PP_ARG4 TK_CUSTOM + 5

#define TYPE_CHAR TK_CUSTOM + 10
#define TYPE_SHORT TK_CUSTOM + 11
#define TYPE_INT TK_CUSTOM + 12
#define TYPE_LONG TK_CUSTOM + 13
#define TYPE_UCHAR TK_CUSTOM + 14
#define TYPE_USHORT TK_CUSTOM + 15
#define TYPE_UINT TK_CUSTOM + 16
#define TYPE_ULONG TK_CUSTOM + 17
#define TYPE_STRUCT TK_CUSTOM + 18
#define TYPE_VOID TK_CUSTOM + 19
#define TYPE_UNSIGNED TK_CUSTOM + 20
#define TYPE_SIGNED TK_CUSTOM + 21

#endif
