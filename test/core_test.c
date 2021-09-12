#include <check.h>
#include <check_stdint.h>
#include <errno.h>
#include <stdlib.h>

#include "check_macros.h"

START_TEST(test_parse_hex_u8s_acceptsnull) {
  //uint8_t buffer[32] = {0};
  //const char *input = "01 02";
  //size_t count = 0;
  //ck_assert_uint_eq(0, parse_hex_u8s(input, NULL, buffer, sizeof(buffer), &count));
  //ck_assert_uint_eq(2, count);
  //ck_assert_uint_eq(buffer[0], 1);
  //ck_assert_uint_eq(buffer[1], 2);
}
END_TEST

// clang-format off
CK_SUITE(core_test,
  CK_CASE(u8,
    CK_TEST(test_parse_hex_u8s_acceptsnull)
  )
)
// clang-format on
