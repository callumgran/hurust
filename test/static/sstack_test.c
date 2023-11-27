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

#include "../../include/static/sstack.h"

void test_int_push_pop_get(void)
{
    SSTACK(int, int);

    struct int_sstack_t stack;
    sstack_init(&stack, HR_GLOBAL_ALLOCATOR, 2);

    int item = 1;
    int item2 = 2;

    sstack_push(&stack, &item);

    int get = sstack_peek(&stack);

    assert(get == 1);

    int pop = sstack_pop(&stack);

    assert(pop == 1);

    assert(sstack_empty(&stack));

    sstack_push(&stack, &item);

    sstack_push(&stack, &item2);

    assert(sstack_full(&stack));

    pop = sstack_pop(&stack);

    assert(pop == 2);

    pop = sstack_pop(&stack);

    assert(pop == 1);

    sstack_free(&stack);

    printf("------------------------------------------\n");
    printf("Completed integer push pop get stack tests\n");
    printf("------------------------------------------\n");
}

void test_str_push_pop_get(void)
{
    SSTACK(char *, str);

    struct str_sstack_t stack;
    sstack_init(&stack, HR_GLOBAL_ALLOCATOR, 3);

    char *item = "Hello";
    char *item2 = "World";
    char *item3 = "Hello World";

    sstack_push(&stack, &item);

    char *get = sstack_peek(&stack);

    assert(strcmp(get, "Hello") == 0);

    char *pop = sstack_pop(&stack);

    assert(strcmp(pop, "Hello") == 0);

    assert(sstack_empty(&stack));

    sstack_push(&stack, &item);

    sstack_push(&stack, &item2);

    sstack_push(&stack, &item3);

    assert(sstack_full(&stack));

    pop = sstack_pop(&stack);

    assert(strcmp(pop, "Hello World") == 0);

    pop = sstack_pop(&stack);

    assert(strcmp(pop, "World") == 0);

    pop = sstack_pop(&stack);

    assert(strcmp(pop, "Hello") == 0);

    sstack_free(&stack);

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