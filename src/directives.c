#include "directives.h"

#include <string.h>

// execute the special directive
void handle_match(struct Context *context, const char *directive) {
  if (strncmp(directive, "<first_dir>", 11) == 0) {
    char *slpos;
    int linelen = strlen(context->matched_matcher);
    if (strncmp(context->matched_matcher, context->cwd, linelen) == 0) {
      if ((slpos = strchr(context->cwd + linelen, '/')) != NULL) {
        *slpos = '\0';
      }
      printf("%s", context->cwd + linelen);
    }
  } else {
    printf("%s", directive);
  }
}
