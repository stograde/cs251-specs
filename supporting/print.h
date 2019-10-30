#ifndef _PRINT_H_
#define _PRINT_H_

// these print functions will send information to the screen
void print_char(char c);
void print_short(short s);
void print_int(int i);
void print_unsigned_int(unsigned int i);
void print_long(long l);
void print_long_long(long long l);
void print_float(float f);
void print_double(double d);
void print(const char *str);

// generate a random number between zero and one
double random_number();

// get input from the user, return an int if possible
int input_int();

// the following functions are intended to help identify errors;
//   you can probably ignore them for now!
void error_for_print_(const char *);
template<class T> void print_char(T t) { error_for_print_("char"); }
template<class T> void print_short(T t) { error_for_print_("short"); }
template<class T> void print_int(T t) { error_for_print_("int"); }
template<class T> void print_unsigned_int(T t) { error_for_print_("unsigned_int"); }
template<class T> void print_long(T t) { error_for_print_("long"); }
template<class T> void print_size_t(T t) { error_for_print_("size_t"); }
template<class T> void print_long_long(T t) { error_for_print_("long_long"); }
template<class T> void print_float(T t) { error_for_print_("float"); }
template<class T> void print_double(T t) { error_for_print_("double"); }
template<class T> void print(T t) { error_for_print_("char*"); }

#endif // _PRINT_H_
