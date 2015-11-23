//
//  Str.c
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright © 2015 David Pickard. All rights reserved.
//

#include "Str.h"

unsigned int charCount(string myString,char c){
    unsigned int i;
    unsigned int count=0;
    if (myString.length>0){
        for (i=0; i<myString.length && myString.cString[i]!=0; i++) {
            if (myString.cString[i]==c) {
                count++;
            }
        }
    }
    return count;
}


unsigned int inStringChar(string myString,char c){ //returns index of first occurence +1 similar to vb.net so a return value of 0 means no match
    unsigned int i;
    for (i=0; i<myString.length && myString.cString[i]!=0; i++) {
        if (myString.cString[i]==c) {
            return ++i;
        }
    }
    return 0;
}

unsigned int inStringString(string myString,string s){ //returns index of first occurence +1 similar to vb.net so a return value of 0 means no match
    unsigned int i,j;
    for (i=0; i<myString.length && myString.cString[i]!=0; i++) {
        for (j=0; j<s.length && (j+i)<myString.length && myString.cString[i+j]==s.cString[j]; j++) {}
        if (j==s.length) {
            return ++i;
        }
    }
    return 0;
}

unsigned int inStringStringLit(string myString,const char *s){ //returns index of first occurence +1 similar to vb.net so a return value of 0 means no match
    unsigned int i,j;
    for (i=0; i<myString.length && myString.cString[i]!=0; i++) {
        for (j=0; s[j]!=0 && (j+i)<myString.length && myString.cString[i+j]==s[j]; j++) {}
        if (s[j]==0) {
            return ++i;
        }
    }
    return 0;
}

unsigned int cStringCmp(char *cString, const char *str){
    unsigned int i;
    for (i=0; cString[i]!=0 && str[i]!=0 && cString[i]==str[i]; i++) {}
    return i;
}

unsigned int longestCommonSubstringInsensitive(string stringA, string stringB){
    double largest=0;
    unsigned long i,j=0;
    unsigned int temp;
    char a,b;
    unsigned int *tempArray[2];
    string small, large;
    if (stringA.length>stringB.length) {
        large=stringA;
        small=stringB;
    }
    else{
        small=stringA;
        large=stringB;
    }
    tempArray[0]=malloc(small.length*sizeof(int));
    tempArray[1]=malloc(small.length*sizeof(int));
    for (i=0; i<large.length; i++) {
        a=large.cString[i];
        for (j=0; j<small.length; j++) {
            b=small.cString[j];
            temp=0;
            if (a==b || ((a>='a' && a<='z') && (a-32==b)) || ((a>='A' && a<='Z') && (a+32==b))) { //match
                if (i==0 || j==0) { //first column
                    temp=1;
                }
                else{
                    temp=tempArray[(i-1)%2][j-1]+1;
                }
                if (temp>largest) { //new largest
                    largest=temp;
                }
            }
            else{
                if ((a==' ' || a=='-' || a=='_') && i>0) {      //stringA character should be skipped
                    temp=tempArray[(i-1)%2][j];
                }
                else if ((b==' ' || b=='-' || b=='_') && j>0){  //stringB character should be skipped
                    temp=tempArray[i%2][j-1];
                }
            }
            tempArray[i%2][j]=temp;
        }
    }
    free(tempArray[0]);
    free(tempArray[1]);
    return largest;
}
