package main

import "fmt"

func modifyArray(arr [3]int) {
	arr[0] = 100
}

func modifySlice(s []int) {
	s[0] = 100
}

func main() {
	arr := [3]int{1, 2, 3}
	modifyArray(arr)
	fmt.Println(arr) // Output: [1 2 3] - unchanged because copy was modified

	var arr2 [3]int
	modifyArray(arr2)
	fmt.Println(arr2) // Output: [0 0 0] - unchanged

	s := []int{1, 2, 3}
	modifySlice(s)
	fmt.Println(s) // Output: [100 2 3] - original slice modified

	var names = []string{"Alice", "Bob"}
	names = append(names, names[0]+" "+names[1])
	fmt.Println(names) // Output: [Alice Bob Alice Bob] - slice with three names

	var arr_names [3]string
	arr_names[0] = "Alice"
	arr_names[1] = "Bob"
	arr_names[2] = arr_names[0] + " " + arr_names[1]
	fmt.Println(arr_names) // Output: [Alice Bob Alice Bob] - array with three names
}
