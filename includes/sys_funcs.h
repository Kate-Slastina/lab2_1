#ifndef SYS_FUNCS_H
#define SYS_FUNCS_H
int clear_input();
void clear_screen();
char* read_char(size_t* length, size_t* capacity);
wchar_t* read_wchart(size_t* length, size_t* capacity);
int str_printf(void* string, int cond);
#endif