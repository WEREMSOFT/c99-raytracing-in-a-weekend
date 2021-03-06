#include "array.h"
#include <stdio.h>

// recomended initial capacity 10
Array *arrayCreate(int initialCapacity, size_t elementSize)
{
    size_t size = elementSize * initialCapacity + sizeof(ArrayHeader);
    Array *array = (Array *)MALLOC_FUNC(size);

    if (!array)
    {
        printf("Error allocation memory for UniversalArray %s::%d\n", __FILE__, __LINE__);
        exit(-1);
    }

    memset(array, 0, size);

    array->header.capacity = initialCapacity;
    array->header.elementSize = elementSize;
    array->header.length = 0;
    return array;
}

Array *arrayCreateFromCArray(void *c_array, size_t elementSize, int length)
{
    Array *array = arrayCreate(length, elementSize);
    array->header.length = length;
    memcpy(array->data, c_array, length * elementSize);
    return array;
}

void arrayInsertElement(Array **this, void *element)
{
    if ((*this)->header.length + 1 == (*this)->header.capacity)
    {
        int size = (*this)->header.capacity * (*this)->header.elementSize * 2 + sizeof(ArrayHeader);
        Array *newPointer = REALLOC_FUNC(*this, size);
        if (*this == NULL)
        {
            printf("Error reallocating array\n");
            exit(-1);
        }
        else
        {
            *this = newPointer;
            (*this)->header.capacity *= 2;
        }
    }

    memcpy(&(**this).data[(*this)->header.elementSize * (*this)->header.length], element, (**this).header.elementSize);
    (*this)->header.length++;
}

void arrayConcatenate(Array **this, Array *source)
{
    if ((*this)->header.elementSize != source->header.elementSize)
    {
        printf("Error: Arrays holds elements of different dizes. %s::%d\n", __FILE__, __LINE__);
        exit(-1);
    }
    for (int i = 0; i < source->header.length; i++)
    {
        arrayInsertElement(this, &source->data[i * source->header.elementSize]);
    }
}

void *arrayGetElementAt(Array *this, int index)
{
    if (index < this->header.length)
    {
        return &this->data[index * this->header.elementSize];
    }
    return NULL;
}
