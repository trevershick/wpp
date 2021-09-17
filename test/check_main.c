#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "format_test.h"

#define ADD_SUITE(runner, factory_method)                                                                              \
  {                                                                                                                    \
    struct Suite *suite = factory_method();                                                                            \
    srunner_add_suite(runner, suite);                                                                                  \
  }



int main(void) {
  struct Suite *s = suite_create(">");
  SRunner *sr = srunner_create(s);

  ADD_SUITE(sr, make_format_test_suite);

  const char* xml_output = getenv("XML_OUTPUT_FILE");
  if (!xml_output) {
    xml_output = "tests.xml";
  }
  fprintf(stdout, "Writing test output to : %s\n", xml_output);
  //srunner_set_tap(sr, "tests.tap");
  srunner_set_xml(sr, xml_output);
  srunner_run_all(sr, CK_VERBOSE /*CK_NORMAL*/);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

