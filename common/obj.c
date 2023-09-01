#include <osdevkit.h>


bool obj_read(FILE * in, obj_t * obj)
{
    if(fread(&obj->type, 1,1, in) != 1) return false;
    if(fread(&obj->size, 1,1, in) != 1) return false;
    memset(obj->body, 0, OBJ_SIZE);
    if(obj->size > 0)
        if(fread(obj->body, 1, obj->size, in) != obj->size) return false;
    return true;
}

bool obj_write(FILE * out, obj_t * obj)
{
    fwrite(&obj->type, 1, 1, out);
    fwrite(&obj->size, 1, 1, out);
    if(obj->size > 0)
    {
        fwrite(obj->body, 1, obj->size, out);
    }
    return true;
}

bool obj_writeraw(FILE * out, uint8_t type, void * body, uint8_t size)
{
    fwrite(&type, 1, 1, out);
    fwrite(&size, 1, 1, out);
    if(size > 0)
    {
        fwrite(body, 1, size, out);
    }
    return true;
}

bool obj_writebyte(FILE * out, uint8_t type, uint8_t value)
{
    return obj_writeraw(out, type, &value, 1);
}

bool obj_writeword(FILE * out, uint8_t type, uint16_t value)
{
    return obj_writeraw(out, type, &value, 2);
}

bool obj_writestr(FILE * out, uint8_t type, char * string)
{
    return obj_writeraw(out, type, string, strlen(string));
}