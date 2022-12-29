/**
 * https://www.doxsey.net/blog/go-concurrency-from-the-ground-up/
 */
package main

import "fmt"

func main() {
	ch1, ch2 := make(chan int), make(chan int)
	go a(ch1)
	go b(ch2)
	select {
	case value := <-ch1:
		println("1:", value)
	case value := <-ch2:
		println("2:", value)
		//default:
		//println("everthing was blocked")
	}
	fmt.Println("vim-go")
}

func a(ch chan int) {
	println("a before")
	ch <- 5
	println("a after")
}
func b(ch chan int) {
	println("b before")
	println(<-ch)
	println("b after")
}
