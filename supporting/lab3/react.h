#ifndef _REACT_H_
#define _REACT_H_

#include<iostream>
#include<map>
#include<vector>
#include<string>
using namespace std;

#define USE_SHARED_DATA

extern int outgoing_mem_size;

extern char *global_mem, *global_shared_mem;
extern string global_yaml;
extern bool skip_data_server, allow_push_from_client;

bool just_starting();
void init(bool _use_cache = false);
void quit(const char *c = 0);
void get_shared_data();

class str_or_num {
  string str;
  long long num_i;
  double num_d;
  const void *ptr;
  enum {STRING, DOUBLE, LONG, POINTER} kind;
public:
  str_or_num(string _s = "") : str(_s), kind(STRING) {}
  str_or_num(const char *_s) : str(_s), kind(STRING) {}
  template<class T> str_or_num(T *_t) : ptr(static_cast<const void *>(_t)), kind(POINTER) {}
  str_or_num(char _i) : num_i(_i), kind(LONG) {}
  str_or_num(short _i) : num_i(_i), kind(LONG) {}
  str_or_num(int _i) : num_i(_i), kind(LONG) {}
  str_or_num(unsigned int _i) : num_i(_i), kind(LONG) {}
  str_or_num(long _i) : num_i(_i), kind(LONG) {}
  str_or_num(size_t _i) : num_i(_i), kind(LONG) {}
  str_or_num(long long _i) : num_i(_i), kind(LONG) {}
  str_or_num(float _d) : num_d(_d), kind(DOUBLE) {}
  str_or_num(double _d) : num_d(_d), kind(DOUBLE) {}
  string as_string() const;
};

typedef map<string, str_or_num> Dict;
typedef vector<str_or_num> sn_vec;
/*
struct Dict {
  map<string, str_or_num> d;
  Dict(const map<string, str_or_num> &_d = {}) : d(_d) {}
  string operator[](const string &k) {
    if (d.end() == d.find(k)) {
      cerr << "Warning:  did not find key " << k << " in Dict." << endl;
      return string();
    } else return d[k].as_string();
  }
};
*/
ostream &operator <<(ostream &os, const str_or_num &s);
ostream &operator <<(ostream &os, const map<string, string> &d);
ostream &operator <<(ostream &os, const Dict &d);
ostream &operator <<(ostream &os, const sn_vec &v);

int length_of(const char *s, int index=0);
int length_of(int index, bool shared=false);

void add_yaml(const char *fname, const Dict &m = {}, const map<string, string> &props = {});
// extern void print(const char *fmt, const map<string, string> &m = {});
//void print(const char *fmt, const Dict &m = {});
void print(const char *fmt, const sn_vec &v = {});
void print_char(char c);
void print_unsigned_char(unsigned char c);
void print_short(short s);
void print_int(int i);
void print_unsigned_int(unsigned int i);
void print_long(long l);
void print_size_t(size_t x);
void print_long_long(long long l);
void print_float(float f);
void print_double(double d);
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

// void print_at(int index, string fmt, const Dict &m = {});
// void print_at(int index, const char fmt[], const Dict &m = {});
// void print_at(int index, bool shared, string fmt, const Dict &m = {});
// void print_at(int index, bool shared, const char fmt[], const Dict &m = {});
void print_at(int index, string fmt, const sn_vec &v = {});
void print_at(int index, const char fmt[], const sn_vec &v = {});
void print_at(int index, bool shared, string fmt, const sn_vec &v = {});
void print_at(int index, bool shared, const char fmt[], const sn_vec &v = {});

bool &bool_at(int index, bool shared=false);
char &char_at(int index, bool shared=false);
unsigned char &uchar_at(int index, bool shared=false);
short &short_at(int index, bool shared=false);
unsigned short &unsigned_short_at(int index, bool shared=false);
int &int_at(int index, bool shared=false);
unsigned int &unsigned_int_at(int index, bool shared=false);
long &long_at(int index, bool shared=false);
size_t &size_t_at(int index, bool shared=false);
long long &long_long_at(int index, bool shared=false);
float &float_at(int index, bool shared=false);
double &double_at(int index, bool shared=false);

// new functions for obtaining and writing data into global_mem
bool get_bool_at(int index, bool shared=false);
char get_char_at(int index, bool shared=false);
unsigned char get_uchar_at(int index, bool shared=false);
short get_short_at(int index, bool shared=false);
unsigned short get_unsigned_short_at(int index, bool shared=false);
int get_int_at(int index, bool shared=false);
unsigned int get_unsigned_int_at(int index, bool shared=false);
long get_long_at(int index, bool shared=false);
long long get_long_long_at(int index, bool shared=false);
float get_float_at(int index, bool shared=false);
double get_double_at(int index, bool shared=false);

void put_bool_at(int index, bool val, bool shared=false);
void put_char_at(int index, char val, bool shared=false);
void put_uchar_at(int index, unsigned char val, bool shared=false);
void put_short_at(int index, short val, bool shared=false);
void put_unsigned_short_at(int index, unsigned short val, bool shared=false);
void put_int_at(int index, int val, bool shared=false);
void put_unsigned_int_at(int index, unsigned int val, bool shared=false);
void put_long_at(int index, long val, bool shared=false);
void put_long_long_at(int index, long long val, bool shared=false);
void put_float_at(int index, float val, bool shared=false);
void put_double_at(int index, float val, bool shared=false);

void copy_from_len_to(int from, bool from_shared, int len, int to, bool to_shared=false);
void copy_from_len_to(int from, const char *raw, int len, int to, bool to_shared);

int index_of(const char *s, const char *query);
int index_of(const char *s, int start, const char *query);
bool in(const char *s, const char *query);
bool in(const char *s, int start, const char *query);

// extern int length_of(const char *s, int index);
int as_integer(const string &s);
int as_integer(const char *s);
int as_integer(int index);

long long current_time();
const char *time_of_day(long t_raw);
const char *time_of_day(const char *mem, int index);
double random_number();
void fetch(const char *url, int index = 0, int len = -1);
// const char *fetch(const char *url, const char *query=0);
// const char *fetch(string url, string query = "");
// these functions assume ppm format images
void read_image(const char *filename);
void get_image_dimensions(int &width, int &height);
void set_image_dimensions(int width, int height);
void write_image(const char *filename);
void read_image(const char *filename, char **image, int width, int height);
void get_image_dimensions(const char *filename, int &width, int &height);
void write_image(const char *filename, char **image, int width, int height);

const char *get_user_name();
bool received_event();
const char *get_event_type();
bool has_event_id();
const char *get_event_id();
bool event_id_is(const char *, int i = -1);
int get_event_index();
int get_event_item_index();
int get_event_point_x();
int get_event_point_y();
int get_event_start_point_x();
int get_event_start_point_y();
int get_event_left_drag();
int get_event_top_drag();

#endif // _REACT_H_
