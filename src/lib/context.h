#ifndef _m_context_h
#define _m_context_h

#include <limits.h>
#include <stdio.h>

struct Context {
  FILE *file;
  FILE *err;
  FILE *out;
  FILE *debug;
  char section[64];
  char cwd[PATH_MAX];
  char rc_file[PATH_MAX];
  char matched_matcher[PATH_MAX];
};

struct Context *new_context();
int init_context(struct Context *);
void destroy_context(struct Context *);
#endif
