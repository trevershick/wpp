#ifndef _m_rcfile_h
#define _m_rcfile_h

#include "context.h"
#include "stdio.h"

typedef void(*fn_t)(struct Context*, const char*);

int process_rcfile(struct Context *, fn_t);
void generate_sample_file(FILE*);

#endif
