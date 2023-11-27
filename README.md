# HURUST C Macro Collections Library

<p align="center">
    <a href="https://github.com/callumgran/hurust/blob/master/LICENSE"><img src="https://img.shields.io/badge/License-GPL--3.0-blue.svg" alt="License"/></a>
    <a href="https://github.com/callumgran/hurust"><img src="https://img.shields.io/badge/GitHub-hurust-2195F3.svg?logo=github" alt="LinkToRepo"/></a>
</p>

<p align="center">
    <a href="https://github.com/callumgran/hurust/stargazers"><img src="https://img.shields.io/github/stars/callumgran/hurust?style=flat&color=ffa000" alt="Stars"/></a>
    <a href="https://github.com/callumgran/hurust"><img src="https://img.shields.io/tokei/lines/github/callumgran/hurust" alt="loc"/></a>
</p>

HURUST is a versatile, easy to use, header based GNU-C collections library.

## Features

- **Fully Macro-Based**: HURUST is fully macro-based, meaning that it is simple to use with minimal boilerplate code.

- **Dynamic and Static Collections**: HURUST offers both dynamically and statically sized collections.

- **Sorting**: Sort collections easily using built-in sorting functions.

- **Memory Management**: HURUST collections require a memory management object to be passed in, allowing for easy integration with your project's memory management system.

## Collections Implemented

### Dynamic Collections

| Collection          | Abstract Datastructure             | Datatype                        |
|----------------------|------------------------------------|---------------------------------|
| Vector       | List                               | Dynamic Array                   |
| Queue        | FIFO                               | Dynamic Circular Array          |
| Stack        | LIFO                               | Dynamic Array                   |
| Binary Heap  | Priority Queue                     | Dynamic Array                   |

### Static Collections

| Collection          | Abstract Datastructure             | Datatype                        |
|----------------------|------------------------------------|---------------------------------|
| Array         | Array                              | Static Array                    |
| Queue         | FIFO                               | Static Circular Array           |
| Stack         | LIFO                               | Static Array                    |

## Extra Features

| Feature              | Description                       | Header File                     |
|----------------------|-----------------------------------|---------------------------------|
| Lambda Expressions   | Support for lambda expressions and anonymous functions in C          | `#include "lambda.h"`           |
| Allocator            | Basic allocator struct and macros                 | `#include "alloc.h"`        |
| Sorting              | Sorting macro that can be applied on any array                | `#include "sort.h"`             |

## Getting Started

### 1. Download

Download the latest release of HURUST from the [Releases](https://github.com/callumgran/hurust/releases) page and include the hurust directory in your project.

### 2. Integration

Include the appropriate collection header file in your project:

For static collections:
```c
#include "hurust/static/collection_name.h"
```
For dynamic collections:
```c
#include "hurust/dynamic/collection_name.h"
```

### 3. Usage

HURUST collections are designed to be easy to use. Simply create a collection template, instatiate an object and perform the desired operations.

#### Example

```c
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "hurust/functional/lambda.h"
#include "hurust/dynamic/vector.h"

int main(void) 
{
    // Create a vector of integers
    VECTOR(int, int);

    struct int_vector_t vector;

    vector_init(&vector, HR_GLOBAL_ALLOCATOR, 2,
        lambda(int, (const int a, const int b), { return a - b; }));


    // Push some elements onto the vector
    vector_push(&vector, &(int){1});
    vector_push(&vector, &(int){3});
    vector_push(&vector, &(int){2});

    // Sort the vector
    vector_sort(&vector);

    // Assert the vector is sorted
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

    // Pop an element off the vector
    int pop = vector_pop(&vector, 2);

    // Print the vector
    vector_foreach(&vector, lambda(void, (const int *const element), { 
        printf("%d\n", *element); 
    }));

    // Free the vector
    vector_free(vec);
}
```

## Using HURUST as a Single-Header Library (STB)
HURUST can be easily integrated into your project as a single-header library (STB), simplifying the setup process and making it convenient for small to medium-sized projects.

### Downloading the STB

Download the latest single-header release of HURUST from the [Releases](https://github.com/callumgran/hurust/releases) page. The single-header file is typically named hurust.h.

### Integration

Copy the downloaded hurust.h header file into your project's source code directory. Include the HURUST header file in your C code:

```c
#define HURUST_IMPLEMENTATION
#include "hurust.h"
```

### Unit Testing
Check the provided unit test examples in the test directory, which showcase the usage of HURUST collections.

### Contributing
Contributions are welcome! If you have enhancements or find issues, please open an issue or submit a pull request