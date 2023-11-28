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

#include "../../include/hurust/functional/lambda.h"
#include "../../include/hurust/static/array.h"

void test_int_push_pop_get(void)
{
    ARRAY(int, int);

    struct int_array_t array;
    array_init(&array, HR_GLOBAL_ALLOCATOR, 2,
               lambda(int, (const int a, const int b), { return a - b; }));

    int item = 1;
    int item2 = 2;

    array_push(&array, &item);

    int get = array_get(&array, 0);

    assert(get == 1);

    int pop = array_pop(&array, 0);

    assert(pop == 1);

    assert(array_empty(&array));

    array_push(&array, &item);

    array_push(&array, &item2);

    int min = array_min(&array);

    assert(min == 1);

    int max = array_max(&array);

    assert(max == 2);

    assert(array_full(&array));

    pop = array_pop(&array, 1);

    assert(pop == 2);

    pop = array_pop(&array, 0);

    assert(pop == 1);

    assert(array_empty(&array));

    printf("------------------------------------------\n");
    printf("Completed integer push pop get array tests\n");
    printf("------------------------------------------\n");
}

void test_str_push_pop_get(void)
{
    ARRAY(char *, str);

    struct str_array_t array;
    array_init(&array, HR_GLOBAL_ALLOCATOR, 3, strcmp);

    char *item = "Hello";
    char *item2 = "World";
    char *item3 = "Hello World";

    array_push(&array, &item);

    char *get = array_get(&array, 0);

    assert(strcmp(get, "Hello") == 0);

    char *pop = array_pop(&array, 0);

    assert(strcmp(pop, "Hello") == 0);

    assert(array_empty(&array));

    array_push(&array, &item);

    array_push(&array, &item2);

    array_push(&array, &item3);

    char *min = array_min(&array);

    assert(strcmp(min, "Hello") == 0);

    char *max = array_max(&array);

    assert(strcmp(max, "World") == 0);

    assert(array_full(&array));

    pop = array_pop(&array, 1);

    assert(strcmp(pop, "World") == 0);

    pop = array_pop(&array, 1);

    assert(strcmp(pop, "Hello World") == 0);

    pop = array_pop(&array, 0);

    assert(strcmp(pop, "Hello") == 0);

    assert(array_empty(&array));

    array_free(&array);

    printf("------------------------------------------\n");
    printf("Completed integer push pop get array tests\n");
    printf("------------------------------------------\n");
}

void test_int_push_many_sort(void)
{
    const int array_size = 10000;
    ARRAY(int, int);

    struct int_array_t array;

    array_init(&array, HR_GLOBAL_ALLOCATOR, array_size,
               lambda(int, (const int a, const int b), { return a - b; }));

    for (int i = 0; i < (array_size / 2); i++) {
        array_push(&array, &i);
        array_push(&array, &(int){ array_size - i - 1 });
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
        array_get_data(&array), array_get_size(&array)));

    array_sort(&array);

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
        array_get_data(&array), array_get_size(&array)));


    printf("------------------------------------------\n");
    printf("Completed integer array sort tests\n");
    printf("------------------------------------------\n");
}

void test_str_push_many_sort(void)
{
    const int array_size = 10000;
    ARRAY(char *, str);

    struct str_array_t array;

    array_init(&array, HR_GLOBAL_ALLOCATOR, array_size, strcmp);

    for (int i = 0; i < (array_size / 4); i++) {
        char *str = anon(
            char *, (const char *_str, size_t size),
            {
                char *new_str = HR_ALLOC(HR_GLOBAL_ALLOCATOR, size + 1);
                memcpy(new_str, _str, size);
                new_str[size] = '\0';
                return new_str;
            },
            "Hello", 5);
        array_push(&array, &str);
        char *str2 = anon(
            char *, (const char *_str, size_t size),
            {
                char *new_str = HR_ALLOC(HR_GLOBAL_ALLOCATOR, size + 1);
                memcpy(new_str, _str, size);
                new_str[size] = '\0';
                return new_str;
            },
            "World", 5);
        array_push(&array, &str2);
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
        array_get_data(&array), array_get_size(&array)));

    array_sort(&array);

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
        array_get_data(&array), array_get_size(&array)));

    array_foreach(&array, lambda(void, (char **str), { HR_DEALLOC(HR_GLOBAL_ALLOCATOR, *str); }));

    array_free(&array);

    printf("------------------------------------------\n");
    printf("Completed string array sort tests\n");
    printf("------------------------------------------\n");
}

int main(void)
{
    printf("Running static array tests...\n");
    test_int_push_pop_get();
    test_str_push_pop_get();
    test_int_push_many_sort();
    test_str_push_many_sort();
    printf("Completed static array tests!\n");
    return 0;
}