package main

import (
	"fmt"
	"math"
	"math/cmplx"
	"math/rand"
	"time"
)

func add(x, y int) int {
	return x + y
}

func swap(x, y string) (string, string) {
	return y, x
}

func split(sum int) (x, y int) {
	x = sum * 4 / 9
	y = sum - x
	return
}

var i, j int = 1, 2
var (
	ToBe   bool       = false
	MaxInt uint64     = 1<<64 - 1
	z      complex128 = cmplx.Sqrt(-5 + 12i)
)

func main() {
	a, c := swap("the playground", "Welcome to")
	fmt.Println(a, c)
	fmt.Println("เวลา is", time.Now())
	fmt.Println("My favorite number is", rand.Intn(10))
	fmt.Println("Pi is", math.Pi)
	fmt.Println("Calc is", add(42, 13))
	fmt.Println(split(17))

	r, python, java := true, false, "no!"
	fmt.Println(i, j, r, python, java)

	const ff = "%T(%v)\n"
	fmt.Printf(ff, ToBe, ToBe)
	fmt.Printf(ff, MaxInt, MaxInt)
	fmt.Printf(ff, z, z)

	var i int
	var f float64
	var b bool
	var s string
	fmt.Printf("%v %v %v %q\n", i, f, b, s)
}
