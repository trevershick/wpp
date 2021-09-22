#include "context.h"

#include <libgen.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>
#include <sys/stat.h>

void destroy_context(struct Context *context) {
  if (context->file) {
    fclose(context->file);
  }
}

int resolve_cwd(struct Context *context) {
  if (strlen(context->cwd)) {
   struct stat s;
    char buffer[PATH_MAX];
    if (realpath(context->cwd, buffer) == NULL) {
      fprintf(stderr, "couldn't resolve absolute path for %s", context->cwd);
      return 1;
    }
    strncpy(context->cwd, buffer, sizeof(context->cwd));

    if (stat(context->cwd, &s)) {
      fprintf(stderr, "file/directory does not exists %s", context->cwd);
      return 1;
    }

    if ((s.st_mode & S_IFREG) == S_IFREG) {
      memset(buffer, 0, sizeof(buffer));
      // it's a regular file, we need to get the parent directory
      if (dirname_r(context->cwd, buffer)) {
        strncpy(context->cwd, buffer, sizeof(context->cwd));
      } else {
        fprintf(stderr, "Unable to determine the directory for %s", context->cwd);
        return 1;
      }
    } else if ((s.st_mode & S_IFDIR) != S_IFDIR) {
        fprintf(stderr, "Don't know what the specified file is %s", context->cwd);
        return 1;
    }
  } else {
    if (getcwd(context->cwd, sizeof(context->cwd)) == NULL) {
      perror("getcwd");
      return 1;
    }
  }
  return 0;
}

int resolve_rcfile(struct Context *context) {
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

int init_context(struct Context *context) {
  return resolve_cwd(context) || resolve_rcfile(context);
}
