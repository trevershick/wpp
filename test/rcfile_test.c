#include <check.h>
#include <check_stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "src/rcfile.h"
#include "tests.h"

char actual_directive[PATH_MAX];

void handle(struct Context *ctx, const char *directive) {
  strncpy(actual_directive, directive, sizeof(actual_directive));
}

char *rccontent = "[section1]\n"
                  "/Users/barney=betty\n"
                  "/Users/fred=wilma\n"
                  "\n"
                  "[section2]\n"
                  "/Users/dino=bone\n";

START_TEST(in_folder) {
  strcpy(actual_directive, "unmatched");
  struct Context *context = test_context(0);
  strncpy(context->section, "section1", sizeof(context->section));
  strncpy(context->cwd, "/Users/barney", sizeof(context->cwd));
  context->file = fmemopen(rccontent, strlen(rccontent), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("betty", actual_directive);
  ck_assert_str_eq("", err);
  destroy_context(context);
}
END_TEST

START_TEST(in_child_of_folder) {
  strcpy(actual_directive, "unmatched");
  struct Context *context = test_context(0);
  strncpy(context->section, "section1", sizeof(context->section));
  strncpy(context->cwd, "/Users/fred/nested", sizeof(context->cwd));
  context->file = fmemopen(rccontent, strlen(rccontent), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("wilma", actual_directive);
  ck_assert_str_eq("", err);
  destroy_context(context);
}
END_TEST

START_TEST(in_folder_section_2) {
  strcpy(actual_directive, "unmatched");
  struct Context *context = test_context(0);
  strncpy(context->cwd, "/Users/dino", sizeof(context->cwd));
  strncpy(context->section, "section2", sizeof(context->section));
  context->file = fmemopen(rccontent, strlen(rccontent), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("bone", actual_directive);
  ck_assert_str_eq("", err);
  destroy_context(context);
}
END_TEST

START_TEST(no_match) {
  strcpy(actual_directive, "unmatched");
  struct Context *context = test_context(0);
  strncpy(context->section, "section1", sizeof(context->section));
  // dino is defined in section 2
  strncpy(context->cwd, "/Users/dino", sizeof(context->cwd));
  context->file = fmemopen(rccontent, strlen(rccontent), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("unmatched", actual_directive);
  ck_assert_str_eq("", err);
  destroy_context(context);
}
END_TEST

START_TEST(generate_sample) {
  struct Context *context = test_context(0);
  generate_sample_file(context->out);
  ck_assert(strstr(out, "[testfiles]"));
  ck_assert_str_eq("", err);
  destroy_context(context);
}
END_TEST

// clang-format off
CK_SUITE(rcfile_test,
  CK_CASE(matching,
    CK_TEST(in_folder)
    CK_TEST(in_child_of_folder)
    CK_TEST(in_folder_section_2)
    CK_TEST(no_match)
  )
  CK_CASE(sample_file,
    CK_TEST(generate_sample)
  )
)
// clang-format on
