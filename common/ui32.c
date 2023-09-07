#include "osdevkit.h"

bool ui32_cmp(ui32_t * val1, ui32_t * val2, uint8_t cmp)
{
    if(val1->high == val2->high)
    {
        if(val1->low == val2->low)
        {
            return cmp == CMP_EQUAL || cmp == CMP_GREATER_EQUAL || cmp == CMP_LESSER_EQUAL;
        }
        if(val1->low > val2->low)
        {
            return cmp == CMP_GREATER || cmp == CMP_GREATER_EQUAL || cmp == CMP_NOT_EQUAL;
        }
        if(val1->low < val2->low)
        {
            return cmp == CMP_LESSER || cmp == CMP_LESSER_EQUAL || cmp == CMP_NOT_EQUAL;
        }
    }
    else if(val1->high > val2->high)
    {
        return cmp == CMP_GREATER || cmp == CMP_GREATER_EQUAL || cmp == CMP_NOT_EQUAL;
    }
    else
    {
        return cmp == CMP_LESSER || cmp == CMP_LESSER_EQUAL || cmp == CMP_NOT_EQUAL;
    }
}

void ui32_copy(ui32_t * dest, ui32_t * orig)
{
    dest->low = orig->low;
    dest->high = orig->high;
}

void ui32_clear(ui32_t * value)
{
    value->low = 0;
    value->high = 0;
}

void ui32_parse(ui32_t * dest, char * orig)
{
    ui32_t val10;
    ui32_t valc;
    val10.low = 10;
    val10.high = 0;
    valc.high = 0;
    uint16_t len = strlen(orig);
    ui32_clear(dest);
    for (size_t i = 0; i < len; i++)
    {
        if(!isdigit(orig[i])) return;
        valc.low = orig[i] - '0';
        ui32_mul(dest, &val10);
        ui32_add(dest, &valc);
    }
}

void ui32_tostring(char * dest, ui32_t * orig, uint16_t len)
{
    ui32_t val, mod, div10;
    div10.low = 10;
    div10.high = 0;
    int16_t i = 0;
    int16_t j = 0;
    char out[32];
    memset(out, 0, 32);
    ui32_copy(&val, orig);
    while(val.low > 0 || val.high > 0)
    {
        ui32_div(&val, &mod, &div10);
        out[i++] = mod.low + '0';
    }
    dest[j] = 0;
    while(i >= 0)
    {
        dest[j++] = out[--i];
        dest[j] = 0;
    }
}

void ui32_add(ui32_t * dest, ui32_t * orig)
{
    if((UINT16_MAX - dest->low) < orig->low)
    {
        dest->low += orig->low;
        dest->high += orig->high + 1;
    }
    else
    {
        dest->low += orig->low;
        dest->high += orig->high;
    }
}

void ui32_sub(ui32_t * dest, ui32_t * orig)
{
    if(dest->low < orig->low)
    {
        dest->low -= orig->low;
        dest->high -= orig->high - 1;
    }
    else
    {
        dest->low -= orig->low;
        dest->high -= orig->high;
    }
}

void ui32_mul(ui32_t * dest, ui32_t * orig)
{
    ui32_t res;
    ui32_t tmp;
    uint16_t bit;
    ui32_clear(&res);
    for (size_t i = 0; i < 32; i++)
    {
        if(i & 0xf0)
        {
            bit = orig->high & (1 << (i & 0x0f));
        }
        else
        {
            bit = orig->low & (1 << i);
        }
        if(bit > 0)
        {
            tmp.low = dest->low;
            tmp.high = dest->high;
            ui32_shl(&tmp, i);
            ui32_add(&res, &tmp);
        }
    }
    dest->low = res.low;
    dest->high = res.high;
}

bool ui32_div(ui32_t * dest, ui32_t * out_mod, ui32_t * orig)
{
    ui32_t res;
    ui32_t tmp;
    if(orig->high == 0 && orig->low == 0) return false;
    ui32_copy(out_mod, orig);
    ui32_clear(&res);
    int16_t pos;
    pos = (int16_t)-1;
    while(ui32_cmp(dest, out_mod, CMP_GREATER_EQUAL))
    {
        pos ++;
        ui32_shl(out_mod, 1);
    }
    if(pos == -1)
    {
        ui32_copy(out_mod, dest);
        ui32_clear(dest);
        return true;
    }
    ui32_shr(out_mod, 1);
    while(pos >= 0)
    {
        if(ui32_cmp(dest, out_mod, CMP_GREATER_EQUAL))
        {
            tmp.low = 1;
            tmp.high = 0;
            ui32_shl(&tmp, pos);
            ui32_add(&res, &tmp);
            ui32_sub(dest, out_mod);
        }
        ui32_shr(out_mod, 1);
        pos --;
    }
    ui32_copy(out_mod, dest);
    dest->low = res.low;
    dest->high = res.high;
    return true;
}

void ui32_shl(ui32_t * dest, uint8_t bits)
{
    for (size_t i = 0; i < bits; i++)
    {
        dest->high = (dest->high << 1) | (dest->low >> 15);
        dest->low = dest->low << 1;
    }
}

void ui32_shr(ui32_t * dest, uint8_t bits)
{
    for (size_t i = 0; i < bits; i++)
    {
        dest->low = (dest->low >> 1) | (dest->high << 15);
        dest->high = dest->high >> 1;
    }
}

void ui32_rol(ui32_t * dest, uint8_t bits)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < bits; i++)
    {
        carry = dest->low & 1;
        dest->low = (dest->low >> 1) | (dest->high << 15);
        dest->high = dest->high >> 1 | (carry << 15);
    }
}

void ui32_ror(ui32_t * dest, uint8_t bits)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < bits; i++)
    {
        carry = dest->high >> 15;
        dest->high = (dest->high << 1) | (dest->low >> 15);
        dest->low = dest->low << 1 | (carry);
    }
}