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
	alloc.h

  PROJECT
	hurust generic library

  DESCRIPTION
	This file contains the basic layout and macros for the alloc module. This
	includes macros for using different allocators, and the basic allocator
	interface.

  PROGRAMMER
	Callum Gran.

  MODIFICATIONS
	20-Nov-23  C.Gran		Created file.

 *==========================================================================*/

#ifndef HURUST_ALLOC_H
#define HURUST_ALLOC_H

#include <stddef.h>
#include <stdlib.h>

/**
 * \brief       Function prototype for an allocator function.
 * \note        This function takes a pointer to the arena,
 *              although if no arena is used, this can be
 *              ignored. When using malloc f.e., the arena is
 *              ignored and a wrapper function is used.
 */
typedef void *(_alloc_fn_t)(void *arena, size_t size);

/**
 * \brief       Function prototype for a reallocator function.
 * \note        This function takes a pointer to the arena,
 *              although if no arena is used, this can be
 *              ignored. When using realloc f.e., the arena is
 *              ignored and a wrapper function is used.
 */
typedef void *(_realloc_fn_t)(void *arena, void *ptr, size_t size);

/**
 * \brief       Function prototype for a deallocator function.
 * \note        This function takes a pointer to the arena,
 *              although if no arena is used, this can be
 *              ignored. When using free f.e., the arena is
 *              ignored and a wrapper function is used.
 */
typedef void(_dealloc_fn_t)(void *arena, void *ptr);

/**
 * \brief       Allocator structure.
 * \note        This structure contains the function pointers
 *              for the allocator functions.
 */
typedef struct hr_allocator_t {
	void *arena;
	_alloc_fn_t *alloc;
	_realloc_fn_t *realloc;
	_dealloc_fn_t *dealloc;
} HRAllocator;

/**
 * \brief       Macro for defining an allocator function that
 *              does not use an arena.
 * \note        All this macro does is define a function that
 *              ignores the arena pointer and calls the
 *              specified function.
 * \param[in]   name  The name of the allocator.
 * \param[in]   alloc_fn  The function to use for allocation.
 */
#define HR_ALLOCATOR_NO_ARENA(name, alloc_fn)           \
	void *_##name##_allocator(void *arena, size_t size) \
	{                                                   \
		(void)arena;                                    \
		return alloc_fn(size);                          \
	}

/**
 * \brief       Macro for defining an reallocator function that
 *              does not use an arena.
 * \note        All this macro does is define a function that
 *              ignores the arena pointer and calls the
 *              specified function.
 * \param[in]   name  The name of the reallocator.
 * \param[in]   realloc_fn  The function to use for reallocation.
 */
#define HR_REALLOCATOR_NO_ARENA(name, realloc_fn)                    \
	void *_##name##_reallocator(void *arena, void *ptr, size_t size) \
	{                                                                \
		(void)arena;                                                 \
		return realloc_fn(ptr, size);                                \
	}

/**
 * \brief       Macro for defining an deallocator function that
 *              does not use an arena.
 * \note        All this macro does is define a function that
 *              ignores the arena pointer and calls the
 *              specified function.
 * \param[in]   name  The name of the deallocator.
 * \param[in]   dealloc_fn  The function to use for deallocation.
 */
#define HR_DEALLOCATOR_NO_ARENA(name, dealloc_fn)      \
	void _##name##_deallocator(void *arena, void *ptr) \
	{                                                  \
		(void)arena;                                   \
		dealloc_fn(ptr);                               \
	}

/**
 * \brief       Macro for initializing an allocator.
 * \note        This macro initializes an allocator with the
 *              specified functions.
 * \param[in]   name  The name of the allocator.
 * \param[in]   arena  The arena to use.
 * \param[in]   alloc_fn  The function to use for allocation.
 * \param[in]   realloc_fn  The function to use for reallocation.
 * \param[in]   dealloc_fn  The function to use for deallocation.
 */
#define HR_ALLOCATOR_INIT(name, arena, alloc_fn, realloc_fn, dealloc_fn) \
	HRAllocator name = { arena, alloc_fn, realloc_fn, dealloc_fn };

/**
 * \brief       Macro for initializing an allocator that
 *              does not use an arena.
 * \note        This macro initializes an allocator with the
 *              specified functions and defines the wrapper
 *              functions to not use an arena.
 * \param[in]   name  The name of the allocator.
 * \param[in]   alloc_fn  The function to use for allocation.
 * \param[in]   realloc_fn  The function to use for reallocation.
 * \param[in]   dealloc_fn  The function to use for deallocation.
 */
#define HR_ALLOCATOR_NO_ARENA_INIT(name, alloc_fn, realloc_fn, dealloc_fn) \
	HR_ALLOCATOR_NO_ARENA(name, alloc_fn)                                  \
	HR_REALLOCATOR_NO_ARENA(name, realloc_fn)                              \
	HR_DEALLOCATOR_NO_ARENA(name, dealloc_fn)                              \
	HRAllocator name = { NULL, _##name##_allocator, _##name##_reallocator, _##name##_deallocator };

/**
 * \brief       Macro for setting the current allocator.
 * \note        This macro sets the current allocator to the
 *              specified allocator.
 * \param[in]   allocator  The allocator to use.
 */
#define HR_SET_ALLOCATOR(allocator) hr_current_allocator = allocator

/**
 * \brief       Macro for resetting the current allocator.
 * \note        This macro resets the current allocator to the
 *              default allocator.
 */
#define HR_RESET_ALLOCATOR() hr_current_allocator = &hr_default_allocator

/**
 * \brief       Macro for allocating memory.
 * \note        This macro allocates memory using the current
 *              allocator.
 * \param[in]   size  The size of the memory to allocate.
 */
#define HR_CUR_ALLOC(size) hr_current_allocator->alloc(hr_current_allocator->arena, (size))

/**
 * \brief       Macro for reallocating memory.
 * \note        This macro reallocates memory using the current
 *              allocator.
 * \param[in]   ptr  The pointer to the memory to reallocate.
 * \param[in]   size  The size of the memory to reallocate.
 */
#define HR_CUR_REALLOC(ptr, size) \
	hr_current_allocator->realloc(hr_current_allocator->arena, ptr, (size))

/**
 * \brief       Macro for deallocating memory.
 * \note        This macro deallocates memory using the current
 *              allocator.
 * \param[in]   ptr  The pointer to the memory to deallocate.
 */
#define HR_CUR_DEALLOC(ptr) hr_current_allocator->dealloc(hr_current_allocator->arena, (ptr))

/**
 * \brief       Macro for allocating memory.
 * \note        This macro allocates memory using a
 *              specified allocator.
 * \param[in]   allocator  The allocator to use.
 * \param[in]   size  The size of the memory to allocate.
 */
#define HR_ALLOC(allocator, size) (allocator)->alloc((allocator)->arena, (size))

/**
 * \brief       Macro for reallocating memory.
 * \note        This macro reallocates memory using a
 *              specified allocator.
 * \param[in]   allocator  The allocator to use.
 * \param[in]   ptr  The pointer to the memory to reallocate.
 * \param[in]   size  The size of the memory to reallocate.
 */
#define HR_REALLOC(allocator, ptr, size) (allocator)->realloc((allocator)->arena, (ptr), (size))

/**
 * \brief       Macro for deallocating memory.
 * \note        This macro deallocates memory using a
 *              specified allocator.
 * \param[in]   allocator  The allocator to use.
 * \param[in]   ptr  The pointer to the memory to deallocate.
 */
#define HR_DEALLOC(allocator, ptr) (allocator)->dealloc((allocator)->arena, (ptr))

// Initialize the default allocator, which uses malloc, realloc, and free.
HR_ALLOCATOR_NO_ARENA_INIT(hr_default_allocator, malloc, realloc, free)

HRAllocator *hr_current_allocator = &hr_default_allocator;

#define HR_GLOBAL_ALLOCATOR hr_current_allocator

#endif