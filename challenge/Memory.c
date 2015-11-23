//
//  Memory.c
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright © 2015 David Pickard. All rights reserved.
//

#include "Memory.h"

//must be called on Memory before Memory is used
void setupMemory(Memory *memory){
    memory->count=0; //number of occupied indexes in Memory->p
    memory->size=0; //size of allocated memory(number of pointers) used by Memory object itself (Memory->p)
    memory->p=0;    //used to access array of pointers to managed memory
}

//allocates memory to *address and stores it in Memory for management
void assignMemory(Memory *memory, void** address,unsigned long size){
    unsigned long i;
    if (memory->size<=memory->count) {
        resizeMemory(memory, memory->size+10);
    }
    *address=malloc(size);
    for (i=1;memory->p[i]; i++) {}

    memory->p[i]=*address;
    memory->count++;
}

//reallocates memory in *address, old contents are transferred to the new location, oldSize is required to zero-out the new space
void reallocateMemory(Memory *memory, void** address, unsigned long oldSize, unsigned long newSize){
    unsigned long i;
    void *temp;
    if (*address && oldSize>0 && memory->count>1) {
        for (i=memory->size-1; i>0 ; i--) {
            if (memory->p[i]==*address) {
                temp=realloc(memory->p[i], newSize);
                if (temp || newSize==0) {
                    memory->p[i]=temp;
                    *address=temp;
                    if (newSize>oldSize) {
                        memset(temp+oldSize, 0, newSize-oldSize);
                    }
                }
                break;
            }
        }
    }
    else{
        assignMemory(memory, address, newSize);
    }
}

//add memory to Memory object for management
void addToMemory(Memory *memory, void* address){
    unsigned long i;
    if (memory->size<=memory->count) {
        resizeMemory(memory, memory->size+10);
    }
    for (i=1;memory->p[i]; i++) {}

    memory->p[i]=address;
    memory->count++;
}

//resize Memory object to hold more addresses, for safety this function cannot be used to shrink Memory
//(you could allow shrinking by sorting Memory->p to populate only the lower indexes and making sure the new size is larger than count)
void resizeMemory(Memory *memory,unsigned long size){
    void **temp;
    if (size>memory->size) {
        temp=realloc(memory->p, size*sizeof(void **));
        if (temp) {
            memory->p=temp;
            memory->p[0]=temp;
            if (size>1) {
                if (memory->size>0) {
                    memset(&(memory->p[memory->size]), 0, sizeof(void **)*(size-memory->size));
                }
                else{
                    memory->count=1;
                    memset(&(memory->p[1]), 0, sizeof(void **)*(size-1));
                }
            }
            memory->size=size;
        }
    }
}

//release a managed address
void freeAddress(Memory *memory, void* address){
    unsigned long i;
    if (address && memory->count>1) {
        for (i=memory->size-1; i>0 ; i--) {
            if (memory->p[i]==address) {
                free(memory->p[i]);
                memory->p[i]=0;
                memory->count--;
                break;
            }
        }
    }
}

//release all managed memory, including Memory->p
void freeMemory(Memory *memory){
    unsigned long i;
    for (i=memory->size-1; memory->count>0; i--) {
        if (memory->p[i]) {
            free(memory->p[i]);
            memory->count--;
        }
    }
    memory->size=0;
}



