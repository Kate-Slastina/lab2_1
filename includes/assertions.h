#ifndef ASSERTIONS_H
#define ASSERTIONS_H

#include <stddef.h>

void inc_success();
void inc_fail();
int pop_fail();
int pop_success();
int print_stats();

int assert_success(const char* expr, const char* file, size_t lineno);
int assert_fail(const char* expr, const char* file, size_t lineno);

#define assert(expr) \
    ((!!(expr)) \
    ? assert_success(#expr, __FILE__, __LINE__) \
    : assert_fail(#expr, __FILE__, __LINE__))

#endif