#include <check.h>
#include <check_stdint.h>
#include <errno.h>
#include <stdlib.h>

#include "src/format.h"
#include "tests.h"

START_TEST(test_is_section_header) {
  ck_assert_int_eq(0, is_section_header("test"));
  ck_assert_int_eq(1, is_section_header("[test"));
}
END_TEST

START_TEST(test_is_matching_section_header) {
  ck_assert_int_eq(1, is_matching_section_header("[test]", "test"));
  ck_assert_int_eq(0, is_matching_section_header("[Test]", "test"));
  ck_assert_int_eq(0, is_matching_section_header("[test]", "Test"));
  ck_assert_int_eq(0, is_matching_section_header("[ test]", "test"));
}
END_TEST

// clang-format off
CK_SUITE(format_test,
  CK_CASE(basic,
    CK_TEST(test_is_section_header)
    CK_TEST(test_is_matching_section_header)
  )
)
// clang-format on
