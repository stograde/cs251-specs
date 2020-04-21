#include "DVD.h"

DVD::DVD(unsigned int i, char* t, char* dir) : id(i) {
    title = new char[strlen(t)];
    strcpy(title, t);

    director = new char[strlen(dir)];
    strcpy(director, dir);
}

DVD::DVD() : id(0) {
    title = new char[50];
    director = new char[50];
}

DVD::DVD(const DVD& d) : id(d.id) {
    title = new char[strlen(d.title)];
    strcpy(title, d.title);

    director = new char[strlen(d.director)];
    strcpy(director, d.director);
}

DVD::~DVD() {
    delete title;
    delete director;
}

void DVD::display() {
    cout << '[' << id << ".  ";
    cout << title << '/';
    cout << director << ']' << endl;
}

void DVD::setTitle(const char* t) {
    delete title;
    title = new char[strlen(t)];
    strcpy(title, t);
}

void DVD::setDirector(const char* d) {
    delete director;
    director = new char[strlen(d)];
    strcpy(director, d);
}

DVD& DVD::operator=(const DVD& dvd) {
    id = dvd.id;

    delete title;
    title = new char[strlen(dvd.title)];
    strcpy(title, dvd.title);

    delete director;
    director = new char[strlen(dvd.director)];
    strcpy(director, dvd.director);

    return *this;
}
