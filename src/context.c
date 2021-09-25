#include "context.h"

#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wordexp.h>

struct Context *new_context() {
  struct Context *ctx = calloc(1, sizeof(struct Context));
  ctx->err = stderr;
  ctx->out = stdout;
  ctx->debug = fopen("/dev/null", "w");
  return ctx;
}

void destroy_context(struct Context *ctx) {
  if (!ctx) return;
  if (ctx->file) {
    fclose(ctx->file);
  }
  if (ctx->err && ctx->err != stderr) {
    fclose(ctx->err);
  }
  if (ctx->out && ctx->out != stdout) {
    fclose(ctx->out);
  }
  if (ctx->debug) {
    fclose(ctx->debug);
  }
}

int resolve_cwd(struct Context *context) {
  if (strlen(context->cwd)) {
    struct stat s;
    char buffer[PATH_MAX];
    if (realpath(context->cwd, buffer) == NULL) {
      fprintf(context->err, "couldn't resolve absolute path for %s",
              context->cwd);
      return 1;
    }
    strncpy(context->cwd, buffer, sizeof(context->cwd));
    fprintf(context->debug, "cwd is %s\n", context->cwd);

    if (stat(context->cwd, &s)) {
      fprintf(context->err, "file/directory does not exists %s", context->cwd);
      return 1;
    }

    if ((s.st_mode & S_IFREG) == S_IFREG) {
      fprintf(context->debug, "cwd is file, finding dirname\n");
      // it's a regular file, we need to get the parent directory
      char *dname = dirname(context->cwd);
      fprintf(context->debug, "dirname is %s\n", dname);

      if (!dname) {
        fprintf(context->err, "Unable to determine the directory for %s",
                context->cwd);
        return 1;
      }
      strncpy(context->cwd, dname, sizeof(context->cwd));
      fprintf(context->debug, "cwd is now %s\n", context->cwd);
    } else if ((s.st_mode & S_IFDIR) != S_IFDIR) {
      fprintf(context->err, "Don't know what the specified file is %s",
              context->cwd);
      return 1;
    }
  } else {
    if (getcwd(context->cwd, sizeof(context->cwd)) == NULL) {
      perror("getcwd");
      return 1;
    }
    fprintf(context->debug, "cwd is finally %s\n", context->cwd);
  }
  return 0;
}

int resolve_rcfile(struct Context *context) {
  // set any defaults
  if (!strlen(context->rc_file)) {
    strcpy(context->rc_file, "~/workspaces.rc");
    fprintf(context->debug, "rcfile not explicitly set, using %s\n",
            context->rc_file);
  }

  wordexp_t we = {0};
  int wordexp_err;
  if ((wordexp_err = wordexp(context->rc_file, &we, 0))) {
    fprintf(context->err, "expansion error on %s - %d\n", context->rc_file,
            wordexp_err);
    return 1;
  }
  strncpy(context->rc_file, we.we_wordv[0], sizeof(context->rc_file));

  if ((context->file = fopen(context->rc_file, "r")) == NULL) {
    fprintf(context->err, "error opening %s: %s\n", context->rc_file,
            strerror(errno));
    wordfree(&we);
    return 1;
  }
  wordfree(&we);
  fprintf(context->debug, "rcfile is finally %s\n", context->rc_file);
  return 0;
}

int init_context(struct Context *context) {
  return resolve_cwd(context) || resolve_rcfile(context);
}
