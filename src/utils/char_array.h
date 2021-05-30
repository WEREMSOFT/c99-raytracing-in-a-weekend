#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include <memory.h>

#include "array_header.h"

typedef struct CharArray {
    ArrayHeader header;
    char data[10];
} CharArray;

// recomended initial capacity 10
CharArray* charArrayInit(int initialCapacity){
    size_t size = sizeof(char) * initialCapacity + sizeof(ArrayHeader);
    CharArray* array = (CharArray*)malloc(size);

    if(!array){
        printf("Error allocation memory for CharArray %s::%d\n", __FILE__, __LINE__);
        exit(-1);
    }

    memset(array, 0, size);

    array->header.capacity = initialCapacity;
    array->header.elementSize = sizeof(char);
    array->header.length = 0;
    return array;
}

void charArrayInsertElement(CharArray** self, char element){
    if((*self)->header.length + 1 == (*self)->header.capacity){
        *self = realloc(*self, (*self)->header.capacity * (*self)->header.elementSize * 2 + sizeof(ArrayHeader));
        if(*self == NULL) {
            printf("Error reallocating array\n");
            exit(-1);
        } else {
            (*self)->header.capacity *= 2;
        }
    }
    (*self)->data[(*self)->header.length++] = element;
}

void charArratFini(CharArray* array) {
    free(array);
}

#endif