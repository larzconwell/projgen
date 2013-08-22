## Projgen

Projgen is a simple program to generate project scaffolding. It supports a number of languages and
platforms, as well as a number of options to customize the project.

### Installation
First you'll need to get `gobundle`. Simply do:
```
go get github.com/alecthomas/gobundle/gobundle
```

After that clone this repo, and then run make:
```
make install
```

### Usage
Use the `-h` flag to get a list of the arguments you can use for the program.

When calling `projgen` with no arguments(not including flags) it uses the current directory
for the project files, to use another directory for the destination give it as an argument.

To set a default language when generating use the `DEFAULT_PROG_LANG` environment variable.

### License
MIT licensed, see [here](https://raw.github.com/larzconwell/projgen/master/LICENSE)
