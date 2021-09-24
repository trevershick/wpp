#ifndef _tests_h
#define _tests_h

#include <memory.h>
#include <stdlib.h>

#include "check_macros.h"
#include "src/context.h"

char out[4096];
char err[4096];
struct Suite *make_format_test_suite();
struct Suite *make_context_test_suite();
struct Suite *make_rcfile_test_suite();
struct Context *test_context();
#endif
