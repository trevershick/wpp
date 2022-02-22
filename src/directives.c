#include "directives.h"

#include <string.h>

// execute the special directive
void handle_match(struct Context *context, const char *directive) {
  fprintf(context->debug, "handle_match %s\n", directive);
  if (strncmp(directive, "<first_dir>", 11) == 0) {
    fprintf(context->debug, "in first_dir\n");
    char *slpos;
    int linelen = strlen(context->matched_matcher);
    fprintf(context->debug, "line length %d\n", linelen);
    if (strncmp(context->matched_matcher, context->cwd, linelen) == 0) {
      fprintf(context->debug, "MATCHed %s %s\n", context->matched_matcher,
              context->cwd);
      if ((slpos = strchr(context->cwd + linelen + 1, '/')) != NULL) {
        fprintf(context->debug, "Replacing slash with null\n");
        *slpos = '\0';
      }
      printf("%s", context->cwd + linelen + 1);
    }
  } else {
    printf("%s", directive);
  }
}
