package main

import "fmt"

// A send to a nil channel blocks forever
func main_send_nil() {
	var c chan string
	c <- "let's get started" // deadlock

	// output go 1.19: fatal error: all goroutines are asleep - deadlock!
}

// A receive from a nil channel blocks forever
func main_recv_nil() {
	var c chan string
	fmt.Println(<-c) // deadlock

	// fatal error: all goroutines are asleep - deadlock!
}

// A send to a closed channel panics
func main_send_closed_ch() {
	var c = make(chan int, 100)
	for i := 0; i < 10; i++ {
		go func() {
			for j := 0; j < 10; j++ {
				c <- j
				// panic: send on closed channel
			}
			close(c)
		}()
	}
	for i := range c {
		fmt.Println(i)
	}
}

// A receive from a closed channel returns the zero value immediately
func main() {
	c := make(chan int, 3)
	c <- 1
	c <- 2
	c <- 3
	close(c)
	for i := 0; i < 4; i++ {
		fmt.Printf("%d ", <-c) // prints 1 2 3 0
	}

	//// correct version
	//for v := range c {
	//fmt.Printf("%d ", v) // do something with v
	//}
}
