#include "osdevkit.h"

bool ui64_cmp(ui64_t * val1, ui64_t * val2, uint8_t cmp)
{
    if(val1->hhigh == val2->hhigh)
    {
        if(val1->hlow == val2->hlow)
        {
            if(val1->lhigh == val2->lhigh)
            {
                if(val1->llow == val2->llow)
                {
                    return cmp == CMP_EQUAL || cmp == CMP_GREATER_EQUAL || cmp == CMP_LESSER_EQUAL;
                }
                if(val1->llow > val2->llow)
                {
                    return cmp == CMP_GREATER || cmp == CMP_GREATER_EQUAL || cmp == CMP_NOT_EQUAL;
                }
                if(val1->llow < val2->llow)
                {
                    return cmp == CMP_LESSER || cmp == CMP_LESSER_EQUAL || cmp == CMP_NOT_EQUAL;
                }
            }
            else if(val1->lhigh > val2->lhigh)
            {
                return cmp == CMP_GREATER || cmp == CMP_GREATER_EQUAL || cmp == CMP_NOT_EQUAL;
            }
            else
            {
                return cmp == CMP_LESSER || cmp == CMP_LESSER_EQUAL || cmp == CMP_NOT_EQUAL;
            }
        }
        else if(val1->hlow > val2->hlow)
        {
            return cmp == CMP_GREATER || cmp == CMP_GREATER_EQUAL || cmp == CMP_NOT_EQUAL;
        }
        else
        {
            return cmp == CMP_LESSER || cmp == CMP_LESSER_EQUAL || cmp == CMP_NOT_EQUAL;
        }
    }
    else if(val1->hhigh > val2->hhigh)
    {
        return cmp == CMP_GREATER || cmp == CMP_GREATER_EQUAL || cmp == CMP_NOT_EQUAL;
    }
    else
    {
        return cmp == CMP_LESSER || cmp == CMP_LESSER_EQUAL || cmp == CMP_NOT_EQUAL;
    }
}

void ui64_copy(ui64_t * dest, ui64_t * orig)
{
    dest->llow = orig->llow;
    dest->lhigh = orig->lhigh;
    dest->hlow = orig->hlow;
    dest->hhigh = orig->hhigh;
}

void ui64_clear(ui64_t * value)
{
    value->llow = 0;
    value->lhigh = 0;
    value->hlow = 0;
    value->hhigh = 0;
}

void ui64_parse(ui64_t * dest, char * orig)
{
    ui64_t val10;
    ui64_t valc;
    val10.llow = 10;
    val10.lhigh = 0;
    val10.hlow = 0;
    val10.hhigh = 0;
    valc.lhigh = 0;
    valc.hlow = 0;
    valc.hhigh = 0;
    uint16_t len = strlen(orig);
    ui64_clear(dest);
    for (size_t i = 0; i < len; i++)
    {
        if(!isdigit(orig[i])) return;
        valc.llow = orig[i] - '0';
        ui64_mul(dest, &val10);
        ui64_add(dest, &valc);
    }
}

void ui64_tostring(char * dest, ui64_t * orig, uint16_t len)
{
    ui64_t val, mod, div10;
    div10.llow = 10;
    div10.lhigh = 0;
    div10.hlow = 0;
    div10.hhigh = 0;
    int16_t i = 0;
    int16_t j = 0;
    char out[32];
    memset(out, 0, 32);
    ui64_copy(&val, orig);
    while(val.llow > 0 || val.lhigh > 0 || val.hlow > 0 || val.hhigh > 0)
    {
        ui64_div(&val, &mod, &div10);
        out[i++] = mod.llow + '0';
    }
    dest[j] = 0;
    while(i >= 0)
    {
        dest[j++] = out[--i];
        dest[j] = 0;
    }
}

void ui64_add(ui64_t * dest, ui64_t * orig)
{
    uint16_t carry0, carry1, carry2;
    carry0 = ((UINT16_MAX - dest->llow) < (orig->llow)) ? 1 : 0;
    dest->llow += orig->llow;
    carry1 = ((UINT16_MAX - dest->lhigh) < (orig->lhigh + carry0)) ? 1 : 0;
    dest->lhigh += orig->lhigh + carry0;
    carry2 = ((UINT16_MAX - dest->hlow) < (orig->hlow + carry1)) ? 1 : 0;
    dest->hlow += orig->hlow + carry1;
    dest->hhigh += orig->hhigh + carry2;
}

void ui64_sub(ui64_t * dest, ui64_t * orig)
{
    uint16_t carry0, carry1, carry2;
    carry0 = (dest->llow < orig->llow) ? 1 : 0;
    dest->llow -= orig->llow;
    carry1 = ((dest->lhigh) < (orig->lhigh + carry0)) ? 1 : 0;
    dest->lhigh -= orig->lhigh + carry0;
    carry2 = (( dest->hlow + carry0) < (orig->hlow +carry1)) ? 1 : 0;
    dest->hlow -= orig->hlow + carry1;
    dest->hhigh -= orig->hhigh + carry2;
}

void ui64_mul(ui64_t * dest, ui64_t * orig)
{
    ui64_t res;
    ui64_t tmp;
    uint16_t bit;
    ui64_clear(&res);
    for (size_t i = 0; i < 64; i++)
    {
        if(i & 0x30)
        {
            bit = orig->hhigh & (1 << (i & 0x0f));
        }
        else if(i & 0x20)
        {
            bit = orig->hlow & (1 << (i & 0x0f));
        }
        else if(i & 0x10)
        {
            bit = orig->lhigh & (1 << (i & 0x0f));
        }
        else
        {
            bit = orig->llow & (1 << i);
        }
        if(bit > 0)
        {
            tmp.llow = dest->llow;
            tmp.lhigh = dest->lhigh;
            tmp.hlow = dest->hlow;
            tmp.hhigh = dest->hhigh;
            ui64_shl(&tmp, i);
            ui64_add(&res, &tmp);
        }
    }
    dest->llow = res.llow;
    dest->lhigh = res.lhigh;
    dest->hlow = res.hlow;
    dest->hhigh = res.hhigh;
}

bool ui64_div(ui64_t * dest, ui64_t * out_mod, ui64_t * orig)
{
    ui64_t res;
    ui64_t tmp;
    if(orig->lhigh == 0 && orig->llow == 0 && orig->hhigh == 0 && orig->hlow == 0) return false;
    ui64_copy(out_mod, orig);
    ui64_clear(&res);
    int16_t pos;
    pos = (int16_t)-1;
    while(ui64_cmp(dest, out_mod, CMP_GREATER_EQUAL))
    {
        pos ++;
        ui64_shl(out_mod, 1);
    }
    if(pos == -1)
    {
        ui64_copy(out_mod, dest);
        ui64_clear(dest);
        return true;
    }
    ui64_shr(out_mod, 1);
    while(pos >= 0)
    {
        if(ui64_cmp(dest, out_mod, CMP_GREATER_EQUAL))
        {
            tmp.llow = 1;
            tmp.lhigh = 0;
            tmp.hlow = 0;
            tmp.hhigh = 0;
            ui64_shl(&tmp, pos);
            ui64_add(&res, &tmp);
            ui64_sub(dest, out_mod);
        }
        ui64_shr(out_mod, 1);
        pos --;
    }
    ui64_copy(out_mod, dest);
    dest->llow = res.llow;
    dest->lhigh = res.lhigh;
    dest->hlow = res.hlow;
    dest->hhigh = res.hhigh;
    return true;
}

void ui64_shl(ui64_t * dest, uint8_t bits)
{
    for (size_t i = 0; i < bits; i++)
    {
        dest->hhigh = (dest->hhigh << 1) | (dest->hlow >> 15);
        dest->hlow = (dest->hlow << 1) | (dest->lhigh >> 15);
        dest->lhigh = (dest->lhigh << 1) | (dest->llow >> 15);
        dest->llow = dest->llow << 1;
    }
}

void ui64_shr(ui64_t * dest, uint8_t bits)
{
    for (size_t i = 0; i < bits; i++)
    {
        dest->llow = (dest->llow >> 1) | (dest->lhigh << 15);
        dest->lhigh = (dest->lhigh >> 1) | (dest->hlow << 15);
        dest->hlow = (dest->hlow >> 1) | (dest->hhigh << 15);
        dest->hhigh = dest->hhigh >> 1;
    }
}

void ui64_rol(ui64_t * dest, uint8_t bits)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < bits; i++)
    {
        carry = dest->llow & 1;
        dest->llow = (dest->llow >> 1) | (dest->lhigh << 15);
        dest->lhigh = (dest->lhigh >> 1) | (dest->hlow << 15);
        dest->hlow = (dest->hlow >> 1) | (dest->hhigh << 15);
        dest->hhigh = dest->hhigh >> 1 | (carry << 15);
    }
}

void ui64_ror(ui64_t * dest, uint8_t bits)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < bits; i++)
    {
        carry = dest->hhigh >> 15;
        dest->hhigh = (dest->hhigh << 1) | (dest->hlow >> 15);
        dest->hlow = (dest->hlow << 1) | (dest->lhigh >> 15);
        dest->lhigh = (dest->lhigh << 1) | (dest->llow >> 15);
        dest->llow = dest->llow << 1 | (carry);
    }
}