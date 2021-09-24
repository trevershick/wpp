#include "tests.h"

struct Context *test_context() {
  struct Context *ctx = calloc(1, sizeof(struct Context));
  memset(out, 0, sizeof(out));
  memset(err, 0, sizeof(err));
  ctx->err = fmemopen(err, sizeof(err), "w");
  ctx->out = fmemopen(out, sizeof(out), "w");
  return ctx;
}
