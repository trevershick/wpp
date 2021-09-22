#include "context.h"

#include <unistd.h>
#include <string.h>

#include "directives.h"
#include "format.h"
#include "cmdline.h"
#include "context.h"
#include "rcfile.h"


int main(int argc, char **argv) {
  struct Context context = {0};

  if (parse_arguments(argc, argv, &context)) {
    return 1;
  }

  if (validate_arguments(&context)) {
    return 1;
  }

  if (init_context(&context)) {
    return 1;
  }

  int result = process_rcfile(&context, handle_match);
  destroy_context(&context);
  return result;
}
