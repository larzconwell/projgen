FLAGS=--package=main --compress --uncompress_on_init -r

build: bundle
	go build

install: bundle
	go install

bundle:
	-(cd licenses && gobundle --bundle=licenses --target=../licenses.go $(FLAGS) .)
	-(cd langs && gobundle --bundle=langs --target=../langs.go $(FLAGS) .)

clean:
	rm -f licenses.go langs.go
