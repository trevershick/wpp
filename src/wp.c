#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>

#include "wp.h"

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

int is_section_header(const char *line) { return *line == '['; }

int is_matching_section_header(const char *line, const char *section) {
  int sz;
  return *line == '[' && (sz = strlen(section)) > 0 &&
         strncmp(line + 1, section, sz) == 0 && line[sz + 1] == ']';
}

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
