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
    common.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains the common data used by the library as a whole.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    20-Nov-23  C.Gran		Created file.

 *==========================================================================*/

#ifndef HURUST_COMMON_H
#define HURUST_COMMON_H

#include <stddef.h>

#define swap(type, a, b)           \
    ({                             \
        register type _tmp = *(a); \
        *(a) = *(b);               \
        *(b) = _tmp;               \
    })

#define rotate(type, a, b, c)      \
    ({                             \
        register type _tmp = *(a); \
        *(a) = *(b);               \
        *(b) = *(c);               \
        *(c) = _tmp;               \
    })

#define _prim_min(type, a, b) ((a) < (b) ? (a) : (b))

#define _prim_max(type, a, b) ((a) > (b) ? (a) : (b))

#define _typeofarray(arr) typeof(*(arr))

/**
 * \brief       A macro ensuring that the given structure has enough capacity
 *              for another item.
 * \note        This macro doubles the capacity of the structure if it is full.
 * \param[in]   structure The structure to ensure capacity for.
 */
#define _ensure_cap(structure)                                                                    \
    {                                                                                             \
        if ((structure)->size == (structure)->cap - 1) {                                          \
            (structure)->cap <<= 1;                                                               \
            (structure)->data =                                                                   \
                (structure)->allocator->realloc((structure)->allocator->arena, (structure)->data, \
                                                (structure)->cap * sizeof(*(structure)->data));   \
        }                                                                                         \
    }

/**
 * \brief       A macro ensuring that the given structure doesn't hold too much
 *              unused capacity.
 * \note        This macro halves the capacity of the structure if it is less
 *              than a quarter full.
 * \param[in]   structure The structure to reduce capacity for.
 */
#define _reduce_cap(structure)                                                                    \
    {                                                                                             \
        if ((structure)->size < (structure)->cap >> 2) {                                          \
            (structure)->cap >>= 1;                                                               \
            (structure)->data =                                                                   \
                (structure)->allocator->realloc((structure)->allocator->arena, (structure)->data, \
                                                (structure)->cap * sizeof(*(structure)->data));   \
        }                                                                                         \
    }

#endif // HURUST_COMMON_H