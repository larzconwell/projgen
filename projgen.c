#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "deps/commander/commander.h"

#include "version.h"

static char *expand_path(char *path) {
  // Just return path if absolute
  if (path != NULL && path[0] == '/') {
    return path;
  }

  char *cwd = getcwd(NULL, 0);
  if (cwd == NULL) {
    return NULL;
  }

  // If path is NULL then the path is just cwd
  if (path == NULL) {
    return cwd;
  }
  size_t cwdLen = strlen(cwd);
  size_t pathLen = strlen(path);
  char *fullPath = malloc(cwdLen + pathLen + 2);

  memcpy(fullPath, cwd, cwdLen);
  fullPath[cwdLen] = '/';
  memcpy(fullPath + cwdLen + 1, path, pathLen + 1);

  free(path);
  free(cwd);
  return fullPath;
}

int main(int argc, char **argv) {
  command_t cmd;
  char *dest = NULL;

  command_init(&cmd, "projgen", VERSION);
  command_parse(&cmd, argc, argv);

  if (cmd.argc > 0) {
    dest = cmd.argv[0];
  }

  dest = expand_path(dest);
  if (dest == NULL) {
    perror("Expand destination");
    goto error;
  }
  printf("%s\n", dest);

  // TODO:
  // get default language from DEFAULT_PROG_LANG
  // get license defaulting to mit
  // ensure license exists
  // ensure language dir exists
  // make destination directory
  // write license
  // write readme.md
  // write language template
  // if makefile opt write makefile

error:
  free(dest);
  command_free(&cmd);
  return 1;

  free(dest);
  command_free(&cmd);
  return 0;
}
