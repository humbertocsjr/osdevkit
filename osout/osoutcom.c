#include "osout.h"

void show_help()
{
    printf("Usage: osoutcom [linker object name]\n");
}

int main(int argc, char ** argv)
{
    int i;
    printf("Old-School COM Executable Generator\n");
    printf("Copyright (c) 2023, Humberto Costa\n\n");
    if(argc != 2)
    {
        show_help();
        exit(1);
    }
    _org_code = 0x100;
    _org_data = _org_code;
    _org_bss = _org_code;
    strncpy(_in_objname, argv[1], NAME_LEN - 5);
    strcat(_in_objname, ".lnk");
    strncpy(_out_name, argv[1], NAME_LEN - 5);
    strcat(_out_name, ".com");
    _out = fopen(_out_name, "wb+");
    if(_out == 0)
    {
        printf("Error: Output file cant be opened\n");
        exit(2);
    }
    init();
    open_in();
    step_calc_pos_code();
    step_calc_pos_data();
    step_calc_pos_bss();
    step_calc_const();
    step_write_code();
    step_write_data();
    close_in();
    fclose(_out);
    printf("\n");
}