#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 
#include <poly_funcs.h>
#define max(a, b) ((a) > (b) ? (a) : (b))


//ПРОТОТИПЫ---------------------------------------------------------------------------------------------------------------------------------------
poly_str* char_concat(poly_str* str1, poly_str* str2);
poly_str* wchar_concat(poly_str* str1, poly_str* str2);

poly_str* char_substr(poly_str* str, size_t start, size_t length);
poly_str* wchar_substr(poly_str* str, size_t start, size_t length);

poly_str* char_to_wchar_recode(poly_str* str);
poly_str* wchar_to_char_recode(poly_str* str);


//РЕАЛИЗАЦИЯ--------------------------------------------------------------------------------------------------------------------------------------
poly_str* char_concat(poly_str* str1, poly_str* str2){
    if(str1 == NULL || str2 == NULL || str1->data == NULL || str2->data == NULL){
        printf("Ошибка передачи данных в char_concat\n");
        return NULL;
    }
    char* concatinated_str = malloc((str1->length + str2->length + 1) * sizeof(char));
    if(concatinated_str == NULL){
        printf("Ошибка выделения памяти для concatinated_str\n");
        return NULL;
    }
    for(size_t i = 0; i < str1->length; i++){
        concatinated_str[i] = ((char*)str1->data)[i];
    }

    for(size_t i = str1->length; i < str1->length + str2->length; i++){
        concatinated_str[i] = ((char*)str2->data)[i - str1->length];
    }
    concatinated_str[str1->length + str2->length] = '\0';
    return create_string(concatinated_str, char_concat, char_substr, char_to_wchar_recode, str1->length + str2->length, str1->length + str2->length + 1, 0);
}

poly_str* wchar_concat(poly_str* str1, poly_str* str2){
    //wprintf(L"Строка 1:%ls, длина строки 1: %zu", (wchar_t*)str1->data, str1->length);
    //wprintf(L"\nСтрока 2:%ls, длина строки 2: %zu\n", (wchar_t*)str2->data, str2->length);
    if(str1 == NULL || str2 == NULL || str1->data == NULL || str2->data == NULL){
        printf("Ошибка передачи данных в wchar_concat\n");
        return NULL;
    }
    wchar_t* concatinated_str = malloc((str1->length + str2->length + 1) * sizeof(wchar_t));
    if(concatinated_str == NULL){
        printf("Ошибка выделения памяти для concatinated_str\n");
        return NULL;
    }
    for(size_t i = 0; i < str1->length; i++){
        concatinated_str[i] = ((wchar_t*)str1->data)[i];
    }

    for(size_t i = str1->length; i < str1->length + str2->length; i++){
        concatinated_str[i] = ((wchar_t*)str2->data)[i - str1->length];
    }
    concatinated_str[str1->length + str2->length] = L'\0';
    //wprintf(L"\nСтрока итог:%ls\n", (wchar_t*)concatinated_str);
    return create_string(concatinated_str, wchar_concat, wchar_substr, wchar_to_char_recode, str1->length + str2->length, str1->length + str2->length + 1, 1);
}

poly_str* char_substr(poly_str* str, size_t start, size_t length){
    if(str == NULL || str->data == NULL){
        printf("Ошибка передачи данных в char_substr\n");
        return NULL;
    }
    if(start + length > str->length){
        printf("Выход за границы строки в char_substr\n");
        return NULL;
    }
    char* substr = malloc((length + 1) * sizeof(char));
    if(substr == NULL){
        printf("Ошибка выделения памяти для substr\n");
        return NULL;
    }
    for(size_t i = start; i < start + length; i++){
        substr[i - start] = ((char*)str->data)[i];
    }
    substr[length] = '\0';
    return create_string(substr, char_concat, char_substr, char_to_wchar_recode, length, length + 1, 0);
}

poly_str* wchar_substr(poly_str* str, size_t start, size_t length){
    if(str == NULL || str->data == NULL){
        printf("Ошибка передачи данных в wchar_substr\n");
        return NULL;
    }
    if(start + length > str->length){
        printf("Выход за границы строки в wchar_substr\n");
        return NULL;
    }
    wchar_t* substr = malloc((length + 1) * sizeof(wchar_t));
    if(substr == NULL){
        printf("Ошибка выделения памяти для substr\n");
        return NULL;
    }
    for(size_t i = start; i < start + length; i++){
        substr[i - start] = ((wchar_t*)str->data)[i];
    }
    substr[length] = L'\0';
    return create_string(substr, wchar_concat, wchar_substr, wchar_to_char_recode, length, length + 1, 1);
}

poly_str* wchar_to_char_recode(poly_str* str){
    if (str == NULL || str->data == NULL) {
        printf("Ошибка передачи данных в wchar_to_char_recode\n");
        return NULL;
    }

    wchar_t* src = (wchar_t*)str->data;
    size_t src_len = str->length;
    
    size_t byte_count = 0;
    size_t i = 0;
    
    while (i < src_len) {
        uint32_t codepoint = (uint32_t)src[i];

        if (codepoint >= 0xD800 && codepoint <= 0xDBFF){//чек является ли первым байтом пары
            if (i + 1 >= src_len) {
                printf("Обрезанная суррогатная пара\n");
                return NULL;
            }
            
            uint32_t low = (uint32_t)src[++i];
            if (low < 0xDC00 || low > 0xDFFF) {
                printf("Невалидный Low Surrogate\n");
                return NULL;
            }
            
            codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
        }
        i++;
        
        // кол-во байт, нужные выделить чару
        if (codepoint < 0x80) {
            byte_count += 1;
        } else if (codepoint < 0x800) {
            byte_count += 2;
        } else if (codepoint < 0x10000) {
            byte_count += 3;
        } else {
            byte_count += 4;
        }
    }

    char* result_str = malloc((byte_count + 1) * sizeof(char));
    if (result_str == NULL) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }

    size_t result_str_idx = 0;
    i = 0;

    while (i < src_len) {
        uint32_t codepoint = (uint32_t)src[i];
        if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
            uint32_t low = (uint32_t)src[i + 1];
            codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
            i += 2;
        } else {
            i += 1;
        }

        //тут часть с документации
        if (codepoint < 0x80) {
            // 1 байт: 0xxxxxxx
            result_str[result_str_idx++] = (char)codepoint;
        }
        else if (codepoint < 0x800) {
            // 2 байта: 110xxxxx 10xxxxxx
            result_str[result_str_idx++] = (char)(0xC0 | (codepoint >> 6));
            result_str[result_str_idx++] = (char)(0x80 | (codepoint & 0x3F));
        }
        else if (codepoint < 0x10000) {
            // 3 байта: 1110xxxx 10xxxxxx 10xxxxxx
            result_str[result_str_idx++] = (char)(0xE0 | (codepoint >> 12));
            result_str[result_str_idx++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
            result_str[result_str_idx++] = (char)(0x80 | (codepoint & 0x3F));
        }
        else {
            // 4 байта: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            result_str[result_str_idx++] = (char)(0xF0 | (codepoint >> 18));
            result_str[result_str_idx++] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
            result_str[result_str_idx++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
            result_str[result_str_idx++] = (char)(0x80 | (codepoint & 0x3F));
        }
    }
    result_str[result_str_idx] = '\0';

    return create_string(result_str, char_concat, char_substr, char_to_wchar_recode, byte_count, byte_count + 1, 0);
}

poly_str* char_to_wchar_recode(poly_str* str) {
    if (str == NULL || str->data == NULL) {
        printf("Ошибка передачи данных в char_to_wchar_recode\n");
        return NULL;
    }

    char* src = (char*)str->data;
    size_t src_len = str->length;
    size_t wchar_count = 0;
    size_t i = 0;
    
    while (i < src_len) {
        unsigned char byte = (unsigned char)src[i];
        uint32_t codepoint;
        int seq_len;
        
        if (byte < 0x80) {
            // 1 байт: 0xxxxxxx
            codepoint = byte;
            seq_len = 1;
        }
        else if ((byte & 0xE0) == 0xC0) {
            // 2 байта: 110xxxxx 10xxxxxx
            seq_len = 2;
            if (i + 1 >= src_len || ((unsigned char)src[i+1] & 0xC0) != 0x80) {
                printf("Обрезанная или невалидная UTF-8 последовательность (2 байта)\n");
                return NULL;
            }
            codepoint = ((byte & 0x1F) << 6) | ((unsigned char)src[i+1] & 0x3F);
            if (codepoint < 0x80) {
                printf("Слишком длинный UTF-8 код (2 байта)\n");
                return NULL;
            }
        }
        else if ((byte & 0xF0) == 0xE0) {
            // 3 байта: 1110xxxx 10xxxxxx 10xxxxxx
            seq_len = 3;
            if (i + 2 >= src_len || 
                ((unsigned char)src[i+1] & 0xC0) != 0x80 ||
                ((unsigned char)src[i+2] & 0xC0) != 0x80) {
                printf("Обрезанная или невалидная UTF-8 последовательность (3 байта)\n");
                return NULL;
            }
            codepoint = ((byte & 0x0F) << 12) | (((unsigned char)src[i+1] & 0x3F) << 6) | ((unsigned char)src[i+2] & 0x3F);
            if (codepoint < 0x800) {
                printf("Слишком длинный UTF-8 код (3 байта)\n");
                return NULL;
            }
            if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
                printf("Невалидная суррогатная кодпоинт в UTF-8\n");
                return NULL;
            }
        }
        else if ((byte & 0xF8) == 0xF0) {
            // 4 байта: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            seq_len = 4;
            if (i + 3 >= src_len || ((unsigned char)src[i+1] & 0xC0) != 0x80 ||
                ((unsigned char)src[i+2] & 0xC0) != 0x80 || ((unsigned char)src[i+3] & 0xC0) != 0x80) {
                printf("Обрезанная или невалидная UTF-8 последовательность (4 байта)\n");
                return NULL;
            }
            codepoint = ((byte & 0x07) << 18) | 
                       (((unsigned char)src[i+1] & 0x3F) << 12) | 
                       (((unsigned char)src[i+2] & 0x3F) << 6) | 
                       ((unsigned char)src[i+3] & 0x3F);
            if (codepoint < 0x10000 || codepoint > 0x10FFFF) {
                printf("Кодпоинт вне допустимого диапазона Unicode\n");
                return NULL;
            }
        }
        else {
            printf("Невалидный ведущий байт UTF-8: 0x%02X\n", byte);
            return NULL;
        }
        
        wchar_count += (codepoint >= 0x10000) ? 2 : 1;
        i += seq_len;
    }
    
    wchar_t* result_data = malloc((wchar_count + 1) * sizeof(wchar_t));
    if (result_data == NULL) {
        printf("Ошибка выделения памяти под wchar_t буфер\n");
        return NULL;
    }
    
    size_t result_idx = 0;
    i = 0;
    
    while (i < src_len) {
        unsigned char byte = (unsigned char)src[i];
        uint32_t codepoint;
        int seq_len;
        
        if (byte < 0x80) {
            codepoint = byte;
            seq_len = 1;
        }
        else if ((byte & 0xE0) == 0xC0) {
            seq_len = 2;
            codepoint = ((byte & 0x1F) << 6) | ((unsigned char)src[i+1] & 0x3F);
        }
        else if ((byte & 0xF0) == 0xE0) {
            seq_len = 3;
            codepoint = ((byte & 0x0F) << 12) | 
                       (((unsigned char)src[i+1] & 0x3F) << 6) | 
                       ((unsigned char)src[i+2] & 0x3F);
        }
        else {
            seq_len = 4;
            codepoint = ((byte & 0x07) << 18) | 
                       (((unsigned char)src[i+1] & 0x3F) << 12) | 
                       (((unsigned char)src[i+2] & 0x3F) << 6) | 
                       ((unsigned char)src[i+3] & 0x3F);
        }
        
        if (codepoint >= 0x10000) {
            uint32_t offset = codepoint - 0x10000;
            result_data[result_idx++] = (wchar_t)(0xD800 + (offset >> 10));
            result_data[result_idx++] = (wchar_t)(0xDC00 + (offset & 0x3FF));
        } else {
            result_data[result_idx++] = (wchar_t)codepoint;
        }
        
        i += seq_len;
    }

    result_data[result_idx] = L'\0';
    return create_string(result_data, wchar_concat, wchar_substr, wchar_to_char_recode, wchar_count, wchar_count + 1, 1);
}

poly_str* create_string(void* str, poly_str* (*concatination)(poly_str* s1, poly_str* s2),
                        poly_str* (*substr)(poly_str* str, size_t start, size_t length), 
                        poly_str* (*recode)(poly_str* str), 
                        size_t length, size_t capacity, int is_wide){

    poly_str* string = malloc(sizeof(poly_str));
    if (string == NULL) {
        return NULL;
    }
    // глубокое копирование
    if (is_wide == 1) {
        string->data = malloc((length + 1) * sizeof(wchar_t));
        if (!string->data) { free(string); return NULL; }
        wmemcpy((wchar_t*)string->data, (wchar_t*)str, length + 1);
    } else {
        string->data = malloc((length + 1) * sizeof(char));
        if (!string->data) { free(string); return NULL; }
        memcpy(string->data, (char*)str, length + 1);
    }
    //string->data = str;
    string->length = length;
    string->capacity = capacity;

    string->ops.concatination = concatination;
    string->ops.substr = substr;
    string->ops.recode = recode;

    return string;
}

poly_str* string_concatination(poly_str* string1, poly_str* string2){
    return string1->ops.concatination(string1, string2);
}

poly_str* string_substr(poly_str* str, size_t start, size_t length){
    return str->ops.substr(str, start, length);
}

poly_str* string_recode(poly_str* str){
    return str->ops.recode(str);
}