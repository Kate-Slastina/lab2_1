#ifndef POLY_FUNCS_H
#define POLY_FUNCS_H
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

typedef struct polystr poly_str;

typedef struct{
    poly_str* (*concatination)(poly_str* s1, poly_str* s2);
    poly_str* (*substr)(poly_str* str, size_t start, size_t length);
    poly_str* (*recode)(poly_str* str);
} str_operations;


typedef struct polystr{
    void* data;
    size_t length;
    size_t capacity;
    str_operations ops;
} poly_str;

poly_str* char_concat(poly_str* str1, poly_str* str2);
poly_str* wchar_concat(poly_str* str1, poly_str* str2);

poly_str* char_substr(poly_str* str, size_t start, size_t length);
poly_str* wchar_substr(poly_str* str, size_t start, size_t length);

poly_str* char_to_wchar_recode(poly_str* str);
poly_str* wchar_to_char_recode(poly_str* str);

poly_str* create_string(void* str, poly_str* (*concatination)(poly_str* s1, poly_str* s2),
                        poly_str* (*substr)(poly_str* str, size_t start, size_t length), 
                        poly_str* (*recode)(poly_str* str), 
                        size_t length, size_t capacity, int is_wide);

poly_str* string_concatination(poly_str* string1, poly_str* string2);
poly_str* string_substr(poly_str* str, size_t start, size_t length);
poly_str* string_recode(poly_str* str); 

#endif