#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "deps/bstring/bstrlib.h"

#include "path.h"

// Expands the given path if not absolute or NULL
extern bstring expand_path(bstring path) {
  int pathLen = blength(path);

  // Just return path if absolute
  if (path != NULL && pathLen > 0 && bchar(path, 0) == '/') {
    return path;
  }

  char *cwd = getcwd(NULL, 0);
  if (cwd == NULL) {
    perror("Expand directory");
    return NULL;
  }

  bstring bcwd = bfromcstr(cwd);
  if (bcwd == NULL) {
    free(cwd);
    fprintf(stderr, "An error occured while getting current directory");
    return NULL;
  }

  // Return bcwd if no path
  if (path == NULL) {
    free(cwd);
    bdestroy(path);
    return bcwd;
  }

  bstring path_seperator = bfromcstr("/");
  if (path_seperator == NULL) {
    free(cwd);
    bdestroy(bcwd);
    fprintf(stderr, "An error occured while getting current directory");
    return NULL;
  }

  int concat = BSTR_OK;
  concat = bconcat(bcwd, path_seperator);
  if (concat == BSTR_OK) {
    concat = bconcat(bcwd, path);
  }
  if (concat == BSTR_ERR) {
    free(cwd);
    bdestroy(path_seperator);
    bdestroy(bcwd);
    fprintf(stderr, "An error occured while expanding directory");
    return NULL;
  }

  free(cwd);
  bdestroy(path_seperator);
  bdestroy(path);
  return bcwd;
}

// Join a number of paths together, requires explicit NULL to terminate join
extern bstring path_join(const char *path, ...) {
  va_list args;
  int concat = BSTR_OK;
  bstring barg;

  bstring joined_path = bfromcstr(path);
  if (joined_path == NULL) {
    fprintf(stderr, "An error occured while joining directories");
    return NULL;
  }

  bstring path_seperator = bfromcstr("/");
  if (path_seperator == NULL) {
    bdestroy(joined_path);
    fprintf(stderr, "An error occured while joining directories");
    return NULL;
  }

  va_start(args, path);
  char *arg = va_arg(args, char *);
  while (arg != NULL) {
    barg = bfromcstr(arg);
    if (barg == NULL) {
      bdestroy(path_seperator);
      bdestroy(joined_path);
      fprintf(stderr, "An error occured while joining directories");
      return NULL;
    }

    concat = bconcat(joined_path, path_seperator);
    if (concat == BSTR_OK) {
      concat = bconcat(joined_path, barg);
    }
    if (concat == BSTR_ERR) {
      bdestroy(barg);
      bdestroy(path_seperator);
      bdestroy(joined_path);
      fprintf(stderr, "An error occured while joining directories");
      return NULL;
    }

    bdestroy(barg);
    arg = va_arg(args, char *);
  }
  va_end(args);

  bdestroy(path_seperator);
  return joined_path;
}

// Create all the directories for the given path
extern int mkdirall(bstring path, mode_t mode) {
  struct bstrList *pathSplit = bsplit(path, '/');
  bstring part;

  for (int i = 0; i < pathSplit->qty; i++) {
    part = pathSplit->entry[i];
  }

  bstrListDestroy(pathSplit);
  return 0;
}
