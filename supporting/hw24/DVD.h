#ifndef _DVD_H_
#define _DVD_H_

#include <string.h>

#include <iostream>
using namespace std;

class DVD {
    unsigned int id;
    char* title;
    char* director;

   public:
    DVD(unsigned int i, char* t, char* dir);

    DVD();

    DVD(const DVD& d);

    ~DVD();

    unsigned int getId() { return id; }

    char* getTitle() { return title; }

    char* getDirector() { return director; }

    void display();

    void setId(unsigned int i) { id = i; }

    void setTitle(const char* t);

    void setDirector(const char* d);

    DVD& operator=(const DVD& dvd);
};

#endif  // _DVD_H_