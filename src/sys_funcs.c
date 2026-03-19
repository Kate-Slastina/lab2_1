#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <poly_funcs.h>
#include <sys_funcs.h>

char* read_char(size_t* length, size_t* capacity){
    *capacity = 16;
    *length = 0;
    char *buffer = malloc((*capacity) * sizeof(char));
    if(buffer == NULL){
        printf("Не удалось выделить память на буффер.\n");
        return NULL;
    }
    int c;
    while((c = getchar()) != '\n' && c != EOF){
        if (*length + 1 >= *capacity) {
            *capacity *= 2;
            char *new_buffer = realloc(buffer, (*capacity) * sizeof(char));
            if (!new_buffer) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[(*length)++] = c;
    }
    buffer[(*length)] = '\0';
    return buffer;
}

wchar_t* read_wchart(size_t* length, size_t* capacity){
    *capacity = 16;
    *length = 0;
    
    // Выделяем буфер под wchar_t
    wchar_t *buffer = malloc((*capacity) * sizeof(wchar_t));
    if(buffer == NULL){
        printf("Не удалось выделить память на буффер.\n");
        return NULL;
    }
    
    char temp[1024]; 
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        free(buffer);
        return NULL;
    }
    
    size_t temp_len = strlen(temp);
    while (temp_len > 0 && (temp[temp_len - 1] == '\n' || temp[temp_len - 1] == '\r')) {
        temp[temp_len - 1] = '\0';
        temp_len--;
    }
    
    // Конвертируем UTF-8 строку в wchar_t
    *length = mbstowcs(buffer, temp, *capacity);
    if (*length == (size_t)-1) {
        // Ошибка конвертации
        free(buffer);
        return NULL;
    }
    
    buffer[*length] = 0;
    *capacity = *length + 1;
    
    return buffer;
}

int str_printf(void* string, int cond){ // 0 - char, 1 - wchar_t
    if(cond == 0){
        return printf("%s", (char*)string);
    }
    return wprintf(L"%ls", (wchar_t*)string);
}

int clear_input(){
    int c, counter = 0;
    while ((c = getchar()) != '\n' && c != EOF){
        counter++;
    }
    return counter;
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}