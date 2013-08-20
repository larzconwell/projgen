## Projgen

Projgen is a simple program to generate project scaffolding. It supports a number of languages and
platforms, as well as a number of options to customize the project.

### Installation
Clone this repository, optionally checking out the version you want, and then compile and install.
```
PREFIX=/usr/local make install
```
Just change the prefix to change the install path, or leave it empty to use `/usr/local`.

### Usage
Use the `-h` flag to get a list of the arguments you can use for the program.

When calling `projgen` with no arguments(not including flags) it uses the current directory
for the project files, to use another directory for the destination give it as an argument.

To set a default language when generating use the `DEFAULT_PROG_LANG` environment variable.

### License
MIT licensed, see [here](https://raw.github.com/larzconwell/projgen/master/LICENSE)
To get details about Projgen's dependencies view the projects directory in the `deps` directory.
