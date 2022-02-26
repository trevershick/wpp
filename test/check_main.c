#include <check.h>
#include <limits.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "tests.h"

struct Suite *make_cmdline_test_suite();
struct Suite *make_context_test_suite();
struct Suite *make_format_test_suite();
struct Suite *make_rcfile_test_suite();

int main(void) {
  SRunner *sr = srunner_create(make_cmdline_test_suite());
  srunner_add_suite(sr, make_context_test_suite());
  srunner_add_suite(sr, make_format_test_suite());
  srunner_add_suite(sr, make_rcfile_test_suite());

  char xml_output[PATH_MAX];
  memset(xml_output, 0, PATH_MAX);
  const char *outputs_dir = getenv("TEST_UNDECLARED_OUTPUTS_DIR");
  if (outputs_dir) {
    sprintf(xml_output, "%s/results.xml", outputs_dir);
  } else {
    sprintf(xml_output, "results.xml");
  }

  char output_path[PATH_MAX];
  memset(output_path, 0, PATH_MAX);
  realpath(output_path, xml_output);
  fprintf(stdout, "Writing test output to : %s\n", xml_output);
  // srunner_set_tap(sr, "tests.tap");
  srunner_set_xml(sr, xml_output);
  // srunner_set_xml_format(sr, CK_XML_FORMAT_JUNIT);
  srunner_run_all(sr, CK_VERBOSE /*CK_NORMAL*/);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
