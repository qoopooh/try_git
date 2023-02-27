package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"sync"
	"text/template"

	"th.berm.trace/trace"
)

const Version = "1.0.0"

type templateHandler struct {
	once     sync.Once
	filename string
	templ    *template.Template
} // templateHandler

func (t *templateHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	t.once.Do(func() {
		t.templ = template.Must(template.ParseFiles(filepath.Join("templates", t.filename)))
	})
	t.templ.Execute(w, r)
}

func main() {
	addr := flag.String("addr", ":8080", "The port of the application.")
	flag.Parse() // parse the flags

	http.Handle("/", &templateHandler{filename: "chat.html"})
	http.HandleFunc("/version", func(w http.ResponseWriter, r *http.Request) {
		w.Write([]byte(Version))
	})
	http.Handle("/login", &templateHandler{filename: "login.html"})
	http.HandleFunc("/auth/", loginHandler)
	http.Handle("/chat", MustAuth(&templateHandler{filename: "chat.html"}))

	r := newRoom()
	r.tracer = trace.New(os.Stdout)
	http.Handle("/room", r)
	go r.run()

	server := *addr
	if (*addr)[0:1] == ":" {
		server = fmt.Sprintf("localhost%s", *addr)
	}

	log.Printf("Starting web server on http://%s\n", server)
	if err := http.ListenAndServe(*addr, nil); err != nil {
		log.Fatal("ListenAndServe:", err)
	}
}
