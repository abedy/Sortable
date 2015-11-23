//
//  Products.c
//  challenge
//
//  Created by ABEDY on 2015-11-15.
//  Copyright © 2015 David Pickard. All rights reserved.
//

#include "Products.h"
#define ISLETTER(_x) (((_x)>='a' && (_x)<='z') || ((_x)>='A' && (_x)<='Z'))
#define ISNUMBER(_x) ((_x)>='0' && (_x)<='9')
#define ISUCASE(_x) ((_x)>='A' && (_x)<='Z')
#define ISLCASE(_x) ((_x)>='a' && (_x)<='z')
#define ISIGNORED(_x) ((_x)==' ' || (_x)=='-' || (_x)=='_')
#define ISALPHANUMERIC(_x) (((_x)>='0' && (_x)<='9') || ((_x)>='a' && (_x)<='z') || ((_x)>='A' && (_x)<='Z'))
#define CASELESSEQUAL(_x,_y) ((_x)==(_y) || (((_x)>='A' && (_x)<='Z') && ((_x)+32)==(_y)) || (((_y)>='A' && (_y)<='Z') && ((_y)+32)==(_x)))
#define MAX(_x,_y) (((_y)>(_x))?(_y):(_x))
#define MIN(_x,_y) (((_y)<(_x))?(_y):(_x))

void associate(Products *products,Listings *listings,Memory *memory){
    unsigned long i,j,bmIndex;
    double bmVal,tempVal;
    
    const double threshold=0.001;
    
    string tempString;
    
    unsigned long tempMemLen=0;
    unsigned int *tempArray[2];
    unsigned long tempLength=0;
    
    
    
    for (i=0; i<listings->count; i++) {
        bmVal=0;
        bmIndex=0;
        
        for (j=0; j<products->count; j++) {
            tempVal=0;
            
            
            tempLength=MAX(MAX(MIN(products->name[j].length,listings->title[i].length),MIN(products->family[j].length,listings->title[i].length)),MIN(products->manufacturer[j].length, listings->manufacturer[i].length));
            if (tempLength>tempMemLen) {
                reallocateMemory(memory,(void **) &(tempArray[0]), tempMemLen*sizeof(unsigned int), tempLength*sizeof(unsigned int));
                reallocateMemory(memory,(void **) &(tempArray[1]), tempMemLen*sizeof(unsigned int), tempLength*sizeof(unsigned int));
                tempMemLen=tempLength;
            }
            //if list and product have the same manufacturer and product model appears in listing title
            if (normalizedLCSM(products->manufacturer[j], listings->manufacturer[i],tempArray)>=0.9 && modelInStr(products->model[j],listings->title[i])){
                
                
                tempString.cString=listings->title[i].cString;
                //filters english and french listings that are accessories for the products and not the products themselves
                if ((!(tempString.length=inStringStringLit(listings->title[i], " for ")) || modelInStr(products->model[j],tempString)) && (!(tempString.length=inStringStringLit(listings->title[i], " pour ")) || modelInStr(products->model[j],tempString))) {
                    
                    //measures similarity between product name and listing title (and product family and listing title, if product family is available)
                    tempVal=CSSumM(products->name[j], listings->title[i],tempArray)*((products->family[j].length>0)?CSSumM(products->family[j], listings->title[i],tempArray):1)+0.00001;
                }
                
            }
            
            if (tempVal>bmVal) {bmIndex=j;bmVal=tempVal;} //if the latest similarity measurement is larger than the previous best match then the best match value and index are updated
        }
        if (bmVal>threshold) {  //if the bestmatchValue is larger than the threshold, the listing is stored in the products match list
            
            //reallocate memory for larger match list (this transfers the content of memory to the new area)
            reallocateMemory(memory, (void **) &(products->listingEntries[bmIndex]), (products->listingCounts[bmIndex])*sizeof(string *), (products->listingCounts[bmIndex]+1)*sizeof(string *));
            reallocateMemory(memory, (void **) &(products->matchStrength[bmIndex]), (products->listingCounts[bmIndex])*sizeof(double), (products->listingCounts[bmIndex]+1)*sizeof(double));
            
            //add latest match to the match list
            products->listingEntries[bmIndex][products->listingCounts[bmIndex]]=&(listings->entry[i]);
            products->matchStrength[bmIndex][products->listingCounts[bmIndex]]=bmVal;
            
            (products->listingCounts[bmIndex])++;//add 1 to the products number of matches
            
        }
    }
    
    //temp
    freeAddress(memory, (void *) tempArray[0]);
    freeAddress(memory, (void *) tempArray[1]);
}


//sum of common substrings lengths(above 1) (insensitive to case and ignores certain characters)
double CSSumM(string stringA, string stringB,unsigned int *tempArray[2]){
    double total=0;
    unsigned long i,j=0;
    unsigned int temp;
    char a,b;
    //unsigned int *tempArray[2];
    string small, large;
    if (!(stringA.length && stringB.length)) {return 0;}
    if (stringA.length>stringB.length) {
        large=stringA;
        small=stringB;
    }
    else{
        small=stringA;
        large=stringB;
    }
    
    //tempArray[0]=malloc(small.length*sizeof(int));
    //tempArray[1]=malloc(small.length*sizeof(int));
    
    
    
    for (i=0; i<large.length; i++) {
        a=large.cString[i];
        for (j=0; j<small.length; j++) {
            b=small.cString[j];
            temp=0;
            //if (a==b || ((a>='a' && a<='z') && (a-32==b)) || ((a>='A' && a<='Z') && (a+32==b))) {
            //if (caselessEqual(a, b)) {
            if (CASELESSEQUAL(a, b)) {
                if (i==0 || j==0) {
                    temp=1;
                }
                else{
                    temp=tempArray[(i-1)%2][j-1]+1;
                }
                if (temp!=1) {
                    total+=temp;
                }
            }
            else{
                //if ((a==' ' || a=='-' || a=='_') && i>0) {
                //if(isIgnored(a) && i>0){
                if(ISIGNORED(a) && i>0){
                    temp=tempArray[(i-1)%2][j];
                }
                //else if ((b==' ' || b=='-' || b=='_') && j>0){
                //else if(isIgnored(b) && j>0){
                else if(ISIGNORED(b) && j>0){
                    temp=tempArray[i%2][j-1];
                }
            }
            tempArray[i%2][j]=temp;
        }
    }
    
    
    
    //free(tempArray[0]);
    //free(tempArray[1]);
    
    
    
    
    
    j=0;
    for (i=0; i<small.length; i++) {
        if (small.cString[i]) {
            j++;
        }
    }
    if (j>=small.length) {
        return total/small.length;
    }
    else{
        return total/(small.length-j);
    }
}



//normalized longest common substring (insensitive to case and ignores certain characters)
double normalizedLCSM(string stringA, string stringB,unsigned int *tempArray[2]){
    double longest=0;
    unsigned long i,j=0;
    unsigned int temp;
    char a,b;
    //unsigned int *tempArray[2];
    string small, large;
    if (!(stringA.length && stringB.length)) {return 0;}
    if (stringA.length>stringB.length) {
        large=stringA;
        small=stringB;
    }
    else{
        small=stringA;
        large=stringB;
    }
    
    //tempArray[0]=malloc(small.length*sizeof(int));
    //tempArray[1]=malloc(small.length*sizeof(int));
    
    
    
    for (i=0; i<large.length; i++) {
        a=large.cString[i];
        for (j=0; j<small.length; j++) {
            b=small.cString[j];
            temp=0;
            //if (a==b || ((a>='a' && a<='z') && (a-32==b)) || ((a>='A' && a<='Z') && (a+32==b))) {
            //if (caselessEqual(large.cString[i], small.cString[j])) {
            //if (caselessEqual(a, b)) {
            if (CASELESSEQUAL(a, b)) {
                //if (large.cString[i]==small.cString[j] || ((large.cString[i]>='a' && large.cString[i]<='z') && (large.cString[i]-32==small.cString[j])) || ((large.cString[i]>='A' && large.cString[i]<='Z') && (large.cString[i]+32==small.cString[j]))) {
                
                if (i==0 || j==0) {
                    temp=1;
                }
                else{
                    temp=tempArray[(i-1)%2][j-1]+1;
                }
                if (temp>longest) {
                    longest=temp;
                }
            }
            else{
                //if ((a==' ' || a=='-' || a=='_') && i>0) {
                //if(isIgnored(a) && i>0){
                if(ISIGNORED(a) && i>0){
                    //if(isIgnored(large.cString[i]) && i>0){
                    //if((large.cString[i]==' ' || large.cString[i]=='-' || large.cString[i]=='_') && i>0){
                    temp=tempArray[(i-1)%2][j];
                }
                //else if ((b==' ' || b=='-' || b=='_') && j>0){
                //else if(isIgnored(b) && j>0){
                else if(ISIGNORED(b) && j>0){
                    //else if(isIgnored(small.cString[j]) && j>0){
                    //else if((small.cString[j]==' ' || small.cString[j]=='-' || small.cString[j]=='_') && j>0){
                    temp=tempArray[i%2][j-1];
                }
            }
            tempArray[i%2][j]=temp;
        }
    }
    
    
    
    //free(tempArray[0]);
    //free(tempArray[1]);
    
    
    
    
    
    j=0;
    for (i=0; i<small.length; i++) {
        if (small.cString[i]) {
            j++;
        }
    }
    if (j>=small.length) {
        return longest/small.length;
    }
    else{
        return longest/(small.length-j);
    }
}


int modelInStr(string model,string title){
    unsigned long start,end,i,j;
    long k;
    //char a,b;
    unsigned long tLength;
    char *tcString;
    for(start=0; start<model.length && (model.cString[start]==' ' || model.cString[start]=='-' || model.cString[start]=='_');start++){}//trim model num beginning
    for(end=model.length-1;end>=start && (model.cString[end]==' ' || model.cString[end]=='-' || model.cString[end]=='_');end--){}//trim model num end
    if(end < start){return 0;}
    tLength=end-start+1;
    tcString=&model.cString[start];
    for(i=0;i<title.length;i++){
        
        if(CASELESSEQUAL(title.cString[i], tcString[0])){
            k=0;
            for(j=1;(j+k)<tLength && (i+j)<title.length;j++){
                if (!CASELESSEQUAL(title.cString[i+j], tcString[j+k])) {
                    if(ISIGNORED(title.cString[i+j])){k--;}//next loop change a but not b
                    else if(ISIGNORED(tcString[j+k])){j--;k++;}//next loop change b but not a
                    else{break;} //not a match
                }
            }
            
            //        if(caselessEqual(title.cString[i], tcString[0])){
            //            k=0;
            //            for(j=1;(j+k)<tLength && (i+j)<title.length;j++){
            //                if (!caselessEqual(title.cString[i+j], tcString[j+k])) {
            //                    if(isIgnored(title.cString[i+j])){k--;}//next loop change a but not b
            //                    else if(isIgnored(tcString[j+k])){j--;k++;}//next loop change b but not a
            //                    else{break;} //not a match
            //                }
            //            }
            //if((j+k)==tLength && (!isNumber(tcString[0]) || i==0 || !(isAlphanumeric(title.cString[i-1]))) && (!isNumber(tcString[tLength-1])|| (i+j)==title.length || !(isAlphanumeric(title.cString[i+j])))){
            
            if((j+k)==tLength && (!ISNUMBER(tcString[0]) || i==0 || !(ISALPHANUMERIC(title.cString[i-1]))) && (!ISNUMBER(tcString[tLength-1])|| (i+j)==title.length || !(ISALPHANUMERIC(title.cString[i+j])))){
                return 1;
            }
        }
    }
    return 0;
}




//takes a pointer to products structure, a path to a file to read, and a pointer to memory (management) structure
int populateProductsFromFile(Products *products,const char *path,Memory *memory){
    unsigned int i;
    unsigned int j=0;
    unsigned int k;
    unsigned int count=0;
    string *temp;
    struct stat fileStat;
    
    unsigned long fileLength;
    char *fileCString;
    unsigned long productCount;
    FILE* f;
    
    //if true something has gone wrong accessing the file
    if (stat(path, &fileStat) || !(f=fopen (path, "rb"))){return -1;}
    
    assignMemory(memory, (void **) &(products->file.cString),fileStat.st_size); //allocate memory for storing the file contents
    products->file.length = fread(products->file.cString,1,fileStat.st_size,f); //read file
    fclose(f); //close file
    
    products->count=charCount(products->file, '\n'); //number of entries set by number of newline characters (not foolproof but it works)

    //makes code easier to read
    fileLength=products->file.length;
    fileCString=products->file.cString;
    productCount=products->count;
    

    assignMemory(memory, (void **) &(products->date),productCount*sizeof(string));
    assignMemory(memory, (void **) &(products->name),productCount*sizeof(string));
    assignMemory(memory, (void **) &(products->entry),productCount*sizeof(string));
    assignMemory(memory, (void **) &(products->family),productCount*sizeof(string));
    assignMemory(memory, (void **) &(products->manufacturer),productCount*sizeof(string));
    assignMemory(memory, (void **) &(products->model),productCount*sizeof(string));
    assignMemory(memory, (void **) &(products->listingCounts),productCount*sizeof(long));
    assignMemory(memory, (void **) &(products->listingEntries),productCount*sizeof(string **));
    assignMemory(memory, (void **) &(products->matchStrength),productCount*sizeof(double *));
    
    for (i=0; i<productCount; i++) {    //set listing entries, listingCounts and matchstrength to null
        products->listingEntries[i]=0;
        products->listingCounts[i]=0;
        products->matchStrength[i]=0;
    }

    //parse product buffer (product file content)
    for (i=0; i<fileLength && count< productCount; i++) {       //loop through entire product buffer, i is index within product buffer, j is index within product entry
        if(j==0 && fileCString[i]!='\r'){   //begining of new entry/product/line
            products->family[count].length=0;
            products->entry[count].cString=&(fileCString[i]);
        }
        if (fileCString[i]=='\n') {         //end of an entry/product/line
            products->entry[count].length=j;
            j=0;
            count++;
        }
        else if(fileCString[i]=='"'){//start of a header
            j++;
            i++;
            //if ((i+8)>=fileLength) {break;}
            //else
            if (cStringCmp(&(fileCString[i]), "model\":\"")==8) {
                j+=8;
                i+=8;
                temp=&(products->model[count]);
            }
            //else if ((i+9)>=fileLength) {break;}
            else if (cStringCmp(&(fileCString[i]), "family\":\"")==9) {
                j+=9;
                i+=9;
                temp=&(products->family[count]);
            }
            //else if ((i+15)>=fileLength) {break;}
            else if (cStringCmp(&(fileCString[i]), "product_name\":\"")==15) {
                j+=15;
                i+=15;
                temp=&(products->name[count]);
            }
            else if (cStringCmp(&(fileCString[i]), "manufacturer\":\"")==15) {
                j+=15;
                i+=15;
                temp=&(products->manufacturer[count]);
            }
            //else if ((i+17)>=fileLength) {break;}
            else if (cStringCmp(&(fileCString[i]), "announced-date\":\"")==17) {
                j+=17;
                i+=17;
                temp=&(products->date[count]);
            }
            else{break;}
            temp->cString=&(fileCString[i]);
            for (k=0; (i+k)<fileLength && fileCString[i+k]!=0 && (fileCString[i+k]!='"' || fileCString[i+k-1]=='\\') && fileCString[i+k]!='\n' && fileCString[i+k]!='\r'; k++) {} //find end of property (property length)
            j+=k+1;
            i+=k;
            temp->length=k;
        }
        else{
            if(fileCString[i]!='\r'){       //other characters not cared about (",{}" and what not)
                j++;
            }
        }
    }
    return 0;
}
