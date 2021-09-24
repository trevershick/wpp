#include "context.h"

#include <string.h>
#include <unistd.h>

#include "cmdline.h"
#include "context.h"
#include "directives.h"
#include "format.h"
#include "rcfile.h"

int main(int argc, char **argv) {
  struct Context *context = new_context();

  // clang-format off
  int result = parse_arguments(argc, argv, context)
    || validate_arguments(context)
    || init_context(context)
    || process_rcfile(context, handle_match);
  // clang-format on
  destroy_context(context);
  return result;
}
