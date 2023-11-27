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
    dstack.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains the dynamic stack data structure and macros for using it.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    27-Nov-23  C.Gran		Created file.

 *==========================================================================*/
#ifndef HURUST_DYNAMIC_STACK_H
#define HURUST_DYNAMIC_STACK_H

#include "../alloc.h"
#include "../common.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief     A macro for defining a stack.
 * \note      This macro defines a stack with the given type and struct prefix.
 * \param[in] type The type of the stack.
 * \param[in] struct_prefix The prefix for the stack struct.
 * \note      The struct prefix parameter is used to define the stack struct
 *            name as some types such as pointers and arrays cannot be used
 *            as struct names.
 */
#define DSTACK(type, struct_prefix)           \
    typedef struct struct_prefix##_dstack_t { \
        type *data;                           \
        size_t size;                          \
        size_t cap;                           \
        struct hr_allocator_t *allocator;     \
    } struct_prefix##_dstack_t;

/**
 * \brief     A macro for initializing a stack.
 * \note      This macro initializes a stack with the given allocator and
 *            capacity.
 * \param[in] _stack The stack to initialize.
 * \param[in] _allocator The allocator to use for the stack.
 * \param[in] _cap The starting capacity of the stack.
 */
#define dstack_init(_stack, _allocator, _cap)                                                    \
    ({                                                                                           \
        (_stack)->allocator = (_allocator);                                                      \
        (_stack)->cap = (_cap);                                                                  \
        (_stack)->size = 0;                                                                      \
        (_stack)->data = HR_ALLOC((_stack)->allocator, sizeof(*(_stack)->data) * (_stack)->cap); \
    })

/**
 * \brief     A macro for freeing a stack.
 * \note      This macro frees a stack using the allocator specified when
 *            initializing the stack.
 * \param[in] _stack The stack to free.
 */
#define dstack_free(_stack) ({ HR_DEALLOC((_stack)->allocator, (_stack)->data); })

// Getters

/**
 * \brief     A macro for getting the size of a stack.
 * \note      This macro gets the size of a stack.
 * \param[in] _stack The stack to get the size of.
 */
#define dstack_get_size(_stack) ({ (_stack)->size; })

/**
 * \brief     A macro for getting the capacity of a stack.
 * \note      This macro gets the capacity of a stack.
 * \param[in] _stack The stack to get the capacity of.
 */
#define dstack_get_cap(_stack) ({ (_stack)->cap; })

/**
 * \brief     A macro for getting the data of a stack.
 * \note      This macro gets the data of a stack.
 * \param[in] _stack The stack to get the data of.
 */
#define dstack_get_data(_stack) ({ (_stack)->data; })

/**
 * \brief     A macro for getting the allocator of a stack.
 * \note      This macro gets the allocator of a stack.
 * \param[in] _stack The stack to get the allocator of.
 */
#define dstack_get_allocator(_stack) ({ (_stack)->allocator; })

// Setters

/**
 * \brief     A macro for setting the size of a stack.
 * \note      This macro sets the size of a stack.
 * \param[in] _stack The stack to set the size of.
 * \param[in] _size The size to set the stack to.
 */
#define dstack_set_size(_stack, _size) ({ (_stack)->size = (_size); })

/**
 * \brief     A macro for setting the capacity of a stack.
 * \note      This macro sets the capacity of a stack.
 * \param[in] _stack The stack to set the capacity of.
 * \param[in] _cap The capacity to set the stack to.
 */
#define dstack_set_cap(_stack, _cap) ({ (_stack)->cap = (_cap); })

/**
 * \brief     A macro for setting the data of a stack.
 * \note      This macro sets the data of a stack.
 * \param[in] _stack The stack to set the data of.
 * \param[in] _data The data to set the stack to.
 */
#define dstack_set_data(_stack, _data) ({ (_stack)->data = (_data); })

/**
 * \brief     A macro for setting the allocator of a stack.
 * \note      This macro sets the allocator of a stack.
 * \param[in] _stack The stack to set the allocator of.
 * \param[in] _allocator The allocator to set the stack to.
 */
#define dstack_set_allocator(_stack, _allocator) ({ (_stack)->allocator = (_allocator); })

/**
 * \brief     A macro for checking if a stack is empty.
 * \note      This macro checks if a stack is empty.
 * \param[in] _stack The stack to check.
 */
#define dstack_empty(_stack) ({ (_stack)->size == 0; })

/**
 * \brief     A macro for popping an item from a stack.
 * \note      This macro pops an item from a stack and stores it in the
 *            specified variable. The item that stored in the variable is
 *            not copied. The item should also not be a pointer to the data
 *            type of the stack, but the data type itself. E.g. if the stack
 *            is a stack of integers, the item should be an integer, not a
 *            pointer to an integer.
 * \param[in] _stack The stack to pop from.
 */
#define dstack_pop(_stack)                                                  \
    ({                                                                      \
        (_stack)->size--;                                                   \
        _typeofarray((_stack)->data) _ret = (_stack)->data[(_stack)->size]; \
        _reduce_cap((_stack));                                              \
        _ret;                                                               \
    })

/**
 * \brief     A macro for pushing an item to a stack.
 * \note      This macro pushes an item to a stack.
 * \param[in] _stack The stack to push to.
 * \param[in] _item The item to push.
 */
#define dstack_push(_stack, _item)                 \
    ({                                             \
        _ensure_cap((_stack));                     \
        (_stack)->data[(_stack)->size] = *(_item); \
        (_stack)->size++;                          \
    })

/**
 * \brief     A macro for getting the item at the front of a stack.
 * \note      This macro gets an item from a stack and stores it in the
 *            specified variable. The item that stored in the variable is
 *            not copied. The item should also not be a pointer to the data
 *            type of the stack, but the data type itself. E.g. if the stack
 *            is a stack of integers, the item should be an integer, not a
 *            pointer to an integer.
 * \param[in] _stack The stack to pop from.
 */
#define dstack_peek(_stack) ({ (_stack)->data[(_stack)->size - 1]; })

#endif // HURUST_DYNAMIC_QUEUE_H
