//
//  main.c
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright (c) 2015 David Pickard. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "Products.h"
#include "Listings.h"
#include "Memory.h"

#define program_name "challenge"

void usage();
void handleError();
void printJSONResults(Products *products,FILE *f);

int main(int argc, const char * argv[]) {
    Products products;
    Listings listings;
    Memory memory;
    FILE *f;
    
    setupMemory(&memory);   //setup memory management
    
    if (argc<3 || argc>4){  //if wrong number of command line arguments then print proper usage and quit
        usage();
        return 0;
    }
    else if(access(argv[1], R_OK)) {    //verify that product file can be accessed for reading. if not: handle the error and quit.
        handleError(argv[1],&memory);
        return 0;
    }
    else if (access(argv[2], R_OK)) {   //verify that listing file can be accessed for reading. if not: handle the error and quit.
        handleError(argv[2],&memory);
        return 0;
    }
    else if (argc==4 && !access(argv[3], F_OK) && access(argv[3], W_OK)){   //verify that output file doesn't exist or can be accessed for writing. if not: handle the error and quit.
        handleError(argv[3],&memory);
        return 0;
    }
    
    if (populateProductsFromFile(&products, argv[1], &memory)) {    //populate products structure from file, handle errors that may result
        handleError(argv[1],&memory);
        return 0;
    }

    if (populateListingsFromFile(&listings, argv[2], &memory)) {    //populate listings structure from file, handle errors that may result
        handleError(argv[2],&memory);
        return 0;
    }
    

    associate(&products, &listings,&memory);    //match listings with products
    
    
    if (argc==4) {  //print to file whose path is 4th command line argument (3rd entered by user)
        if ((f=fopen(argv[3], "w"))) {      //make sure file was opened
            printJSONResults(&products, f);
            fclose(f);
        }
        else{                               //otherwise handle error and quit
            handleError(argv[3],&memory);
            return 0;
        }
    }
    else{
        printJSONResults(&products, stdout);    //no output file path was provided so print to stdout
    }
    
    freeMemory(&memory);    //free all managed memory

    return 0;
}

void usage(){
    printf("usage: %s product_file listing_file [output_file]\n",program_name);
}

void handleError(char *msg, Memory *memory){
    char *temp;
    assignMemory(memory, (void **) &(temp),12+strlen(msg));
    perror(strcat(strcat(strcpy(temp, program_name), ": "), msg));
    freeMemory(memory);
    usage();
}

void printJSONResults(Products *products,FILE *f){
    unsigned long i,j;
    for (i=0; i<products->count; i++) {
        fprintf(f,"{\"product_name\":\"%.*s\",\"listings\":[",(int)products->name[i].length,products->name[i].cString);
        if (products->listingCounts[i]>0) {
            fprintf(f,"%.*s",(int)products->listingEntries[i][0]->length,products->listingEntries[i][0]->cString);
            for (j=1; j<products->listingCounts[i]; j++) {
                fprintf(f,",%.*s",(int)products->listingEntries[i][j]->length,products->listingEntries[i][j]->cString);
            }
        }
        fprintf(f,"]}\n");
    }
}