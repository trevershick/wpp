#include <check.h>
#include <check_stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "src/rcfile.h"
#include "tests.h"

char actual_directive[PATH_MAX] = "unmatched";

void handle(struct Context *ctx, const char *directive) {
  strncpy(actual_directive, directive, sizeof(actual_directive));
}

START_TEST(test_simple) {
  struct Context *context = test_context();
  strncpy(context->cwd, "/Users/barney", sizeof(context->cwd));
  strncpy(context->section, "section1", sizeof(context->section));
  char *content = "[section1]\n"
                  "/Users/barney=betty\n";
  context->file = fmemopen(content, strlen(content), "r");
  process_rcfile(context, handle);
  ck_assert_str_eq("betty", actual_directive);
  destroy_context(context);
}
END_TEST

// clang-format off
CK_SUITE(rcfile_test,
  CK_CASE(basic,
    CK_TEST(test_simple)
  )
)
// clang-format on
