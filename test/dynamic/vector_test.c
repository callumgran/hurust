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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/hurust/dynamic/vector.h"
#include "../../include/hurust/functional/lambda.h"

void test_int_push_pop_get(void)
{
    VECTOR(int, int);

    struct int_vector_t vector;
    vector_init(&vector, HR_GLOBAL_ALLOCATOR, 2,
                lambda(int, (const int a, const int b), { return a - b; }));

    int item = 1;
    int item2 = 2;

    vector_push(&vector, &item);

    int get = vector_get(&vector, 0);

    assert(get == 1);

    int pop = vector_pop(&vector, 0);

    assert(pop == 1);

    assert(vector_empty(&vector));

    vector_push(&vector, &item);

    vector_push(&vector, &item2);

    pop = vector_pop(&vector, 1);

    assert(pop == 2);

    pop = vector_pop(&vector, 0);

    assert(pop == 1);

    assert(vector_empty(&vector));

    vector_free(&vector);

    printf("------------------------------------------\n");
    printf("Completed integer push pop get vector tests\n");
    printf("------------------------------------------\n");
}

void test_str_push_pop_get(void)
{
    VECTOR(char *, str);

    struct str_vector_t vector;
    vector_init(&vector, HR_GLOBAL_ALLOCATOR, 2, strcmp);

    char *item = "Hello";
    char *item2 = "World";
    char *item3 = "Hello World";

    vector_push(&vector, &item);

    char *get = vector_get(&vector, 0);

    assert(strcmp(get, "Hello") == 0);

    char *pop = vector_pop(&vector, 0);

    assert(strcmp(pop, "Hello") == 0);

    assert(vector_empty(&vector));

    vector_push(&vector, &item);

    vector_push(&vector, &item2);

    vector_push(&vector, &item3);

    pop = vector_pop(&vector, 1);

    assert(strcmp(pop, "World") == 0);

    pop = vector_pop(&vector, 1);

    assert(strcmp(pop, "Hello World") == 0);

    pop = vector_pop(&vector, 0);

    assert(strcmp(pop, "Hello") == 0);

    assert(vector_empty(&vector));

    vector_free(&vector);

    printf("------------------------------------------\n");
    printf("Completed integer push pop get vector tests\n");
    printf("------------------------------------------\n");
}

void test_int_push_many_sort(void)
{
    const int vector_size = 10000;
    VECTOR(int, int);

    struct int_vector_t vector;

    vector_init(&vector, HR_GLOBAL_ALLOCATOR, 50,
                lambda(int, (const int a, const int b), { return a - b; }));

    for (int i = 0; i < (vector_size / 2); i++) {
        vector_push(&vector, &i);
        vector_push(&vector, &(int){ vector_size - i - 1 });
    }

    assert(!anon(
        bool, (int *arr, size_t size),
        {
            for (size_t i = 0; i < (size - 1); i++) {
                if (arr[i] > arr[i + 1]) {
                    return false;
                }
            }
            return true;
        },
        vector_get_data(&vector), vector_get_size(&vector)));

    vector_sort(&vector);

    assert(anon(
        bool, (int *arr, size_t size),
        {
            for (size_t i = 0; i < (size - 1); i++) {
                if (arr[i] > arr[i + 1]) {
                    return false;
                }
            }
            return true;
        },
        vector_get_data(&vector), vector_get_size(&vector)));


    printf("------------------------------------------\n");
    printf("Completed integer vector sort tests\n");
    printf("------------------------------------------\n");
}

void test_str_push_many_sort(void)
{
    const int vector_size = 10000;
    VECTOR(char *, str);

    struct str_vector_t vector;

    vector_init(&vector, HR_GLOBAL_ALLOCATOR, vector_size, strcmp);

    for (int i = 0; i < (vector_size / 4); i++) {
        char *str = anon(
            char *, (const char *_str, size_t size),
            {
                char *new_str = HR_ALLOC(HR_GLOBAL_ALLOCATOR, size + 1);
                memcpy(new_str, _str, size);
                new_str[size] = '\0';
                return new_str;
            },
            "Hello", 5);
        vector_push(&vector, &str);
        char *str2 = anon(
            char *, (const char *_str, size_t size),
            {
                char *new_str = HR_ALLOC(HR_GLOBAL_ALLOCATOR, size + 1);
                memcpy(new_str, _str, size);
                new_str[size] = '\0';
                return new_str;
            },
            "World", 5);
        vector_push(&vector, &str2);
    }

    assert(!anon(
        bool, (char **arr, size_t size),
        {
            for (size_t i = 0; i < (size - 1); i++) {
                if (strcmp(arr[i], arr[i + 1]) > 0) {
                    return false;
                }
            }
            return true;
        },
        vector_get_data(&vector), vector_get_size(&vector)));

    vector_sort(&vector);

    assert(anon(
        bool, (char **arr, size_t size),
        {
            for (size_t i = 0; i < (size - 1); i++) {
                if (strcmp(arr[i], arr[i + 1]) > 0) {
                    return false;
                }
            }
            return true;
        },
        vector_get_data(&vector), vector_get_size(&vector)));

    vector_foreach(&vector, lambda(void, (char **str), { HR_DEALLOC(HR_GLOBAL_ALLOCATOR, *str); }));

    vector_free(&vector);

    printf("------------------------------------------\n");
    printf("Completed string vector sort tests\n");
    printf("------------------------------------------\n");
}

int main(void)
{
    printf("Running dynamic vector tests...\n");
    test_int_push_pop_get();
    test_str_push_pop_get();
    test_int_push_many_sort();
    test_str_push_many_sort();
    printf("Completed dynamic vector tests!\n");
    return 0;
}