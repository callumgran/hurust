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
    dqueue.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains the dynamic queue data structure and macros for using it.
    The queue is a circular buffer, and is not thread safe.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    21-Nov-23  C.Gran		Created file.

 *==========================================================================*/
#ifndef HURUST_DYNAMIC_QUEUE_H
#define HURUST_DYNAMIC_QUEUE_H

#include "../alloc.h"
#include "../common.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief     A macro for defining a queue.
 * \note      This macro defines a queue with the given type and struct prefix.
 * \param[in] type The type of the queue.
 * \param[in] struct_prefix The prefix for the queue struct.
 * \note      The struct prefix parameter is used to define the queue struct
 *            name as some types such as pointers and arrays cannot be used
 *            as struct names.
 */
#define DQUEUE(type, struct_prefix)           \
    typedef struct struct_prefix##_dqueue_t { \
        type *data;                           \
        size_t start;                         \
        size_t end;                           \
        size_t size;                          \
        size_t cap;                           \
        struct hr_allocator_t *allocator;     \
    } struct_prefix##_dqueue_t;

/**
 * \brief     A macro for initializing a queue.
 * \note      This macro initializes a queue with the given allocator and
 *            capacity.
 * \param[in] _queue The queue to initialize.
 * \param[in] _allocator The allocator to use for the queue.
 * \param[in] _cap The starting capacity of the queue.
 */
#define dqueue_init(_queue, _allocator, _cap)                                                    \
    ({                                                                                           \
        (_queue)->allocator = (_allocator);                                                      \
        (_queue)->cap = (_cap);                                                                  \
        (_queue)->size = 0;                                                                      \
        (_queue)->start = 0;                                                                     \
        (_queue)->end = 0;                                                                       \
        (_queue)->data = HR_ALLOC((_queue)->allocator, sizeof(*(_queue)->data) * (_queue)->cap); \
    })

/**
 * \brief     A macro for freeing a queue.
 * \note      This macro frees a queue using the allocator specified when
 *            initializing the queue.
 * \param[in] _queue The queue to free.
 */
#define dqueue_free(_queue) ({ HR_DEALLOC((_queue)->allocator, (_queue)->data); })

// Getters

/**
 * \brief     A macro for getting the size of a queue.
 * \note      This macro gets the size of a queue.
 * \param[in] _queue The queue to get the size of.
 */
#define dqueue_get_size(_queue) ({ (_queue)->size; })

/**
 * \brief     A macro for getting the capacity of a queue.
 * \note      This macro gets the capacity of a queue.
 * \param[in] _queue The queue to get the capacity of.
 */
#define dqueue_get_cap(_queue) ({ (_queue)->cap; })

/**
 * \brief     A macro for getting the data of a queue.
 * \note      This macro gets the data of a queue.
 * \param[in] _queue The queue to get the data of.
 */
#define dqueue_get_data(_queue) ({ (_queue)->data; })

/**
 * \brief     A macro for getting the allocator of a queue.
 * \note      This macro gets the allocator of a queue.
 * \param[in] _queue The queue to get the allocator of.
 */
#define dqueue_get_allocator(_queue) ({ (_queue)->allocator; })

/**
 * \brief     A macro for getting the start of a queue.
 * \note      This macro gets the start of a queue.
 * \param[in] _queue The queue to get the start of.
 */
#define dqueue_get_start(_queue) ({ (_queue)->start; })

/**
 * \brief     A macro for getting the end of a queue.
 * \note      This macro gets the end of a queue.
 * \param[in] _queue The queue to get the end of.
 */
#define dqueue_get_end(_queue) ({ (_queue)->end; })

// Setters

/**
 * \brief     A macro for setting the size of a queue.
 * \note      This macro sets the size of a queue.
 * \param[in] _queue The queue to set the size of.
 * \param[in] _size The size to set the queue to.
 */
#define dqueue_set_size(_queue, _size) ({ (_queue)->size = (_size); })

/**
 * \brief     A macro for setting the capacity of a queue.
 * \note      This macro sets the capacity of a queue.
 * \param[in] _queue The queue to set the capacity of.
 * \param[in] _cap The capacity to set the queue to.
 */
#define dqueue_set_cap(_queue, _cap) ({ (_queue)->cap = (_cap); })

/**
 * \brief     A macro for setting the data of a queue.
 * \note      This macro sets the data of a queue.
 * \param[in] _queue The queue to set the data of.
 * \param[in] _data The data to set the queue to.
 */
#define dqueue_set_data(_queue, _data) ({ (_queue)->data = (_data); })

/**
 * \brief     A macro for setting the allocator of a queue.
 * \note      This macro sets the allocator of a queue.
 * \param[in] _queue The queue to set the allocator of.
 * \param[in] _allocator The allocator to set the queue to.
 */
#define dqueue_set_allocator(_queue, _allocator) ({ (_queue)->allocator = (_allocator); })

/**
 * \brief     A macro for setting the start of a queue.
 * \note      This macro sets the start of a queue.
 * \param[in] _queue The queue to set the start of.
 * \param[in] _start The start to set the queue to.
 */
#define dqueue_set_start(_queue, _start) ({ (_queue)->start = (_start); })

/**
 * \brief     A macro for setting the end of a queue.
 * \note      This macro sets the end of a queue.
 * \param[in] _queue The queue to set the end of.
 * \param[in] _end The end to set the queue to.
 */
#define dqueue_set_end(_queue, _end) ({ (_queue)->end = (_end); })

/**
 * \brief     A macro for checking if a queue is empty.
 * \note      This macro checks if a queue is empty.
 * \param[in] _queue The queue to check.
 */
#define dqueue_empty(_queue) ({ (_queue)->size == 0; })

/**
 * \brief     A macro for popping an item from a queue.
 * \note      This macro pops an item from a queue and stores it in the
 *            specified variable. The item that stored in the variable is
 *            not copied. The item should also not be a pointer to the data
 *            type of the queue, but the data type itself. E.g. if the queue
 *            is a queue of integers, the item should be an integer, not a
 *            pointer to an integer.
 * \param[in] _queue The queue to pop from.
 * \param[in] _ret The variable to store the popped item in.
 */
#define dqueue_pop(_queue, _ret)                                 \
    ({                                                           \
        *(_ret) = (_queue)->data[(_queue)->start];               \
        (_queue)->start = ((_queue)->start + 1) % (_queue)->cap; \
        (_queue)->size--;                                        \
    })

/**
 * \brief     A macro for pushing an item to a queue.
 * \note      This macro pushes an item to a queue.
 * \param[in] _queue The queue to push to.
 * \param[in] _item The item to push.
 */
#define dqueue_push(_queue, _item)                                                \
    ({                                                                            \
        if ((_queue)->size == (_queue)->cap) {                                    \
            (_queue)->cap *= 2;                                                   \
            (_queue)->data = HR_REALLOC((_queue)->allocator, (_queue)->data,      \
                                        sizeof(*(_queue)->data) * (_queue)->cap); \
            if ((_queue)->start >= (_queue)->end) {                               \
                size_t i = 0;                                                     \
                for (i = 0; i < (_queue)->end; i++) {                             \
                    (_queue)->data[i + (_queue)->size] = (_queue)->data[i];       \
                }                                                                 \
                (_queue)->end += (_queue)->size;                                  \
            }                                                                     \
        }                                                                         \
        (_queue)->data[(_queue)->end] = *(_item);                                 \
        (_queue)->end = ((_queue)->end + 1) % (_queue)->cap;                      \
        (_queue)->size++;                                                         \
    })

/**
 * \brief     A macro for getting the item at the front of a queue.
 * \note      This macro gets an item from a queue and stores it in the
 *            specified variable. The item that stored in the variable is
 *            not copied. The item should also not be a pointer to the data
 *            type of the queue, but the data type itself. E.g. if the queue
 *            is a queue of integers, the item should be an integer, not a
 *            pointer to an integer.
 * \param[in] _queue The queue to pop from.
 * \param[in] _ret The variable to store the popped item in.
 */
#define dqueue_peek(_queue, _ret) ({ *(_ret) = (_queue)->data[(_queue)->start]; })

#endif // HURUST_DYNAMIC_QUEUE_H
