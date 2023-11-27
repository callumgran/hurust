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
    vector.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains the vector implementation, which is a dynamic array.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    25-Nov-23  C.Gran		Created file.

 *==========================================================================*/
#ifndef HURUST_VECTOR_H
#define HURUST_VECTOR_H

#include "../alloc.h"
#include "../common.h"
#include "../sort.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * \brief     A macro for defining an vector.
 * \note      This macro defines an vector with the given type and struct prefix.
 * \param[in] type The type of the vector.
 * \param[in] struct_prefix The prefix for the vector struct.
 * \note      The struct prefix parameter is used to define the vector struct
 *            name as some types such as pointers and vectors cannot be used
 *            as struct names.
 */
#define VECTOR(type, struct_prefix)           \
    typedef struct struct_prefix##_vector_t { \
        type *data;                           \
        size_t size;                          \
        size_t cap;                           \
        int (*cmp)(const type, const type);   \
        struct hr_allocator_t *allocator;     \
    } struct_prefix##_vector_t;

/**
 * \brief     A macro for initializing an vector.
 * \note      This macro initializes an vector with the given allocator,
 * 		  	  capacity and comparison function for sorting and searching.
 * \param[in] _vector The vector to initialize.
 * \param[in] _allocator The allocator to use for the vector.
 * \param[in] _cap The starting capacity of the vector.
 * \param[in] _cmp The comparison function for sorting and searching.
 */
#define vector_init(_vector, _allocator, _cap, _cmp)                                   \
    ({                                                                                 \
        (_vector)->allocator = (_allocator);                                           \
        (_vector)->cap = (_cap);                                                       \
        (_vector)->size = 0;                                                           \
        (_vector)->cmp = (_cmp);                                                       \
        (_vector)->data =                                                              \
            HR_ALLOC((_vector)->allocator, sizeof(*(_vector)->data) * (_vector)->cap); \
    })

/**
 * \brief     A macro for freeing an vector.
 * \note      This macro frees an vector using the allocator specified when
 * 		  	  initializing the vector.
 * \param[in] _vector The vector to free.
 */
#define vector_free(_vector) ({ HR_DEALLOC((_vector)->allocator, (_vector)->data); })

// Getters

/**
 * \brief     A macro for getting the size of an vector.
 * \note      This macro gets the size of an vector.
 * \param[in] _vector The vector to get the size of.
 */
#define vector_get_size(_vector) ({ (_vector)->size; })

/**
 * \brief     A macro for getting the capacity of an vector.
 * \note      This macro gets the capacity of an vector.
 * \param[in] _vector The vector to get the capacity of.
 */
#define vector_get_cap(_vector) ({ (_vector)->cap; })

/**
 * \brief     A macro for getting the data of an vector.
 * \note      This macro gets the data of an vector.
 * \param[in] _vector The vector to get the data of.
 */
#define vector_get_data(_vector) ({ (_vector)->data; })

/**
 * \brief     A macro for getting the comparison function of an vector.
 * \note      This macro gets the comparison function of an vector.
 * \param[in] _vector The vector to get the comparison function of.
 */
#define vector_get_cmp(_vector) ({ (_vector)->cmp; })

/**
 * \brief     A macro for getting the allocator of an vector.
 * \note      This macro gets the allocator of an vector.
 * \param[in] _vector The vector to get the allocator of.
 */
#define vector_get_allocator(_vector) ({ (_vector)->cmp; })

// Setters

/**
 * \brief     A macro for setting the size of an vector.
 * \note      This macro sets the size of an vector.
 * \param[in] _vector The vector to set the size of.
 * \param[in] _size The size to set the vector to.
 */
#define vector_set_size(_vector, _size) ({ (_vector)->size = (_size); })

/**
 * \brief     A macro for setting the capacity of an vector.
 * \note      This macro sets the capacity of an vector.
 * \param[in] _vector The vector to set the capacity of.
 * \param[in] _cap The capacity to set the vector to.
 */
#define vector_set_cap(_vector, _cap) ({ (_vector)->cap = (_cap); })

/**
 * \brief     A macro for setting the data of an vector.
 * \note      This macro sets the data of an vector.
 * \param[in] _vector The vector to set the data of.
 * \param[in] _data The data to set the vector to.
 */
#define vector_set_data(_vector, _data) ({ (_vector)->data = (_data); })

/**
 * \brief     A macro for setting the comparison function of an vector.
 * \note      This macro sets the comparison function of an vector.
 * \param[in] _vector The vector to set the comparison function of.
 * \param[in] _cmp The comparison function to set the vector to.
 */
#define vector_set_cmp(_vector, _cmp) ({ (_vector)->cmp = (_cmp); })

/**
 * \brief     A macro for setting the allocator of an vector.
 * \note      This macro sets the allocator of an vector.
 * \param[in] _vector The vector to set the allocator of.
 * \param[in] _allocator The allocator to set the vector to.
 */
#define vector_set_allocator(_vector, _allocator) ({ (_vector)->allocator = (_allocator); })

// Methods

/**
 * \brief     A macro for checking if an vector is empty.
 * \note      This macro checks if an vector is empty.
 * \param[in] _vector The vector to check.
 */
#define vector_empty(_vector) ({ (_vector)->size == 0; })

/**
 * \brief     A macro for popping an item from an vector.
 * \note      This macro pops an item from an vector.
 * \param[in] _vector The vector to pop the item from.
 * \param[in] _i The index of the item to pop.
 */
#define vector_pop(_vector, _i)                                     \
    ({                                                              \
        _typeofarray((_vector)->data) _ret = (_vector)->data[(_i)]; \
        (_vector)->size--;                                          \
        for (size_t _j = (_i); _j < (_vector)->size; _j++) {        \
            (_vector)->data[_j] = (_vector)->data[_j + 1];          \
        }                                                           \
        _reduce_cap(_vector);                                       \
        _ret;                                                       \
    })

/**
 * \brief     A macro for removing an item from an vector.
 * \note      This macro removes an item from an vector.
 * \param[in] _vector The vector to remove the item from.
 * \param[in] _item The item to remove.
 */
#define vector_remove(_vector, _item)                              \
    ({                                                             \
        size_t _i = 0;                                             \
        for (; _i < (_vector)->size; _i++)                         \
            if ((_vector)->cmp((_vector)->data[_i], (_item)) == 0) \
                break;                                             \
        vector_pop(_vector, _i);                                   \
    })

/**
 * \brief     A macro for getting an item from an vector.
 * \note      This macro gets an item from an vector.
 * \param[in] _vector The vector to get the item from.
 * \param[in] _i The index of the item to get.
 */
#define vector_get(_vector, _i) ({ (_vector)->data[(_i)]; })

/**
 * \brief     A macro for setting an item in an vector.
 * \note      This macro sets an item in an vector.
 * \param[in] _vector The vector to set the item in.
 * \param[in] _item The item to set in the vector.
 * \param[in] _i The index of the item to set.
 */
#define vector_set(_vector, _item, _i) ({ (_vector)->data[(_i)] = *(_item); })

/**
 * \brief     A macro for pushing an item to an vector.
 * \note      This macro pushes an item to an vector.
 * \param[in] _vector The vector to push the item to.
 * \param[in] _item The item to push to the vector.
 */
#define vector_push(_vector, _item)                  \
    ({                                               \
        _ensure_cap(_vector);                        \
        (_vector)->data[(_vector)->size] = *(_item); \
        (_vector)->size++;                           \
    })

/**
 * \brief     A macro for sorting an vector.
 * \note      This macro sorts an vector using the comparison function
 *            specified when initializing the vector.
 * \param[in] _vector The vector to sort.
 */
#define vector_sort(_vector) ({ sort((_vector)->data, (_vector)->size, (_vector)->cmp); })

/**
 * \brief     A macro for getting the maximum item in an vector.
 * \note      This macro gets the maximum item in an vector.
 * \param[in] _vector The vector to get the maximum item from.
 */
#define vector_max(_vector)                                      \
    ({                                                           \
        _typeofarray((_vector)->data) _ret = (_vector)->data[0]; \
        for (size_t _i = 1; _i < (_vector)->size; _i++) {        \
            if ((_vector)->cmp(_ret, (_vector)->data[_i]) < 0)   \
                _ret = (_vector)->data[_i];                      \
        }                                                        \
        _ret;                                                    \
    })

/**
 * \brief     A macro for getting the minimum item in an vector.
 * \note      This macro gets the minimum item in an vector.
 * \param[in] _vector The vector to get the minimum item from.
 */
#define vector_min(_vector)                                      \
    ({                                                           \
        _typeofarray((_vector)->data) _ret = (_vector)->data[0]; \
        for (size_t _i = 1; _i < (_vector)->size; _i++) {        \
            if ((_vector)->cmp(_ret, (_vector)->data[_i]) > 0)   \
                _ret = (_vector)->data[_i];                      \
        }                                                        \
        _ret;                                                    \
    })

/**
 * \brief     A macro for performing a for each loop on an vector.
 * \note      This macro performs a for each loop on an vector.
 * \param[in] _vector The vector to perform the for each loop on.
 * \param[in] _func The function to perform on each item in the vector.
 */
#define vector_foreach(_vector, _func)                    \
    ({                                                    \
        for (size_t _i = 0; _i < (_vector)->size; _i++) { \
            _func(&(_vector)->data[_i]);                  \
        }                                                 \
    })

#endif // HURUST_VECTOR_H
