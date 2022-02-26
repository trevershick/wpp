#ifndef _m_cmdline_h
#define _m_cmdline_h

#include "context.h"
#include "stdio.h"

void print_usage(char *, FILE *);

/**
 * @return 0 = ok, -1 = ok, anything positive = error
 */
int parse_arguments(int, char **, struct Context *);

int validate_arguments(struct Context *);
#endif
