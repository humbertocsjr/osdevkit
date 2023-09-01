#include <osdevkit.h>

#define NAME_LEN 80
#define DICT_LEN 4096
#define FUNCS_LEN 1024
typedef struct func_s
{
    char * name;
    struct func_s * parent;
    bool use;
} func_t;


FILE * _in;
char _in_objname[NAME_LEN];
char _in_filename[NAME_LEN];
uint16_t _in_line;
uint16_t _in_col;
FILE * _out;
char _out_name[NAME_LEN];
obj_t _obj;
char _dict[DICT_LEN];
uint16_t _dict_len = 0;
func_t _funcs[FUNCS_LEN];
uint16_t _funcs_len = 0;
uint16_t _file_count;
uint16_t _line_count;
uint16_t _funcs_found_count;
uint16_t _refs_included_count;
bool _start_func_found = false;
char _start_func_file[NAME_LEN];

void error(char * msg)
{
    fprintf(stderr, "%s:%d:%d: Error: %s\n", _in_filename, _in_line, _in_col, msg);
    fclose(_out);
    remove(_out_name);
    exit(1);
}

char * search(char * dict, uint16_t dict_len, char * name)
{
    uint16_t i;
    char * ptr;
    for (i = 0; i < dict_len; i++)
    {
        ptr = &dict[i];
        if(!strcmp(ptr, name))
        {
            return ptr;
        }
        i += strlen(ptr);
    }
    return 0;
}

char * add(char * dict, uint16_t * dict_len, uint16_t dict_capacity, char * name, char * overflow_msg)
{
    uint16_t len = strlen(name);
    char * ptr = search(dict, *dict_len, name);
    if(ptr) return ptr;
    ptr = &dict[*dict_len];
    if((*dict_len + len + 1) >= dict_capacity) error(overflow_msg);
    memcpy(ptr, name, len + 1);
    *dict_len += len + 1;
    dict[*dict_len] = 0;
    return ptr;
}

func_t * search_func(char * name)
{
    for (size_t i = 0; i < _funcs_len; i++)
    {
        if(!strcmp(name, _funcs[i].name) && _funcs[i].parent == 0)
        {
            return &_funcs[i];
        }
    }
    return 0;
}

func_t * add_func(char * name)
{
    if((_funcs_len +1) >= FUNCS_LEN) error("Function list overflow");
    _funcs[_funcs_len].use = false;
    _funcs[_funcs_len].name = add(_dict, &_dict_len, DICT_LEN, name, "Function name buffer overflow");
    _funcs[_funcs_len].parent = 0;
    return &_funcs[_funcs_len++];
}

func_t * add_use(char * name, func_t * parent)
{
    if((_funcs_len +1) >= FUNCS_LEN) error("Function list overflow");
    _funcs[_funcs_len].use = false;
    _funcs[_funcs_len].name = add(_dict, &_dict_len, DICT_LEN, name, "Function name buffer overflow");
    _funcs[_funcs_len].parent = parent;
    return &_funcs[_funcs_len++];
}

bool equals(uint8_t type, char * txt)
{
    if(type != _obj.type) return false;
    if(txt == 0) return true;
    return !strcmp(txt, _obj.body) ? true : false;
}

bool next()
{
    bool ok;
    ok = obj_read(_in, &_obj);
    if(equals(LNK_FILE, 0))
    {
        if(_file_count > 0) printf("[ OK ]\n");
        strncpy(_in_filename, _obj.body, NAME_LEN);
        printf("  - %s", _obj.body);
        _file_count ++;
    }
    if(equals(LNK_FILE_LINE, 0))
    {
        _in_line = *((uint16_t *)_obj.body);
        printf(".");
    }
    if(equals(LNK_FILE_COL, 0))
    {
        _in_col = *((uint16_t *)_obj.body);
    }
    if(equals(LNK_END, 0)) ok = false;
    if(!ok) printf("[ OK ]\n");
    return ok;
}

void copy_to_out()
{
    obj_write(_out, &_obj);
}

bool step_find_refs()
{
    func_t * current = 0;
    while(next())
    {
        if(equals(LNK_FUNC_START, 0))
        {
            current = add_func(_obj.body);
            if(equals(LNK_FUNC_START, "_start"))
            {
                _start_func_found = true;
                memcpy(_start_func_file, _in_objname, NAME_LEN);
            }
        }
        else if(equals(LNK_FUNC_END, 0))
        {
            current = 0;
        }
        else if(equals(LNK_FUNC_USE, 0))
        {
            current = add_use(_obj.body, current);
        }
    }
    return true;
}

void step_build_refs_table(char * funcname)
{
    func_t * func = search_func(funcname);
    func_t * child = 0;
    if(func == 0) return;
    printf(".");
    for (size_t i = 0; i < _funcs_len; i++)
    {
        if(_funcs[i].parent == func)
        {
            child = search_func(_funcs[i].name);
            if(child)
            {
                child->use = true;
                step_build_refs_table(child->name);
            }
        }
    }
}

bool step_copy_start()
{
    while(next())
    {
        if(equals(LNK_FUNC_START, "_start"))
        {
            _start_func_found = true;
            do
            {
                copy_to_out();
            } while (next() && !equals(LNK_FUNC_END, 0));
            printf("[ OK ]\n");
            return false;
        }
    }
    return true;
}

bool step_copy_refs()
{
    bool copy = true;
    func_t * func;
    while(next())
    {
        if(equals(LNK_FUNC_START, 0))
        {
            _funcs_found_count++;
            if(func = search_func(_obj.body))
            {
                copy = func->use;
            }
            else
            {
                copy = false;
            }
            if(copy) _refs_included_count++;
        }
        if(copy)
        {
            if(equals(LNK_FILE_LINE,0)) _line_count++;
            copy_to_out();
        }
        if(equals(LNK_FUNC_END, 0))
        {
            copy = true;
        }
    }
    return true;
}

bool execute_one_step(char * filename, bool (*step)())
{
    int filename_len;
    bool looping = true;
    strncpy(_in_objname, filename, NAME_LEN - 5);
    filename_len = strlen(_in_objname);
    if(strcmp(".obj", &_in_objname[filename_len - 4]) && strcmp(".OBJ", &_in_objname[filename_len - 4]))
    {
        strcat(_in_objname, ".obj");
    }
    memcpy(_in_filename, _in_objname, NAME_LEN);
    _in = fopen(_in_objname, "rb");
    _in_line = 0;
    _in_col = 0;
    if(!_in) error("File can't be opened");
    looping = step();
    fclose(_in);
    memcpy(_in_filename, _out_name, NAME_LEN);
    _in_line = 0;
    _in_col = 0;
    return looping;
}

void execute_step(int argc, char ** argv, bool (*step)())
{
    int i;
    _file_count = 0;
    _line_count = 0;
    for(i = 1; i < argc; i++)
    {
        if(!execute_one_step(argv[i], step)) break;
    }
}

void show_help()
{
    printf("Usage: oslink [main] [extras ...]\n");
}


int main(int argc, char ** argv)
{
    int i;
    printf("Old-School Multiplatform Linker\n");
    printf("Copyright (c) 2023, Humberto Costa\n\n");
    if(argc < 2)
    {
        show_help();
        exit(1);
    }
    strncpy(_out_name, argv[1], NAME_LEN - 5);
    strcat(_out_name, ".lnk");
    _out = fopen(_out_name, "wb+");
    obj_writeraw(_out, LNK_SEG_CODE, 0, 0);
    if(_out == 0)
    {
        printf("Error: Output file cant be opened\n");
        exit(2);
    }

    printf(" -= Finding references =-\n");
    execute_step(argc, argv, &step_find_refs);
    if(!_start_func_found) error("'_start' function not found.");
    printf("  - Building reference table");
    step_build_refs_table("_start");
    printf("[ OK ]\n");

    printf(" -= Copying '_start' function =-\n");
    _file_count = 0;
    execute_one_step(_start_func_file, &step_copy_start);

    printf(" -= Copying referenced functions =-\n");
    _refs_included_count = 1;
    _funcs_found_count = 0;
    _line_count = 0;
    execute_step(argc, argv, &step_copy_refs);
    
    printf(" -= Statistics =-\n");
    printf("  - %d functions found\n", _funcs_found_count);
    printf("  - %d functions included\n", _refs_included_count);
    printf("  - %d lines of code included\n", _line_count);
    obj_writeraw(_out, LNK_END, 0, 0);
    fclose(_out);
    printf("\n");
}