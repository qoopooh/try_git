package main

import (
	"fmt"
	"strconv"
)

type Car struct {
	Make  string
	Model string
	Year  int
}

func main() {
	userDat := map[string]string{
		"first": "John",
		"last":  "Doe",
	}
	age := 42
	userDat["age"] = fmt.Sprint(age)
	gpa := 3.14
	userDat["gpa"] = strconv.FormatFloat(float64(gpa), 'f', 2, 64)

	num := "1234.56"
	var nn float64
	nn, err := strconv.ParseFloat(num, 2)
	if err != nil {
		fmt.Println("Error parsing float:", err)
		return
	}
	println("Parsed float:", nn)

	fmt.Println("userDat %+v\n", userDat)

	corolla := Car{
		Make:  "Toyota",
		Model: "Corolla",
		Year:  2020,
	}
	fmt.Println("Car struct: %+v\n", corolla)
}
