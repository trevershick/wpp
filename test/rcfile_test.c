#include <check.h>
#include <check_stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "src/rcfile.h"
#include "tests.h"

static struct Context *context;
static char *rccontent = "[section1]\n"
                         "/Users/barney=betty\n"
                         "/Users/fred=wilma\n"
                         "\n"
                         "[section2]\n"
                         "/Users/dino=bone\n";
static char actual_directive[PATH_MAX];

static void setup() {
  memset(actual_directive, 0, sizeof(actual_directive));
  strcpy(actual_directive, "unmatched");
  context = test_context(0);
}

static void teardown() {
  if (context)
    destroy_context(context);
}

static void handle(struct Context *ctx, const char *directive) {
  strncpy(actual_directive, directive, sizeof(actual_directive));
}

START_TEST(in_folder) {
  strncpy(context->section, "section1", sizeof(context->section));
  strncpy(context->cwd, "/Users/barney", sizeof(context->cwd));
  context->file = fmemopen(rccontent, strlen(rccontent), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("betty", actual_directive);
  ck_assert_stderr_empty();
}
END_TEST

START_TEST(in_child_of_folder) {
  strncpy(context->section, "section1", sizeof(context->section));
  strncpy(context->cwd, "/Users/fred/nested", sizeof(context->cwd));
  context->file = fmemopen(rccontent, strlen(rccontent), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("wilma", actual_directive);
  ck_assert_stderr_empty();
}
END_TEST

START_TEST(in_folder_section_2) {
  strncpy(context->cwd, "/Users/dino", sizeof(context->cwd));
  strncpy(context->section, "section2", sizeof(context->section));
  context->file = fmemopen(rccontent, strlen(rccontent), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("bone", actual_directive);
  ck_assert_stderr_empty();
}
END_TEST

START_TEST(no_match) {
  strncpy(context->section, "section1", sizeof(context->section));
  // dino is defined in section 2
  strncpy(context->cwd, "/Users/dino", sizeof(context->cwd));
  context->file = fmemopen(rccontent, strlen(rccontent), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("unmatched", actual_directive);
  ck_assert_stderr_empty();
}
END_TEST

START_TEST(generate_sample) {
  generate_sample_file(context->out);
  ck_assert_stdout_contains("[testfiles]");
  ck_assert_stderr_empty();
}
END_TEST

Suite *make_rcfile_test_suite() {
  Suite *s = suite_create("rcfile");
  {
    TCase *tc = tcase_create("matching");
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, in_folder);
    tcase_add_test(tc, in_child_of_folder);
    tcase_add_test(tc, in_folder_section_2);
    tcase_add_test(tc, no_match);
    suite_add_tcase(s, tc);
  }
  {
    TCase *tc = tcase_create("sample_file");
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, generate_sample);
    suite_add_tcase(s, tc);
  }
  return s;
}
