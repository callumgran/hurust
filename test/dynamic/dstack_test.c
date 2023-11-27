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

#include "../../include/dynamic/dstack.h"

void test_int_push_pop_get(void)
{
    DSTACK(int, int);

    struct int_dstack_t stack;
    dstack_init(&stack, HR_GLOBAL_ALLOCATOR, 1);

    int item = 1;
    int item2 = 2;

    dstack_push(&stack, &item);

    int get = dstack_peek(&stack);

    assert(get == 1);

    int pop = dstack_pop(&stack);

    assert(pop == 1);

    assert(dstack_empty(&stack));

    dstack_push(&stack, &item);

    dstack_push(&stack, &item2);

    pop = dstack_pop(&stack);

    assert(pop == 2);

    pop = dstack_pop(&stack);

    assert(pop == 1);

    dstack_free(&stack);

    printf("------------------------------------------\n");
    printf("Completed integer push pop get stack tests\n");
    printf("------------------------------------------\n");
}

void test_str_push_pop_get(void)
{
    DSTACK(char *, str);

    struct str_dstack_t stack;
    dstack_init(&stack, HR_GLOBAL_ALLOCATOR, 2);

    char *item = "Hello";
    char *item2 = "World";
    char *item3 = "Hello World";

    dstack_push(&stack, &item);

    char *get = dstack_peek(&stack);

    assert(strcmp(get, "Hello") == 0);

    char *pop = dstack_pop(&stack);

    assert(strcmp(pop, "Hello") == 0);

    assert(dstack_empty(&stack));

    dstack_push(&stack, &item);

    dstack_push(&stack, &item2);

    dstack_push(&stack, &item3);

    pop = dstack_pop(&stack);

    assert(strcmp(pop, "Hello World") == 0);

    pop = dstack_pop(&stack);

    assert(strcmp(pop, "World") == 0);

    pop = dstack_pop(&stack);

    assert(strcmp(pop, "Hello") == 0);

    dstack_free(&stack);

    printf("-----------------------------------------\n");
    printf("Completed string push pop get stack tests\n");
    printf("-----------------------------------------\n");
}

int main(void)
{
    printf("Running dynamic stack tests...\n");
    test_int_push_pop_get();
    test_str_push_pop_get();
    return 0;
}