#include "format.h"

#include "string.h"

int is_section_header(const char *line) { return *line == '['; }

int is_matching_sechdr(const char *line, const char *section) {
  int sz;
  return *line == '[' && (sz = strlen(section)) > 0 &&
         strncmp(line + 1, section, sz) == 0 && line[sz + 1] == ']';
}
