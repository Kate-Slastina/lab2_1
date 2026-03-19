#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <text_blocks.h>
#include <sys_funcs.h>
#include <poly_funcs.h>

int main(){
    setlocale(LC_ALL, "");
    
    int check = 0, check2 = 0, choice = 0, left = 0, right = 0, start = 0;
    size_t mass_size = 1, capacity = 0, length = 0, mass_last_elem = 0;
    void* temp_str = NULL;
    
    poly_str** str_mass = malloc(mass_size * sizeof(poly_str*));
    int* additional_info = malloc(mass_size * sizeof(int));
    
    if(str_mass == NULL){
        printf("Не удалось выделить память на массив.\n");
        return 0;
    }
    
    while(choice != 6){
        do{
            main_operations_text(str_mass, additional_info, mass_last_elem);
            check = scanf("%d", &choice);
            if(choice < 1 || choice > 6){
                clear_screen();
                printf("Такого варианта не существует, введите заново\n");
            }
        }while(clear_input() != 0 || check != 1 || (choice < 1 || choice > 6));
        
        if((choice == 1 || choice == 2) && mass_last_elem >= mass_size){
            mass_size *= 2;
            poly_str** new_mass = realloc(str_mass, mass_size * sizeof(poly_str*));
            int* new_int_mass = realloc(additional_info, mass_size * sizeof(int));
            if (new_mass == NULL || new_int_mass == NULL){
                printf("Не удалось довыделить память\n");
                if(new_mass == NULL){ free(str_mass);}
                else{free(new_mass);}
                if(new_int_mass == NULL){ free(additional_info);}
                else{ free(new_int_mass);}
                return 0;
            }
            str_mass = new_mass;
            additional_info = new_int_mass;
        }
        
        switch(choice){
            case 1:
                printf("Введите строку (wchar): ");
                temp_str = read_wchart(&length, &capacity);
                
                if (temp_str == NULL) {
                    printf("Ошибка ввода строки\n");
                    break;
                }

                str_mass[mass_last_elem] = create_string(temp_str, wchar_concat, wchar_substr, wchar_to_char_recode, length, capacity, 1);
                additional_info[mass_last_elem] = 1;
                free(temp_str);
                
                if(str_mass[mass_last_elem] == NULL){
                    printf("Не удалось выделить памяти на create_string\n");
                } else {
                    mass_last_elem++;
                }
                break;
                
            case 2:
                printf("Введите строку (char): ");
                temp_str = read_char(&length, &capacity);
                if (temp_str == NULL) {
                    printf("Ошибка ввода строки\n");
                    break;
                }
                str_mass[mass_last_elem] = create_string(temp_str, char_concat, char_substr, char_to_wchar_recode, length, capacity, 0);
                additional_info[mass_last_elem] = 0;
                free(temp_str);
                
                printf("\nnew element: %s\n", (char *)str_mass[mass_last_elem]->data);
                if(str_mass[mass_last_elem] == NULL){
                    printf("Не удалось выделить памяти на create_string\n");
                } else {
                    mass_last_elem++;
                }
                break;
                
            case 3:
                do{
                    printf("Укажите номера строк для конкатенации\n");
                    check = scanf("%d", &left);
                    check2 = scanf("%d", &right);
                    left--;
                    right--;
                    if(left < 0 || (size_t)left >= mass_last_elem || right < 0 || (size_t)right >= mass_last_elem){
                        clear_screen();
                        printf("Такого варианта не существует, введите заново\n");
                    }
                }while(clear_input() != 0 || check!= 1 || check2 != 1 || left < 0 || (size_t)left >= mass_last_elem || right < 0 || (size_t)right >= mass_last_elem);
                
                if(additional_info[left] != additional_info[right]){
                    printf("Данные имеют разные типы\n");
                } else {
                    temp_str = string_concatination(str_mass[left], str_mass[right]);
                    printf("Итоговая строка: ");
                    if(temp_str) str_printf(((poly_str*)temp_str)->data, additional_info[left]);
                    printf("\n");
                    if (temp_str) {
                         free(((poly_str*)temp_str)->data);
                         free(temp_str);
                    }
                }
                break;
            case 4:
                do{
                    printf("Укажите номер строки для взятия подстроки\n");
                    check = scanf("%d", &start);
                    start--;
                    if(start < 0 || (size_t)start >= mass_last_elem){
                        clear_screen();
                        printf("Такого варианта не существует, введите заново\n");
                    }
                }while(clear_input() != 0 || check!= 1 || left < 0 || (size_t)left >= mass_last_elem);
                do{
                    printf("Укажите начало строки для подстроки: ");
                    check = scanf("%d", &left);
                    printf("Укажите длинну подстроки: ");
                    check2 = scanf("%d", &right);
                    left--;
                    if(left < 0 || (size_t)left >= str_mass[start]->length || right <= 0){
                        clear_screen();
                        printf("Такого варианта не существует, введите заново\n");
                    }
                }while(clear_input() != 0 || check!= 1 || check2 != 1 || left < 0 || (size_t)left >= str_mass[start]->length || right <= 0);
                temp_str = string_substr(str_mass[start], left, right);
                if(temp_str != NULL){
                    printf("\n Итоговая подстрока: ");
                    str_printf(((poly_str*)temp_str)->data, additional_info[start]);
                }
                break;
            case 5:
                do{
                    printf("Укажите номер строки для рекодировки\n");
                    check = scanf("%d", &start);
                    start--;
                    if(start < 0 || (size_t)start >= mass_last_elem){
                        clear_screen();
                        printf("Такого варианта не существует, введите заново\n");
                    }
                }while(clear_input() != 0 || check!= 1 || left < 0 || (size_t)left >= mass_last_elem);
                temp_str = string_recode(str_mass[start]);
                if(temp_str != NULL){
                    additional_info[start] = 1 - additional_info[start];
                    printf("\n Итоговая подстрока: ");
                    str_printf(((poly_str*)temp_str)->data, additional_info[start]);
                    str_mass[start] = temp_str;
                }
        }
        
        if(choice != 6){
            if(choice > 2) clear_input();
            clear_screen();
        }
    }
    for(size_t i = 0; i < mass_last_elem; i++){
        if(str_mass[i]) {
            free(str_mass[i]->data);
            free(str_mass[i]);
        }
    }
    free(str_mass);
    free(additional_info);
    
    return 0;
}