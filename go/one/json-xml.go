package main

import (
	"encoding/json"
	"encoding/xml"
	"fmt"
	"io"
	"log"
	"os"
)

type Lang struct {
	Name string
	Year int
	URL  string
}

func readFile(f string) {
	input, err := os.Open(f)
	if err != nil {
		log.Fatal(err)
	}
	io.Copy(os.Stdout, input)
}

func decode(f func(Lang), file string) {
	input, err := os.Open(file)
	if err != nil {
		log.Fatal(err)
	}
	dec := json.NewDecoder(input)
	for {
		var lang Lang
		err := dec.Decode(&lang)
		if err != nil {
			if err == io.EOF {
				break
			}
			log.Fatal(err)
		}
		f(lang)
	}
}

func main() {
	lang := Lang{"Go", 2009, "http://golang.org/"}
	data, err := json.Marshal(lang)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("%s\n", data)
	data, err = xml.MarshalIndent(lang, "", "  ")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("%s\n", data)

	readFile("lang.json")
	decode(func(lang Lang) {
		data, err = xml.MarshalIndent(lang, "", "  ")
		if err != nil {
			log.Fatal(err)
		}
		fmt.Printf("%s\n", data)
	}, "lang.json")
}
