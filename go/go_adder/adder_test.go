// adder_test.go
package main

import "testing"

func TestAdder(t *testing.T) {
	cases := []struct {
		a, b, expected int
	}{
		{1, 2, 3},
		{0, 0, 0},
		{-1, 1, 0},
		{10, -5, 5},
	}

	for _, c := range cases {
		result := Adder(c.a, c.b)
		if result != c.expected {
			t.Errorf("Adder(%d, %d) == %d, expected %d", c.a, c.b, result, c.expected)
		}
	}
}
