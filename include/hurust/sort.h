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
    sort.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains macros used for sorting arrays using
    a simple stack based hoare quicksort.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    24-Nov-23  C.Gran		Created file.

 *==========================================================================*/

#ifndef HURUST_SORT_H
#define HURUST_SORT_H

#include <stddef.h>
#include <stdint.h>

#define INSERTION_SORT_THRESHOLD 27

#define _median_three(left, right, cmp)                       \
    ({                                                        \
        _type *_med_mid = (left) + (((right) - (left)) >> 1); \
        if ((cmp)(*_med_mid, *(left)) < 0) {                  \
            if ((cmp)(*(right), *(left)) < 0) {               \
                if ((cmp)(*(right), *_med_mid) < 0) {         \
                    swap(_type, (left), (right));             \
                } else {                                      \
                    rotate(_type, (left), _med_mid, (right)); \
                }                                             \
            } else {                                          \
                swap(_type, (left), _med_mid);                \
            }                                                 \
        } else {                                              \
            if ((cmp)(*(right), *_med_mid) < 0) {             \
                if ((cmp)(*(right), *(left)) < 0) {           \
                    rotate(_type, (right), _med_mid, (left)); \
                } else {                                      \
                    swap(_type, _med_mid, (right));           \
                }                                             \
            }                                                 \
        }                                                     \
        _med_mid;                                             \
    })

#define _partition(left, right, cmp)                         \
    ({                                                       \
        _type *_p_mid = _median_three((left), (right), cmp); \
        const _type _piv = *_p_mid;                          \
        _type *_l = (left) + 1;                              \
        _type *_r = (right);                                 \
        *_p_mid = *_l;                                       \
        *_l = _piv;                                          \
        swap(_type, _p_mid, (right)-1);                      \
        do {                                                 \
            do                                               \
                _l++;                                        \
            while ((cmp)(*_l, _piv) < 0);                    \
            do                                               \
                _r--;                                        \
            while ((cmp)(_piv, *_r) < 0);                    \
            if (_l >= _r)                                    \
                break;                                       \
            swap(_type, _l, _r);                             \
        } while (true);                                      \
        *((left) + 1) = *_r;                                 \
        *_r = _piv;                                          \
        _r;                                                  \
    })

#define sort(arr, size, cmp)                                      \
    ({                                                            \
        typedef _typeofarray((arr)) _type;                        \
        _type *_left = (arr);                                     \
        _type *_right = (arr) + (size)-1;                         \
        struct _qs_stack {                                        \
            _type *left;                                          \
            _type *right;                                         \
        } _stack[sizeof(size_t) * 8];                             \
        struct _qs_stack *_top = _stack;                          \
        _top->left = _left;                                       \
        _top->right = _right;                                     \
        _top++;                                                   \
        do {                                                      \
            if (_right - _left <= INSERTION_SORT_THRESHOLD) {     \
                for (_type *_i = _left + 1; _i <= _right; _i++)   \
                    if (cmp(*_i, *_left) < 0)                     \
                        swap(_type, _i, _left);                   \
                for (_type *_i = _left + 2; _i <= _right; _i++) { \
                    _type _high = *_i;                            \
                    _type *_j = _i - 1;                           \
                    for (; cmp(_high, *(_j)) < 0; _j--)           \
                        *(_j + 1) = *_j;                          \
                    *(_j + 1) = _high;                            \
                }                                                 \
                _top--;                                           \
                _left = _top->left;                               \
                _right = _top->right;                             \
            } else {                                              \
                _type *_mid = _partition(_left, _right, cmp);     \
                if (_mid - _left >= _right - _mid) {              \
                    _top->left = _left;                           \
                    _top->right = _mid - 1;                       \
                    _top++;                                       \
                    _left = _mid + 1;                             \
                } else {                                          \
                    _top->left = _mid + 1;                        \
                    _top->right = _right;                         \
                    _top++;                                       \
                    _right = _mid - 1;                            \
                }                                                 \
            }                                                     \
        } while (_top > _stack);                                  \
    })


#endif // HURUST_SORT_H