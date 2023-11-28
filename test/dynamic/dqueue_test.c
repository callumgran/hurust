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

#include "../../include/hurust/dynamic/dqueue.h"

void test_int_push_pop_get(void)
{
    DQUEUE(int, int);

    struct int_dqueue_t queue;
    dqueue_init(&queue, HR_GLOBAL_ALLOCATOR, 1);

    int item = 1;
    int item2 = 2;

    dqueue_push(&queue, &item);

    int get = dqueue_peek(&queue);

    assert(get == 1);

    int pop = dqueue_pop(&queue);

    assert(pop == 1);

    assert(dqueue_empty(&queue));

    dqueue_push(&queue, &item);

    dqueue_push(&queue, &item2);

    pop = dqueue_pop(&queue);

    assert(pop == 1);

    pop = dqueue_pop(&queue);

    assert(pop == 2);

    dqueue_free(&queue);

    printf("------------------------------------------\n");
    printf("Completed integer push pop get queue tests\n");
    printf("------------------------------------------\n");
}

void test_str_push_pop_get(void)
{
    DQUEUE(char *, str);

    struct str_dqueue_t queue;
    dqueue_init(&queue, HR_GLOBAL_ALLOCATOR, 2);

    char *item = "Hello";
    char *item2 = "World";
    char *item3 = "Hello World";

    dqueue_push(&queue, &item);

    char *get = dqueue_peek(&queue);

    assert(strcmp(get, "Hello") == 0);

    char *pop = dqueue_pop(&queue);

    assert(strcmp(pop, "Hello") == 0);

    assert(dqueue_empty(&queue));

    dqueue_push(&queue, &item);

    dqueue_push(&queue, &item2);

    dqueue_push(&queue, &item3);

    pop = dqueue_pop(&queue);

    assert(strcmp(pop, "Hello") == 0);

    pop = dqueue_pop(&queue);

    assert(strcmp(pop, "World") == 0);

    pop = dqueue_pop(&queue);

    assert(strcmp(pop, "Hello World") == 0);

    dqueue_free(&queue);

    printf("-----------------------------------------\n");
    printf("Completed string push pop get queue tests\n");
    printf("-----------------------------------------\n");
}

int main(void)
{
    printf("Running dynamic queue tests...\n");
    test_int_push_pop_get();
    test_str_push_pop_get();
    return 0;
}