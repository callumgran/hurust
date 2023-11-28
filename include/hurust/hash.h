/*
 *  Copyright (C) 2023 Callum Gran
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/*==========================================================================*

  FILE
    hash.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains functions for hashing data.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    27-Nov-23  C.Gran		Created file.

 *==========================================================================*/

#ifndef HURUST_HASH_H
#define HURUST_HASH_H

#include <stddef.h>
#include <stdint.h>

size_t hash_char(char key)
{
    return (size_t)key;
}

size_t hash_short(short key)
{
    return (size_t)key;
}

size_t hash_int(int key)
{
    return (size_t)key;
}

size_t hash_long(long key)
{
    return (size_t)key;
}

size_t hash_long_long(long long key)
{
    return (size_t)key;
}

size_t hash_unsigned_char(unsigned char key)
{
    return (size_t)key;
}

size_t hash_unsigned_short(unsigned short key)
{
    return (size_t)key;
}

size_t hash_unsigned_int(unsigned int key)
{
    return (size_t)key;
}

size_t hash_unsigned_long(unsigned long key)
{
    return (size_t)key;
}

size_t hash_unsigned_long_long(unsigned long long key)
{
    return (size_t)key;
}

size_t hash_float(float key)
{
    return *(size_t *)&key;
}

size_t hash_double(double key)
{
    return *(size_t *)&key;
}

size_t hash_long_double(long double key)
{
    return *(size_t *)&key;
}

size_t hash_str(char *str)
{
    size_t A = 1327217885;
    size_t k = 0;
    for (size_t i = 0; i < strlen(str); i++)
        k += (k << 5) + str[i];

    return k * A;
}

#endif // HURUST_HASH_H