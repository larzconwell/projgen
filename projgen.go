package main

import (
	"bytes"
	"flag"
	"fmt"
	"github.com/alecthomas/gobundle"
	"io"
	"os"
	"path"
	"path/filepath"
	"strings"
)

var (
	project  string
	license  string
	lang     string
	makefile bool
	force    bool
	err      error
	readme   = "## {{name}}"
)

const usage = `Projgen

Usage:
  projgen [options] [destination]

Options:
  --license, -i [arg]  License to use(Default: mit)
  --language, -l [arg] Language template to use
  --makefile, -m       Create empty Makefile(Default: false)
  --force, -f          Force overwriting if the directory isn't
                       empty

If the DEFAULT_PROG_LANG environment variable is set and no
language is given then it's used, otherwise an error is
printed.

Supported licenses: mit, bsd2, bsd3, and apache
Supported languages: js, go, and c`

func init() {
	flag.StringVar(&license, "license", "mit", "")
	flag.StringVar(&license, "i", "mit", "")
	flag.StringVar(&lang, "language", "", "")
	flag.StringVar(&lang, "l", "", "")
	flag.BoolVar(&makefile, "makefile", false, "")
	flag.BoolVar(&makefile, "m", false, "")
	flag.BoolVar(&force, "force", false, "")
	flag.BoolVar(&force, "f", false, "")

	flag.Usage = func() {
		fmt.Fprintln(os.Stderr, usage)
	}
}

func main() {
	flag.Parse()
	var langFiles []string
	args := flag.Args()
	dst := ""

	// Get the full destination path
	if len(args) <= 0 {
		dst, err = os.Getwd()
	} else {
		dst, err = filepath.Abs(args[0])
	}
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	project = strings.Replace(filepath.Base(dst), filepath.Ext(dst), "", -1)

	// Ensure license exists
	_, err = LicensesBundle.Bytes(license)
	if os.IsNotExist(err) {
		fmt.Fprintln(os.Stderr, "License doesn't exist")
		os.Exit(1)
	}

	// Get DEFAULT_PROG_LANG
	if lang == "" {
		lang = os.Getenv("DEFAULT_PROG_LANG")
		if lang == "" {
			fmt.Fprintln(os.Stderr, "No language template to use")
			os.Exit(1)
		}
	}

	// Ensure language exists
	langFiles = LangsBundle.Files()
	found := false
	for _, file := range langFiles {
		if strings.Contains(file, lang) {
			found = true
			break
		}
	}
	if !found {
		fmt.Fprintln(os.Stderr, "Language doesn't exist")
		os.Exit(1)
	}
	lang += "/"

	// Create destination
	err = os.MkdirAll(dst, os.ModeDir|os.ModePerm)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	// Don't write files if destination isn't empty, and force isn't given
	dstF, err := os.Open(dst)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	defer dstF.Close()

	dstNames, err := dstF.Readdirnames(1)
	if err != nil && err != io.EOF {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	if len(dstNames) > 0 && !force {
		fmt.Fprintln(os.Stderr, "Directory isn't empty")
		os.Exit(1)
	}

	// Write the license to destination
	err = WriteBundle(LicensesBundle, license, filepath.Join(dst, "LICENSE"))
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	// Write the readme to destination
	readme = strings.Replace(readme, "{{name}}", project, -1)
	err = WriteText(readme, filepath.Join(dst, "README.md"))
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	// Write an .gitignore file
	err = WriteText("", filepath.Join(dst, ".gitignore"))
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	// Write an CHANGELOG.md file
	err = WriteText("", filepath.Join(dst, "CHANGELOG.md"))
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	// Write Makefile if flag was given
	if makefile {
		err = WriteText("", filepath.Join(dst, "Makefile"))
		if err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
	}

	// Write the language files to destination
	for _, file := range langFiles {
		if !strings.Contains(file, lang) {
			continue
		}

		// Slice the language name from split
		fileSplit := strings.Split(file, "/")[1:]
		name := fileSplit[len(fileSplit)-1]
		fileSplit = fileSplit[:len(fileSplit)-1]
		if strings.Contains(name, "name") {
			name = project + path.Ext(name)
		}
		name = filepath.Join(append(fileSplit, name)...)

		WriteBundle(LangsBundle, file, filepath.Join(dst, name))
	}
}

// Write a string to the dst
func WriteText(text, dst string) error {
	return Write(dst, bytes.NewBufferString(text))
}

// Write the contents of a bundle name to the dst
func WriteBundle(bundle *gobundle.Bundle, name, dst string) error {
	contents, err := bundle.Bytes(name)
	if err != nil {
		return err
	}

	// Replace template values
	str := string(contents)
	str = strings.Replace(str, "{{name}}", project, -1)
	str = strings.Replace(str, "{{license}}", license, -1)

	return Write(dst, bytes.NewBuffer([]byte(str)))
}

// Does the actual writing to the destination file
func Write(dst string, src io.Reader) error {
	err = os.MkdirAll(filepath.Dir(dst), os.ModeDir|os.ModePerm)
	if err != nil {
		return err
	}

	dstF, err := os.Create(dst)
	if err != nil {
		return err
	}
	defer dstF.Close()

	_, err = io.Copy(dstF, src)
	return err
}
