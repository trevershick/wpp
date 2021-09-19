#include <check.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <memory.h>

#include "format_test.h"

#define ADD_SUITE(runner, factory_method)                                                                              \
  {                                                                                                                    \
    struct Suite *suite = factory_method();                                                                            \
    srunner_add_suite(runner, suite);                                                                                  \
  }



int main(void) {
  struct Suite *s = make_format_test_suite();
  SRunner *sr = srunner_create(s);

  char xml_output[PATH_MAX];
  memset(xml_output, 0, PATH_MAX);
  const char* outputs_dir = getenv("TEST_UNDECLARED_OUTPUTS_DIR");
  if (outputs_dir) {
    sprintf(xml_output, "%s/results.xml", outputs_dir);  
  } else {
    sprintf(xml_output, "results.xml");  
  }

  char output_path[PATH_MAX];
  memset(output_path, 0, PATH_MAX);
  realpath(output_path, xml_output);
  fprintf(stdout, "Writing test output to : %s\n", xml_output);
  //srunner_set_tap(sr, "tests.tap");
  srunner_set_xml(sr, xml_output);
  srunner_set_xml_format(sr, CK_XML_FORMAT_JUNIT);
  srunner_run_all(sr, CK_VERBOSE /*CK_NORMAL*/);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

