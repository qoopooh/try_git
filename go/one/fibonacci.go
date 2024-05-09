package main

import (
	"fmt"
	"os"
	"strconv"
)

func fibonacci(n int) int {
	if n <= 1 {
		return n
	}
	return fibonacci(n-1) + fibonacci(n-2)
}

func fibonacciDP(n int) int {
	seq := fib(n)
	return seq[n-1] + seq[n-2]
}

func fib(n int) []int {
	if n <= 0 {
		return []int{}
	} else if n == 1 {
		return []int{1}
	}

	seq := make([]int, n)
	seq[0], seq[1] = 0, 1
	for i := 2; i < n; i++ {
		seq[i] = seq[i-1] + seq[i-2]
	}
	return seq
}

func main() {
	var n int

	if len(os.Args) < 2 {
		fmt.Print("Enter a number: ")
		fmt.Scan(&n)
	} else {
		var err error
		n, err = strconv.Atoi(os.Args[1])
		if err != nil {
			fmt.Println("Invalid input. Please provide a valid integer.")
			return
		}
	}

	if len(os.Args) > 2 {
		fmt.Printf("Fibonacci (dp) of %d is %d\n", n, fibonacciDP(n))
	} else {
		fmt.Printf("Fibonacci of %d is %d\n", n, fibonacci(n))
	}
}
