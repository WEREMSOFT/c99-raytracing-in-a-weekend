#ifndef __STACK_ALLOCATOR_H__
#define __STACK_ALLOCATOR_H__

/**
 * StackAllocator is a custom allocator that returns pointers to a memory arena and grows itself 
 * when is asked to reserve more memory than capacity.
 * 
 * Using this type of custom allocator has the following advantages over using malloc and free
 * in a clasical way:
 * 
 * 1. Allocate memory and free memory takes time because the operative system need to do bookkeeping
 * of the memory that was allocated. Using a StackAllocator with a reasonable gessed initial size
 * will avoid the cycle of alloc/free removing the performance penalty.
 * 
 * 2. Memory fragmentation is reduced, specially when working with a lot of small objects.
 * 
 * 3. You can handle process that requires allocate and free memory in a more maintainable way
 * you create the StackAllocator in at the start of the process(let's say, in the begining of the
 * asset loading routine) allocate how many objects you need, and they you free only the allocator.
 * Reducing the stress to the operative system and the memory fragmentation for your program.
 * 
 */

#include <string.h>
#include <stdio.h>

typedef struct {
    int capacity;
    int length;
    int allocationCount;
} StackAllocatorHeader;

typedef struct {
    StackAllocatorHeader header;
    char data[1];
} StackAllocator;

StackAllocator* stackAllocatorCreate(size_t size) {
    size_t sizeToAllocate = size + sizeof(StackAllocatorHeader);
    StackAllocator* this = (StackAllocator*)calloc(sizeof(char), sizeToAllocate);

    if(!this){
        printf("Error creating stack allocator %s::%d\n", __FILE__, __LINE__);
        return NULL;
    }

    this->header.capacity = size;
    return this;
}

void* stackAllocatorAlloc(StackAllocator *this, size_t size) {
    if(this->header.length + size > this->header.capacity){
        // TODO: realloc will invalidate all the pointers provided before.
        // For now, we just abort the program, in the future we should create
        // A handle table
        printf("Allocator ran out of space.\nActual capacity %d\nRequired capacity %d\n", this->header.capacity, this->header.capacity + size);
        return NULL;
    }
    void *returnValue = &this->data[this->header.length];
    this->header.length += size;
    this->header.allocationCount++;
    return returnValue;
}

void stackAllocatorFree(StackAllocator *this){
    this->header.allocationCount--;
    if(this->header.allocationCount == 0){
        this->header.length = 0;
    }
}

void* stackAllocatorRealloc(StackAllocator *this, void* pointer, size_t size) {
    void *returnValue = stackAllocatorAlloc(this, size);
    
    if(pointer != NULL)
        memcpy(returnValue, pointer, size);

    return returnValue;
}

#endif