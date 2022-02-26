#include <check.h>
#include <check_stdint.h>
#include <errno.h>
#include <stdlib.h>

#include "format.h"
#include "tests.h"

START_TEST(is_header) {
  ck_assert_int_eq(0, is_section_header("test"));
  ck_assert_int_eq(1, is_section_header("[test"));
}
END_TEST

START_TEST(is_matching) {
  ck_assert_int_eq(1, is_matching_section_header("[test]", "test"));
  ck_assert_int_eq(0, is_matching_section_header("[Test]", "test"));
  ck_assert_int_eq(0, is_matching_section_header("[test]", "Test"));
  ck_assert_int_eq(0, is_matching_section_header("[ test]", "test"));
}
END_TEST

Suite *make_format_test_suite() {
  Suite *s = suite_create("format");
  {
    TCase *tc = tcase_create("sections");
    tcase_add_test(tc, is_header);
    tcase_add_test(tc, is_matching);
    suite_add_tcase(s, tc);
  }
  return s;
}
