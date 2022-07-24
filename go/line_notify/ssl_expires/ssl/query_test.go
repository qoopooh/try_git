package ssl

import "testing"

func TestFacebook(t *testing.T) {

	c := make(chan Result, 1)

	go Query("facebook.com", c)

	result := <-c

	if result.DaysToExpires < 1 {
		t.Fatalf("Wrong query result: %q", result)
	}
}
