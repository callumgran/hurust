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
	array.h

  PROJECT
	hurust generic library

  DESCRIPTION
	This file contains the array structure and methods. The array structure
	is an expanded version of the C-array, with a few extra features such
	as keeping track of size and being able to sort itself.

  PROGRAMMER
	Callum Gran.

  MODIFICATIONS
	21-Nov-23  C.Gran		Created file.

 *==========================================================================*/
#ifndef HURUST_ARRAY_H
#define HURUST_ARRAY_H

#include "../alloc.h"
#include "../common.h"
#include "../sort.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * \brief     A macro for defining an array.
 * \note      This macro defines an array with the given type and struct prefix.
 * \param[in] type The type of the array.
 * \param[in] struct_prefix The prefix for the array struct.
 * \note      The struct prefix parameter is used to define the array struct
 *            name as some types such as pointers and arrays cannot be used
 *            as struct names.
 */
#define ARRAY(type, struct_prefix)           \
	typedef struct struct_prefix##_array_t { \
		type *data;                          \
		size_t size;                         \
		size_t cap;                          \
		int (*cmp)(const type, const type);  \
		struct hr_allocator_t *allocator;    \
	} struct_prefix##_array_t;

/**
 * \brief     A macro for initializing an array.
 * \note      This macro initializes an array with the given allocator,
 * 		  	  capacity and comparison function for sorting and searching.
 * \param[in] _array The array to initialize.
 * \param[in] _allocator The allocator to use for the array.
 * \param[in] _cap The starting capacity of the array.
 * \param[in] _cmp The comparison function for sorting and searching.
 */
#define array_init(_array, _allocator, _cap, _cmp)                                               \
	({                                                                                           \
		(_array)->allocator = (_allocator);                                                      \
		(_array)->cap = (_cap);                                                                  \
		(_array)->size = 0;                                                                      \
		(_array)->cmp = (_cmp);                                                                  \
		(_array)->data = HR_ALLOC((_array)->allocator, sizeof(*(_array)->data) * (_array)->cap); \
	})

/**
 * \brief     A macro for freeing an array.
 * \note      This macro frees an array using the allocator specified when
 * 		  	  initializing the array.
 * \param[in] _array The array to free.
 */
#define array_free(_array) ({ HR_DEALLOC((_array)->allocator, (_array)->data); })

// Getters

/**
 * \brief     A macro for getting the size of an array.
 * \note      This macro gets the size of an array.
 * \param[in] _array The array to get the size of.
 */
#define array_get_size(_array) ({ (_array)->size; })

/**
 * \brief     A macro for getting the capacity of an array.
 * \note      This macro gets the capacity of an array.
 * \param[in] _array The array to get the capacity of.
 */
#define array_get_cap(_array) ({ (_array)->cap; })

/**
 * \brief     A macro for getting the data of an array.
 * \note      This macro gets the data of an array.
 * \param[in] _array The array to get the data of.
 */
#define array_get_data(_array) ({ (_array)->data; })

/**
 * \brief     A macro for getting the comparison function of an array.
 * \note      This macro gets the comparison function of an array.
 * \param[in] _array The array to get the comparison function of.
 */
#define array_get_cmp(_array) ({ (_array)->cmp; })

/**
 * \brief     A macro for getting the allocator of an array.
 * \note      This macro gets the allocator of an array.
 * \param[in] _array The array to get the allocator of.
 */
#define array_get_allocator(_array) ({ (_array)->cmp; })

// Setters

/**
 * \brief     A macro for setting the size of an array.
 * \note      This macro sets the size of an array.
 * \param[in] _array The array to set the size of.
 * \param[in] _size The size to set the array to.
 */
#define array_set_size(_array, _size) ({ (_array)->size = (_size); })

/**
 * \brief     A macro for setting the capacity of an array.
 * \note      This macro sets the capacity of an array.
 * \param[in] _array The array to set the capacity of.
 * \param[in] _cap The capacity to set the array to.
 */
#define array_set_cap(_array, _cap) ({ (_array)->cap = (_cap); })

/**
 * \brief     A macro for setting the data of an array.
 * \note      This macro sets the data of an array.
 * \param[in] _array The array to set the data of.
 * \param[in] _data The data to set the array to.
 */
#define array_set_data(_array, _data) ({ (_array)->data = (_data); })

/**
 * \brief     A macro for setting the comparison function of an array.
 * \note      This macro sets the comparison function of an array.
 * \param[in] _array The array to set the comparison function of.
 * \param[in] _cmp The comparison function to set the array to.
 */
#define array_set_cmp(_array, _cmp) ({ (_array)->cmp = (_cmp); })

/**
 * \brief     A macro for setting the allocator of an array.
 * \note      This macro sets the allocator of an array.
 * \param[in] _array The array to set the allocator of.
 * \param[in] _allocator The allocator to set the array to.
 */
#define array_set_allocator(_array, _allocator) ({ (_array)->allocator = (_allocator); })

// Methods

/**
 * \brief     A macro for checking if an array is full.
 * \note      This macro checks if an array is full.
 * \param[in] _array The array to check.
 */
#define array_full(_array) ({ (_array)->size == (_array)->cap; })

/**
 * \brief     A macro for checking if an array is empty.
 * \note      This macro checks if an array is empty.
 * \param[in] _array The array to check.
 */
#define array_empty(_array) ({ (_array)->size == 0; })

/**
 * \brief     A macro for popping an item from an array.
 * \note      This macro pops an item from an array.
 * \param[in] _array The array to pop the item from.
 * \param[in] _i The index of the item to pop.
 * \param[in] _ret The item to pop from the array.
 */
#define array_pop(_array, _i, _ret)                         \
	({                                                      \
		*(_ret) = (_array)->data[(_i)];                     \
		(_array)->size--;                                   \
		for (size_t _j = (_i); _j < (_array)->size; _j++) { \
			(_array)->data[_j] = (_array)->data[_j + 1];    \
		}                                                   \
	})

/**
 * \brief     A macro for removing an item from an array.
 * \note      This macro removes an item from an array.
 * \param[in] _array The array to remove the item from.
 * \param[in] _ret The item to remove from the array.
 * \param[in] _item The item to remove.
 */
#define array_remove(_array, _ret, _item)                        \
	({                                                           \
		size_t _i = 0;                                           \
		for (; _i < (_array)->size; _i++)                        \
			if ((_array)->cmp((_array)->data[_i], (_item)) == 0) \
				break;                                           \
		array_pop(_array, _ret, _i);                             \
	})

/**
 * \brief     A macro for getting an item from an array.
 * \note      This macro gets an item from an array.
 * \param[in] _array The array to get the item from.
 * \param[in] _i The index of the item to get.
 * \param[in] _ret The item to get from the array.
 */
#define array_get(_array, _i, _ret) ({ *(_ret) = (_array)->data[(_i)]; })

/**
 * \brief     A macro for setting an item in an array.
 * \note      This macro sets an item in an array.
 * \param[in] _array The array to set the item in.
 * \param[in] _item The item to set in the array.
 * \param[in] _i The index of the item to set.
 */
#define array_set(_array, _item, _i) ({ (_array)->data[(_i)] = *(_item); })

/**
 * \brief     A macro for pushing an item to an array.
 * \note      This macro pushes an item to an array.
 * \param[in] _array The array to push the item to.
 * \param[in] _item The item to push to the array.
 */
#define array_push(_array, _item)                  \
	({                                             \
		(_array)->data[(_array)->size] = *(_item); \
		(_array)->size++;                          \
	})

/**
 * \brief     A macro for sorting an array.
 * \note      This macro sorts an array using the comparison function
 *            specified when initializing the array.
 * \param[in] _array The array to sort.
 */
#define array_sort(_array) ({ sort((_array)->data, (_array)->size, (_array)->cmp); })

/**
 * \brief     A macro for getting the maximum item in an array.
 * \note      This macro gets the maximum item in an array.
 * \param[in] _array The array to get the maximum item from.
 * \param[in] _ret The maximum item in the array.
 */
#define array_max(_array, _ret)                                 \
	({                                                          \
		*(_ret) = (_array)->data[0];                            \
		for (size_t _i = 1; _i < (_array)->size; _i++) {        \
			if ((_array)->cmp(*(_ret), (_array)->data[_i]) < 0) \
				*(_ret) = (_array)->data[_i];                   \
		}                                                       \
	})

/**
 * \brief     A macro for getting the minimum item in an array.
 * \note      This macro gets the minimum item in an array.
 * \param[in] _array The array to get the minimum item from.
 * \param[in] _ret The minimum item in the array.
 */
#define array_min(_array, _ret)                                 \
	({                                                          \
		*(_ret) = (_array)->data[0];                            \
		for (size_t _i = 1; _i < (_array)->size; _i++) {        \
			if ((_array)->cmp(*(_ret), (_array)->data[_i]) > 0) \
				*(_ret) = (_array)->data[_i];                   \
		}                                                       \
	})

/**
 * \brief     A macro for performing a for each loop on an array.
 * \note      This macro performs a for each loop on an array.
 * \param[in] _array The array to perform the for each loop on.
 * \param[in] _func The function to perform on each item in the array.
 */
#define array_foreach(_array, _func)                     \
	({                                                   \
		for (size_t _i = 0; _i < (_array)->size; _i++) { \
			_func(&(_array)->data[_i]);                  \
		}                                                \
	})

#endif // HURUST_ARRAY_H
