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
    heap.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains the binary heap implementation.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    27-Nov-23  C.Gran		Created file.

 *==========================================================================*/
#ifndef HURUST_HEAP_H
#define HURUST_HEAP_H

#include "../alloc.h"
#include "../common.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief     A macro for defining an heap.
 * \note      This macro defines an heap with the given type and struct prefix.
 * \param[in] type The type of the heap.
 * \param[in] struct_prefix The prefix for the heap struct.
 * \note      The struct prefix parameter is used to define the heap struct
 *            name as some types such as pointers and heaps cannot be used
 *            as struct names.
 */
#define HEAP(type, struct_prefix)           \
    typedef struct struct_prefix##_heap_t { \
        type *data;                         \
        size_t size;                        \
        size_t cap;                         \
        int (*cmp)(const type, const type); \
        struct hr_allocator_t *allocator;   \
    } struct_prefix##_heap_t;

/**
 * \brief     A macro for initializing a heap.
 * \note      This macro initializes a heap with the given allocator,
 * 		  	  capacity and comparison function for arranging the heap.
 * \param[in] _heap The heap to initialize.
 * \param[in] _allocator The allocator to use for the heap.
 * \param[in] _cap The starting capacity of the heap.
 * \param[in] _cmp The comparison function for sorting and searching.
 */
#define heap_init(_heap, _allocator, _cap, _cmp)                                             \
    ({                                                                                       \
        (_heap)->allocator = (_allocator);                                                   \
        (_heap)->cap = (_cap);                                                               \
        (_heap)->size = 0;                                                                   \
        (_heap)->cmp = (_cmp);                                                               \
        (_heap)->data = HR_ALLOC((_heap)->allocator, sizeof(*(_heap)->data) * (_heap)->cap); \
    })

/**
 * \brief     A macro for freeing a heap.
 * \note      This macro frees a heap using the allocator specified when
 * 		  	  initializing the heap.
 * \param[in] _heap The heap to free.
 */
#define heap_free(_heap) ({ HR_DEALLOC((_heap)->allocator, (_heap)->data); })

#define _heap_left_child_idx(_parent_idx) (((_parent_idx) << 1) + 1)
#define _heap_right_child_idx(_parent_idx) (((_parent_idx) + 1) << 1)
#define _heap_parent_idx(_child_idx) (((_child_idx)-1) >> 1)

#define _heap_has_left_child(_heap, _parent_idx) \
    (_heap_left_child_idx(_parent_idx) < (__ssize_t)(_heap)->size)

#define _heap_has_right_child(_heap, _parent_idx) \
    (_heap_right_child_idx(_parent_idx) < (__ssize_t)(_heap)->size)

#define heap_left_child(_heap, _parent_idx) ((_heap)->data[_heap_left_child_idx(_parent_idx)])

#define heap_right_child(_heap, _parent_idx) ((_heap)->data[_heap_right_child_idx(_parent_idx)])

#define heap_parent(_heap, _child_idx) ((_heap)->data[_heap_parent_idx(_child_idx)])

/**
 * \brief     Internal function for heapifying up a heap.
 * \note      This function heapifies up a heap.
 * \param[in] _heap The heap to heapify up.
 * \note      This function is internal and should not be used.
 */
#define _heapify_up(_heap)                                                            \
    ({                                                                                \
        __ssize_t _idx = (_heap)->size - 1;                                           \
        __ssize_t _parent_idx = _heap_parent_idx(_idx);                               \
        while (_parent_idx >= 0 &&                                                    \
               (_heap)->cmp(((_heap)->data[_idx]), (_heap)->data[_parent_idx]) < 0) { \
            swap(_typeofarray((_heap)->data), ((_heap)->data + _parent_idx),          \
                 ((_heap)->data + _idx));                                             \
            _idx = _parent_idx;                                                       \
            _parent_idx = _heap_parent_idx(_idx);                                     \
        }                                                                             \
    })

/**
 * \brief     Internal function for heapifying down a heap.
 * \note      This function heapifies down a heap.
 * \param[in] _heap The heap to heapify down.
 * \note      This function is internal and should not be used.
 */
#define _heapify_down(_heap)                                                                       \
    ({                                                                                             \
        __ssize_t _idx = 0;                                                                        \
        __ssize_t _min_idx;                                                                        \
        while (_heap_has_left_child(_heap, _idx)) {                                                \
            _min_idx = _heap_left_child_idx(_idx);                                                 \
            if (_heap_has_right_child(_heap, _idx) &&                                              \
                (_heap)->cmp(heap_right_child(_heap, _idx), heap_left_child(_heap, _idx)) < 0) {   \
                _min_idx = _heap_right_child_idx(_idx);                                            \
            }                                                                                      \
            if ((_heap)->cmp(((_heap)->data[_min_idx]), (_heap)->data[_idx]) > 0) {                \
                break;                                                                             \
            } else {                                                                               \
                swap(_typeofarray((_heap)->data), (_heap)->data + _idx, (_heap)->data + _min_idx); \
                _idx = _min_idx;                                                                   \
            }                                                                                      \
        }                                                                                          \
    })

// Getters

/**
 * \brief     A macro for getting the size of a heap.
 * \note      This macro gets the size of a heap.
 * \param[in] _heap The heap to get the size of.
 */
#define heap_get_size(_heap) ({ (_heap)->size; })

/**
 * \brief     A macro for getting the capacity of a heap.
 * \note      This macro gets the capacity of a heap.
 * \param[in] _heap The heap to get the capacity of.
 */
#define heap_get_cap(_heap) ({ (_heap)->cap; })

/**
 * \brief     A macro for getting the data of a heap.
 * \note      This macro gets the data of a heap.
 * \param[in] _heap The heap to get the data of.
 */
#define heap_get_data(_heap) ({ (_heap)->data; })

/**
 * \brief     A macro for getting the comparison function of a heap.
 * \note      This macro gets the comparison function of a heap.
 * \param[in] _heap The heap to get the comparison function of.
 */
#define heap_get_cmp(_heap) ({ (_heap)->cmp; })

/**
 * \brief     A macro for getting the allocator of a heap.
 * \note      This macro gets the allocator of a heap.
 * \param[in] _heap The heap to get the allocator of.
 */
#define heap_get_allocator(_heap) ({ (_heap)->cmp; })

// Setters

/**
 * \brief     A macro for setting the size of a heap.
 * \note      This macro sets the size of a heap.
 * \param[in] _heap The heap to set the size of.
 * \param[in] _size The size to set the heap to.
 */
#define heap_set_size(_heap, _size) ({ (_heap)->size = (_size); })

/**
 * \brief     A macro for setting the capacity of a heap.
 * \note      This macro sets the capacity of a heap.
 * \param[in] _heap The heap to set the capacity of.
 * \param[in] _cap The capacity to set the heap to.
 */
#define heap_set_cap(_heap, _cap) ({ (_heap)->cap = (_cap); })

/**
 * \brief     A macro for setting the data of a heap.
 * \note      This macro sets the data of a heap.
 * \param[in] _heap The heap to set the data of.
 * \param[in] _data The data to set the heap to.
 */
#define heap_set_data(_heap, _data) ({ (_heap)->data = (_data); })

/**
 * \brief     A macro for setting the comparison function of a heap.
 * \note      This macro sets the comparison function of a heap.
 * \param[in] _heap The heap to set the comparison function of.
 * \param[in] _cmp The comparison function to set the heap to.
 */
#define heap_set_cmp(_heap, _cmp) ({ (_heap)->cmp = (_cmp); })

/**
 * \brief     A macro for setting the allocator of a heap.
 * \note      This macro sets the allocator of a heap.
 * \param[in] _heap The heap to set the allocator of.
 * \param[in] _allocator The allocator to set the heap to.
 */
#define heap_set_allocator(_heap, _allocator) ({ (_heap)->allocator = (_allocator); })

/**
 * \brief     A macro for checking if a heap is empty.
 * \note      This macro checks if a heap is empty.
 * \param[in] _heap The heap to check.
 */
#define heap_empty(_heap) ({ (_heap)->size == 0; })

/**
 * \brief     A macro for pushing an item to a heap.
 * \note      This macro pushes an item to a heap,
 *            it also ensures the item is ish in the correct position.
 * \param[in] _heap The heap to push the item to.
 * \param[in] _item The item to push to the heap.
 */
#define heap_push(_heap, _item)                    \
    ({                                             \
        _ensure_cap(_heap);                        \
        (_heap)->data[(_heap)->size++] = *(_item); \
        _heapify_up(_heap);                        \
    })

/**
 * \brief     A macro for popping an item from a heap.
 * \note      This macro pops an item from a heap,
 *            it also ensures the next item is in the 
 *            correct position.
 * \param[in] _heap The heap to pop the item from.
 */
#define heap_pop(_heap)                                      \
    ({                                                       \
        _typeofarray((_heap)->data) _ret = (_heap)->data[0]; \
        (_heap)->data[0] = (_heap)->data[--(_heap)->size];   \
        _heapify_down(_heap);                                \
        _reduce_cap(_heap);                                  \
        _ret;                                                \
    })

/**
 * \brief     A macro for getting the item at the front of a heap.
 * \note      This macro gets the item at the front of a heap.
 * \note      This macro does not pop the item from the heap.
 * \param[in] _heap The heap to get the item from.
 */
#define heap_peek(_heap) ({ (_heap)->data[0]; })

#endif // HURUST_HEAP_H