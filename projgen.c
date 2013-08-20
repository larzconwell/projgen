#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "deps/commander/commander.h"
#include "deps/bstring/bstrlib.h"

#include "version.h"
#include "projgen.h"

char *lang = NULL;
int lang_env = 0;

static bstring expand_path(bstring path, const int pathLen) {
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

static void set_lang(command_t *cmd) {
  lang = (char *)cmd->arg;
}

int main(int argc, char **argv) {
  command_t cmd;
  bstring dest = NULL;

  command_init(&cmd, "projgen", VERSION);
  command_option(&cmd, "-l", "--lang [arg]", "Language to use", set_lang);
  command_parse(&cmd, argc, argv);

  if (cmd.argc > 0) {
    dest = bfromcstr(cmd.argv[0]);
    if (dest == NULL) {
      fprintf(stderr, "An error occured while getting the destination path");
      goto error;
    }
  }

  dest = expand_path(dest, blength(dest));
  if (dest == NULL) {
    goto error;
  }

  // If no language given check DEFAULT_PROG_LANG
  if (lang == NULL) {
    lang = getenv("DEFAULT_PROG_LANG");
    if (lang != NULL) {
      lang_env = 1;
    }
  }

  // TODO:
  // get license defaulting to mit
  // ensure license exists
  // ensure language dir exists if given
  // make destination directory
  // write license
  // write readme.md
  // write language template if given
  // if makefile opt write makefile

  if (!lang_env) {
    free(lang);
  }
  bdestroy(dest);
  command_free(&cmd);
  return 0;

error:
  if (!lang_env) {
    free(lang);
  }
  bdestroy(dest);
  command_free(&cmd);
  return 1;
}
