#include "tests.h"

char out[4096];
char err[4096];

void flush_fps(struct Context* ctx) {
  fflush(ctx->err);
  fflush(ctx->out);
  if (ctx->debug)
    fflush(ctx->debug);
}

struct Context *test_context(int debug) {
  struct Context *ctx = calloc(1, sizeof(struct Context));
  memset(out, 0, sizeof(out));
  memset(err, 0, sizeof(err));
  ctx->err = fmemopen(err, sizeof(err), "w");
  ctx->out = fmemopen(out, sizeof(out), "w");
  if (debug)
    ctx->debug = stdout;
  else
    ctx->debug = fopen("/dev/null", "w");
  return ctx;
}
