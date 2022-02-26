#include "cmdline.h"

#include <check.h>
#include <string.h>

#include "tests.h"

static struct Context *c;

static void setup() { c = test_context(0); }

static void teardown() {
  if (c)
    destroy_context(c);
}

START_TEST(arg_debug) {
  char *args[] = {"arg_debug", "-x"};
  ck_assert_int_eq(1, parse_arguments(2, args, c));
  flush_fps(c);
  ck_assert_stderr_contains("Unknown option '-x'");
}
END_TEST

START_TEST(arg_file) {
  char *args[] = {
      "arg_file",
      "-f",
      "howard",
  };
  ck_assert_int_eq(0, parse_arguments(3, args, c));
  ck_assert_str_eq("howard", c->cwd);
  flush_fps(c);
  ck_assert_stderr_empty();
}
END_TEST

START_TEST(arg_file_noarg) {
  char *args[] = {"arg_file_noarg", "-f"};
  ck_assert_int_eq(1, parse_arguments(2, args, c));
  flush_fps(c);
  ck_assert_stderr_contains("Option -f requires an argument.");
}
END_TEST

START_TEST(arg_generate) {
  char *args[] = {"arg_generate", "-g"};
  ck_assert_int_eq(-1, parse_arguments(2, args, c));
  flush_fps(c);
  ck_assert_stdout_contains("[testfiles]");
}
END_TEST

START_TEST(arg_help) {
  char *args[] = {"arg_help", "-h"};
  ck_assert_int_eq(-1, parse_arguments(2, args, c));
  flush_fps(c);
  ck_assert_stdout_contains("EXAMPLE CONFIG FILE");
}
END_TEST

START_TEST(arg_section) {
  char *args[] = {
      "arg_section",
      "-s",
      "googly",
  };
  ck_assert_int_eq(0, parse_arguments(3, args, c));
  ck_assert_str_eq("googly", c->section);
  flush_fps(c);
  ck_assert_stderr_empty();
}
END_TEST

START_TEST(arg_section_noarg) {
  char *args[] = {"arg_section_noarg", "-s"};
  ck_assert_int_eq(1, parse_arguments(2, args, c));
  flush_fps(c);
  ck_assert_stderr_contains("Option -s requires an argument.");
}
END_TEST

START_TEST(arg_rcfile) {
  char *args[] = {"arg_rcfile", "-r", "ducky"};
  ck_assert_int_eq(0, parse_arguments(3, args, c));
  ck_assert_str_eq("ducky", c->rc_file);
  flush_fps(c);
  ck_assert_stderr_empty();
}
END_TEST

START_TEST(arg_rcfile_noarg) {
  char *args[] = {"arg_rcfile_noarg", "-r"};
  ck_assert_int_eq(1, parse_arguments(2, args, c));
  flush_fps(c);
  ck_assert_stderr_contains("Option -r requires an argument.");
}
END_TEST

START_TEST(section_is_valid) {
  char *args[] = {"section_is_valid", "-s", "abc"};
  ck_assert_int_eq(0, parse_arguments(3, args, c));
  flush_fps(c);
  ck_assert_stderr_empty();
}
END_TEST

START_TEST(section_is_invalid) {
  ck_assert_int_eq(1, validate_arguments(c));
  flush_fps(c);
  ck_assert_stderr_contains("Section Name (-s) required");
}
END_TEST

Suite *make_cmdline_test_suite() {
  Suite *s = suite_create("cmdline");
  {
    TCase *tc = tcase_create("arguments");
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, arg_debug);
    tcase_add_test(tc, arg_file);
    tcase_add_test(tc, arg_file_noarg);
    tcase_add_test(tc, arg_generate);
    tcase_add_test(tc, arg_help);
    tcase_add_test(tc, arg_rcfile);
    tcase_add_test(tc, arg_rcfile_noarg);
    tcase_add_test(tc, arg_section);
    tcase_add_test(tc, arg_section_noarg);
    suite_add_tcase(s, tc);
  }
  {
    TCase *tc = tcase_create("validation");
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, section_is_invalid);
    tcase_add_test(tc, section_is_valid);
    suite_add_tcase(s, tc);
  }
  return s;
}
