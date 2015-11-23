//
//  Memory.h
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright © 2015 David Pickard. All rights reserved.
//

#ifndef Memory_h
#define Memory_h

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef struct {
    unsigned long size;
    unsigned long count;
    void **p;
}Memory;


void setupMemory(Memory *memory);
void assignMemory(Memory *memory, void** address,unsigned long size);
void reallocateMemory(Memory *memory, void** address, unsigned long oldSize, unsigned long newSize);
void addMemory(Memory *memory, void* address);
void resizeMemory(Memory *memory,unsigned long size);
void freeAddress(Memory *memory, void* address);
void freeMemory(Memory *memory);

#endif /* Memory_h */
