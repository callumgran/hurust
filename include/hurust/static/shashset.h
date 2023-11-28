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
    shashtable.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains the fixed-size hashset data structure.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    27-Nov-23  C.Gran		Created file.

 *==========================================================================*/

#ifndef HURUST_STATIC_HASHSET_H
#define HURUST_STATIC_HASHSET_H

#include "../alloc.h"
#include "../common.h"
#include "../hash.h"
#include <stdbool.h>
#include <stdint.h>

/* Helper function for determining if a number is prime. */
static bool is_prime(size_t n)
{
    if (n <= 1) {
        return false;
    }

    if (n <= 3) {
        return true;
    }

    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }

    for (size_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

/**
 * \brief     A macro for defining a stack.
 * \note      This macro defines a stack with the given type and struct prefix.
 * \param[in] type The type of the stack.
 * \param[in] struct_prefix The prefix for the stack struct.
 * \note      The struct prefix parameter is used to define the stack struct
 *            name as some types such as pointers and arrays cannot be used
 *            as struct names.
 */
#define SHASHSET(type, struct_prefix)           \
    typedef struct struct_prefix##_shashset_t { \
        type *data;                             \
        size_t size;                            \
        size_t cap;                             \
        int (*cmp)(const type a, const type b); \
        size_t (*hash)(const type a);           \
        struct hr_allocator_t *allocator;       \
    } struct_prefix##_shashset_t;

#define shashset_init(_hashset, _allocator, cap, _cmp, _hash)                                 \
    ({                                                                                        \
        (_hashset)->allocator = _allocator;                                                   \
        (_hashset)->cap = cap;                                                                \
        (_hashset)->size = 0;                                                                 \
        (_hashset)->cmp = _cmp;                                                               \
        (_hashset)->hash = _hash;                                                             \
        (_hashset)->data = HR_ALLOC((_hashset)->_allocator, sizeof(*(_hashset)->data) * cap); \
    })

#define shashset_free(_hashset) HR_FREE((_hashset)->allocator, (_hashset)->data)

#define shashset_insert(_hashset, _item)                             \
    ({                                                               \
        size_t _hash = (_hashset)->hash(_item);                      \
        size_t _index = _hash % (_hashset)->cap;                     \
        size_t _i = _index;                                          \
        bool _success = true;                                        \
        while ((_hashset)->data[_i] != NULL_VAL(_item)) {            \
            if ((_hashset)->cmp((_hashset)->data[_i], _item) == 0) { \
                break;                                               \
            }                                                        \
            _i = (_i + 1) % (_hashset)->cap;                         \
            if (_i == _index) {                                      \
                _success = false;                                    \
                break;                                               \
            }                                                        \
        }                                                            \
        if (_success) {                                              \
            (_hashset)->data[_i] = _item;                            \
            (_hashset)->size++;                                      \
        }                                                            \
        _success;                                                    \
    })

#define shashset_remove(_hashset, _item)                             \
    ({                                                               \
        size_t _hash = (_hashset)->hash(_item);                      \
        size_t _index = _hash % (_hashset)->cap;                     \
        size_t _i = _index;                                          \
        bool _success = false;                                       \
        while ((_hashset)->data[_i] != NULL_VAL(_item)) {            \
            if ((_hashset)->cmp((_hashset)->data[_i], _item) == 0) { \
                (_hashset)->data[_i] = NULL_VAL(_item);              \
                (_hashset)->size--;                                  \
                _success = true;                                     \
                break;                                               \
            }                                                        \
            _i = (_i + 1) % (_hashset)->cap;                         \
            if (_i == _index) {                                      \
                break;                                               \
            }                                                        \
        }                                                            \
        _success;                                                    \
    })

#define shashset_contains(_hashset, _item)                           \
    ({                                                               \
        size_t _hash = (_hashset)->hash(_item);                      \
        size_t _index = _hash % (_hashset)->cap;                     \
        size_t _i = _index;                                          \
        bool _success = false;                                       \
        while ((_hashset)->data[_i] != NULL_VAL(_item)) {            \
            if ((_hashset)->cmp((_hashset)->data[_i], _item) == 0) { \
                _success = true;                                     \
                break;                                               \
            }                                                        \
            _i = (_i + 1) % (_hashset)->cap;                         \
            if (_i == _index) {                                      \
                break;                                               \
            }                                                        \
        }                                                            \
        _success;                                                    \
    })

#define shashset_clear(_hashset) \
    {                            \
        (_hashset)->size = 0;    \
    }

#define shashset_size(_hashset) ((_hashset)->size)

#define shashset_empty(_hashset) ((_hashset)->size == 0)

#define shashset_capacity(_hashset) ((_hashset)->cap)

#define shashset_load_factor(_hashset) ((float)(_hashset)->size / (_hashset)->cap)

#endif // HURUST_STATIC_HASHSET_H