#include "context.h"

#include <unistd.h>
#include <wordexp.h>
#include <string.h>

#include "directives.h"
#include "format.h"


int main(int argc, char **argv) {
  struct Context context = {0};

  if (parse_arguments(argc, argv, &context)) {
    return 1;
  }

  if (init_context(&context)) {
    return 1;
  }

  // parse the file
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  wordexp_t we = {0};
  int wordexp_err;
  int result = 1;

  // for any section, need to find the start of the section
  // then read only up to the next section

  int in_section = 0;
  while (result && (linelen = getline(&line, &linecap, context.file)) > 0) {
    // find section
    if (!in_section) {
      in_section = is_matching_section_header(line, context.section);
      continue;
    }

    // if we hit a new section while we were in the matching section
    // then we didn't find what we were looking for, so break out of the loop
    if (in_section) {
      if (is_section_header(line)) {
        in_section = 0;
        break;
      }
    }

    char *nlpos, *eqpos;
    // if there's a \n in the line, convert it to 0 for easier
    // string processing, same thing for an equal size
    if ((nlpos = strchr(line, '\n')) != NULL) {
      *nlpos = '\0';
    } else {
      fprintf(stderr, "directive format error, expected newline\n");
      destroy_context(&context);
      return 1;
    }

    if ((eqpos = strchr(line, '=')) != NULL) {
      *eqpos = '\0';
    } else {
      fprintf(stderr, "directive format error, expected '='\n");
      destroy_context(&context);
      return 1;
    }

    // we're expanding the line up to the '=' if there is one
    // because we replaced = with 0
    if ((wordexp_err = wordexp(line, &we, WRDE_REUSE))) {
      fprintf(stderr, "expansion error - %d\n", wordexp_err);
      destroy_context(&context);
      return 1;
    }

    if (strlen(context.cwd) < strlen(we.we_wordv[0])) {
      // if the cwd is < the current line's matcher just exit, it can't match
      continue;
    }
    // copy the 'matched matcher' into the context
    strncpy(context.matched_matcher, we.we_wordv[0],
            sizeof(context.matched_matcher));

    handle_match(&context, eqpos + 1);
    result = 0;
  }
  wordfree(&we);
  destroy_context(&context);
  return result;
}
