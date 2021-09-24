#ifndef _tests_h
#define _tests_h

#include <check.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "src/context.h"

extern char out[4096];
extern char err[4096];

struct Context *test_context(int debug);

#define ck_assert_stderr_contains(value)                                       \
  ck_assert_msg(strstr(err, value), "expect %s (stderr) to contain %s", err,   \
                value);

#define ck_assert_stdout_contains(value)                                       \
  ck_assert_msg(strstr(out, value), "expect %s (stdout) to contain %s", out,   \
                value);

#define ck_assert_stdout_eq(value)                                             \
  ck_assert_msg(strcmp(value, out) == 0, "expect %s (stdout) to equal %s",     \
                err, value);

#define ck_assert_stderr_eq(value)                                             \
  ck_assert_msg(strcmp(value, err) == 0, "expect %s (stderr) to equal %s",     \
                err, value);

#define ck_assert_stderr_empty()                                               \
  ck_assert_msg(strcmp("", err) == 0, "expect %s (stderr) to be empty", err);

#endif
