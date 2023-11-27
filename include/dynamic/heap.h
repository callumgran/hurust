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
    This file contains the heap implementation, which is a priority queue.

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

#define heap_init(_heap, _allocator, _cap, _cmp)                                       \
    ({                                                                                 \
        (_vector)->allocator = (_allocator);                                           \
        (_vector)->cap = (_cap);                                                       \
        (_vector)->size = 0;                                                           \
        (_vector)->cmp = (_cmp);                                                       \
        (_vector)->data =                                                              \
            HR_ALLOC((_vector)->allocator, sizeof(*(_vector)->data) * (_vector)->cap); \
    })

#define heap_free(_heap) ({ HR_DEALLOC((_vector)->allocator, (_vector)->data); })

#define _heap_left_child_idx(_parent_idx) (((_parent_idx) << 1) + 1)
#define _heap_right_child_idx(_parent_idx) (((_parent_idx) + 1) << 1)
#define _heap_parent_idx(_child_idx) (((_child_idx)-1) >> 1)

#define _heap_has_left_child(_heap, _parent_idx) (_heap_left_child_idx(_parent_idx) < (_heap)->size)

#define _heap_has_right_child(_heap, _parent_idx) \
    (_heap_right_child_idx(_parent_idx) < (_heap)->size)

#define heap_left_child(_heap, _parent_idx) ((_heap)->data[_heap_left_child_idx(_parent_idx)])

#define heap_right_child(_heap, _parent_idx) ((_heap)->data[_heap_right_child_idx(_parent_idx)])

#define heap_parent(_heap, _child_idx) ((_heap)->data[_heap_parent_idx(_child_idx)])

#define _heapify_up(_heap)                                                                  \
    ({                                                                                      \
        size_t _idx = (_heap)->size - 1;                                                    \
        size_t _parent_idx = _heap_parent_idx(_idx);                                        \
        while (_parent_idx > 0 &&                                                           \
               _heap->cmp(heap_parent(_heap, _idx), heap_parent(_heap, _parent_idx)) < 0) { \
            swap(heap_parent(_heap, _idx), heap_parent(_heap, _parent_idx));                \
            _idx = _parent_idx;                                                             \
            _parent_idx = _heap_parent_idx(_idx);                                           \
        }                                                                                   \
    })


#define _heapify_down(_heap)                                                                   \
    ({                                                                                         \
        size_t _idx = 0;                                                                       \
        int _min_idx;                                                                          \
        while (_heap_has_left_child(_heap, _idx)) {                                            \
            _min_idx = _heap_left_child_idx(_idx);                                             \
            if (_heap_has_right_child(_heap, _idx) &&                                          \
                _heap->cmp(heap_right_child(_heap, _idx), heap_left_child(_heap, _idx)) < 0) { \
                _min_idx = _heap_right_child_idx(_idx);                                        \
            }                                                                                  \
            if (_heap->cmp(heap_parent(_heap, _min_idx), heap_parent(_heap, _idx)) < 0) {      \
                break;                                                                         \
            } else {                                                                           \
                swap(heap_parent(_heap, _idx), heap_parent(_heap, _min_idx));                  \
                _idx = _min_idx;                                                               \
            }                                                                                  \
        }                                                                                      \
    })

#endif // HURUST_HEAP_H