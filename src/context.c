#include "context.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>

void destroy_context(struct Context *context) {
  if (context->file) {
    fclose(context->file);
  }
}

int init_context(struct Context *context) {
  if (getcwd(context->cwd, sizeof(context->cwd)) == NULL) {
    perror("getcwd");
    return 1;
  }
  // set any defaults
  if (!strlen(context->rc_file)) {
    strcpy(context->rc_file, "~/workspaces.rc");
  }

  wordexp_t we = {0};
  int wordexp_err;
  if ((wordexp_err = wordexp(context->rc_file, &we, 0))) {
    fprintf(stderr, "expansion error on %s - %d\n", context->rc_file,
            wordexp_err);
    return 1;
  }
  strncpy(context->rc_file, we.we_wordv[0], sizeof(context->rc_file));

  if ((context->file = fopen(context->rc_file, "r")) == NULL) {
    fprintf(stderr, "error opening %s: %s\n", context->rc_file,
            strerror(errno));
    wordfree(&we);
    return 1;
  }
  wordfree(&we);
  return 0;
}

int parse_arguments(int argc, char **argv, struct Context *context) {
  int c;
  while ((c = getopt(argc, argv, "s:r:")) != -1) {
    switch (c) {
    case 'r':
      strncpy(context->rc_file, optarg, sizeof(context->rc_file));
      break;
    case 's':
      strncpy(context->section, optarg, sizeof(context->section));
      break;
    case '?':
      if (optopt == 's') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      } else if (isprint(optopt)) {
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      } else {
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      }
      return 1;
    default:
      abort();
    }
  }
  if (!strlen(context->section)) {
    fprintf(stderr, "Section Name (-s) required");
    return 1;
  }
  return 0;
}


