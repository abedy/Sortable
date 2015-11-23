//
//  Str.h
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright © 2015 David Pickard. All rights reserved.
//

#ifndef Str_h
#define Str_h

#include <stdio.h>
#include <stdlib.h>


typedef struct {
    char *cString;
    unsigned long length;
} string;


//mostly unused, threw these together in case I needed them (i think I'm only using charCount and inStringStringLit)

//returns the number of occurences of c in myString
unsigned int charCount(string myString,char c);

unsigned int inStringChar(string myString,char c);

unsigned int inStringString(string myString, string s);

//returns position of first occurence of string literal s in myString
unsigned int inStringStringLit(string myString,const char * s);//returns index of first occurence +1 similar to vb.net so a return value of 0 means no match

unsigned int cStringCmp(char *cString, const char *str);

unsigned int longestCommonSubstring(string stringA, string stringB);

#endif /* Str_h */
