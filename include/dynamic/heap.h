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

#define heap_init(_heap, _allocator, _cap, _cmp)                                             \
    ({                                                                                       \
        (_heap)->allocator = (_allocator);                                                   \
        (_heap)->cap = (_cap);                                                               \
        (_heap)->size = 0;                                                                   \
        (_heap)->cmp = (_cmp);                                                               \
        (_heap)->data = HR_ALLOC((_heap)->allocator, sizeof(*(_heap)->data) * (_heap)->cap); \
    })

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

#define heap_empty(_heap) ({ (_heap)->size == 0; })

#define heap_push(_heap, _item)                    \
    ({                                             \
        _ensure_cap(_heap);                        \
        (_heap)->data[(_heap)->size++] = *(_item); \
        _heapify_up(_heap);                        \
    })

#define heap_pop(_heap)                                      \
    ({                                                       \
        _typeofarray((_heap)->data) _ret = (_heap)->data[0]; \
        (_heap)->data[0] = (_heap)->data[--(_heap)->size];   \
        _heapify_down(_heap);                                \
        _reduce_cap(_heap);                                  \
        _ret;                                                \
    })

#define heap_peek(_heap) ({ (_heap)->data[0]; })

#endif // HURUST_HEAP_H