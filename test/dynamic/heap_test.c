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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../include/hurust/dynamic/heap.h"
#include "../../include/hurust/functional/lambda.h"

void test_int_push_pop_get(void)
{
    HEAP(int, int);

    struct int_heap_t heap;
    heap_init(&heap, HR_GLOBAL_ALLOCATOR, 2,
              lambda(int, (const int a, const int b), { return a - b; }));

    int item = 3;
    int item2 = 10;
    int item3 = 1;
    int item4 = 4;

    heap_push(&heap, &item);

    int get = heap_peek(&heap);

    assert(get == 3);

    int pop = heap_pop(&heap);

    assert(pop == 3);

    assert(heap_empty(&heap));

    heap_push(&heap, &item);

    heap_push(&heap, &item2);

    heap_push(&heap, &item3);

    heap_push(&heap, &item4);

    pop = heap_pop(&heap);

    assert(pop == 1);

    pop = heap_pop(&heap);

    assert(pop == 3);

    pop = heap_pop(&heap);

    assert(pop == 4);

    pop = heap_pop(&heap);

    assert(pop == 10);

    heap_free(&heap);

    printf("------------------------------------------\n");
    printf("Completed integer push pop get heap tests\n");
    printf("------------------------------------------\n");
}

void test_str_push_pop_get(void)
{
    HEAP(char *, str);

    struct str_heap_t heap;
    heap_init(&heap, HR_GLOBAL_ALLOCATOR, 3, strcmp);

    char *item = "Hello";
    char *item2 = "World";
    char *item3 = "Hello World";
    char *item4 = "Cello World!";

    heap_push(&heap, &item);

    char *get = heap_peek(&heap);

    assert(strcmp(get, "Hello") == 0);

    char *pop = heap_pop(&heap);

    assert(strcmp(pop, "Hello") == 0);

    assert(heap_empty(&heap));

    heap_push(&heap, &item);

    heap_push(&heap, &item2);

    heap_push(&heap, &item3);

    heap_push(&heap, &item4);

    pop = heap_pop(&heap);

    assert(strcmp(pop, "Cello World!") == 0);

    pop = heap_pop(&heap);

    assert(strcmp(pop, "Hello") == 0);

    pop = heap_pop(&heap);

    assert(strcmp(pop, "Hello World") == 0);

    pop = heap_pop(&heap);

    assert(strcmp(pop, "World") == 0);

    heap_free(&heap);

    printf("-----------------------------------------\n");
    printf("Completed string push pop get heap tests\n");
    printf("-----------------------------------------\n");
}

int main(void)
{
    printf("Running heap tests...\n");
    test_int_push_pop_get();
    test_str_push_pop_get();
    return 0;
}
