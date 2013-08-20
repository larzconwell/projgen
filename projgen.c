#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "deps/commander/commander.h"
#include "deps/bstring/bstrlib.h"

#include "path.h"
#include "projgen.h"

char *lang = NULL;
const char *license = "mit";
char *licensePath = NULL;
int lang_env = 0;

static void set_lang(command_t *cmd) {
  lang = (char *)cmd->arg;
}

static void set_license(command_t *cmd) {
  license = cmd->arg;
}

int main(int argc, char **argv) {
  command_t cmd;
  bstring dest = NULL;

  command_init(&cmd, "projgen", VERSION);
  command_option(&cmd, "-l", "--lang [arg]", "Language to use", set_lang);
  command_option(&cmd, "-i", "--license [arg]", "License to use", set_license);
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

  // Get path to license
  licensePath = path_join("licenses", license);
  if (licensePath == NULL) {
    goto error;
  }

  // Ensure the license file exists
  if (access(licensePath, F_OK) < 0) {
    perror("License file");
    goto error;
  }

  // TODO:
  // ensure language dir exists if given
  // make destination directory
  // write license
  // write readme.md
  // write language template if given
  // if makefile opt write makefile

  if (!lang_env) {
    free(lang);
  }
  free(licensePath);
  bdestroy(dest);
  command_free(&cmd);
  return 0;

error:
  if (!lang_env) {
    free(lang);
  }
  free(licensePath);
  bdestroy(dest);
  command_free(&cmd);
  return 1;
}
