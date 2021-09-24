#include "tests.h"

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
