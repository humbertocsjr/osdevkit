#include <osdevkit.h>

uint16_t parsehex(char * txt, int len)
{
    uint16_t num;
    int i;
    i = 0;
    num = 0;
    for (; i < len; i++)
    {
        if(!isxdigit(txt[i])) return num;
        if(isdigit(txt[i]))
            num = (num << 4) | (txt[i] - '0');
        else if(txt[i] >= 'A' && txt[i] <= 'F')
            num = (num << 4) | (txt[i] - 'A' + 10);
        else if(txt[i] >= 'a' && txt[i] <= 'f')
            num = (num << 4) | (txt[i] - 'f' + 10);
    }
    return num;
}

uint16_t parsenum(char * txt, int len)
{
    uint16_t num;
    int i;
    int div;
    i = 0;
    if(len == 0) return 0;
    if(txt[0] == '0' && (txt[1] == 'X' || txt[1] == 'x'))
    {
        div = 16;
        i = 2;
    }
    else if(txt[0] == '0' && (txt[1] == 'B' || txt[1] == 'b'))
    {
        div = 2;
        i = 2;
    }
    else if(txt[len-1] == 'h' || txt[len-1] == 'H')
    {
        len--;
        div = 10;
    }
    else if(txt[len-1] == 'b' || txt[len-1] == 'B')
    {
        len--;
        div = 2;
    }
    else div = 10;
    num = 0;
    for (; i < len; i++)
    {
        if(div == 2)
        {
            if(txt[i] < '0' && txt[i] > '1') return num;
            num = (num << 1) | (txt[i] - '0');
        }
        else if(div == 10)
        {
            if(txt[i] < '0' && txt[i] > '9') return num;
            num = (num * 10) + (txt[i] - '0');
        }
        else if(div == 16)
        {
            if(!isxdigit(txt[i])) return num;
            if(isdigit(txt[i]))
                num = (num << 4) | (txt[i] - '0');
            else if(txt[i] >= 'A' && txt[i] <= 'F')
                num = (num << 4) | (txt[i] - 'A' + 10);
            else if(txt[i] >= 'a' && txt[i] <= 'f')
                num = (num << 4) | (txt[i] - 'f' + 10);
        }
    }
    return num;
}