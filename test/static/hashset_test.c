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

#include "../../include/hurust/static/shashset.h"

char * const randomWords[50] = {
    "apple", "banana", "chocolate", "dog", "elephant",
    "fantastic", "guitar", "happy", "island", "jazz",
    "kangaroo", "lemon", "mango", "notebook", "ocean",
    "penguin", "quasar", "rhythm", "sunflower", "tiger",
    "umbrella", "victory", "whisper", "xylophone", "zeppelin",
    "airplane", "butterfly", "carousel", "dragon", "enchanted",
    "fireworks", "giraffe", "harmony", "iceberg", "jungle",
    "kiwi", "lighthouse", "mountain", "nebula", "octopus",
    "paradise", "quokka", "rainbow", "serenade", "telescope",
    "unicorn", "volcano", "waterfall", "xanadu", "yogurt"
};

int cmp_int(const int a, const int b)
{
    return a - b;
}

int cmp_u8(const uint8_t a, const uint8_t b)
{
    return a - b;
}

void test_int_hashset(void)
{
    SHASHSET(int, int);

    struct int_shashset_t hashset;
    shashset_init(&hashset, HR_GLOBAL_ALLOCATOR, 50, cmp_int, hash_int);

    for (int i = 0; i < 25; i++) {
        shashset_insert(&hashset, i * 2);
    }

    for (int i = 0; i < 50; i++) {
        if (i % 2 == 0 && i != 0) {
            assert(shashset_contains(&hashset, i));
        } else {
            assert(!shashset_contains(&hashset, i));
        }
    }

    for (int i = 0; i < 25; i++) {
        shashset_remove(&hashset, i * 2);
    }

    for (int i = 1; i < 50; i++) {
        assert(!shashset_contains(&hashset, i));
    }

    shashset_free(&hashset);

    printf("SHASHSET(int, int) passed.\n");
}

void test_uint8_hashset(void)
{
    SHASHSET(uint8_t, u8);

    struct u8_shashset_t hashset;
    shashset_init(&hashset, HR_GLOBAL_ALLOCATOR, 50, cmp_u8, hash_unsigned_char);

    for (int i = 0; i < 25; i++) {
        shashset_insert(&hashset, i * 2);
    }

    for (int i = 0; i < 50; i++) {
        if (i % 2 == 0 && i != 0) {
            assert(shashset_contains(&hashset, i));
        } else {
            assert(!shashset_contains(&hashset, i));
        }
    }

    for (int i = 0; i < 25; i++) {
        shashset_remove(&hashset, i * 2);
    }

    for (int i = 1; i < 50; i++) {
        assert(!shashset_contains(&hashset, i));
    }

    shashset_free(&hashset);

    printf("SHASHSET(uint8_t, u8) passed.\n");
}

void test_str_hashset(void)
{
    SHASHSET(char *, str);

    struct str_shashset_t hashset;
    shashset_init(&hashset, HR_GLOBAL_ALLOCATOR, 75, strcmp, hash_str);

    for (int i = 0; i < 50; i++) {
        shashset_insert(&hashset, randomWords[i]);
    }

    for (int i = 0; i < 50; i++) {
        assert(shashset_contains(&hashset, randomWords[i]));
    }

    for (int i = 0; i < 50; i++) {
        shashset_remove(&hashset, randomWords[i]);
    }

    for (int i = 0; i < 50; i++) {
        assert(!shashset_contains(&hashset, randomWords[i]));
    }

    shashset_free(&hashset);

    printf("SHASHSET(char *, str) passed.\n");
}

int main(void)
{
    printf("Running hashset tests...\n");
    test_int_hashset();
    test_uint8_hashset();
    test_str_hashset();
    printf("Done.\n");
    return 0;
}