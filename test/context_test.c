#include "src/context.h"

#include <check.h>
#include <string.h>

#include "tests.h"

START_TEST(test_init_context_usessetrcfile) {
  struct Context *c = test_context(0);
  strcpy(c->rc_file, "~/trevers_rc");
  int result = init_context(c);
  ck_assert_msg(strstr(c->rc_file, "trevers_rc") != NULL,
                "RC file should not override explicit setting but is '%s'",
                c->rc_file);
  ck_assert_msg(strstr(c->rc_file, "~") == NULL,
                "RC file should have been 'expanded'");
  // should fail loading the file
  ck_assert_int_eq(1, result);
  destroy_context(c);
}
END_TEST

START_TEST(test_init_context_default) {
  struct Context *c = test_context(0);
  init_context(c);
  ck_assert_msg(strstr(c->rc_file, "workspaces.rc") != NULL,
                "RC file should contain workspaces.rc but is '%s'", c->rc_file);
  ck_assert_msg(strstr(c->rc_file, "~") == NULL,
                "RC file should have been 'expanded'");
  ck_assert_msg(c->cwd[0] != 0, "CWD should be populated");
  destroy_context(c);
}
END_TEST

START_TEST(test_init_context_opensfile) {
  char buffer[80] = {0};
  // write a file
  {
    struct Context *c = test_context(0);
    strcpy(c->rc_file, "$TEST_TMPDIR/file_to_open");
    ck_assert_int_eq(1, init_context(c));
    ck_assert(strstr(c->rc_file, "TEST") == NULL);
    FILE *out = fopen(c->rc_file, "w");
    ck_assert(out != NULL);
    fprintf(out, "nothing special");
    fclose(out);
    destroy_context(c);
  }

  {
    struct Context *c = test_context(0);
    strcpy(c->rc_file, "$TEST_TMPDIR/file_to_open");
    ck_assert_int_eq(0, init_context(c));
    fgets(buffer, sizeof(buffer), c->file);
    ck_assert_str_eq(buffer, "nothing special");
    destroy_context(c);
  }
}
END_TEST
// clang-format off
CK_SUITE(context_test,
  CK_CASE(init_context,
    CK_TEST(test_init_context_default)
    CK_TEST(test_init_context_usessetrcfile)
    CK_TEST(test_init_context_opensfile)
  )
)
// clang-format on
