#include "osout.h"

void show_help()
{
    printf("Usage: osoutbin [origin] [linker object name]\n");
}

int main(int argc, char ** argv)
{
    int i;
    printf("Old-School Generic Binary Generator\n");
    printf("Copyright (c) 2023, Humberto Costa\n\n");
    if(argc != 3)
    {
        show_help();
        exit(1);
    }
    _org_code = parsenum(argv[1], strlen(argv[1]));
    _org_data = _org_code;
    _org_bss = _org_code;
    strncpy(_in_objname, argv[2], NAME_LEN - 5);
    strcat(_in_objname, ".lnk");
    strncpy(_out_name, argv[2], NAME_LEN - 5);
    strcat(_out_name, ".bin");
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