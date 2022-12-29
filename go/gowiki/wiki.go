package main

import (
	"fmt"
	"html/template"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"regexp"
)

type Page struct {
	Title string
	Body  []byte
}

func (p *Page) save() error {
	return os.WriteFile(dataPath(p.Title), p.Body, 0666)
}

func loadPage(title string) (*Page, error) {
	body, err := os.ReadFile(dataPath(title))
	if err != nil {
		return nil, err
	}

	return &Page{Title: title, Body: body}, nil
}

func dataPath(title string) string {
	return filepath.Join("data", title+".txt")
}

func handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hi there, I love %s!", r.URL.Path[1:])
}

func viewHandler(w http.ResponseWriter, r *http.Request, title string) {

	p, err := loadPage(title)
	if err != nil {
		http.Redirect(w, r, "/edit/"+title, http.StatusFound)
		return
	}

	renderTemplate(w, "view", p)
}

func editHandler(w http.ResponseWriter, r *http.Request, title string) {

	p, err := loadPage(title)
	if err != nil {
		p = &Page{Title: title}
	}

	//fmt.Fprintf(w, "<h1>Editing %s</h1>"+
	//"<form action=\"/save/%s\" method=\"POST\">"+
	//"<textarea name=\"body\">%s</textarea><br>"+
	//"<input type=\"submit\" value=\"Save\">"+
	//"</form>",
	//p.Title, p.Title, p.Body)

	renderTemplate(w, "edit", p)
}

func saveHandler(w http.ResponseWriter, r *http.Request, title string) {

	body := r.FormValue("body")
	p := &Page{Title: title, Body: []byte(body)}
	err := p.save()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	http.Redirect(w, r, "/view/"+title, http.StatusFound)
}

func makeHandler(fn func(http.ResponseWriter, *http.Request, string)) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		if r.URL.Path == "/" {
			fn(w, r, "FrontPage")
			return
		}
		m := validPath.FindStringSubmatch(r.URL.Path)
		if m == nil {
			http.NotFound(w, r)
			return
		}
		fn(w, r, m[2])
	}
}

func renderTemplate(w http.ResponseWriter, action string, p *Page) {
	tmplPath := filepath.Join("tmpl", action+".html")
	t, err := template.ParseFiles(tmplPath)

	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	err = t.Execute(w, p)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

var validPath = regexp.MustCompile("^/(edit|save|view)/([a-zA-Z0-9]+)$")

func main() {

	//p1 := &Page{Title: "TestPage", Body: []byte("Test is a sample Page.")}
	//p1.save()
	//p2, _ := loadPage("TestPage")
	//fmt.Println(string(p2.Body))

	http.HandleFunc("/view/", makeHandler(viewHandler))
	http.HandleFunc("/edit/", makeHandler(editHandler))
	http.HandleFunc("/save/", makeHandler(saveHandler))
	http.HandleFunc("/", makeHandler(saveHandler))
	port := ":8080"
	fmt.Printf("Server run on http://localhost%s", port)

	log.Fatal(http.ListenAndServe(port, nil))
}
