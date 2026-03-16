#include <stdio.h>
#include <stdlib.h>
#include "text_blocks.h"
#include "poly_funcs.h"
#include "sys_funcs.h"

void main_operations_text(poly_str** mass, int* add_info, size_t mass_size) {
    printf("mass_size: %zu\n", mass_size);
    
    if (mass_size != 0) {
        printf("[");
        for (size_t i = 0; i < mass_size - 1; i++) {
            str_printf(mass[i]->data, add_info[i]);
            printf(", ");
        }
        str_printf(mass[mass_size - 1]->data, add_info[mass_size - 1]);
        printf("]\n");
    }
    
       printf("\nМеню выбора действия\n");
        printf("  1 - Добавить элемент типа wchar_t\n");
        printf("  2 - Добавить элемент типа char\n");
        printf("  3 - Провести конкатенацию строк\n");
        printf("  4 - Получение подстроки\n");
        printf("  5 - Перекодирование строки\n");
        printf("  6 - Закончить работу с программой\n");
        printf("  > ");
}