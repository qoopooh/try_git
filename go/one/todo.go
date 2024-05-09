package main

import "fmt"
import "net/http"

var taskItems = []string{"Buy groceries", "Walk the dog", "Read a book"}

func main() {
	fmt.Println("Welcome to the Todo List App!")

	http.HandleFunc("/", welcomeMessage)
	http.HandleFunc("/tasks", showTasks)

	http.ListenAndServe(":8080", nil)
}

func showTasks(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Tasks for today:\n")
	for index, task := range taskItems {
		fmt.Fprintf(w, "%d. %s\n", index+1, task)
	}
}

func welcomeMessage(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Welcome to the Todo List App!")
}

func printTasks(taskItems []string) {
	const taskListHeader = "Tasks for today:"
	fmt.Println(taskListHeader)
	for index, task := range taskItems {
		fmt.Printf("%d. %s\n", index+1, task)
	}
}

func addTask(taskItems []string, task string) []string {
	taskItems = append(taskItems, task)
	return taskItems
}
