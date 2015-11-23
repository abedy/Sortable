//
//  Listings.h
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright © 2015 David Pickard. All rights reserved.
//

#ifndef Listings_h
#define Listings_h

#include <stdio.h>
#include <sys/stat.h>
#include "Str.h"
#include "Memory.h"


typedef struct {
    string file;
    string *entry;
    string *title;
    string *manufacturer;
    string *currency;
    string *price;
    unsigned long count;
}Listings;

//void populateListings(Listings *listings);
int populateListingsFromFile(Listings *listings,const char *path,Memory *memory);

#endif /* Listings_h */
