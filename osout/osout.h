#ifndef OSOUT_H
#define OSOUT_H

#include <osdevkit.h>

#define NAME_LEN 80
extern FILE * _in;
extern char _in_objname[NAME_LEN];
extern char _in_filename[NAME_LEN];
extern uint16_t _in_line;
extern uint16_t _in_col;
extern FILE * _out;
extern char _out_name[NAME_LEN];
extern obj_t _obj;
extern uint16_t _file_count;
extern uint16_t _org;
extern uint16_t _pos;

void out_byte(uint8_t value);
void out_word(uint16_t value);
void out_str(char * value);
void out_data(uint8_t * value, uint16_t len);

void open_in();
void close_in();

void step_calc_pos_code();
void step_calc_pos_data();
void step_calc_pos_bss();
void step_calc_const();
void step_write_code();
void step_write_data();

#endif