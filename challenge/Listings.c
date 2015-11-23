//
//  Listings.c
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright © 2015 David Pickard. All rights reserved.
//

#include "Listings.h"

//this function could be merged with populateProductsFromFile to reduce the program size
//takes a pointer to listings structure, a path to a file to read, and a pointer to memory (management) structure
int populateListingsFromFile(Listings *listings,const char *path,Memory *memory){
    unsigned int i;
    unsigned int j=0;
    unsigned int k;
    unsigned int count=0;
    string *temp;
    struct stat fileStat;
    unsigned long fileLength;
    char *fileCString;
    unsigned long listingCount;
    
    FILE* f;
    
    //if true something has gone wrong accessing the file
    if (stat(path, &fileStat)  || !(f= fopen (path, "rb"))){return -1;}
    
    assignMemory(memory, (void **) &(listings->file.cString),fileStat.st_size); //allocate memory for storing the file contents
    listings->file.length = fread(listings->file.cString,1,fileStat.st_size,f); //read file
    fclose(f);//close file
    
    listings->count=charCount(listings->file, '\n'); //number of entries set by number of newline characters (not foolproof but it works)
    
    //makes code easier to read
    fileLength=listings->file.length;
    fileCString=listings->file.cString;
    listingCount=listings->count;
    
    assignMemory(memory, (void **) &(listings->entry),listingCount*sizeof(string));
    assignMemory(memory, (void **) &(listings->currency),listingCount*sizeof(string));
    assignMemory(memory, (void **) &(listings->manufacturer),listingCount*sizeof(string));
    assignMemory(memory, (void **) &(listings->price),listingCount*sizeof(string));
    assignMemory(memory, (void **) &(listings->title),listingCount*sizeof(string));
    
    //parse listing buffer (listing file content)
    for (i=0; i<fileLength && count< listingCount; i++) {       //loop through entire listing buffer
        if(j==0 && fileCString[i]!='\r'){   //begining of new entry/listing/line
            listings->entry[count].cString=&(fileCString[i]);
        }
        if (fileCString[i]=='\n') {     //end of an entry/listing/line
            listings->entry[count].length=j;
            j=0;
            count++;
        }
        else if(fileCString[i]=='"'){   //start of a header
            j++;
            i++;
            if (cStringCmp(&(fileCString[i]), "price\":\"")==8) {
                j+=8;
                i+=8;
                temp=&(listings->price[count]);
            }
            else if (cStringCmp(&(fileCString[i]), "title\":\"")==8) {
                j+=8;
                i+=8;
                temp=&(listings->title[count]);
            }
            else if (cStringCmp(&(fileCString[i]), "currency\":\"")==11) {
                j+=11;
                i+=11;
                temp=&(listings->currency[count]);
            }
            else if (cStringCmp(&(fileCString[i]), "manufacturer\":\"")==15) {
                j+=15;
                i+=15;
                temp=&(listings->manufacturer[count]);
            }
            else{break;}
            temp->cString=&(fileCString[i]);
            for (k=0; (i+k)<fileLength && fileCString[i+k]!=0 && (fileCString[i+k]!='"' || fileCString[i+k-1]=='\\') && fileCString[i+k]!='\n' && fileCString[i+k]!='\r'; k++) {}//find end of property (property length)
            j+=k+1;
            i+=k;
            temp->length=k;
        }
        else{
            if(fileCString[i]!='\r'){   //other characters not cared about (",{}" and what not)
                j++;
            }
        }
    }
    return 0;
}