#include "cmdline.h"

#include "context.h"
#include "rcfile.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int validate_arguments(struct Context *context) {
  if (!strlen(context->section)) {
    fprintf(context->err, "Section Name (-s) required");
    return 1;
  }
  return 0;
}

void print_usage(char *prog, FILE *fp) {
  fprintf(fp, "Usage:\n");
  fprintf(fp, "%s -s <section> [options]\n", prog);
  fprintf(fp, "\n");
  fprintf(fp, "  -h               show list of command-line options\n");
  fprintf(fp, "  -d               show debug output\n");
  fprintf(fp, "  -g               generate a sample rc file\n");
  fprintf(fp, "\n");
  fprintf(fp, "CONFIG OPTIONS\n");
  fprintf(fp, "  -s <section>     the section in which to look\n");
  fprintf(fp, "  -r <config file> the RC file to load\n");
  fprintf(fp,
          "  -r               (optional), ~/workspaces.rc is the default\n");
  fprintf(fp, "  -f <file/dir>    use this file or directory for lookup\n");
  fprintf(fp, "                   (optional), PWD is used by default\n");
  fprintf(fp, "\n");
  fprintf(fp, "EXAMPLE CONFIG FILE (example_config.rc)\n");
  fprintf(fp, "[section1]\n");
  fprintf(fp, "~/workspaces=value1\n");
  fprintf(fp, "~/whatever=value2\n");
  fprintf(fp, "[section2]\n");
  fprintf(fp, "~=my home\n");
  fprintf(fp, "\n");
  fprintf(fp, "executing the command :\n");
  fprintf(fp,
          "%s -f example_config.rc -s section1 -f ~/workspaces/something.txt",
          prog);
  fprintf(fp, " would yield value1\n");
}

int parse_arguments(int argc, char **argv, struct Context *context) {
  int c;
  if (argc == 1) {
    print_usage(argv[0], context->out);
    exit(0);
  }

  while ((c = getopt(argc, argv, "dghs:r:f:")) != -1) {
    switch (c) {
    case 'd':
      context->debug = stdout;
      break;
    case 'f':
      strncpy(context->cwd, optarg, sizeof(context->cwd));
      break;
    case 'g':
      generate_sample_file(context->out);
      return -1;
    case 'h':
      print_usage(argv[0], context->out);
      return -1;
    case 'r':
      strncpy(context->rc_file, optarg, sizeof(context->rc_file));
      break;
    case 's':
      strncpy(context->section, optarg, sizeof(context->section));
      break;
    case '?':
      if (optopt == 's' || optopt == 'f' || optopt == 'r') {
        fprintf(context->err, "Option -%c requires an argument.\n", optopt);
      } else if (isprint(optopt)) {
        fprintf(context->err, "Unknown option '-%c'.\n", optopt);
      } else {
        fprintf(context->err, "Unknown option character '\\x%x'.\n", optopt);
      }
      return 1;
    default:
      abort();
    }
  }
  return 0;
}
