#include "context.h"

#include <check.h>
#include <string.h>

#include "tests.h"

static struct Context *c;

static void setup() { c = test_context(0); }

static void teardown() {
  if (c)
    destroy_context(c);
}

START_TEST(explicitly_set_rcfile) {
  strcpy(c->rc_file, "~/trevers_rc");
  int result = init_context(c);
  ck_assert_msg(strstr(c->rc_file, "trevers_rc") != NULL,
                "RC file should not override explicit setting but is '%s'",
                c->rc_file);
  ck_assert_msg(strstr(c->rc_file, "~") == NULL,
                "RC file should have been 'expanded'");
  // should fail loading the file
  ck_assert_int_eq(1, result);
  flush_fps(c);
  ck_assert_stderr_contains("error opening ");
}
END_TEST

START_TEST(defaults) {
  init_context(c);
  ck_assert_msg(strstr(c->rc_file, "workspaces.rc") != NULL,
                "RC file should contain workspaces.rc but is '%s'", c->rc_file);
  ck_assert_msg(strstr(c->rc_file, "~") == NULL,
                "RC file should have been 'expanded'");
  ck_assert_msg(c->cwd[0] != 0, "CWD should be populated");
  flush_fps(c);
  ck_assert_stderr_contains("error opening ");
}
END_TEST

START_TEST(opens_rcfile) {
  char buffer[80] = {0};
  {
    // write the file to read using the context to calculate
    // the filename to write
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

Suite *make_context_test_suite() {
  Suite *s = suite_create("context");
  {
    TCase *tc = tcase_create("init_context");
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, defaults);
    tcase_add_test(tc, opens_rcfile);
    tcase_add_test(tc, explicitly_set_rcfile);
    suite_add_tcase(s, tc);
  }
  return s;
}
