#ifndef __UNIVERSAL_ARRAY_H__
#define __UNIVERSAL_ARRAY_H__

#include "array_header.h"
#include <stdlib.h>
#include <string.h>

#ifndef MALLOC_FUNC
#define MALLOC_FUNC malloc
#endif

#ifndef REALLOC_FUNC
#define REALLOC_FUNC realloc
#endif

typedef struct Array
{
    ArrayHeader header;
    char data[1];
} Array;

// recomended initial capacity 10
Array *arrayCreate(int initialCapacity, size_t elementSize);
Array *arrayCreateFromCArray(void *c_array, size_t elementSize, int length);
void arrayInsertElement(Array **this, void *element);
void arrayConcatenate(Array **this, Array *source);
void *arrayGetElementAt(Array *this, int index);

#endif
