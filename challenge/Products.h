//
//  Products.h
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright © 2015 David Pickard. All rights reserved.
//

#ifndef Products_h
#define Products_h

#include <stdio.h>
#include <sys/stat.h>
#include "Str.h"
#include "Memory.h"
#include "Listings.h"

typedef struct {
    string file;
    string *entry;
    string *name;
    string *manufacturer;
    string *model;
    string *family;
    string *date;
    string ***listingEntries;
    double **matchStrength;
    unsigned long *listingCounts;
    unsigned long count;
}Products;



void associate(Products *products,Listings *listings,Memory *memory);
int modelInStr(string model,string title);
double CSSumM(string stringA, string stringB,unsigned int *tempArray[2]);
double normalizedLCSM(string stringA, string stringB,unsigned int *tempArray[2]);

int populateProductsFromFile(Products *products,const char *path,Memory *memory);

#endif /* Products_h */
