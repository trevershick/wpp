#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <wordexp.h>


int main() {

  char cwd[PATH_MAX];
  char * line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  wordexp_t we;
  FILE *fp;
  char* pos;
  int wordexp_err;

  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("getcwd");
    return 1;
  } 

  if ((wordexp_err = wordexp("~/.workspaces", &we, 0))) {
    fprintf(stderr, "expansion error on ~/.workspaces - %d\n", wordexp_err);
    return 1;
  }

  if ((fp = fopen(we.we_wordv[0], "r")) == NULL) {
    perror("opening ~/.workspaces");
    return 1;
  }

  while  ((linelen = getline(&line, &linecap, fp)) > 0) {
    if ((pos=strchr(line, '\n')) != NULL) {
      *pos = '\0';
    }
    if ((wordexp_err = wordexp(line, &we, WRDE_REUSE))) {
      fprintf(stderr, "expansion error - %d\n", wordexp_err);
      fclose(fp);
      return 1;
    }
    linelen = strlen(we.we_wordv[0]);

    // read the ~/.workspaces file
    if (strlen(cwd) < linelen) {
      continue;
    }
    if (strncmp(we.we_wordv[0], cwd, linelen) == 0) {
      if ((pos=strchr(cwd + linelen, '/')) != NULL) {
        *pos = '\0';
      }
      printf("%s", cwd + linelen);
      break;
    }

  }
  wordfree(&we);
  fclose(fp);
  return 0;
}
