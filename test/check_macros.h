
#define QUOTE(n) "\"" #n "\""

#define CK_SUITE(nm, ...)                                                                                              \
  Suite *make_##nm##_suite(void) {                                                                                     \
    Suite *suite = suite_create(#nm);                                                                                  \
    __VA_ARGS__                                                                                                        \
    return suite;                                                                                                      \
  }

#define CK_CASE(nm, ...)                                                                                               \
  {                                                                                                                    \
    TCase *test_case = tcase_create(#nm);                                                                              \
    __VA_ARGS__                                                                                                        \
    suite_add_tcase(suite, test_case);                                                                                 \
  }

#define CK_TEST(nm) tcase_add_test(test_case, nm);
