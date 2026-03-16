#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assertions.h"
#include "../built/poly_funcs.h"
#include "../built/sys_funcs.h"
#include "testing.h"

TEST(test_char_concatination) {
    // Тест 1: Простая конкатенация
    {
        poly_str* str1 = create_string("Hello", char_concat, char_substr, char_to_wchar_recode, 5, 6, 0);
        poly_str* str2 = create_string(" World", char_concat, char_substr, char_to_wchar_recode, 6, 7, 0);
        poly_str* result = string_concatination(str1, str2);
        
        assert(result != NULL);
        assert(strcmp((char*)result->data, "Hello World") == 0);
        assert(result->length == 11);
        
        free(str1->data); free(str1);
        free(str2->data); free(str2);
        free(result->data); free(result);
    }
    
    // Тест 2: Передача NULL
    {
        poly_str* result = string_concatination(NULL, NULL);
        assert(result == NULL);
        
        result = string_substr(NULL, 0, 5);
        assert(result == NULL);
        
        result = string_recode(NULL);
        assert(result == NULL);
    }
    
    // Тест 3: Конкатенация с пустой строкой
    {
        poly_str* str1 = create_string("", char_concat, char_substr, char_to_wchar_recode, 0, 1, 0);
        poly_str* str2 = create_string("Hello", char_concat, char_substr, char_to_wchar_recode, 5, 6, 0);
        poly_str* result = string_concatination(str1, str2);
        
        assert(result != NULL);
        assert(strcmp((char*)result->data, "Hello") == 0);
        
        free(str1->data); free(str1);
        free(str2->data); free(str2);
        free(result->data); free(result);
    }
}

TEST(test_wchar_concatination) {
    // Тест 1: Конкатенация широких строк
    {
        wchar_t* ws1 = L"Привет";
        wchar_t* ws2 = L" Мир";
        poly_str* str1 = create_string(ws1, wchar_concat, wchar_substr, wchar_to_char_recode, wcslen(ws1), wcslen(ws1)+1, 1);
        poly_str* str2 = create_string(ws2, wchar_concat, wchar_substr, wchar_to_char_recode, wcslen(ws2), wcslen(ws2)+1, 1);
        poly_str* result = string_concatination(str1, str2);
        
        assert(result != NULL);
        assert(result->length == 10);
        
        free(str1->data); free(str1);
        free(str2->data); free(str2);
        free(result->data); free(result);
    }
    
    // Тест 2: Передача NULL
    {
        poly_str* result = string_concatination(NULL, NULL);
        assert(result == NULL);
        
        result = string_substr(NULL, 0, 5);
        assert(result == NULL);
        
        result = string_recode(NULL);
        assert(result == NULL);
    }
}

TEST(test_char_substr) {
    {
        poly_str* str = create_string("Hello World", char_concat, char_substr, char_to_wchar_recode, 11, 12, 0);
        poly_str* result = string_substr(str, 0, 5);
        
        assert(result != NULL);
        assert(strcmp((char*)result->data, "Hello") == 0);
        assert(result->length == 5);
        
        free(str->data); free(str);
        free(result->data); free(result);
    }
}

TEST(test_wchar_substr) {
    {
        wchar_t* ws = L"Привет Мир";
        poly_str* str = create_string(ws, wchar_concat, wchar_substr, wchar_to_char_recode, wcslen(ws), wcslen(ws)+1, 1);
        poly_str* result = string_substr(str, 0, 6);
        
        assert(result != NULL);
        assert(result->length == 6);
        
        free(str->data); free(str);
        free(result->data); free(result);
    }
}

TEST(test_char_to_wchar_recode) {
    {
        poly_str* str = create_string("Hello", char_concat, char_substr, char_to_wchar_recode, 5, 6, 0);
        poly_str* result = string_recode(str);
        
        assert(result != NULL);
        assert(result->length == 5);
        
        free(str->data); free(str);
        if(result) { free(result->data); free(result); }
    }
}

TEST(test_wchar_to_char_recode) {
    {
        wchar_t* ws = L"Test";
        poly_str* str = create_string(ws, wchar_concat, wchar_substr, wchar_to_char_recode, wcslen(ws), wcslen(ws)+1, 1);
        poly_str* result = string_recode(str);
        
        assert(result != NULL);
        assert(result->length == 4);
        
        free(str->data); free(str);
        if(result) { free(result->data); free(result); }
    }
}

TEST(test_create_string) {
    // Тест 1
    {
        poly_str* str1 = create_string("Test", char_concat, char_substr, char_to_wchar_recode, 4, 5, 0);
        
        assert(str1 != NULL);
        assert(str1->length == 4);
        assert(str1->capacity == 5);
        assert(strcmp((char*)str1->data, "Test") == 0);
        
        free(str1->data); free(str1);
    }
    
    // Тест 2
    {
        wchar_t* ws = L"Тест";
        poly_str* str2 = create_string(ws, wchar_concat, wchar_substr, wchar_to_char_recode, wcslen(ws), wcslen(ws)+1, 1);
        
        assert(str2 != NULL);
        assert(str2->length == wcslen(ws));
        
        free(str2->data); free(str2);
    }
}

TEST_ENTRY_POINT