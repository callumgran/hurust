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

#include "../../include/static/squeue.h"

void test_int_push_pop_get(void)
{
	SQUEUE(int, int);

	struct int_squeue_t queue;
	squeue_init(&queue, HR_GLOBAL_ALLOCATOR, 2);

	int item = 1;
	int item2 = 2;

	squeue_push(&queue, &item);

	int get;

	squeue_peek(&queue, &get);

	assert(get == 1);

	int pop;

	squeue_pop(&queue, &pop);

	assert(pop == 1);

	assert(squeue_empty(&queue));

	squeue_push(&queue, &item);

	squeue_push(&queue, &item2);

	squeue_pop(&queue, &pop);

	assert(pop == 1);

	squeue_pop(&queue, &pop);

	assert(pop == 2);

	squeue_free(&queue);

	printf("------------------------------------------\n");
	printf("Completed integer push pop get queue tests\n");
	printf("------------------------------------------\n");
}

void test_str_push_pop_get(void)
{
	SQUEUE(char *, str);

	struct str_squeue_t queue;
	squeue_init(&queue, HR_GLOBAL_ALLOCATOR, 3);

	char *item = "Hello";
	char *item2 = "World";
	char *item3 = "Hello World";

	squeue_push(&queue, &item);

	char *get;

	squeue_peek(&queue, &get);

	assert(strcmp(get, "Hello") == 0);

	char *pop;

	squeue_pop(&queue, &pop);

	assert(strcmp(pop, "Hello") == 0);

	assert(squeue_empty(&queue));

	squeue_push(&queue, &item);

	squeue_push(&queue, &item2);

	squeue_push(&queue, &item3);

	squeue_pop(&queue, &pop);

	assert(strcmp(pop, "Hello") == 0);

	squeue_pop(&queue, &pop);

	assert(strcmp(pop, "World") == 0);

	squeue_pop(&queue, &pop);

	assert(strcmp(pop, "Hello World") == 0);

	squeue_free(&queue);

	printf("-----------------------------------------\n");
	printf("Completed string push pop get queue tests\n");
	printf("-----------------------------------------\n");
}

int main(void)
{
	printf("Running static queue tests...\n");
	test_int_push_pop_get();
	test_str_push_pop_get();
	return 0;
}